library_manager:main.c
	@gcc -c -O2 -Wno-unused-result main.c book_management.c strsplit.c
	@gcc -o library_manager main.o
all:library_manager

clean:
	@rm -f library_manager *.o

