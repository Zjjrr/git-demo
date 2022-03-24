#ifndef BOOK_COMMON_GUARD__H
#define BOOK_COMMON_GUARD__H

#include "book_management.h"

#define BOOK_TITLE_LEN 64 // maximum book title length
#define BOOK_AUTHORS_LEN 64 // maximum book authors length

/* here I define a struct of header node
 * for the convenience of editing the linked list */
typedef struct _BookHeadNode {
    Book* book; // pointer to the first element of book list
    int length; // length of the book list
} BookHeadNode;

// free any resource allocated for the book
void clean_book(Book* book);

// free any resource allocated for the books
void clean_books(BookHeadNode* bookHeadNode);

// get the next available book ID
// return available id (ID >= 1 ) correctly, or -1 otherwise
unsigned int available_book_id(BookHeadNode* bookheadNode);

// create a book with id, title, authors, year and copies
// return a book if create correctly, or NULL otherwise
Book* create_book(unsigned int id, char* title, char* authors, unsigned int year, unsigned int copies);

#endif
