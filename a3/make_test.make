all : test1
test1 : test1.o
	gcc -o test1 test1.o
test1.o : test1.c common.h
clean :
	echo cleaning
	rm test1.o test1
