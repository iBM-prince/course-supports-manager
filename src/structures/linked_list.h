#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#ifndef LINKED_LIST_H 
#define LINKED_LIST_H 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Structure for a support */
typedef struct Support {
    int id_support;
    char titre[100];
    char module[100];
    char type[50];
    char enseignant[100];
    char chemin_fichier[200];
    char date_ajout[20];
} Support;

/* Linked list element */
typedef struct Element {
    Support data;
    struct Element *suivant;
} Element;

/* Control structure */
typedef struct Liste {
    Element *premier;
} Liste;

/* Prototypes of the functions */

Liste* initialisation();

void addFront(Liste *l, Support s);
void addBack(Liste *l, Support s);

void removeFront(Liste *l);
void removeBack(Liste *l);
int removeById(Liste *l, int id);

void printList(Liste *l);

Element* findById(Liste *l, int id);
Element* findByKeyword(Liste *l, const char* mot);

#endif

