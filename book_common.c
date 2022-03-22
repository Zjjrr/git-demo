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
