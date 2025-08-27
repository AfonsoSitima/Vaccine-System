/**
 * @file vaccine.c
 * @brief Implements operations for managing vaccine batches.
 *
 * Includes validation, parsing, sorting, and insertion of vaccine batches.
 * Also provides support functions for printing and removing batches.
 * 
 * This module is responsible for maintaining the batch list in correct order
 * and checking batch integrity.
 * 
 * @author Afonso Sítima - 114018
 */


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "date.h"
#include "vaccine.h"
#include "system.h"




void read_vaccine(Vaccine *batch_list[], Vaccine *new_vaccine, int num, int *error, char *buf, Date present) {
    char *segment = strtok(buf, SPACE); //Gives first chat 
    int dose;
    Date date;

    segment = strtok(NULL, SPACE); //batch

    if (check_dup_batch(batch_list, segment, num) != VALID) {
        *error = NUM_DUP_BATCH; return;
    }
    if (check_inv_batch(segment) != VALID) {
        *error = NUM_INV_BATCH; return;
    }

    new_vaccine->batch = (char*) malloc((strlen(segment) + 1) * sizeof(char));
    strcpy(new_vaccine->batch, segment); 

    segment = strtok(NULL, SPACE); //date
    
    if (read_date(segment, &date, present) != VALID) {
        free(new_vaccine->batch); *error = NUM_INV_DATE; return;
    }
    new_vaccine->date = date;  
    
    segment = strtok(NULL, SPACE); //dose
    if (check_inv_qnt(segment, &dose) != VALID) {
        free(new_vaccine->batch); *error = NUM_INV_QNT; return;
    }

    new_vaccine->dose = dose;
    segment = strtok(NULL, SPACE); //name
    if (check_inv_name(segment) != VALID) {
        free(new_vaccine->batch); *error = NUM_INV_NAME; return;
    }

    segment[strcspn(segment, "\n")] = '\0';
    new_vaccine->name = strdup(segment);
}


int binary_search(Vaccine *batch_list[], int entries, Vaccine *target) {
    int mid, left = START, right = entries;
    int date_cmp;
    while (left < right) {
        mid = (left + right) / 2;
        date_cmp = past_date(target->date, batch_list[mid]->date);
        if (date_cmp < 0) right = mid;
        else if (date_cmp > 0) left = mid + 1;
        else {
            if (strcmp(target->batch, batch_list[mid]->batch) < 0) {
                right = mid;
            } 
            else {
                left = mid + 1;
            }
            
        }
    }
    return left;
}


void add_batch(Vaccine *batch_list[], Vaccine* batch, int entries) {
    int i, index = binary_search(batch_list, entries, batch);
    
    for (i = entries; i > index; i--)
        batch_list[i] = batch_list[i - 1];

    batch->uses = START;
    batch_list[index] = batch;

} 


int search_vaccine(Vaccine *batch_list[], int entries, char *name, Vaccine *search_list[]) {
    int i, count = START;
    for (i = 0; i < entries; i++) {
        if (strcmp(batch_list[i]->name, name) == 0) {
            search_list[count] = batch_list[i];
            count++;
        }
    }
    return count;
}


int check_dup_batch(Vaccine *batch_list[], char *batch, int num_batch) {
    int i;
    if (batch == NULL) return NUM_INV_BATCH;

    for (i = 0; i < num_batch; i++) {
        if (strcmp(batch_list[i]->batch, batch) == 0) 
            return NUM_DUP_BATCH;
    }
    return VALID;
}


int check_inv_batch(char *batch) {
    int i, size;

    if ((size = strlen(batch)) > BATCH_SIZE - 1)
        return NUM_INV_BATCH;
    for (i = 0; i < size; i++) {
        if (batch[i] > ASCII_BATCH)
            return NUM_INV_BATCH;
    }
    return VALID;
}


int is_valid_char(char c) {
    int i;
    static const char acentos[] = VALID_CHAR;

    if (isalpha(c) || c == '-' || c == '_' || !isspace(c))
        return NUM_INV_NAME;
    
    for (i = 0; acentos[i] != '\0'; i++) {
        if (c == acentos[i])
            return VALID;
    }
    return NUM_INV_NAME;
}


int check_inv_name(char *name) {
    int i, size; 

    if (name == NULL) return NUM_INV_NAME;

    if ((size = strlen(name)) > NAME_SIZE - 1) 
        return NUM_INV_NAME;
    for (i = 0; i < size; i++) { 
        if (is_valid_char(name[i]) == 0) {
            return NUM_INV_NAME;  
            }
    }
    return VALID; 
} 


int check_inv_qnt(char *segment, int *dose) {
    if (segment == NULL) return NUM_INV_QNT;
    if (sscanf(segment, "%d", dose) != 1) return NUM_INV_QNT;
    if (*dose <= VALID)
        return NUM_INV_QNT;
    return VALID;
}


void free_vaccine(Vaccine *vaccine) {
    free(vaccine->name);
    free(vaccine->batch);
    free(vaccine);
}


void print_list(Vaccine *batch_list[], int size) {
    int i;
    for (i = 0; i < size; i++) {
        if(batch_list[i] == NULL) return;
        printf("%s %s ", batch_list[i]->name, batch_list[i]->batch);
        print_date(batch_list[i]->date);
        printf(" %d %d\n", batch_list[i]->dose, batch_list[i]->uses);
    }
}


void remove_batch(Vaccine *batch_list[], int *entries, int pos) {
    int i;
    if (pos == *entries - 1) {      /* Last vaccine on the array */
        free_vaccine(batch_list[pos]);
        (*entries)--;
        return;
    }
    free_vaccine(batch_list[pos]);

    for (i = pos; i < *entries - 1; i++) {
        batch_list[i] = batch_list[i + 1];
    }

    batch_list[*entries - 1] = NULL;
    (*entries)--;
}


int comp(Vaccine *vaccine1, Vaccine *vaccine2) {
    if (past_date(vaccine1->date, vaccine2->date) == VALID) {
        return strcmp(vaccine1->batch, vaccine2->batch);
    }
    return past_date(vaccine1->date, vaccine2->date);
}
