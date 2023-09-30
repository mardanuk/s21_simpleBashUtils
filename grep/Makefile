CC= gcc -std=c11
CFLAGS= -Wall -Wextra -Werror
SOURCE_GREP= s21_grep.c

all: s21_grep

s21_grep:
	$(CC) $(CFLAGS) $(SOURCE_GREP) -o s21_grep

test_1:
	chmod 777 test_dir/test_2.sh
	sh test_dir/test_2.sh

test_2:
	chmod 777 test_dir/test_3.sh
	sh test_dir/test_3.sh

test_3:
	chmod 777 test_dir/test_grep/test_4.sh
	sh test_dir/test_grep/test_4.sh

test_4:
	chmod 777 test_dir/test_grep/test_4_leaks.sh
	sh test_dir/test_grep/test_4_leaks.sh

test:
	echo "\n--------FIRST TEST------- (GREP)"
	make test_1
	sleep 5
	echo "\n--------SECOND TEST-------(GREP)"
	make test_2
	sleep 5
	echo "\n--------THIRD TEST------- (GREP)"
	make test_3
	sleep 5
	echo "\n--------LEAK TEST ------- (GREP)"
	make test_4

clean:
	rm -rf s21_grep *.a *.out *.o

check:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c
	clang-format -i *.c -style=Google
	clang-format -n *.c -style=Google

rebuild:
	make clean
	make all
