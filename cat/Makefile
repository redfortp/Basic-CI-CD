CC = gcc -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all : clean s21_cat
s21_cat : s21_cat.o
	$(CC) -o s21_cat s21_cat.o

s21_cat.o : s21_cat.c
	$(CC) -c s21_cat.c

test: s21_cat
	bash tests.sh 
	
style_code :
	cp ../../materials/linters/.clang-format .
	clang-format -n *.c
	rm -rf .clang-format

clean :
	rm -rf *.o
	rm -rf s21_cat
