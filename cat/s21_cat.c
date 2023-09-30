#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct flags {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
};

int get_flags(int argc, char **argv, struct flags *flag);

void apply_flags(struct flags flag, FILE *file);

int process_file(char *name, struct flags flag);

void format(int *ch);
void duplicate_input();

int main(int argc, char **argv) {
  int error = 0;
  if (argc >= 2) {
    struct flags flag;
    memset(&flag, 0, sizeof flag);
    error = get_flags(argc, argv, &flag);
    while (optind < argc && !error) {
      error = process_file(argv[optind], flag);
      optind++;
    }
  } else if (argc == 1) {
    duplicate_input();
  }
  return (error);
}
void duplicate_input() {
  const int buffer_size = 4096;
  char str[buffer_size];
  while (fgets(str, buffer_size, stdin)) {
    printf("%s", str);
  }
}
int get_flags(int argc, char **argv,
              struct flags *flag) {  // TODO flag at the end
  int rez, option_index, error = 0;
  const struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {0, 0, 0, 0}};

  while ((rez = getopt_long(argc, argv, "+benstv", long_options,
                            &option_index)) != -1 &&
         !error) {
    if (rez == 'b')
      flag->b = 1;
    else if (rez == 'e')
      flag->e = 1;
    else if (rez == 'n')
      flag->n = 1;
    else if (rez == 's')
      flag->s = 1;
    else if (rez == 't')
      flag->t = 1;
    else if (rez == 'v')
      flag->v = 1;
    else {
      error = 1;
      fprintf(stderr, "usage: s21_cat [-benstv] [file ...]");
    }
  }
  return error;
}

int process_file(char *name, struct flags flag) {
  int error = 0;
  FILE *file = fopen(name, "r");
  if (file) {
    apply_flags(flag, file);
  } else {
    fprintf(stderr, "s21_cat: %s: No such file or directory\n", name);
    error = 1;
  }
  if (file) fclose(file);
  return error;
}

void apply_flags(struct flags flag, FILE *file) {
  int number = 1, new_lines = 0;
  int curr_ch = fgetc(file), prev_ch = '\n';
  while (curr_ch != EOF) {
    if (flag.s == 1 && curr_ch == '\n' && prev_ch == '\n')
      new_lines += 1;
    else
      new_lines = 0;
    if (flag.b == 1 && curr_ch != '\n' && prev_ch == '\n') {
      printf("%6d\t", number);
      number++;
    }
    if (new_lines <= 1 && flag.b == 0 && flag.n == 1 && prev_ch == '\n') {
      printf("%6d\t", number);
      number++;
    }
    if (new_lines <= 1 && flag.e == 1 && curr_ch == '\n') {
      printf("$");
    }
    if (flag.t == 1 && curr_ch == '\t') {
      printf("^");
      curr_ch = 'I';
    }
    if ((curr_ch != '\t') && (flag.v == 1 || flag.t == 1 || flag.e == 1)) {
      format(&curr_ch);
    }
    if (new_lines <= 1) printf("%c", curr_ch);
    prev_ch = curr_ch;
    curr_ch = fgetc(file);
  }
}

void format(int *ch) {
  if (*ch < ' ' && *ch != '\n') {
    printf("^");
    *ch = *ch + '@';
  } else if (*ch == 127) {
    printf("^");
    *ch = '?';
  }
}
