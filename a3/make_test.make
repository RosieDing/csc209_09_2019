all : test1
test1 : test1.o
	gcc -o test1 test1.o
clean :
	echo cleaning
	rm test1.o test1
