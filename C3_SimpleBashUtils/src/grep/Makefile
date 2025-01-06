CC = gcc
FLAGS =-c -Wall -Werror -Wextra

s21_grep: s21_grep.o
	$(CC) s21_grep.o -o s21_grep

s21_grep.o: s21_grep.c s21_grep.h
	$(CC) $(FLAGS) s21_grep.c

clean:
	rm -rf *.o