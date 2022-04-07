#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "book_management.h"
#include "book_common.h"
#include "strsplit.h"

// remove a node from the linked list by position
// return 0 if succeed to remove a node, or -1 otherwise
static int remove_book_node(int pos);

// append a node to the linked list
// return 0 if succeed to add a node, or -1 otherwise
static int append_book_node(Book* book);

// create an empty BookList
// remember to call free(BookList -> list) to release memory after using it
static BookList create_empty_booklist();

// create an empty book
static Book* create_empty_book();

BookHeadNode* bookHeadNodePt = NULL;

int store_books(FILE *file) {
    Book* book = NULL;

    // check if parameters are valid
    if (!file || !bookHeadNodePt)
        return -1;
    else
        book = bookHeadNodePt -> book;
    
    // write the number of books
    fprintf(file, "%u\n", bookHeadNodePt -> length);
    
    // write books info
    for (int i = 0; i < bookHeadNodePt -> length; i++) {
    
        // Book ID:
        fprintf(file, "ID:\t\t\t\t%u\n", book -> id);
        
        // Book Title:
        fprintf(file, "Book Title:\t\t%s\n", book -> title);
        
        // Book Authors:
        fprintf(file, "Book Authors:\t%s\n", book -> authors);
        
        // Book Year:
        fprintf(file, "Book Year:\t\t%u\n", book -> year);
        
        // Book Copies:
        fprintf(file, "Book Copies:\t%u\n", book -> copies);
        
        book = book -> next;
    }
    fclose(file);
    return 0;
}

int load_books(FILE *file) {
    int nbook = 0;
    BookHeadNode* headNodePt = NULL;
    Book* book = NULL;
    char buf[BUF_LEN];
    
    // check if fp is valid
    if (!file)
        return -1;
    
    // read first line as the number of books
    if (fgets(buf, BUF_LEN, file) != NULL)
        sscanf(buf, "%d\n", &nbook);
    else
        return -1;
        
    // init head node
    headNodePt = (BookHeadNode*) malloc(sizeof(BookHeadNode));
    
    // read book info
    for (int i = 0; i < nbook; i++) {
        if (book) {
            book -> next = create_empty_book();
            book = book -> next;
        } else {
            book = create_empty_book();
            headNodePt -> book = book;
        }
        
        // load book id
        fgets(buf, BUF_LEN, file);
        sscanf(buf, "ID:\t\t\t\t%u\n", &(book -> id));
        
        // load book title
        fgets(buf, BUF_LEN, file);
		sscanf(buf, "Book Title:\t\t%[^\n]", book -> title);
		
		// load book authors
		fgets(buf, BUF_LEN, file);
		sscanf(buf, "Book Authors:\t%[^\n]", book -> authors);
		
		// load book year
		fgets(buf, BUF_LEN, file);
		sscanf(buf, "Book Year:\t\t%u\n", &(book -> year));
		
		// load book copies
		fgets(buf, BUF_LEN, file);
		sscanf(buf, "Book Copies:\t%u\n", &(book -> copies));
    }
    fclose(file);
    
    // try cleaning up the old books
    clean_books(bookHeadNodePt);
    
    // update global value
    headNodePt -> length = nbook;
    bookHeadNodePt = headNodePt;
    
    return 0;
}

int add_book(Book book) {
    // make a deep copy of the Book
    Book* abook = create_empty_book();
    abook -> id = book.id;
    strncpy(abook-> title, book.title, BOOK_TITLE_LEN);
    strncpy(abook -> authors, book.authors, BOOK_AUTHORS_LEN);
    abook -> year = book.year;
    abook -> copies = book.copies;
    return (bookHeadNodePt == NULL) ? -1 : append_book_node(abook);
}

int remove_book(Book book) {
    if (bookHeadNodePt == NULL)
        return -1;

    Book* bookNode = bookHeadNodePt -> book;
    for (int i = 0; i < bookHeadNodePt -> length; i++) {
        if (bookNode -> id == book.id)
            return remove_book_node(i);
        bookNode = bookNode -> next;
    }
    return -1;
}

BookList find_book_by_title (const char *title) {
    BookList bookList = create_empty_booklist();
    Book* book = bookHeadNodePt -> book;
    for (int i = 0; i < bookHeadNodePt -> length; i++) {
        if (strstr(book -> title, title))
            (bookList.list)[bookList.length ++] = *book;
        book = book -> next;
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
        book = book -> next;
    }
    return bookList;
}

BookList find_book_by_year (unsigned int year) {
    BookList bookList = create_empty_booklist();
    Book* book = bookHeadNodePt -> book;
    for (int i = 0; i < bookHeadNodePt -> length; i++) {
        if (book -> year == year)
            (bookList.list)[bookList.length ++] = *book;
        book = book -> next;
    }
    return bookList;
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
    for (int i = 0; i < bookHeadNodePt -> length - 1; i++)
        bookNode = bookNode -> next;
    // append the target node
    bookNode -> next = book;
    (bookHeadNodePt -> length)++;
    return 0;
}

static BookList create_empty_booklist() {
    Book* books = (Book*) malloc(sizeof(Book) * LIST_LEN);
    BookList bookList = {
        books, // pointer to an array of struct book
        0 // number of elements in the array
    };
    return bookList;
}

static Book* create_empty_book() {
    // create and init a book with default values
    Book* book = (Book*) malloc(sizeof(Book));
    book -> id = 0;
    book -> title = (char*) malloc(sizeof(char) * BOOK_TITLE_LEN);
    book -> authors = (char*) malloc(sizeof(char) * BOOK_AUTHORS_LEN);
    book -> year = 0;
    book -> copies = 0;
    book -> next = NULL;
    return book;
}
