#ifndef USER_GUARD__H
#define USER_GUARD__H

#define USERNAME_LEN 64 // maximum user name length
#define PASSWORD_LEN 64 // maximum password length

typedef struct _User {
    char* username;
    char* password;
    struct _User* next;
} User;

// reserved for administrators
static User librarian = {"librarian", "librarian", NULL};

//saves the database of users in the specified file
//returns 0 if users were stored correctly, or an error code otherwise
int store_users(FILE *file);

//loads the database of users from the specified file
//the file must have been generated by a previous call to store_users()
//returns 0 if users were loaded correctly, or an error code otherwise
int load_users(FILE *file);

// login a user by username and password
// return status code
int user_login(const char* username, const char* password);

// register a user by username and password
// return 0 if register a user correctly, or an error code otherwise
int user_register(char* username, char* password);

// logout
void user_logout();

// free any resource allocated for the users
void clean_users(User* userHeadNode);

#endif
