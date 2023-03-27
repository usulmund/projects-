CC=gcc 
CFLAGC= -Wall -Werror -Wextra


all: s21_cat

s21_cat: cat.o cat_src.o
	$(CC) cat.o cat_src.o -o s21_cat

cat.o: cat.c cat_src.h
	$(CC) $(CFLAGC) -c cat.c

cat_src.o: cat_src.c cat_src.h
	$(CC) $(CFLAGC) -c cat_src.c

clean: 
	rm -rf *.o s21_cat

tests:	s21_cat
	./tests.sh

tests_sh: s21_cat
	sh tests.sh
