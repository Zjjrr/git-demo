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

// menu and interaction
static void menu_interface();

// refresh standard input buffer
static void flush_buffer();

// print book information
static void print_book(Book* book);

// print book list information
static void print_book_list(Book* book, int length);

static void menu_register_user(char* username, char* password);

static void menu_login_user(char* username, char* password);

static void menu_add_book(char* title, char* author);

static void menu_remove_book();

static void menu_search_book(char* title, char* author);

static void menu_display_all_books();

extern BookHeadNode* bookHeadNodePt;
extern User* userHeadNodePt;
extern User* currentUser;
extern Loan* loanHeadNodePt;

static LibraryData* libraryData = NULL;
static UserStatus userStatus = GUEST;
char (*menuItems)[BUF_LEN] = NULL;;

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
    
    menu_interface();
    
    // store data
    if (store_books(fopen(libraryData -> books, "w+")))
        printf(menuItems[28], libraryData -> books);
    if (store_users(fopen(libraryData -> users, "w+")))
        printf(menuItems[29], libraryData -> users);
    if (store_loans(fopen(libraryData -> loans, "w+")))
        printf(menuItems[30], libraryData -> loans);
    
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
    Loan* loan = NULL;
    Id* idNode = NULL;
    
    // check if the user and books is valid
    if (!user || !bookHeadNodePt)
        return STATUS_ERROR;
    else
        book = bookHeadNodePt -> book;
    
    if (!(loan = get_loans(user -> username))) 
        return STATUS_USER_INVALID;
    else
        idNode = loan -> idNode;
    
    // check if the user has borrowed the book
    while (idNode) {
        if (id == idNode -> id)
            return STATUS_BOOK_LOANED;
        idNode = idNode -> next;
    }
    
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

static int return_book(const User* user, const unsigned int id) {
    return STATUS_OK;
}

