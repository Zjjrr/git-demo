#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "book_common.h"
#include "user.h"
#include "loan.h"
#include "strsplit.h"
#include "type_return_code.h"

// borrow a book from the library
// return 0 if borrow the book successfully, or an error code otherwise
static int borrow_book(const User* user, const unsigned int id);

// return a book to the library
// return 0 if return the book successfully, or an error code otherwise
static int return_book(const User* user, const unsigned int id);

// init library data parameters
// return 0 if the parameters are correctly loaded, or -1 otherwise
static int init_parameters(int argc, char** args);

// free any resource allocated for parameters data
static void free_parameters();

extern BookHeadNode* bookHeadNodePt;
extern User* userHeadNodePt;
extern User* currentUser;
extern Loan* loanHeadNodePt;

static LibraryData* libraryData = NULL;

#ifdef TESTMODE

int main(int argc, char** args) {
    if (!load_books(fopen(BOOKS_DATA, "r")))
        printf("succeed to load books\n");
    if (!store_books(fopen(BOOKS_DATA, "w+")))
        printf("succeed to store books\n");
    return 0;
}

#else

int main(int argc, char** args) {
    int parameters_status;

    // check parameters
    parameters_status = init_parameters(argc, args);
    if (parameters_status == STATUS_PARAMETER_INVALID) {
        printf("Invalid parameters.\n<Books Data> <Users Data> <Loans Data> are required, or input nothing to use default paths.\n");
        return -1;
    } else if (parameters_status == STATUS_PARAMETER_DEFAULT)
        fprintf(stderr, "No parameter provioded, will use default values.\n");
        
    // initialize data
    if (load_books(fopen(libraryData -> books, "r"))) {
        fprintf(stderr, "Error in loading books data.\n");
        return -1;
    }
    if (load_users(fopen(libraryData -> users, "r"))) {
        fprintf(stderr, "Error in loading users data.\n");
        return -1;
    }
    if (load_loans(fopen(libraryData -> loans, "r"))) {
        fprintf(stderr, "Error in loading loans data.\n");
        return -1;
    }
    
    // free any resource
    clean_books(bookHeadNodePt);
    clean_users(userHeadNodePt);
    clean_loans(loanHeadNodePt);
    free_parameters();
    
    return 0;
    
}

#endif

static int borrow_book(const User* user, const unsigned int id) {
    
    Book* book = NULL;
    IdList ids;
    
    // check if the user and books is valid
    if (!user || !bookHeadNodePt)
        return STATUS_ERROR;
    ids = get_loans(user -> username);
    
    // check if the user has borrowed the book
    for (int i = 0; i < ids.length; i++) {
        if (id == ids.list[i])
            return STATUS_BOOK_LOANED;
    }
    book = bookHeadNodePt -> book;
    
    // check if the book has been lent out
    for (int j = 0; j < bookHeadNodePt -> length; j++) {
        if (book -> id == id) {
            if (! (book -> copies))
                return STATUS_BOOK_OUT;
            else
                break;
        } else
            book = book -> next;
    }
    
    // check if the book was found
    if (!book)
        return STATUS_BOOK_INVALID;
}

static int init_parameters(int argc, char** args) {
    // free library data before creating it
    if (libraryData)
        free_parameters();

    if (argc == 1) {
        libraryData = (LibraryData*) malloc(sizeof(LibraryData));
        libraryData -> books = BOOKS_DATA;
        libraryData -> users = USERS_DATA;
        libraryData -> loans = LOANS_DATA;
        libraryData -> isDynamic = 0;
        return STATUS_PARAMETER_DEFAULT;
    } else if (argc == 4) {
        libraryData = (LibraryData*) malloc(sizeof(LibraryData));
        libraryData -> books = args[1];
        libraryData -> users = args[2];
        libraryData -> loans = args[3];
        libraryData -> isDynamic = 1;
        return STATUS_OK;
    } else
        return STATUS_PARAMETER_INVALID;
}

static void free_parameters() {
    if (libraryData && libraryData -> isDynamic) {
        free(libraryData -> books);
        free(libraryData -> users);
        free(libraryData -> loans);
        free(libraryData);
    }
}
