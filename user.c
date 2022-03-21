#include <stdio.h>
#include <stdlib.h>
#include "user.h"
#include "common.h"
#include "type_return_code.h"

// create an empty user
static User* create_empty_user();

// frees any resource allocated to store users
static void user_cleanup();

User* userHeadNodePt = NULL;

int store_users(FILE* file) {
    User* user;
    
    // check if parameters are valid
    if (!userHeadNodePt || !file)
        return -1;
    else
        user = userHeadNodePt;
    
    // write user info
    do
        fprintf(file, "%s\t%s\n", user -> username, user -> password);
    while (user = user -> next);

    fclose(file);
    return 0;
}

int load_users(FILE* file) {
    char buf[BUF_LEN];
    User* headNodePt = create_empty_user();
    User* user = headNodePt;
    User** temp = NULL;
    
    // check if fp is valid
    if (!file)
        return -1;
    // read users info
    while (fgets(buf, BUF_LEN, file)) {
        if (temp) {
            *temp = create_empty_user();
            user = *temp;
        }
        sscanf(buf, "%s\t%s\n", user -> username, user -> password);
        temp = &(user -> next);
    }

    fclose(file);
    
    // update
    userHeadNodePt = headNodePt;

    return 0;
}

static User* create_empty_user() {
    User* user = (User*) malloc(sizeof(User));
    user -> username = (char*) malloc(sizeof(char) * USERNAME_LEN);
    user -> password = (char*) malloc(sizeof(char) * PASSWORD_LEN);
    return user;
}
