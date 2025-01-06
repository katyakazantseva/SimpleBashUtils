CC = gcc
FLAGS =-c -Wall -Werror -Wextra

s21_cat: s21_cat.o
	$(CC) s21_cat.o -o s21_cat

s21_cat.o: s21_cat.c s21_cat.h
	$(CC) $(FLAGS) s21_cat.c

clean:
	rm -rf *.o