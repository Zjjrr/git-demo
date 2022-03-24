#ifndef COMMON_GUARD__H
#define COMMON_GUARD__H

#define BUF_LEN 128 // buffer size
#define SYM_SPLIT ", " // separator of combined string
#define LIST_LEN 128 // maximum length of the list
#define DATA_LEN 32 // maximum length of the path of data file

#define BOOKS_DATA "books.txt" // default data file of books
#define USERS_DATA "users.txt" // default data file of users
#define LOANS_DATA "loans.txt" // default data file of loans

typedef struct _LibraryData {
    char* books;
    char* users;
    char* loans;
    int isDynamic;
} LibraryData;

typedef enum _UserStatus {
    GUEST,
    USER,
    LIBRARIAN
} UserStatus;

#endif
