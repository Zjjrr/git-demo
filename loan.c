#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "loan.h"
#include "strsplit.h"
#include "type_return_code.h"

static Loan* create_empty_loan();

static void clean_loan(Loan* loan);

static int add_id(Loan* loan, unsigned int id);

static int remove_id(Loan* loan, unsigned int id);

Loan* loanHeadNodePt = NULL;

int store_loans(FILE *file) {
    Loan* loan = loanHeadNodePt;
    Id* idPt = NULL;
    char buf[BUF_LEN], buf2[BUF_LEN];

    // check if parameters are valid
    if (!file || !loanHeadNodePt)
        return -1;
    // write loan info
    do {
        // format
        idPt = loan -> idNode;
        sprintf(buf, "%u", idPt -> id);
        while (idPt = idPt -> next) {
            sprintf(buf2, "%u", idPt -> id);
            strcat(strcat(buf, SYM_SPLIT), buf2);
        }
        
        // write a line
        fprintf(file, "%s\t%s\n", loan -> username, buf);
    } while (loan = loan -> next);
    
    fclose(file);
    return 0;
}

int load_loans(FILE *file) {
    char buf[BUF_LEN], buf2[BUF_LEN];
    Loan* headNodePt = NULL;
    Loan** loan = &headNodePt;
    StrSplit split;
    Id** idPt = NULL;

    // check if fp is valid
    if (!file)
        return -1;
    
    // read loans info
    while (fgets(buf, BUF_LEN, file)) {
        *loan = create_empty_loan();
        sscanf(buf, "%s\t%[^\n]\n", (*loan) -> username, buf2);
        // check if buf2 is valid
        if (buf2) {
            // create id linked list
            split = str_split(buf2, SYM_SPLIT);
            if (split.count) {
                idPt = &((*loan) -> idNode);
                for (int i = 0; i < split.count; i++) {
                    *idPt = (Id*) malloc(sizeof(Id));
                    sscanf(split.strings[i], "%u", &((*idPt) -> id));
                    (*idPt) -> next = NULL;
                    idPt = &((*idPt) -> next);
                }
                split.free(split);
            } else {
                clean_loans(headNodePt);
                split.free(split);
                return -1;
            }
        } else {
            clean_loans(headNodePt);
            return -1;
        }
        loan = &((*loan) -> next);
    }
    
    // update
    loanHeadNodePt = headNodePt;

    fclose(file);
    return 0;
}

Loan* get_loans(const char* username) {
    Loan* loan = loanHeadNodePt;

    // check if parameters are valid
    if (!username || !loanHeadNodePt)
        return NULL;

    while (loan) {
        if (!strcmp(loan -> username, username))
            return loan;
        loan = loan -> next;
    }

    return NULL;
}

int add_loans(char* username, unsigned int id) {
    Loan** loan = &loanHeadNodePt;
    Loan* parent = NULL;
    int status = 0, new = 0;

    // check if username is valid
    if (!username)
        return -1;

    // try getting the loan
    while (*loan) {
        if (!strcmp((*loan) -> username, username))
            break;
        parent = *loan;
        loan = &((*loan) -> next);
    }
    
    // if not found, create it
    if (!(*loan)) {
        *loan = create_empty_loan();
        // deep copy
        strncpy((*loan) -> username, username, USERNAME_LEN);
        new = 1;
    }
    
    // try adding id to the loan
    status = add_id(*loan, id);
    if (status && new) {
        clean_loan(*loan);
        if (parent)
            parent -> next = NULL;
    }
    return status;
}

int remove_loans(char* username, unsigned int id) {
    Loan** loan = &loanHeadNodePt;
    Loan** parent = NULL;
    Loan* temp;

    // check if username is valid
    if (!username || !loanHeadNodePt)
        return -1;

    // try getting the loan
    while (*loan) {
        if (!strcmp((*loan) -> username, username))
            break;
        parent = loan;
        loan = &((*loan) -> next);
    }

    // if there is no id after removing the target id, 
    // remove the loan from linked list
    if (((*loan) -> idNode) -> next == NULL && (*loan) -> idNode -> id == id) {
        temp = *loan;
        if (parent)
            (*parent) -> next = (*loan) -> next;
        clean_loan(temp);
        return 0;
    } else {
        return remove_id(*loan, id);
    }
}

void clean_loans(Loan* loan) {
    Loan* loanNode = loan;
    Loan* temp = NULL;

    while(loanNode) {
        temp = loanNode -> next;
        clean_loan(loanNode);
        loanNode = temp;
    }
}

static int add_id(Loan* loan, unsigned int id) {
    Id** idPt = NULL;
    
    // check if loan is valid
    if (!loan)
        return -1;
    else 
        idPt = &(loan -> idNode);

    // check if the id has been contained already
    while (*idPt) {
        if ((*idPt) -> id == id)
            return STATUS_BOOK_LOANED;
        idPt = &((*idPt) -> next);
    }
    
    // not contained, create new node
    *idPt = (Id*) malloc(sizeof(Id));
    (*idPt) -> id = id;
    (*idPt) -> next = NULL;
    
    return 0;
}

static int remove_id(Loan* loan, unsigned int id) {
    Id* idPt = NULL;
    Id* parent = NULL;
    
    // check if loan is valid
    if (!loan)
        return -1;
    else
        idPt = loan -> idNode;

    if (!idPt)
        return -1;

    if (idPt -> id == id) {
        // is head
        loan -> idNode = idPt -> next;
        free(idPt);
        return 0;
    } else {
        // not head
        while(idPt) {
            if (idPt -> id == id) {
                parent -> next = idPt -> next;
                free(idPt);
		break;
            }
            parent = idPt;
            idPt = idPt -> next;
        }
        // not found the book
        return -1;
    }
}

static void clean_loan(Loan* loan) {
    Id* idNode = loan -> idNode;
    Id* temp = NULL;

    while(idNode) {
        temp = idNode -> next;
        free(idNode);
        idNode = temp;
    }

    free(loan -> username);
    free(loan);
}

static Loan* create_empty_loan() {
    Loan* loan = (Loan*) malloc(sizeof(Loan));
    loan -> username = (char*) malloc(sizeof(char) * USERNAME_LEN);
    loan -> idNode = NULL;
    loan -> next = NULL;
    return loan;
}
