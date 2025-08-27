/**
 * @file user.h
 * @brief Header file for user and hash table management.
 *
 * Declares data structures and functions related to user data,
 * such as insertion into hash tables, deletion, and search utilities.
 * Also includes memory management helpers.
 * 
 * Include this file where user-related operations are needed.
 * 
 * @author Afonso Sítima - 114018
 */


#ifndef USER_H
#define USER_H


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "date.h"
#include "vaccine.h"
#include "inoculation.h"

#define NUM_USERS   1100      /**< Maximum initial number of users in the hash table */
#define PERCENT     0.7       /**< percentage to trigger hash table resizing */
#define HASH        127       /**< Initial seed for hash calculation */
#define ONLY_NAME   0         /**< Flag for removal using only username */
#define WITH_DATE   1         /**< Flag for removal using username and date */
#define WITH_BATCH  2         /**< Flag for removal using username, date and batch */

#define NO_USER_NUM -1        /**< Error code: user does not exist */

#define NO_USER(A) ((A == ENG) ? ": no such user" : ": utente inexistente") /**< Error message: user does not exist */


/**
 * @brief Structure that stores user data.
 */
typedef struct user {
    char *name;              /**< User's name */
    LinkInl *ino_list;       /**< Array of inoculations linked to the user */
    int count;               /**< Number of inoculations the user has */
    struct user *next;       /**< Pointer to the next user in case of collision (linked list) */
} User;


/**
 * @brief Hash table for storing and managing users.
 */
typedef struct hash {
    User **user_list;        /**< Array of pointers to user entries (buckets) */
    int size;                /**< Total size of the hash table */
    int count;               /**< Current number of users in the table */
} HashTable;


/**
 * @brief Calculates the hash index for a given user name.
 * 
 * @param name The name of the user.
 * @return The computed hash index.
 */
int hash(char *name);


/**
 * @brief Inserts a new inoculation entry into the hash table.
 *
 * If the user already exists, the inoculation is added to their list.
 * If not, a new user is created and added to the hash table.
 *
 * @param ht Pointer to the hash table.
 * @param ino Pointer to the inoculation to insert.
 * @param name Name of the user.
 */
void insert_hash(HashTable *ht, LinkInl ino, char *name);


/**
 * @brief Resizes the hash table by doubling its size.
 * 
 * Called when the load factor exceeds the defined threshold.
 *
 * @param ht Pointer to the hash table to resize.
 */
void resize_hash(HashTable *ht);


/**
 * @brief Frees all memory allocated for the hash table and users.
 * 
 * @param ht Pointer to the hash table to be freed.
 */
void free_user(HashTable *ht);


/**
 * @brief Finds a user in the hash table by name.
 *
 * @param ht Pointer to the hash table.
 * @param name Name of the user to search.
 * @param user Pointer to the result (NULL if not found).
 */
void find_hash(HashTable *ht, char *name, User **user);


/**
 * @brief Prints all inoculations associated with a user.
 * 
 * @param user Pointer to the user.
 */
void print_user(User *user);


/**
 * @brief Checks whether a user has already received two inoculations of the same vaccine on or before the given date.
 *
 * @param ht Pointer to the hash table containing all users.
 * @param present The current date to compare against the inoculation dates.
 * @param user_name The name of the user to check.
 * @param vaccine_name The name of the vaccine to check for.
 * @return int Returns 0 if the user already has two valid inoculations of the vaccine,
 *         otherwise returns 1.
 */
int comp_inoculation(HashTable *ht, Date present, char *user_name, char *vaccine_name);


/**
 * @brief Removes a specific user from the hash table.
 *
 * Frees memory and updates the list of users.
 *
 * @param ht Pointer to the hash table.
 * @param remove Pointer to the user to remove.
 */
void remove_user_ptr(HashTable *ht, User *remove);


/**
 * @brief Removes all inoculations of a user.
 * 
 * @param inolink Pointer to the inoculation manager (contains head/last).
 * @param ht Pointer to the hash table.
 * @param user Pointer to the user to remove.
 * @return Number of inoculations removed, or error code.
 */
int remove_user(Ino *inolink, HashTable *ht, User *user);


/**
 * @brief Removes inoculations of a user that match a specific date and optionally batch.
 * 
 * @param inolink Pointer to the inoculation manager.
 * @param ht Pointer to the hash table.
 * @param user Pointer to the user.
 * @param date Date of the inoculation to match.
 * @param batch (Optional) Batch name to match.
 * @param num Flag to determine removal mode (WITH_DATE or WITH_BATCH).
 * @param present Current date of the system.
 * @return Number of inoculations removed, or error code.
 */
int remove_by_date(Ino *inolink, HashTable *ht, User *user, char *date, char* batch, int num, Date present);


/**
 * @brief Main function to remove user inoculations based on the input command.
 * 
 * Decides the type of removal (by name, date or batch) and calls the appropriate helper.
 * 
 * @param inolink Pointer to the inoculation manager.
 * @param ht Pointer to the hash table.
 * @param present Current date of the system.
 * @param name Name of the user.
 * @param date (Optional) Date of the inoculation.
 * @param batch (Optional) Batch name of the inoculation.
 * @param check Removal mode indicator.
 * @return Number of removals or an error code.
 */
int remove_application(Ino *inolink, HashTable *ht, Date present, char *name, char *date, char *batch, int check);



#endif