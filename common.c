#include <stdio.h>
#include <stdlib.h>
#include "common.h"

void clean_book(Book* book) {
    free(book -> title);
    free(book -> authors);
    free(book);
}