static void menu_interface() {
    int option = 0, status, quit = 0;
    char* username = (char*) malloc(sizeof(char) * USERNAME_LEN);
    char* password = (char*) malloc(sizeof(char) * PASSWORD_LEN);
    char* title = (char*) malloc(sizeof(char) * BOOK_TITLE_LEN);
    char* author = (char*) malloc(sizeof(char) * BOOK_AUTHORS_LEN);

    if (!menuItems) {
        static char items[][BUF_LEN] = {
            /*  0   */ "Please choose an option:",
            /*  1   */ "1) Register an account",
            /*  2   */ "2) Login in",
            /*  3   */ "3) Search for books",
            /*  4   */ "4) Display all books",
            /*  5   */ "1) Borrow a book",
            /*  6   */ "2) Return a book",
            /*  7   */ "1) Add a book",
            /*  8   */ "2) Remove a book",
            /*  9   */ "5) Quit",
            /*  10  */ "5) Log out",
            /*  11  */ " Option: ",
            /*  12  */ "Sorry, the option you entered was invalid, please try again.",
            /*  13  */ "Please enter a username: ",
            /*  14  */ "Please enter a password: ",
            /*  15  */ "Sorry, registration unsuccessful, the username you entered already exists.",
            /*  16  */ "Registered library account successfully!",
            /*  17  */ "Enter the title of the book you wish to add: ",
            /*  18  */ "Enter the author of the book you wish to add: ",
            /*  19  */ "Enter the year that the book you wish to add: ",
            /*  20  */ "Enter the number of copies of the book that you wish to add: ",
            /*  21  */ "Book was successfully added!",
            /*  22  */ "Sorry, you attempted to add an invalid book, please try again.",
            /*  23  */ "ID\tTitle\t\t\t\t\t\tAutors\t\t\t\t\t\tyear\tcopies",
            /*  24  */ "Please enter your username: ",
            /*  25  */ "Please enter your password: ",
            /*  26  */ "Sorry, the username or password you entered is incorrect, please try again.",
            /*  27  */ "Sorry, registration unsuccessful, the username you entered is invalid.",
            /*  28  */ "Error: Failed to store books to %s\n",
            /*  29  */ "Error: Failed to store users to %s\n",
            /*  30  */ "Error: Failed to store loans to %s\n",
            /*  31  */ "Enter the ID of the book you wish to remove: ",
            /*  32  */ "Sorry, the book you selected doesn't esist, please try again.",
            /*  33  */ "Book was successfully removed!",
            /*  34  */ "Sorry, failed to remove the book, an error occurred.",
            /*  35  */ "1) Find books by title",
            /*  36  */ "2) Find books by author",
            /*  37  */ "3) Find books by year",
            /*  38  */ "4) Return to pervious menu",
            /*  39  */ "Loading search menu...",
            /*  40  */ "Please enter title: ",
            /*  41  */ "Please enter author: ",
            /*  42  */ "Please enter year: ",
            /*  43  */ "Returning to previous menu...",
            /*  44  */ "Sorry, no books found, please try again.",
            /*  45  */ "Logging out..."
        };
        menuItems = items;
    }
    
    while (1) {
        // print head
        switch (userStatus) {
            case GUEST:
                printf("\n");
                break;
            case USER:
            case LIBRARIAN:
                printf("\n(logged in as: %s)\n", currentUser -> username);
        }
        
        // print menu
        switch (userStatus) {
            case GUEST:
                printf("%s\n%s\n%s\n%s\n%s\n%s\n%s", menuItems[0], menuItems[1], menuItems[2], 
                    menuItems[3], menuItems[4], menuItems[9], menuItems[11]);
                break;
            case USER:
                printf("%s\n%s\n%s\n%s\n%s\n%s\n%s", menuItems[0], menuItems[5], menuItems[6], 
                    menuItems[3], menuItems[4], menuItems[10], menuItems[11]);
                break;
            case LIBRARIAN:
                printf("%s\n%s\n%s\n%s\n%s\n%s\n%s", menuItems[0], menuItems[7], menuItems[8], 
                    menuItems[3], menuItems[4], menuItems[10], menuItems[11]);
                break;
        }
        
        // get option
        scanf("%d", &option);
        flush_buffer();
        
        // check option
        switch (option) {
            case 1:
                if (userStatus == GUEST) {
                    // register a user
                    menu_register_user(username, password);
                } else if (userStatus == USER) {
                    // TODO: borrow a book
                } else if (userStatus == LIBRARIAN) {
                    // add a book
                    menu_add_book(title, author);
                }
                break;
            case 2:
                if (userStatus == GUEST) {
                    // login
                    menu_login_user(username, password);
                } else if (userStatus == USER) {
                    // TODO: return a book
                } else if (userStatus == LIBRARIAN) {
                    // remove a book
                    menu_remove_book();
                }
                break;
            case 3:
                // search for a book
                    printf("\n%s\n", menuItems[39]);
                    menu_search_book(title, author);
                    printf("\n%s\n", menuItems[43]);
                break;
            case 4:
                // display all books
                menu_display_all_books();
                break;
            case 5:
                if (userStatus == USER || userStatus == LIBRARIAN) {
                    // log out
                    user_logout();
                    userStatus = GUEST;
                    printf("\n%s\n", menuItems[45]);
                } else {
                    // quit
                    quit = 1;
                }
                break;
            default:
                // try again
                printf("%s\n", menuItems[12]);
        }
        option = 0;
        if (quit) {
            // free resources
            free(username);
            free(password);
            free(title);
            free(author);
            break;
        }
    }
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

static void flush_buffer() {
    static char temp = 0;
    while ((temp = getchar() != '\n') && temp != EOF);
}

static void print_book(Book* book) {
    printf("%-8u%-48s%-48s%-8u%-8u\n", book -> id, book -> title, book -> authors, book -> year, book -> copies);
}

static void print_book_list(Book* book, int length) {
    for (int i = 0; i < length; i++)
        print_book(&book[i]);
}

static void menu_register_user(char* username, char* password) {
    printf("\n%s", menuItems[13]);
    scanf("%s", username);
    flush_buffer();
    printf("%s", menuItems[14]);
    scanf("%s", password);
    flush_buffer();
    switch (user_register(username, password)) {
        case STATUS_OK:
            printf("%s\n", menuItems[16]);
            break;
        case STATUS_USER_EXIST:
            printf("%s\n", menuItems[15]);
            break;
        case STATUS_USER_INVALID:
            printf("%s\n", menuItems[27]);
        break;
    }
}

static void menu_login_user(char* username, char* password) {
    printf("\n%s", menuItems[24]);
    scanf("%s", username);
    flush_buffer();
    printf("%s", menuItems[25]);
    scanf("%s", password);
    flush_buffer();
    switch (user_login(username, password)) {
        case STATUS_ERROR:
            printf("%s\n", menuItems[26]);
            break;
        case STATUS_USER_COMMON:
            userStatus = USER;
            break;
        case STATUS_USER_LIBRARIAN:
            userStatus = LIBRARIAN;
            break;
    }
}

static void menu_add_book(char* title, char* author) {
    unsigned int year, copies;
    Book* bookTemp = NULL;;
    
    printf("\n%s", menuItems[17]);
    scanf("%s", title);
    flush_buffer();
    printf("%s", menuItems[18]);
    scanf("%s", author);
    flush_buffer();
    printf("%s", menuItems[19]);
    scanf("%u", &year);
    flush_buffer();
    printf("%s", menuItems[20]);
    scanf("%u", &copies);
    flush_buffer();
    bookTemp = create_book(available_book_id(bookHeadNodePt), title, author, year, copies);
    if (add_book(*bookTemp))
        printf("%s\n", menuItems[22]);
    else
        printf("%s\n", menuItems[21]);
    free(bookTemp);
}

static void menu_remove_book() {
    unsigned int choice = 0;
    Book* bookTemp = bookHeadNodePt -> book;
    
    menu_display_all_books();
    printf("\n%s", menuItems[31]);
    scanf("%u", &choice);
    flush_buffer();
    for (int i = 0; i < bookHeadNodePt -> length; i++) {
        if (bookTemp -> id == choice) {
            if (!remove_book(*bookTemp))
                printf("%s\n", menuItems[33]);
            else
                printf("%s\n", menuItems[34]);
            return;
        }
        bookTemp = bookTemp -> next;
    }
    printf("%s\n", menuItems[32]);
}

static void menu_search_book(char* title, char* author) {
    int option = 0, quit = 0, year;
    BookList bookList;
    while (1) {
        // print menu
        printf("\n%s\n%s\n%s\n%s\n%s\n%s", menuItems[0], menuItems[35], menuItems[36], 
            menuItems[37], menuItems[38], menuItems[11]);
        scanf("%d", &option);
        flush_buffer();
        switch(option) {
            case 1:
                // by title
                printf("%s", menuItems[40]);
                scanf("%s", title);
                flush_buffer();
                bookList = find_book_by_title(title);
                if (!bookList.length)
                    printf("%s\n", menuItems[44]);
                else {
                    print_book_list(bookList.list, bookList.length);
                    // free resource after using it
                    free(bookList.list);
                }
                break;
            case 2:
                // by author
                printf("%s", menuItems[41]);
                scanf("%s", author);
                flush_buffer();
                bookList = find_book_by_author(author);
                if (!bookList.length)
                    printf("%s\n", menuItems[45]);
                else {
                    print_book_list(bookList.list, bookList.length);
                    free(bookList.list);
                }
                break;
            case 3:
                // by year
                printf("%s", menuItems[42]);
                scanf("%d", &year);
                flush_buffer();
                bookList = find_book_by_year(year);
                if (!bookList.length)
                    printf("%s\n", menuItems[46]);
                else {
                    print_book_list(bookList.list, bookList.length);
                    free(bookList.list);
                }
                break;
            case 4:
                // return
                quit = 1;
                break;
            default:
                // tyr again
                printf("%s\n", menuItems[12]);
        }
        option = 0;
        if (quit)
            break;
    }
}

static void menu_display_all_books() {
     Book* bookTemp = bookHeadNodePt -> book;
     
     printf("\n%s\n", menuItems[23]);
     for (int i = 0; i < bookHeadNodePt -> length; i++) {
          print_book(bookTemp);
          bookTemp = bookTemp -> next;
     }
}
