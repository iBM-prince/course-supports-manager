#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the Support structure
typedef struct Support {
    int id_support;
    char title[100];
    char module[100];
    char teacher[100];
    char type[50];
    char file_path[200];
    char date_added[20];
} Support;

// Define the Element structure for the linked list
typedef struct Element {
    Support data;
    struct Element *suivant;
} Element;

// Define the Liste structure
typedef struct Liste {
    Element *premier;
} Liste;

// Function declarations
Liste* initialisation();
void addBack(Liste *l, Support s);
Element* findById(Liste *l, int id);
int removeById(Liste *l, int id);
void removeFront(Liste *l);

#endif
