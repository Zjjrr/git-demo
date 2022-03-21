#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "user.h"
#include "book_management.h"
#include "type_return_code.h"

#ifdef TESTMODE

int main(int argc, char** args) {
    if (!load_books(fopen(BOOKS_DATA, "r")))
        printf("succeed to load books\n");
    if (!store_books(fopen(BOOKS_DATA, "w+")))
        printf("succeed to store books\n");
    if (!load_users(fopen(USERS_DATA, "r")))
        printf("succeed to load users\n");
    if (!store_users(fopen(USERS_DATA, "w+")))
        printf("succeed to store users\n");
    return 0;
}

#else

int main(int argc, char** args) {
    // TODO: implement CLI
}

#endif
