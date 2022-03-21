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
    Book* book;

    // check if fp is valid
    if (!file) {
        fprintf(stderr, "Error in opening file\n");
        return -1;
    }
    
    // check if book list is valid
    if (!bookHeadNodePt) {
        fprintf(stderr, "There is nothing to store\n");
        return -1;
    } else
        book = bookHeadNodePt -> book;
    
    // write the number of books
    fprintf(file, "%ud\n", bookHeadNodePt -> length);
    
    // write books info
    for (int i = 0; i < bookHeadNodePt -> length; i++) {
        // Book ID:
        fprintf(file, "ID:%ud\n", book -> id);
        
        // Book Title:
        fprintf(file, "Book Title:%s\n", book -> title);
        
        // Book Authors:
        fprintf(file, "Book Authors:%s\n", book -> authors);
        
        // Book Year:
        fprintf(file, "Book Year:%ud\n", book -> year);
        
        // Book Copies:
        fprintf(file, "Book Copies:%ud\n", book -> copies);
        
        book = book -> next;
    }
    fclose(file);
    return 0;
}

int load_books(FILE *file) {
    int nbook = 0;
    BookHeadNode* headNodePt;
    Book* book;
    char buf[BUF_LEN];
    
    // check if fp is valid
    if (!file) {
        fprintf(stderr, "Error in opening file\n");
        return -1;
    }
    
    // init head node
    headNodePt = (BookHeadNode*) malloc(sizeof(BookHeadNode));
    
    // read first line as the number of books
    if (fgets(buf, BUF_LEN, file) != NULL) {
        sscanf(buf, "%d\n", &nbook);
    } else {
        printf("Error in reading file\n");
        return -1;
    }
    
    // read book info
    for (int i = 0; i < nbook; i++) {
        if (book) {
            book -> next = (Book*) malloc(sizeof(Book));
            book = book -> next;
        } else {
            book = (Book*) malloc(sizeof(Book));
            headNodePt -> book = book;
        }
        
        // load book id
        fgets(buf, BUF_LEN, file);
        sscanf(buf, "ID:%ud\n", &(book -> id));
        
        // load book title
        fgets(buf, BUF_LEN, file);
		sscanf(buf, "Book Title:\%[^\n]", book -> title);
		
		// load book authors
		fgets(buf, BUF_LEN, file);
		sscanf(buf, "Book Authors:\%[^\n]", book -> authors);
		
		// load book year
		fgets(buf, BUF_LEN, file);
		sscanf(buf, "Book Year:%ud\n", &(book -> year));
		
		// load book copies
		fgets(buf, BUF_LEN, file);
		sscanf(buf, "Book Copies:%ud\n", &(book -> copies));
    }
    fclose(file);
    
    // try cleaning up the old books
    book_cleanup();
    
    // update
    headNodePt -> length = nbook;
    bookHeadNodePt = headNodePt;
    
    return 0;
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
