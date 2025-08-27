/* iaed25 - ist1114018 - project */

/**
 * @file project.c
 * @brief Main entry point and command interpreter for the vaccination management system.
 *
 * Handles the input commands from the user and dispatches the appropriate
 * functionality such as adding vaccines, listing data, managing users, and changing dates.
 * 
 * This is the core file that drives the program execution.
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


/**
 * @brief Frees all dynamically allocated memory in the system.
 * 
 * @param sys Pointer to the system structure.
 */
void command_q(Sys *sys) {
    int i;
    for (i = 0; i < sys->entries; i++) {
        free_vaccine(sys->batch_list[i]);
    }
    free_list_ino(sys->inolink->head);
    free_user(sys->user);
    free(sys->inolink);
}


/**
 * @brief Adds a new vaccine batch to the system if valid.
 * 
 * @param buf Input buffer containing the command.
 * @param sys Pointer to the system structure.
 */
void command_c(char *buf, Sys *sys) {
    int error = START;
    Vaccine *batch = malloc(sizeof(Vaccine));
    if (sys->entries == MAX_BRATCH) {
        puts(TOO_MANY(sys->language));
        free_vaccine(batch);
        return;
    }

    read_vaccine(sys->batch_list, batch, sys->entries, &error, buf, sys->present); 

    if (error != 0) {
        switch (error) {
            case NUM_DUP_BATCH: puts(DUP_BATCH(sys->language));  break;
            case NUM_INV_BATCH: puts(INV_BATCH(sys->language)); break;
            case NUM_INV_NAME: puts(INV_NAME(sys->language)); break;
            case NUM_INV_DATE: puts(INV_DATE(sys->language)); break;
            case NUM_INV_QNT: puts(INV_QTY(sys->language)); break;
            default: break;
        }
        free(batch);
    }

    else {
    add_batch(sys->batch_list, batch, sys->entries);
    (sys->entries)++;
    printf("%s\n",batch->batch);
    }
}

/**
 * @brief Lists vaccine batches either for all vaccines or specific ones.
 * 
 * @param buf Input buffer containing the command.
 * @param sys Pointer to the system structure.
 */
void command_l(char *buf, Sys *sys) {  
    char *segment;
    Vaccine *list[MAX_BRATCH];      // elems that are going to be printed
    int count;                     // number of elems
    segment = strtok(buf, SPACE);
    segment = strtok(NULL, SPACE);

    if (segment != NULL) { 
        while (segment != NULL) {       /* read all the vaccine names that are on the input*/
            segment[strcspn(segment, "\n")] = '\0';
            count = search_vaccine(sys->batch_list, sys->entries, segment, list);
            if (count == 0) {
                printf("%s%s\n", segment, NO_VAC_FOUND(sys->language));
            }
            print_list(list, count);
            segment = strtok(NULL, SPACE);
        }
    }

    else  {
        print_list(sys->batch_list, sys->entries);
    }
}

/**
 * @brief Registers a new inoculation for a user.
 * 
 * @param buf Input buffer containing the command.
 * @param sys Pointer to the system structure.
 */
void command_a(char *buf, Sys *sys) {
    int i = START, k = START, size_list;
    Vaccine *vac_list[MAX_BRATCH];
    LinkInl new_inoculation = malloc(sizeof(struct inoculation));
    char *segment = strtok(buf, SPACE), vaccine_name[NAME_SIZE], *name = malloc(sizeof(char) * 200);
     
    segment = strtok(NULL, "\n");

    read_username(segment, &(name), &i);

    while(segment[i] == ' ') i++;   /* Goes to the next space */
    strcpy(vaccine_name, &segment[i]);  
    size_list = search_vaccine(sys->batch_list, sys->entries, vaccine_name, vac_list);

    while(k < size_list) {
        if (past_date(vac_list[k]->date, sys->present) > 0 && vac_list[k]->dose > 0) break;
        k++;        /* Gets the index of the oldest batch */
    }
    if (k == size_list) {   /* if k >= size_list means that it didnt found any vaccine */
        free(name); free_inoculation(new_inoculation);
        puts(NO_STOCK(sys->language));
        return;
    }
    new_inoculation->date = sys->present;
    new_inoculation->vaccine = vac_list[k];

    if (comp_inoculation(sys->user, sys->present, name, vaccine_name) != VALID) {
        free(name); free_inoculation(new_inoculation);
        puts(ALREADY(sys->language));
        return;
    }
    insert_hash(sys->user, new_inoculation, name);
    add_inoculation(sys->inolink, new_inoculation);
    printf("%s\n", new_inoculation->vaccine->batch);
}

