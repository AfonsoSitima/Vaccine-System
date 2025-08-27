/**
 * @file inoculation.c
 * @brief Implements inoculation management and linked list operations.
 *
 * Contains functions for adding, removing, printing, and freeing inoculations.
 * Also manages user vaccination history and the double-linked list structure.
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



void read_username(char *segment, char **name, int *i) {
    int div, j = START;

    div = (segment[*i] == '"') ? '"' : ' '; 
    *i = (segment[*i] == '"') ? *i + 1 : *i;

    while(segment[*i] != div) {
        (*name)[j++] = segment[(*i)++];
        if (*i % 200 == 0) {
            *name = realloc(*name, sizeof(char) * (*i * 2));
        }
    }
    *name = realloc(*name, sizeof(char) * (*i + 1)),
    (*name)[j] = '\0';
    if (div == '"')(*i)++;
}


void add_inoculation(Ino *inolink, LinkInl ino) {
    ino->vaccine->dose--;
    ino->vaccine->uses++;

    ino->next = inolink->head;
    ino->prev = NULL;
    if (inolink->head != NULL) {
        (inolink->head)->prev = ino;
    }
    if (inolink->head == NULL) inolink->last = ino;

    inolink->head = ino;
}

void remove_inoculation(Ino *inolink, LinkInl ino) {
    if (ino->prev == NULL && ino->next == NULL) {  /*Only one ino*/
        inolink->head = NULL;
        inolink->last = NULL;
    }
    else if (ino->prev == NULL) {       /*ino is head*/
        inolink->head = ino->next;
        inolink->head->prev = NULL;
    }
    else if (ino->next == NULL) {       /*ino is last*/
        inolink->last = ino->prev;
        inolink->last->next = NULL;
    }
    else {                              /*ino is in the middle*/
        ino->prev->next = ino->next;
        ino->next->prev = ino->prev;
    }
    free_inoculation(ino);
}

void free_inoculation(LinkInl ino) {
    free(ino);    /* Removes the user name after (in free_user) */
}

void free_list_ino(LinkInl head) {
    LinkInl t;
    while (head != NULL) {
        t = head->next;  
        free_inoculation(head);
        head = t;      
    }
}

void print_inoculations(LinkInl last) {
    LinkInl i;
    for (i = last; i != NULL; i = i->prev) {
        printf("%s %s ", i->name, i->vaccine->batch);
        print_date(i->date);
        printf("\n");
    }
}

void reorganize_array(LinkInl *ino_list, LinkInl ino, int count) {
    int i = 0;

    while (i < count && ino_list[i] != ino) {
        i++;
    }
    while (i < count - 1) {
        ino_list[i] = ino_list[i + 1];
        i++;
    }
    ino_list[count - 1] = NULL;
}