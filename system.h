/**
 * @file system.h
 * @brief Head file for the registry system.
 *
 * Defines the main system structure `Sys`, which contains the current date,
 * list of vaccine batches, linked list of inoculations, hash table of users,
 * and language preferences. This module acts as a central point for managing
 * all application-level data.
 *
 * Also defines useful constants related to system configuration.
 *
 * @author Afonso Sítima - 114018
 */


#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "date.h"
#include "vaccine.h"
#include "inoculation.h"
#include "user.h"

#define START   0         /**< Starting index or default value used for counters and initializations. */
#define MAXBUF  65536     /**< Maximum buffer size for reading input data. */

#define TOO_MANY(A)         ((A == ENG) ? "too many vaccines" : "demasiadas vacinas") /**< Error message: batch limit exceeded */
#define DUP_BATCH(A)        ((A == ENG) ? "duplicate batch number" : "número de lote duplicado") /**< Error: duplicate batch */
#define INV_BATCH(A)        ((A == ENG) ? "invalid batch" : "lote inválido") /**< Error: invalid batch */
#define INV_NAME(A)         ((A == ENG) ? "invalid name" : "nome inválido") /**< Error: invalid vaccine name */
#define INV_DATE(A)         ((A == ENG) ? "invalid date" : "data inválida") /**< Error: invalid date */
#define INV_QTY(A)          ((A == ENG) ? "invalid quantity" : "quantidade inválida") /**< Error: invalid dose quantity */
#define NO_VAC_FOUND(A)     ((A == ENG) ? ": no such vaccine" : ": vacina inexistente") /**< Error: vaccine not found */
#define NO_STOCK(A)         ((A == ENG) ? "no stock" : "esgotado") /**< Error: no available doses */
#define NO_BATCH_FOUND(A)   ((A == ENG) ? ": no such batch" : ": lote inexistente") /**< Error: batch not found */
#define ALREADY(A)          ((A == ENG) ? "already vaccinated" : "já vacinado") /**< Error: vaccine already applied */

/**
 * @struct Sys
 * @brief Main structure that holds the global state of the vaccination system.
 */
typedef struct system {
    Date present;                          /**< Current system date. */
    int entries;                           /**< Number of vaccine batches currently registered. */
    Vaccine *batch_list[MAX_BRATCH];       /**< Array of pointers to vaccine batch records. */
    Ino *inolink;                          /**< Pointer to structure managing the linked list of inoculations. */
    HashTable *user;                       /**< Pointer to hash table storing user records and their inoculations. */
    int language;                          /**< Language setting (e.g., 0 for PT, 1 for ENG). */
} Sys;


/**
 * @brief Initializes the system structure with default values.
 *
 * Allocates memory for the inoculation linked list manager (`inolink`) and user hash table.
 * Sets the initial date to 1 January 2025 and the number of vaccine entries to 0.
 * Also sets the language of the messages depending on the program argument.
 *
 * @param sys Pointer to the Sys structure to initialize.
 * @param arg1 Command-line argument used to determine language (ENG if > 1).
 */
void start_sys(Sys *sys, int arg1);


/**
 * @brief Exchanges the values of two integers.
 *
 * @param A Pointer to the first integer.
 * @param B Pointer to the second integer.
 */
void exch(int *A, int *B);



#endif