#ifndef TYPE_RETURN_CODE_GUARD__H
#define TYPE_RETURN_CODE_GUARD__H

typedef enum _Status {
    STATUS_ERROR = -1, // return -1 if meet any error
    STATUS_OK, // return 0 if success
    STATUS_BOOK_INVALID, // return 1 if the book id is invalid
    STATUS_BOOK_OUT, // return 2 if the book has been lent out
    STATUS_BOOK_LOANED, // return 3 if the user has borrowed the book
    STATUS_PARAMETER_INVALID, // return 4 if the parameter is invalid
    STATUS_PARAMETER_DEFAULT, // return 5 if use default parameter
    STATUS_USER_INVALID, // return 6 if the user's information is invalid
    STATUS_USER_EXIST, // return 7 if the user is already exist
    STATUS_USER_NONEXIST //return 8 if the user is not exist
} Status;

#endif
