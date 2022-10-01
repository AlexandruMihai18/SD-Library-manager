// Copyright 2021-2022 Alexandru Mihai 313CA

#ifndef FUNCTIONS_H__
#define FUNCTIONS_H__

#include "Hashtable.h"

#define MAX_BOOK_NAME_SIZE 40
#define MAX_USER_NAME_SIZE 20
#define MAX_DEF_SIZE 20
#define INIT_POINTS 100

typedef struct book_t book_t;

struct book_t {
	char name[MAX_BOOK_NAME_SIZE];
	double rating;
	double total_rating;
	int purchases;
	int borrowed;
	hashtable_t *ht_book;
};

typedef struct user_t user_t;

struct user_t {
	char name[MAX_USER_NAME_SIZE];
	int points;
	int banned;
	int borrower;
	char borrowed_book[MAX_BOOK_NAME_SIZE];
};

void
ht_linked_free(hashtable_t *ht);

void
ht_library_free_data(void *value);

void
get_book_name(char *dest);

double
compare_rating(book_t *book1, book_t *book2);

int
compare_purchases(book_t *book1, book_t *book2);

int
compare_lexicographically(char *name1, char *name2);

int
compare_points(user_t *user1, user_t *user2);

void
print_top_books(hashtable_t *ht_library);

void
print_top_users(hashtable_t *ht_users);

book_t*
find_top_book(hashtable_t *ht_library);

user_t*
find_top_user(hashtable_t *ht_users);


#endif  // FUNCTIONS_H__
