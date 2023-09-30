#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_LENGTH 4096
#define ARR_LENGTH 4096

struct flags {
  int e;  // Шаблон
  int i;  // Игнорирует различия регистра.
  int v;  // Инвертирует смысл поиска соответствий.
  int c;  // Выводит только количество совпадающих строк.
  int l;  // Выводит только совпадающие файлы.
  int n;  // Предваряет каждую строку вывода номером строки из файла ввода.
  int h;  // Выводит совпадающие строки, не предваряя их именами файлов.
  int s;  // Подавляет сообщения об ошибках о несуществующих или нечитаемых
  // файлах.
  int f;  // Получает регулярные выражения из файла.
  int o;  // Печатает только совпадающие (непустые) части совпавшей строки.
};

int get_flags(int argc, char **argv, struct flags *flag, char **patterns,
              int *pattern_size);

int find_patterns(char **patterns, int size, struct flags *flag, FILE *file,
                  int files_count, char *filename);

int process_file(char *name, struct flags *flag, char **patterns,
                 int pattern_size, int files_count);

int parse_patterns_file(char *patterns_file_name, struct flags *flag,
                        char **patterns, int *patterns_count);

int push_pattern(char **patterns, int index, char *new_pattern);

void free_str_arr(char **str_arr);

int main(int argc, char **argv) {
  int error = 0;
  struct flags flag = {0};
  char **patterns = calloc(ARR_LENGTH, sizeof(char *));
  int patterns_count = 0;
  error = get_flags(argc, argv, &flag, patterns, &patterns_count);
  if (argc != 1 && !flag.e && !flag.f && !error) {
    patterns_count = push_pattern(patterns, patterns_count, argv[optind]);
    optind++;
  }
  if (argc > 2 && !error) {
    int files_count = argc - optind;
    for (int i = optind; i < argc; i++) {
      error =
          process_file(argv[i], &flag, patterns, patterns_count, files_count);
    }
  } else {
    fprintf(stderr,
            "s21_grep: usage: s21_grep [-ivclnhso] [-e pattern_1] [-f file] "
            "[file] ...]\n");
    error = 1;
  }
  free_str_arr(patterns);
  return error;
}

void free_str_arr(char **str_arr) {
  for (int i = 0; i < ARR_LENGTH; i++)
    if (str_arr[i]) free(str_arr[i]);
  if (str_arr) free(str_arr);
}

int push_pattern(char **patterns, int index, char *new_pattern) {
  patterns[index] = calloc(STR_LENGTH, sizeof(char));
  strcpy(patterns[index], new_pattern);
  if (patterns[index][strlen(patterns[index]) - 1] == '\n' &&
      strlen(patterns[index]) != 1)
    patterns[index][strlen(patterns[index]) - 1] = '\0';
  index++;
  return index;
}

int get_flags(int argc, char **argv, struct flags *flag, char **patterns,
              int *patterns_count) {
  int rez = 0, option_index = 0, error = 0;
  const struct option long_options[] = {};
  while (!error && (rez = getopt_long(argc, argv, "e:ivclnhsf:o", long_options,
                                      &option_index)) != -1) {
    if (rez == 'e') {
      *patterns_count = push_pattern(patterns, *patterns_count, optarg);
      flag->e = 1;
    } else if (rez == 'i') {
      flag->i = 1;
    } else if (rez == 'v') {
      flag->v = 1;
    } else if (rez == 'c') {
      flag->c = 1;
    } else if (rez == 'l') {
      flag->l = 1;
    } else if (rez == 'n') {
      flag->n = 1;
    } else if (rez == 'h') {
      flag->h = 1;
    } else if (rez == 's') {
      flag->s = 1;
    } else if (rez == 'f') {
      error = parse_patterns_file(optarg, flag, patterns, patterns_count);
      flag->f = 1;
    } else if (rez == 'o') {
      flag->o = 1;
    } else {
      error = 1;
    }
  }
  return error;
}

int parse_patterns_file(char *patterns_file_name, struct flags *flag,
                        char **patterns, int *patterns_count) {
  int error = 0;
  FILE *file = fopen(patterns_file_name, "r");
  if (file != NULL) {
    char str[STR_LENGTH];

    while (fgets(str, STR_LENGTH - 1, file)) {
      *patterns_count = push_pattern(patterns, *patterns_count, str);
    }
    fclose(file);
  } else {
    error = 1;
    if (!flag->s)
      fprintf(stderr, "s21_grep: %s: No such file or directory\n",
              patterns_file_name);
  }
  return error;
}

void slice(char *src, size_t start, size_t end) {
  if (start < end) {
    char temp_str[(end - start) + 1];
    strncpy(temp_str, src + (start == 0 ? start + 1 : start),
            (size_t)(end - (start == 0 ? start + 1 : start)));
    temp_str[(end - start)] = '\0';
    strcpy(src, temp_str);
  } else {
    strcpy(src, "");
  }
}

