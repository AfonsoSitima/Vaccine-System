/**
 * @file user.c
 * @brief Implementation of user-related operations and hash table management.
 *
 * Provides insertion, lookup, and removal functionalities for users and their
 * associated inoculation data using a hash table.
 * Also handles dynamic resizing and memory management for user lists.
 * 
 * @author Afonso Sítima - 114018
 */


#include "date.h"
#include "vaccine.h"
#include "inoculation.h"
#include "user.h"


int hash(char *name) {
    int i, length, hash_value = HASH;
    length = strlen(name);
    
    for  (i = 0; i < length; i++) {
        hash_value += name[i];
        hash_value = (hash_value * name[i]);
    }
    return abs(hash_value);
}




void insert_hash(HashTable *ht, LinkInl ino, char *name) {
    int index;
    User *user, *new_user;
    if (ht->count / ht->size > PERCENT) resize_hash(ht);
    index = hash(name) % ht->size;
    user = ht->user_list[index];
    while (user != NULL) {
        if (strcmp(user->name, name) == 0) break;
        user = user->next;
    }
    if (user == NULL) {             /* First time seeing this user (creats a space for it) */      
        new_user = malloc(sizeof(User));
        new_user->name = strdup(name);
        ino->name = new_user->name;
        new_user->ino_list = malloc(sizeof(LinkInl) * 40);
        new_user->count = 0;
        ht->count++;
        new_user->next = ht->user_list[index];  /* Add to the list */
        ht->user_list[index] = new_user;
        user = new_user;
    }
    else {                          /* If there's already a user */
        ino->name = user->name;     /* To make it easier to free and it takes less memory */
        }
    if (user->count != 0 && user->count % 40 == 0) {
        user->ino_list = realloc(user->ino_list, sizeof(LinkInl) * (user->count + 40));
    }
    free(name);
    user->ino_list[user->count++] = ino;
}




void resize_hash(HashTable *ht) {
    int i;
    int index;
    int new_size = ht->size * 2;
    User *user, *next_user;
    User **new_list = calloc(new_size, sizeof(User*)); /* New bigger list that will be filled with users from the smaller one */
        for (i = 0; i < ht->size; i++) {
        user = ht->user_list[i];
        while (user != NULL) {      /* Going throught the linked list of users */
            next_user = user->next;
            index = hash(user->name) % new_size;    
            user->next = new_list[index];   /* user turns into the head of new_list[index] (there can be other users already there) */
            new_list[index] = user;     
            user = next_user;   /* Continue to the next user of the list */
        }
    }
    free(ht->user_list);
    ht->user_list = new_list;
    ht->size = new_size;
}


void free_user(HashTable *ht) {
    int i;
    User *user, *next;
    for (i = 0; i < ht->size; i++) {
        user = ht->user_list[i];
        while (user != NULL) {      /* Going throught the list */
            next = user->next;
            free(user->name);
            free(user->ino_list);
            free(user);
            user = next;
        }
    }
    free(ht->user_list);
    free(ht);
}


void find_hash(HashTable *ht, char *name, User **user) {
    int index;
    User *find;
    index = hash(name) % ht->size;
    find = ht->user_list[index];
    while (find != NULL) {
        if (strcmp(find->name, name) == 0) { 
            *user = find;
            return;
        }
        find = find->next;
    }
    *user = NULL;
}


void print_user(User *user) {
    int i;
    for (i = 0; i < user->count; i++) {
        printf("%s %s ", user->name, user->ino_list[i]->vaccine->batch);
        print_date(user->ino_list[i]->date);
        printf("\n");
    }
}


int comp_inoculation(HashTable *ht, Date present, char *user_name, char *vaccine_name) {   
   int i;
   User *user;
   find_hash(ht, user_name, &user);
   if (user == NULL) return VALID;
   for (i = 0; i < user->count; i++) {
       if (strcmp(vaccine_name, user->ino_list[i]->vaccine->name) == VALID &&
               past_date(user->ino_list[i]->date, present) == VALID) {
               return INVALID;
       }
   }    
   return VALID;
}


int remove_application(Ino *inolink, HashTable *ht, Date present, char *name, char *date, char *batch, int check) {
    User *user;

    find_hash(ht, name, &user);
    switch (check){
        case ONLY_NAME:
            return remove_user(inolink, ht, user);  
            break;
        default:
            return remove_by_date(inolink, ht, user, date, batch, check, present);
            break;
    }
}


int remove_user(Ino *inolink, HashTable *ht, User *user) {
    int i, count = START;
    if (user == NULL) return NO_USER_NUM;
    for (i = 0; i < user->count; i++) {
        remove_inoculation(inolink, user->ino_list[i]);
        count+=1;
    }
    remove_user_ptr(ht, user);
    return count;
}


int remove_by_date(Ino *inolink, HashTable *ht, User *user, char *date, char* batch, int num, Date present) {
    int i, count = START;
    Date check_date;
    LinkInl keep;
    if (user == NULL) return NO_USER_NUM;
    
    read_date(date, &check_date, present);
    if (past_date(check_date, present) > 0) return NUM_INV_DATE;
    
    for (i = 0; i < user->count; i++) {
        if (past_date(check_date, user->ino_list[i]->date) == 0) {
            if (num != WITH_BATCH) {
                keep = user->ino_list[i];
                reorganize_array(user->ino_list, keep, user->count);
                remove_inoculation(inolink, keep);
                user->count--; count += 1; i--;
            }
            else {
                if (strcmp(batch, user->ino_list[i]->vaccine->batch) == 0) {
                    keep = user->ino_list[i];
                    reorganize_array(user->ino_list, keep, user->count);
                    remove_inoculation(inolink, keep);
                    user->count--; count += 1; i--;
                }
            }
        } 
    }
    if (num == WITH_BATCH && count == 0) return NUM_NO_BATCH;
    if (user->count <= 0) remove_user_ptr(ht, user);    /* If there's no inoculation, removes the user */
    return count;
}


void remove_user_ptr(HashTable *ht, User *remove) {
    int index = hash(remove->name) % ht->size;
    User *prev = NULL;
    User *curr = ht->user_list[index];
    while (curr != NULL) {
        if (curr == remove) {
            if (prev == NULL) {
                ht->user_list[index] = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr->name);
            free(curr->ino_list);
            free(curr);
            ht->count--;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}
