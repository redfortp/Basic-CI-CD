CC = gcc -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: s21_grep

s21_grep: clean s21_grep.o
	$(CC) -o s21_grep s21_grep.o

s21_grep.o: s21_grep.c
	$(CC) -c s21_grep.c

test: test_grep.sh
	bash test_grep.sh

style_code:
	cp ../../materials/linters/.clang-format ./
	clang-format -n *.c
	rm -rf .clang-format

clean:
	rm -rf *.o
	rm -rf s21_grep
