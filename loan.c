#include <stdio.h>
#include <stdlib.h>
#include "user.h"
#include "loan.h"
#include "strsplit.h"

static Loan* create_empty_loan();

static void clean_loan(Loan* loan);

Loan* loanHeadNodePt = NULL;

int store_loans(FILE *file) {
    return 0;
}

int load_loans(FILE *file) {
    char buf[BUF_LEN], buf2[BUF_LEN];
    Loan* headNodePt = NULL;
    Loan** loan = &headNodePt;

    // check if fp is valid
    if (!file)
        return -1;
    
    // read loans info
    while (fgets(buf, BUF_LEN, file)) {
        *loan = create_empty_loan();
        sscanf(buf, "%s\t%[^\n]\n", (*loan) -> username, buf2);
        // check if buf2 is valid
        if (buf2) {
            // TODO: create id linked list
        } else {
            clean_loan(*loan);
            return -1;
        }
        loan = &((*loan) -> next);
    }

    fclose(file);
    return 0;
}

Loan* get_loans(const char* username) {
    
}

int add_loans(char* username, unsigned int id) {

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
    loan -> next = NULL;
    return loan;
}
