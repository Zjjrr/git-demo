#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "common.h"
#include "type_return_code.h"

// create an empty user
static User* create_empty_user();

// frees any resource allocated to store users
static void user_cleanup();

User* userHeadNodePt = NULL;
User* currentUser = NULL;

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

void clean_users(User* userHeadNode) {
    if (userHeadNode != NULL) {
        User* user = userHeadNode;
        User* next;
        while(user) {
            next = user -> next;
            free(user -> username);
            free(user -> password);
            free(user);
            user = next;
        }
    }
}

User* user_login(const char* username, const char* password) {
    User* user = userHeadNodePt;
    User* temp = NULL;
    
    // check if username and password are valid
    if (!username || !password)
        return NULL;
    
    // logout before logging in
    user_logout();
    
    // check if user is valid
    if (!userHeadNodePt)
        return NULL;
    
    // login
    while (user) {
        temp = user -> next;
        if (!strcmp(username, user -> username) && !strcmp(password, user -> password)) {
            currentUser = user;
            return user;
        }
        user = temp;
    }
    
    return NULL;
}

int user_register(char* username, char* password) {
    User* user = NULL;
    User* last = NULL;
    User* temp = NULL;
    
    // check if username and password are valid
    if (!username || !password)
        return STATUS_USER_INVALID;
    
    // check if the username is exist already
    if (userHeadNodePt) {
        temp = userHeadNodePt;
        while (temp) {
            if (!strcmp(temp -> username, username))
                return STATUS_USER_EXIST;
            last = temp;
            temp = temp -> next;
        }
    }
    
    // create the user node
    user = create_empty_user();
    strncpy(user -> username, username, USERNAME_LEN);
    strncpy(user -> password, password, PASSWORD_LEN);
    
    // find the last node of users linked list and append the new user node
    if (userHeadNodePt)
        last -> next = user;
    else
        userHeadNodePt = user;
        
    return STATUS_OK;
}

void user_logout() {
    currentUser = NULL;
}


static User* create_empty_user() {
    User* user = (User*) malloc(sizeof(User));
    user -> username = (char*) malloc(sizeof(char) * USERNAME_LEN);
    user -> password = (char*) malloc(sizeof(char) * PASSWORD_LEN);
    user -> next = NULL;
    return user;
}
