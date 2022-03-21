library_manager:main.c
	@gcc -c -O2 -Wno-unused-result main.c book_management.c strsplit.c common.c user.c
	@gcc -o library_manager main.o book_management.o strsplit.o common.o user.o

library_manager_test:main.c
	@gcc -c -O2 -Wno-unused-result main.c book_management.c strsplit.c common.c user.c -DTESTMODE
	@gcc -o library_manager main.o book_management.o strsplit.o common.o user.o

test:library_manager_test

all:library_manager

clean:
	@rm -f library_manager *.o

