/**
 * @file vaccine.h
 * @brief Header file for vaccine batch operations.
 *
 * Declares functions, constants, and data structures for managing vaccines,
 * including creation, validation, and batch sorting logic.
 * 
 * To be included in files that use vaccine-related functionalities.
 * 
 * @author Afonso Sítima - 114018
 */


#ifndef VACCINE_H
#define VACCINE_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "date.h"

#define BATCH_SIZE  21      /**< Maximum size of a batch code string including \0 */
#define NAME_SIZE   51      /**< Maximum size of a vaccine name string including \0 */
#define MAX_BRATCH  1000    /**< Maximum number of vaccine batches in the system */
#define SPACE       " "     /**< Delimiter used for tokenizing input strings */
#define ASCII_BATCH 70      /**< ASCII code for the char F */
#define ENG         1       /**< Language flag: English */
#define VALID       0       /**< Valid statment */
#define INVALID     1       /**< Inalid statment */  

#define menor(a,b) ((a < b && a != 0) ? a : b) /**< Returns the smaller of two values, ignoring zero */

/* Vaccine Errors */
#define NUM_DUP_BATCH   1   /**< Error code: duplicate batch */
#define NUM_INV_BATCH   2   /**< Error code: invalid batch */
#define NUM_INV_NAME    3   /**< Error code: invalid vaccine name */
#define NUM_INV_DATE    4   /**< Error code: invalid date */
#define NUM_INV_QNT     5   /**< Error code: invalid dose quantity */
#define ERROR_LIST_SIZE 6   /**< Total number of defined errors for vaccines */
#define NUM_NO_BATCH   -2   /**< Error code: batch not found */

#define VALID_CHAR  "áéíóúãõâêîôûàèìòùçÁÉÍÓÚÃÕÂÊÎÔÛÀÈÌÒÙÇ" /**< Accepted special characters in vaccine names */

 


/**
 * @brief Structure representing a vaccine batch.
 */
typedef struct vaccine {
    char *name;      /**< Vaccine name (up to 50 characters) */
    char *batch;     /**< Unique batch code (up to 20 characters) */
    Date date;       /**< Expiration date of the batch */
    int dose;        /**< Number of doses available */
    int uses;        /**< Number of doses already used */
} Vaccine;




/**
 * @brief Performs a binary search to find the correct insertion index for a vaccine.
 *
 * This function searches for the correct position to insert a new vaccine into an already 
 * sorted array of vaccines. Vaccines are sorted by date (ascending), and if the dates are 
 * equal, by batch code (ascending order).
 *
 * @param batch_list The array of existing vaccine pointers.
 * @param entries The current number of entries in the batch_list.
 * @param target Pointer to the vaccine to insert.
 * @return The index at which the vaccine should be inserted.
 */
int binary_search(Vaccine *batch_list[], int entries, Vaccine *target);

/**
 * @brief Checks if a batch code is already present in the batch list.
 * 
 * @param batch_list Array of pointers to vaccine batches.
 * @param batch Batch code to check.
 * @param num_batch Number of batches currently in the list.
 * @return 1 if duplicate exists, 0 otherwise.
 */
int check_dup_batch(Vaccine *batch_list[], char *batch, int num_batch);


/**
 * @brief Validates the format of a batch code.
 * 
 * @param batch The batch code string to validate.
 * @return 0 if valid, error code otherwise.
 */
int check_inv_batch(char *batch);


/**
 * @brief Checks whether a character is valid in a vaccine name.
 * 
 * @param c Character to validate.
 * @return 0 if valid, error code otherwise.
 */
int is_valid_char(char c);


/**
 * @brief Validates the format of a vaccine name.
 * 
 * @param name The vaccine name to check.
 * @return 0 if valid, error code otherwise.
 */
int check_inv_name(char *name);


/**
 * @brief Validates the quantity of doses.
 * 
 * @param segment String containing the quantity.
 * @param dose Pointer to store the parsed dose count.
 * @return 0 if valid, error code otherwise.
 */
int check_inv_qnt(char *segment, int *dose);


/**
 * @brief Reads and validates vaccine information from a command buffer.
 * 
 * If all validations pass, populates the new vaccine structure.
 * 
 * @param batch_list Array of existing vaccine batches.
 * @param new_vaccine Pointer to the vaccine to be filled.
 * @param num Number of existing batches.
 * @param error Pointer to store error code.
 * @param buf Input buffer containing the vaccine info.
 * @param present Current system date.
 */
void read_vaccine(Vaccine *batch_list[], Vaccine *new_vaccine, int num, int *error, char *buf, Date present);


/**
 * @brief Inserts a vaccine into the batch list in the correct sorted position.
 *
 * @param batch_list The array of vaccine pointers.
 * @param batch Pointer to the new vaccine to insert.
 * @param entries The current number of entries in the batch_list.
 */
void add_batch(Vaccine *batch_list[], Vaccine *batch, int entries); 


/**
 * @brief Searches for all vaccine batches with a specific vaccine name.
 *
 * @param batch_list The array of existing vaccine pointers.
 * @param entries The current number of entries in the batch_list.
 * @param name The name of the vaccine to search for.
 * @param search_list The output array to store matching vaccines.
 * @return The number of vaccines found that match the given name.
 */
int search_vaccine(Vaccine *batch_list[], int entries, char *name, Vaccine *search_list[]);


/**
 * @brief Frees all memory allocated to a vaccine batch.
 * 
 * @param vaccine Pointer to the vaccine to free.
 */
void free_vaccine(Vaccine *vaccine);


/**
 * @brief Prints a list of vaccines in a specified order.
 *
 * @param batch_list Array containing vaccine batches.
 * @param size Number of elements to print from the list.
 */
void print_list(Vaccine *batch_list[], int size);


/**
 * @brief Removes a vaccine batch from the list.
 *
 * The batch is removed by shifting all elements after the given position.
 * This function also frees the memory of the removed batch.
 *
 * @param batch_list Array containing all vaccine batches.
 * @param entries Pointer to the total number of entries (will be decremented).
 * @param pos Index of the batch to remove.
 */
void remove_batch(Vaccine *batch_list[], int *entries, int pos);


/**
 * @brief Compares two vaccine batches by expiration date.
 * 
 * Used in sorting algorithms.
 * 
 * @param vaccine1 First vaccine to compare.
 * @param vaccine2 Second vaccine to compare.
 * @return Negative if vaccine1 < vaccine2, positive if >, 0 if equal.
 */
int comp(Vaccine *vaccine1, Vaccine *vaccine2);


#endif