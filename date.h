/**
 * @file date.h
 * @brief Header file for date-related operations and constants.
 *
 * Declares the `Date` structure and functions for checking, comparing,
 * printing, and reading date inputs.
 * 
 * Used by all modules that require date validation or manipulation.
 * 
 * @author Afonso Sítima - 114018
 */


#ifndef DATE_H
#define DATE_H

#include <stdio.h>


#define DATE_SIZE      13     /**< Maximum size of a date string in format "dd-mm-yyyy", including null terminator. */
#define DAYS_IN_MONTH  {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}  /**< Days in each month for a non-leap year. */
#define FIRST_DAY      1      /**< First default day used for system initialization. */
#define FIRST_MONTH    1      /**< First valid month in a year (January). */
#define LAST_MONTH     12     /**< Last valid month in a year (December). */
#define FIRST_YEAR     2025   /**< Default starting year for the system. */

#define Zero(A)        (A < 10 ? "0" : "")  /**< Adds leading zero to single-digit numbers for date. */



/**
 * @brief Structure representing a date.
 */
typedef struct date {
    int day;    /**< Day of the month (1–31). */
    int month;  /**< Month of the year (1–12). */
    int year;   /**< Year (2025- ). */
} Date;


/**
 * @brief Validates whether a given date is valid.
 *
 * @param date The date to validate.
 * @param presente The reference date to compare against (typically the current system date).
 * @return int Returns 0 if the date is valid, or a non-zero error code if invalid.
 */
int check_inv_date(Date date, Date presente);


/**
 * @brief Reads a date string in the format "dd-mm-yyyy" and fills a Date structure.
 *
 * Also validates the date against the current system date.
 *
 * @param data The input string containing the date.
 * @param new_date Pointer to the Date structure to populate.
 * @param present The current system date to validate against.
 * @return int Returns 0 if the date was successfully read and is valid, otherwise returns an error code.
 */
int read_date(char *data, Date *new_date, Date present);


/**
 * @brief Prints a date in the format "dd-mm-yyyy".
 *
 * @param date The date to print.
 */
void print_date(Date date);


/**
 * @brief Checks if a given date has valid day, month, and year values.
 *
 * This does not check whether the date is in the future or past compared to the present date.
 *
 * @param date The date to check.
 * @return int Returns 0 if valid, 1 if invalid.
 */
int is_date(Date date);


/**
 * @brief Compares two dates to determine chronological order.
 *
 * @param date1 The first date to compare.
 * @param date2 The second date to compare.
 * @return int Returns Date1 - Date2
 */
int past_date(Date date1, Date date2);


/**
 * @brief Advances the current system date by one day.
 *
 * Handles month and year transitions, including leap year logic if implemented.
 *
 * @param presente Pointer to the current system date to update.
 */
void change_date(Date *presente);


#endif
