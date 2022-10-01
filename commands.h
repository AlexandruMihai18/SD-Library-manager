// Copyright 2021-2022 Alexandru Mihai 313CA

#ifndef COMMANDS_H__
#define COMMANDS_H__

#include "functions.h"

void add_book_command(hashtable_t **ht_library);

void get_book_command(hashtable_t *ht_library);

void rmv_book_command(hashtable_t *ht_library);

void add_def_command(hashtable_t *ht_library);

void get_def_command(hashtable_t *ht_library);

void rmv_def_command(hashtable_t *ht_library);

void add_user_command(hashtable_t **ht_users);

void borrow_command(hashtable_t *ht_library, hashtable_t *ht_users);

void return_command(hashtable_t *ht_library, hashtable_t *ht_users);

void lost_command(hashtable_t *ht_library, hashtable_t *ht_users);

void exit_command(hashtable_t *ht_library, hashtable_t *ht_users);

#endif  // COMMANDS_H__