/**
 * @brief Removes a batch or sets its dose to 0 if already used.
 * 
 * @param buf Input buffer containing the command.
 * @param sys Pointer to the system structure.
 */
void command_r(char *buf, Sys *sys) {
    int i;
    char batch[BATCH_SIZE];

    sscanf(buf, "r %20s", batch);

    for (i = 0; i < sys->entries; i++) {
        if (strcmp(batch, sys->batch_list[i]->batch) == 0) {
            if (sys->batch_list[i]->uses == 0){ 
                remove_batch(sys->batch_list, &sys->entries, i);
            }
            else {
                sys->batch_list[i]->dose = 0;
            }
            printf("%d\n", sys->batch_list[i]->uses);
            return;
        }
    }
   printf("%s%s\n", batch, NO_BATCH_FOUND(sys->language));
}


/**
 * @brief Removes user inoculations based on name, date and/or batch.
 * 
 * @param buf Input buffer containing the command.
 * @param sys Pointer to the system structure.
 */
void command_d(char *buf, Sys *sys) {
    int result, check, i = START;
    char *name, date[DATE_SIZE], batch[BATCH_SIZE], *segment;

    segment = strtok(buf, SPACE); 
    segment = strtok(NULL, "\n");

    name = malloc(sizeof(char) * 200);
    read_username(segment, &name, &i);
    if (segment[i - 1] != '\0') {   /* There is something after the l command*/
        check = sscanf(&segment[i], "%12s %20s", date, batch);
    } 
    else {          /* There is nothing after the l command */
        check = 0;
    }

    result = remove_application(sys->inolink, sys->user, sys->present, name, date, batch, check);

    switch (result){
        case NO_USER_NUM: printf("%s%s\n",name, NO_USER(sys->language)); break;
        case NUM_INV_DATE: puts(INV_DATE(sys->language)); break;
        case NUM_NO_BATCH: printf("%s%s\n", batch, NO_BATCH_FOUND(sys->language)); break;
        default: printf("%d\n",result); break;
    }
    free(name);
}


/**
 * @brief Prints inoculations for a specific user or all if no user is given.
 * 
 * @param buf Input buffer containing the command.
 * @param sys Pointer to the system structure.
 */
void command_u(char *buf, Sys *sys) {
    User *user;
    int div;
    char *name = buf + 2;


    if (*name == '\0') {        /* There is no name */
        print_inoculations(sys->inolink->last);
        return;
    }
    div = (*name == '"') ? 1 : 0;
    name = name + div;

    name[strcspn(name, "\n")] = '\0';
    if (div == 1) name[strcspn(name, "\"")] = '\0';

    find_hash(sys->user, name, &user);

    if (user == NULL) {
        printf("%s%s\n", name, NO_USER(sys->language));
        return;
    }

    print_user(user);
}


/**
 * @brief Updates the present date in the system.
 * 
 * @param buf Input buffer containing the command.
 * @param sys Pointer to the system structure.
 */
void command_t(char *buf, Sys *sys) {
    Date date;
    char *segment;
    segment = strtok(buf, SPACE);


    segment = strtok(NULL, SPACE);
    if (read_date(segment, &date, sys->present) != 0) {
        puts(INV_DATE(sys->language));
        return;
    }
    sys->present = date;
    print_date(sys->present);
    printf("\n");
}


/**
 * @brief Main function. Initializes the system and handles command dispatching.
 * 
 * @param arg1 Language flag.
 * @param arg2 Not used (placeholder for compatibility).
 * @return int Exit status.
 */
int main(int arg1, char **arg2) {
    Sys sys;
    char buf[MAXBUF];
    (void)arg2;

    start_sys(&sys, arg1);

    while (fgets(buf, MAXBUF, stdin)) {
        switch (buf[0]) {
            case 'q': command_q(&sys); return 0;
            case 'c': command_c(buf, &sys); break;
            case 'l': command_l(buf, &sys); break;
            case 'a': command_a(buf, &sys); break;
            case 'r': command_r(buf, &sys); break;
            case 'd': command_d(buf, &sys); break;
            case 'u': command_u(buf, &sys); break;
            case 't': command_t(buf, &sys); break;
            default: break;
        }
    }
    return 0;
}


