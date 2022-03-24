#include <stdio.h>
#include <stdlib.h>
#include "book_common.h"

void clean_book(Book* book) {
    free(book -> title);
    free(book -> authors);
    free(book);
}

void clean_books(BookHeadNode* bookHeadNodePt) {
    if (bookHeadNodePt != NULL) {
        Book* temp = bookHeadNodePt -> book;
        Book* current;
        while (temp != NULL) {
            current = temp;
            temp = current -> next;
            clean_book(current);
        }
        free(bookHeadNodePt);
        bookHeadNodePt = NULL;
    }
}

unsigned int available_book_id(BookHeadNode* bookHeadNode) {
    Book* book;
    unsigned int max = 0;
    if (!bookHeadNode)
        return -1;
    else if (bookHeadNode -> length == 0)
        return 1;
    book = bookHeadNode -> book;
    for (int i = 0; i < bookHeadNode -> length - 1; i++)
        book = book -> next;
    return book -> id + 1;
}

Book* create_book(unsigned int id, char* title, char* authors, unsigned int year, unsigned int copies) {
    Book* book;
    if (!title || !authors || year < 0 || year > 2099 || id <= 0)
        return NULL;
    book = (Book*) malloc(sizeof(Book));
    book -> id = id;
    book -> title = title;
    book -> authors = authors;
    book -> year = year;
    book -> copies = copies;
    book -> next = NULL;
    return book;
}
