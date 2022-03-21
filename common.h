#ifndef COMMON_GUARD__H
#define COMMON_GUARD__H

#define BUF_LEN 128 // buffer size
#define USERNAME_LEN 64 // maximum user name length
#define PASSWORD_LEN 64 // maximum password length
#define BOOK_TITLE_LEN 64 // maximum book title length
#define BOOK_AUTHORS_LEN 64 // maximum book authors length
#define BOOKS_DATA "books.txt"
#define USERS_DATA "users.txt"
#define LOANS_DATA "loads.txt"

#include "book_management.h"

// free any resource allocated for the book
void clean_book(Book* book);

#endif
