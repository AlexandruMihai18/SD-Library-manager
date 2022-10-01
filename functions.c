// Copyright 2021-2022 Alexandru Mihai 313CA

#include <stdio.h>
#include <string.h>

#include "functions.h"

// function to free specifically the library hashtable
void
ht_linked_free(hashtable_t *ht)
{
    DIE(!ht, "Create a hashtable first!\n");

    for (unsigned int i = 0; i < ht->hmax; i++) {
        ll_node_t *current = ht->buckets[i]->head;
        for (unsigned int k = 0; k < ht->buckets[i]->size; k++) {
            free(((info *)current->data)->key);
            ht_free(((book_t *)((info *)current->data)->value)->ht_book);
            free(((info *)current->data)->value);
            current = current->next;
        }
        ll_free(&ht->buckets[i]);
    }

    free(ht->buckets);

    free(ht);
}

// function to free a book type hashtable
void
ht_library_free_data(void *value)
{
    ht_free(((book_t *)value)->ht_book);
    free(value);
}

// function to get the book name in order to exclude the
// quotation marks
void
get_book_name(char dest[])
{
    char part_name[MAX_BOOK_NAME_SIZE];
    scanf("%s", part_name);

    int offset = 0;
    memcpy(dest, part_name + 1, strlen(part_name) - 1);
    offset = strlen(part_name) - 1;

    while (part_name[strlen(part_name) - 1] != '\"') {
        scanf("%s", part_name);

        dest[offset] = ' ';
        offset++;

        memcpy(dest + offset, part_name, strlen(part_name));
        offset += strlen(part_name);
    }

    dest[offset - 1] = '\0';
}

// comparing the given ratings of two books
double
compare_rating(book_t *book1, book_t *book2)
{
    double ranking_difference = (book1->rating - book2->rating) * 1000;

    if ((int)ranking_difference != 0)
        return ranking_difference;

    return compare_purchases(book1, book2);
}

// comparing the given purchases of two books
int
compare_purchases(book_t *book1, book_t *book2)
{
    int purchases_difference = book1->purchases - book2->purchases;

    if (purchases_difference != 0)
        return purchases_difference;

    return compare_lexicographically(book1->name, book2->name);
}

// comparing two names lexicographically
int
compare_lexicographically(char *name1, char *name2)
{
    while (*name1 && *name1 == *name2) {
        name1++;
        name2++;
    }
    return (-1) * (*name1 - *name2);
}

// comparing two users' points
int
compare_points(user_t *user1, user_t *user2)
{
    int points_difference = user1->points - user2->points;

    if (points_difference)
        return points_difference;

    return compare_lexicographically(user1->name, user2->name);
}

// printing the top books by determing the highest rating
void
print_top_books(hashtable_t *ht_library)
{
    int index = 1;
    printf("Books ranking:\n");

    book_t *top_book = find_top_book(ht_library);

    while (top_book) {
        printf("%d. Name:%s Rating:%.3f Purchases:%d\n", index,
            top_book->name, top_book->rating, top_book->purchases);
        // removing a book after printing it in order to find the next one
        ht_remove_entry(ht_library, top_book->name, ht_library_free_data);
        top_book = find_top_book(ht_library);
        index++;
    }
}

// printing the top users by determining the most amount of points
void
print_top_users(hashtable_t *ht_users)
{
    int index = 1;
    printf("Users ranking:\n");

    user_t *top_user = find_top_user(ht_users);

    while (top_user) {
        printf("%d. Name:%s Points:%d\n", index,
            top_user->name, top_user->points);
        ht_remove_entry(ht_users, top_user->name, free);
        top_user = find_top_user(ht_users);
        index++;
    }
}

book_t*
find_top_book(hashtable_t *ht_library)
{
    book_t *top_book = NULL;
    book_t *current_book;

    for (unsigned int i = 0; i < ht_library->hmax; i++) {
        ll_node_t *current = ht_library->buckets[i]->head;
        for (unsigned int j = 0; j < ht_library->buckets[i]->size; j++) {
            current_book = (book_t *)((info *)current->data)->value;
            // determing the top book by comparing their ratings
            if (!top_book || compare_rating(top_book, current_book) < 0)
                top_book = current_book;
            current = current->next;
        }
    }

    return top_book;
}

user_t*
find_top_user(hashtable_t *ht_users)
{
    user_t *top_user = NULL;
    user_t *current_user;

    for (unsigned int i = 0; i < ht_users->hmax; i++) {
        ll_node_t *current = ht_users->buckets[i]->head;
        for (unsigned int j = 0; j < ht_users->buckets[i]->size; j++) {
            current_user = (user_t *)((info *)current->data)->value;
            // determining the top non-banned user by comparing their points
            if (!current_user->banned) {
                if (!top_user || compare_points(top_user, current_user) < 0)
                    top_user = current_user;
            }
            current = current->next;
        }
    }
    return top_user;
}
