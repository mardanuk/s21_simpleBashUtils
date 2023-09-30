CC= gcc -std=c11
CFLAGS= -Wall -Wextra -Werror
SOURCE_CAT= s21_cat.c

all: s21_cat

s21_cat:
	$(CC) $(CFLAGS) $(SOURCE_CAT) -o s21_cat

test_1:
	chmod 777 test_cat/test_1.sh
	sh test_cat/test_1.sh

test_2:
	chmod 777 test_cat/test_2.sh
	sh test_cat/test_2.sh

test:
	echo "\n--------FIRST TEST------- (CAT)"
	make test_1
	sleep 5
	echo "\n--------SECOND TEST-------(CAT)"
	make test_2

clean:
	rm -rf s21_cat *.a *.out *.o

check:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c
	clang-format -i *.c -style=Google
	clang-format -n *.c -style=Google

rebuild:
	make clean
	make all
