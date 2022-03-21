#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "book_management.h"
#include "strsplit.h"

#define MAX_LIST_LEN 128
#define BUF_LEN 128
#define SYM_SPLIT ","

// frees any resource allocated to store books
static void book_cleanup();

// remove a node from the linked list by position
// return 0 if succeed to remove a node, or -1 otherwise
static int remove_book_node(int pos);

// append a node to the linked list
// return 0 if succeed to add a node, or -1 otherwise
static int append_book_node(Book* book);

// create an empty BookList
// remember to call free(BookList -> list) to release memory after using it
static BookList create_empty_booklist();

/* here I define a struct of header node
 * for the convenience of editing the linked list */
typedef struct _BookHeadNode {
    Book* book; // pointer to the first element of book list
    int length; // length of the book list
} BookHeadNode;

BookHeadNode* bookHeadNodePt = NULL;

int store_books(FILE *file) {
    // TODO: store books to file
}

int load_books(FILE *file) {
    if (file == NULL) {
        fprintf(stderr, "File is invalid\n");
        return -1;
    }
    // TODO: load books from file
    
}

int add_book(Book book) {
    return (bookHeadNodePt == NULL) ? -1 : append_book_node(&book);
}

int remove_book(Book book) {
    if (bookHeadNodePt == NULL)
        return -1;

    Book* bookNode = bookHeadNodePt -> book;
    for (int i = 0; i < bookHeadNodePt -> length; i++) {
        if (bookNode == &book)
            return remove_book_node(i);
        bookNode = bookNode -> next;
    }
    return -1;
}

BookList find_book_by_title (const char *title) {
    BookList bookList = create_empty_booklist();
    Book* book = bookHeadNodePt -> book;
    for (int i = 0; i < bookHeadNodePt -> length; i++) {
        if (!strcmp(book -> title, title))
            (bookList.list)[bookList.length ++] = *book;
    }
    return bookList;
}

BookList find_book_by_author (const char *author) {
    BookList bookList = create_empty_booklist();
    Book* book = bookHeadNodePt -> book;
    for (int i = 0; i < bookHeadNodePt -> length; i++) {
        StrSplit authors = str_split(book -> authors, SYM_SPLIT);
        for (int j = 0; j < authors.count; j++) {
            if (!strcmp(author, authors.strings[j])) {
                (bookList.list)[bookList.length ++] = *book;
                break;
            }
        }
        authors.free(authors);
    }
    return bookList;
}

BookList find_book_by_year (unsigned int year) {
    BookList bookList = create_empty_booklist();
    Book* book = bookHeadNodePt -> book;
        for (int i = 0; i < bookHeadNodePt -> length; i++) {
        if (book -> year == year)
            (bookList.list)[bookList.length ++] = *book;
    }
    return bookList;
}

static void book_cleanup() {
    if (bookHeadNodePt != NULL) {
        Book* temp = bookHeadNodePt -> book;
        Book* current;
        while (temp != NULL) {
            current = temp;
            temp = current -> next;
            free(current);
        }
        free(bookHeadNodePt);
        bookHeadNodePt = NULL;
    }
}

static int remove_book_node(int pos) {
    if (pos < 0 || pos > bookHeadNodePt -> length - 1)
        return -1;
        
    if (pos == 0) {
        Book* bookNode = bookHeadNodePt -> book;
        // if the position of node is 0, just edit the head node
        bookHeadNodePt -> book = bookNode -> next;
        // release node memory
        free(bookNode);
    } else {
        Book* perBookNode = bookHeadNodePt -> book;
        // find the previous node of the target position
        for (int i = 0; i != pos - 1; i++)
            perBookNode = perBookNode -> next;
        // find the node of the target position
        Book* bookNode = perBookNode -> next;
        // pointer field of redirection node
        perBookNode -> next = bookNode -> next;
        // release node memory
        free(bookNode);
    }
    
    (bookHeadNodePt -> length)--;
    return 0;
}

static int append_book_node(Book* book) {
    Book* bookNode = bookHeadNodePt -> book;
    // find the last node
    for (int i = 0; i < bookHeadNodePt -> length - 1; i++) {
        bookNode = bookNode -> next;
    }
    // append the target node
    bookNode -> next = book;
    return 0;
}

static BookList create_empty_booklist() {
    Book* books = (Book*) malloc(sizeof(Book) * MAX_LIST_LEN);
    BookList bookList = {
        books, // pointer to an array of struct book
        0 // number of elements in the array
    };
    return bookList;
}
