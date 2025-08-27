/**
 * @file date.c
 * @brief Implements operations for managing and validating date structures.
 *
 * Includes functions for date comparison, validation, parsing from strings,
 * and formatted output.
 * 
 * @author Afonso Sítima - 114018
 */


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "date.h"
#include "system.h"


int check_inv_date(Date date, Date presente) {
    if (past_date(date, presente) < VALID || is_date(date) != VALID || date.year < FIRST_YEAR)
        return NUM_INV_DATE;
    
    return VALID;
}


int read_date(char *date, Date *new_date, Date presente) {
    if (date == NULL) return NUM_INV_DATE;
    
    if(sscanf(date, "%d-%d-%d", &new_date->day, &new_date->month, &new_date->year) != 3) {
        return NUM_INV_DATE; 
    }
    
    if (check_inv_date(*new_date, presente) != VALID) return NUM_INV_DATE;
    return VALID;
}


void print_date(Date date) {
    printf("%s%d-%s%d-%d", Zero(date.day), date.day, Zero(date.month), date.month, date.year);
}


int is_date(Date date) {
    int days_in_month[12] = DAYS_IN_MONTH;
    if (FIRST_DAY <= date.day && date.day <= days_in_month[date.month - 1] &&
    FIRST_MONTH <= date.month && date.month <= LAST_MONTH &&
    FIRST_YEAR <= date.year) 
        return VALID;
    return INVALID;
}


int past_date(Date date1, Date date2) {
    if (date1.year != date2.year)
        return date1.year - date2.year;
    if (date1.month != date2.month)
        return date1.month - date2.month;
    return date1.day - date2.day;
}


void change_date(Date *presente) {
    int days_in_month[12] = DAYS_IN_MONTH;
    
    if (presente->day < days_in_month[presente->month - 1]) {
        presente->day++;
    }
    
    else { 
        presente->day = FIRST_DAY;
        
        if (presente->month == LAST_MONTH) {
            presente->month = FIRST_DAY;
            presente->year++;
        }
        
        else {
            presente->month ++;
        }
    }
}



