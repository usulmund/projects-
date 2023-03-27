CC=gcc 
CFLAGC= -Wall -Werror -Wextra


all: s21_grep

s21_grep: s21_grep.o grep.o
	$(CC) s21_grep.o grep.o -o s21_grep

s21_grep.o: s21_grep.c grep.h
	$(CC) $(CFLAGC) -c s21_grep.c

grep.o: grep.c grep.h
	$(CC) $(CFLAGC) -c grep.c

clean: 
	rm -rf *.o s21_grep


tests:	s21_grep
	./tests.sh

tests_sh: s21_grep
	sh tests.sh