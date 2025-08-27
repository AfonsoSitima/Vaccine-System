/**
 * @file system.c
 * @brief Implementation of system-level functionality for vaccination registry.
 *
 * Provides core system functions such as initialization, element swapping,
 * and overall setup of the main application structure. Supports handling
 * of input commands, buffer size limits, and system state during execution.
 *
 * This module interacts with all major components of the program:
 * vaccines, inoculations, users, and dates.
 *
 * @author Afonso Sítima - 114018
 */


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


#include "date.h"
#include "vaccine.h"
#include "inoculation.h"
#include "user.h"
#include "system.h"



void start_sys(Sys *sys, int arg1) {
    sys->language = arg1;

    sys->inolink = malloc(sizeof(Ino));
    sys->inolink->head = NULL;
    sys->inolink->last = NULL;



    sys->user = malloc(sizeof(HashTable));
    sys->user->count = START;
    sys->user->size = NUM_USERS;
    sys->user->user_list = calloc(NUM_USERS, sizeof(User*));

    sys->entries = START;


    sys->present.day = FIRST_DAY;
    sys->present.month = FIRST_MONTH;
    sys->present.year = FIRST_YEAR;
}


void exch(int *A, int *B) {
    int temp = *A;
    *A = *B;
    *B = temp;
}













