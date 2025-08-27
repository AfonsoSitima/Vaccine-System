/**
 * @file inoculation.h
 * @brief Header file for inoculation and vaccination list operations.
 *
 * Declares data structures and functions related to the inoculation linked list,
 * including addition, removal, and memory cleanup of records.
 * 
 * To be included by modules that manipulate vaccination applications.
 * 
 * @author Afonso Sítima - 114018
 */


#ifndef INOCULATION_H
#define INOCULATION_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "date.h"
#include "vaccine.h"

#define START   0         /**< Starting index or default value used for counters and initializations. */

/**
 * @brief Represents a single inoculation record for a user.
 */
typedef struct inoculation {
    char *name;               /**< Name of the user who received the inoculation. */
    Date date;                /**< Date when the inoculation occurred. */
    Vaccine *vaccine;         /**< Pointer to the vaccine used in the inoculation. */
    struct inoculation *prev; /**< Pointer to the previous inoculation in the linked list. */
    struct inoculation *next; /**< Pointer to the next inoculation in the linked list. */
} *LinkInl;


/**
 * @brief Stores the head and tail pointers of the linked list of inoculations.
 */
typedef struct {
    LinkInl head; /**< Pointer to the first inoculation in the list. */
    LinkInl last; /**< Pointer to the last inoculation in the list. */
} Ino;


/**
 * @brief Reads a username from the input buffer.
 * 
 * @param buf The input string containing the username.
 * @param name Pointer to a dynamically allocated string where the username will be stored.
 * @param i Pointer to the index used for parsing the buffer.
 */
void read_username(char *buf, char **name, int *i);


/**
 * @brief Adds an inoculation record to the linked list.
 * 
 * @param inolink Pointer to the inoculation list structure.
 * @param ino Pointer to the inoculation to be added.
 */
void add_inoculation(Ino *inolink, LinkInl ino);


/**
 * @brief Removes a specific inoculation from the linked list.
 * 
 * @param inolink Pointer to the inoculation list structure.
 * @param ino Pointer to the inoculation to be removed.
 */
void remove_inoculation(Ino *inolink, LinkInl ino);


/**
 * @brief Frees memory associated with a inoculation.
 * 
 * @param ino Pointer to the inoculation to be freed.
 */
void free_inoculation(LinkInl ino);


/**
 * @brief Frees all inoculations in the list starting from the head.
 * 
 * @param head Pointer to the first inoculation in the list.
 */
void free_list_ino(LinkInl head);


/**
 * @brief Prints all inoculations in reverse order starting from the last in the list.
 * 
 * @param last Pointer to the last inoculation in the list.
 */
void print_inoculations(LinkInl last);


/**
 * @brief Reorganizes the inoculation list array after an inoculation is removed.
 * 
 * @param ino_list Array of pointers to inoculation records.
 * @param ino The inoculation to be removed from the array.
 * @param count Total number of inoculations in the array.
 */
void reorganize_array(LinkInl *ino_list, LinkInl ino, int count);


#endif