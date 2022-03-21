#ifndef TYPE_RETURN_CODE_GUARD__H
#define TYPE_RETURN_CODE_GUARD__H

typedef enum _Code {
    ERROR = -1, // return -1 if meet any error
    OK, // return 0 if success
    BOOK_INVALID, // return 1 if the book id is invalid
    BOOK_OUT, // return 2 if the book has been lent out
    BOOK_LOANED // return 3 if the user has borrowed the book
} Code;

#endif
