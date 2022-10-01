// Copyright 2021-2022 Alexandru Mihai 313CA

#include <stdio.h>
#include <string.h>

#include "commands.h"

void add_book_command(hashtable_t **ht_library)
{
	char book_name[MAX_BOOK_NAME_SIZE];
	get_book_name(book_name);

	int def_number;
	char def_key[MAX_DEF_SIZE], def_val[MAX_DEF_SIZE];

	scanf("%d", &def_number);

	book_t book;

	// checking is the book is already in the library
	if (ht_has_key(*ht_library, book_name))
		ht_remove_entry(*ht_library, book_name, ht_library_free_data);

	// creating a new book
	memcpy(book.name, book_name, MAX_BOOK_NAME_SIZE);
	book.rating = 0;
	book.total_rating = 0;
	book.purchases = 0;
	book.borrowed = -1;
	book.ht_book = ht_create(HMAX, hash_function_string,
					compare_function_strings);

	for (int i = 0; i < def_number; i++) {
		scanf("%s%s", def_key, def_val);
		ht_put(&(book.ht_book), def_key, strlen(def_key) + 1,
			def_val, strlen(def_val) + 1);
	}

	// adding the book to the library
	ht_put(ht_library, book_name, strlen(book_name) + 1, &book, sizeof(book));
}

void get_book_command(hashtable_t *ht_library)
{
	char book_name[MAX_BOOK_NAME_SIZE];
	get_book_name(book_name);

	// printing the book information in the required format
	if (!ht_has_key(ht_library, book_name)) {
		printf("The book is not in the library.\n");
	} else {
		book_t book = *(book_t *)ht_get(ht_library, book_name);
		printf("Name:%s Rating:%.3f Purchases:%d\n", book.name,
			book.rating, book.purchases);
	}
}

void rmv_book_command(hashtable_t *ht_library)
{
	char book_name[MAX_BOOK_NAME_SIZE];
	get_book_name(book_name);

	if (!ht_has_key(ht_library, book_name))
		printf("The book is not in the library.\n");

	// removing the book from the library
	ht_remove_entry(ht_library, book_name, ht_library_free_data);
}

void add_def_command(hashtable_t *ht_library)
{
	char book_name[MAX_BOOK_NAME_SIZE];
	get_book_name(book_name);

	char def_key[MAX_DEF_SIZE], def_val[MAX_DEF_SIZE];
	scanf("%s%s", def_key, def_val);

	if (!ht_has_key(ht_library, book_name)) {
		printf("The book is not in the library.\n");
	} else {
		// extracting the book from the library to add the new definition
		book_t *book = (book_t *)ht_get(ht_library, book_name);
		ht_put(&(book->ht_book), def_key, strlen(def_key) + 1,
			def_val, strlen(def_val) + 1);
	}
}

void get_def_command(hashtable_t *ht_library)
{
	char book_name[MAX_BOOK_NAME_SIZE];
	get_book_name(book_name);

	char def_key[MAX_DEF_SIZE];
	char *def_val;
	scanf("%s", def_key);

	if (!ht_has_key(ht_library, book_name)) {
		printf("The book is not in the library.\n");
	} else {
		// extracting the book in order to get the definition
		book_t book = *(book_t *)ht_get(ht_library, book_name);
		if (!ht_has_key(book.ht_book, def_key)) {
			printf("The definition is not in the book.\n");
		} else {
			def_val = (char *)ht_get(book.ht_book, def_key);
			printf("%s\n", def_val);
		}
	}
}

void rmv_def_command(hashtable_t *ht_library)
{
	char book_name[MAX_BOOK_NAME_SIZE];
	get_book_name(book_name);

	char def_key[MAX_DEF_SIZE];
	scanf("%s", def_key);

	if (!ht_has_key(ht_library, book_name)) {
		printf("The book is not in the library.\n");
	} else {
		// extracting the book in order to remove a definition
		book_t book = *(book_t *)ht_get(ht_library, book_name);
		if (!ht_has_key(book.ht_book, def_key)) {
			printf("The definition is not in the book.\n");
		} else {
			ht_remove_entry(book.ht_book, def_key, free);
		}
	}
}

