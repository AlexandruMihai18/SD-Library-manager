// Copyright 2021-2022 Alexandru Mihai 313CA

#include <stdio.h>
#include <string.h>

#include "commands.h"

#define MAX_COMMAND_SIZE 16

int main(void) {
	// hashtable for the books in the library
	hashtable_t *ht_library = ht_create(HMAX,
		hash_function_string, compare_function_strings);

	// hashtable for the users
	hashtable_t *ht_users = ht_create(HMAX,
		hash_function_string, compare_function_strings);

	char command[MAX_COMMAND_SIZE];

	while (1) {
		scanf("%s", command);

		if (strcmp(command, "ADD_BOOK") == 0) {
			add_book_command(&ht_library);
		} else if (strcmp(command, "GET_BOOK") == 0) {
			get_book_command(ht_library);
		} else if (strcmp(command, "RMV_BOOK") == 0) {
			rmv_book_command(ht_library);
		} else if (strcmp(command, "ADD_DEF") == 0) {
			add_def_command(ht_library);
		} else if (strcmp(command, "GET_DEF") == 0) {
			get_def_command(ht_library);
		} else if (strcmp(command, "RMV_DEF") == 0) {
			rmv_def_command(ht_library);
		} else if (strcmp(command, "ADD_USER") == 0) {
			add_user_command(&ht_users);
		} else if (strcmp(command, "BORROW") == 0) {
			borrow_command(ht_library, ht_users);
		} else if (strcmp(command, "RETURN") == 0) {
			return_command(ht_library, ht_users);
		} else if (strcmp(command, "LOST") == 0) {
			lost_command(ht_library, ht_users);
		} else if (strcmp(command, "EXIT") == 0) {
			exit_command(ht_library, ht_users);
			break;
		}
	}

	ht_free(ht_users);
	ht_linked_free(ht_library);

	return 0;
}