void print_substr(struct flags *flag, char *str, regmatch_t *reg_match,
                  regex_t reg, int *is_first_substr, char *last_printed) {
  for (regoff_t i = reg_match[0].rm_so; i < reg_match[0].rm_eo; i++)
    printf("%c", str[i]);
  printf("\n");
  *last_printed = '\n';
  slice(str, reg_match[0].rm_eo, strlen(str));
  int status = regexec(&reg, str, 1, reg_match, 0), match = 0;
  if (status == 0) match = 1;
  if (match) {
    *is_first_substr = 0;
    print_substr(flag, str, reg_match, reg, is_first_substr, last_printed);
  }
}

void print_intermediate_info(struct flags *flag, char *filename,
                             int files_count, int str_num, char *str,
                             regmatch_t *reg_match, regex_t reg,
                             int *is_first_substr, int found_in_str,
                             char *last_printed) {
  if (!flag->c && !flag->l) {
    if (!flag->h && files_count > 1) printf("%s:", filename);
    if (flag->n && (found_in_str == 0 || files_count > 1))
      printf("%d:", str_num + 1);
    if (!flag->o && found_in_str == 0) {
      printf("%s", str);
      *last_printed = str[strlen(str) - 1];
    } else if (flag->o)
      print_substr(flag, str, reg_match, reg, is_first_substr, last_printed);
  }
}

void print_final_info(struct flags *flag, char *filename, int files_count,
                      int found_in_file_count) {
  if (flag->c && !flag->l && files_count == 1)
    printf("%d\n", found_in_file_count);
  else if (flag->c && !flag->h && files_count > 1)
    printf("%s:", filename);
  if (flag->c && flag->l)
    printf("%d\n", found_in_file_count > 0);
  else if (flag->c && !flag->l && files_count > 1)
    printf("%d\n", found_in_file_count);
  if (flag->l && found_in_file_count > 0)
    printf("%s\n", filename);
  else if ((flag->v && !flag->l && !flag->c))
    printf("\n");
}

void print_invert_intermediate_info(struct flags *flag, char *filename,
                                    int files_count, int str_num, char *str) {
  if (!flag->c && !flag->l) {
    if (!flag->h && files_count > 1) printf("%s:", filename);
    if (flag->n) printf("%d:", str_num + 1);
    printf("%s", str);
  }
}

int file_len(char *name) {
  FILE *file = fopen(name, "r");
  int len = 0;
  if (file) {
    char str[STR_LENGTH];
    while (fgets(str, STR_LENGTH, file)) len++;
    fclose(file);
  }
  return len;
}

void remove_last_new_line(char *str) {
  if (str[strlen(str) - 1] == '\n') str[strlen(str) - 1] = '\0';
}

int find_patterns(char **patterns, int patterns_count, struct flags *flag,
                  FILE *file, int files_count, char *filename) {
  int error = 0, type = 0, found_in_file_count = 0;
  char str[STR_LENGTH], last_printed = '\0';
  int last_line_index = file_len(filename) - 1;
  if (flag->i == 1) type = REG_ICASE;

  for (int i = 0; fgets(str, STR_LENGTH, file); i++) {
    if (flag->v && i == last_line_index) remove_last_new_line(str);
    int found_in_str = 0;
    for (int j = 0; j < patterns_count; j++) {
      const char *pattern = (const char *)patterns[j];
      regex_t regex;
      regmatch_t reg_match[1];
      regcomp(&regex, pattern, type);
      int reg_result = regexec(&regex, str, 1, reg_match, 0);
      if (!reg_result && !flag->v) {
        int is_first_substr = 1;
        print_intermediate_info(flag, filename, files_count, i, str, reg_match,
                                regex, &is_first_substr, found_in_str,
                                &last_printed);
        found_in_str == 0 ? found_in_file_count++ : 1;
        found_in_str += 1;
      }
      if (flag->v && reg_result == REG_NOMATCH) found_in_str += 1;
      regfree(&regex);
    }
    if (flag->v && found_in_str == patterns_count) {
      print_invert_intermediate_info(flag, filename, files_count, i, str);
      found_in_file_count++;
    }
  }
  print_final_info(flag, filename, files_count, found_in_file_count);
  if (last_printed != '\n' && found_in_file_count > 0 && !flag->c && !flag->l &&
      !flag->v)
    printf("\n");
  return error;
}

int process_file(char *name, struct flags *flag, char **patterns,
                 int patterns_count, int files_count) {
  int error = 0;
  FILE *file = fopen(name, "r");
  if (file) {
    find_patterns(patterns, patterns_count, flag, file, files_count, name);
  } else {
    if (!flag->s)
      fprintf(stderr, "s21_grep: %s: No such file or directory\n", name);
    error = 1;
  }
  if (file) fclose(file);
  return error;
}