void add_user_command(hashtable_t **ht_users)
{
	char user_name[MAX_USER_NAME_SIZE];

	scanf("%s", user_name);

	if (ht_has_key(*ht_users, user_name)) {
		printf("User is already registered.\n");
		return;
	}

	// creating a new user
	user_t user;
	memcpy(user.name, user_name, MAX_USER_NAME_SIZE);
	user.points = INIT_POINTS;
	user.banned = 0;
	user.borrower = -1;

	// adding the user to the user hashtable
	ht_put(ht_users, user_name, strlen(user_name) + 1, &user, sizeof(user));
}

void borrow_command(hashtable_t *ht_library, hashtable_t *ht_users)
{
	char user_name[MAX_USER_NAME_SIZE];

	scanf("%s", user_name);

	char book_name[MAX_BOOK_NAME_SIZE];
	get_book_name(book_name);

	int days;

	scanf("%d", &days);

	if (!ht_has_key(ht_users, user_name)) {
		printf("You are not registered yet.\n");
		return;
	}

	user_t *user = (user_t *)ht_get(ht_users, user_name);

	if (user->banned == 1) {
		printf("You are banned from this library.\n");
		return;
	}

	if (user->borrower != -1) {
		printf("You have already borrowed a book.\n");
		return;
	}

	if (!ht_has_key(ht_library, book_name)) {
		printf("The book is not in the library.\n");
		return;
	}

	// extracting the book to mark it as borrowed
	book_t *book = (book_t *)ht_get(ht_library, book_name);

	if (book->borrowed == 1) {
		printf("The book is borrowed.\n");
		return;
	}

	book->borrowed = 1;
	// marking the user as the borrower (according to the borrowing days)
	user->borrower = days;

	// copying the borrowed book name in the user's profile
	memcpy(user->borrowed_book, book_name, MAX_BOOK_NAME_SIZE);
}

void return_command(hashtable_t *ht_library, hashtable_t *ht_users)
{
	char user_name[MAX_USER_NAME_SIZE];

	scanf("%s", user_name);

	char book_name[MAX_BOOK_NAME_SIZE];
	get_book_name(book_name);

	int days, rating;

	scanf("%d%d", &days, &rating);

	user_t *user = (user_t *)ht_get(ht_users, user_name);

	if (user->banned == 1) {
		printf("You are banned from this library.\n");
		return;
	}

	if (user->borrower == -1 ||
		(user->borrower != -1 && strcmp(user->borrowed_book, book_name) != 0)) {
		printf("You didn't borrow this book.\n");
		return;
	}

	// extracting the book from the library in order to mark it as returned
	book_t *book = (book_t *)ht_get(ht_library, book_name);

	if (user->borrower > days) {
		user->points += (user->borrower - days);
		user->borrower = -1;
		book->borrowed = -1;
	} else {
		user->points += 2 * (user->borrower - days);
		user->borrower = -1;
		book->borrowed = -1;
	}

	// banning the user
	if (user->points < 0) {
		printf("The user %s has been banned from this library.\n", user->name);
		user->banned = 1;
	}

	// determining the new book rating and marking the purchase
	book->total_rating += rating;
	book->rating = (double)(book->total_rating / (book->purchases + 1));
	book->purchases++;
}

void lost_command(hashtable_t *ht_library, hashtable_t *ht_users)
{
	char user_name[MAX_USER_NAME_SIZE];

	scanf("%s", user_name);

	char book_name[MAX_BOOK_NAME_SIZE];
	get_book_name(book_name);

	if (!ht_has_key(ht_users, user_name)) {
		printf("You are not registered yet.\n");
		return;
	}

	user_t *user = (user_t *)ht_get(ht_users, user_name);

	if (user->banned == 1) {
		printf("You are banned from this library.\n");
		return;
	}

	// removing the book from the library considering that it was lost
	ht_remove_entry(ht_library, book_name, ht_library_free_data);

	// deducting the required points
	user->points -= 50;
	user->borrower = -1;

	// banning the user
	if (user->points < 0) {
		printf("The user %s has been banned from this library.\n", user->name);
		user->banned = 1;
	}
}

void exit_command(hashtable_t *ht_library, hashtable_t *ht_users)
{
	print_top_books(ht_library);
	print_top_users(ht_users);
}
