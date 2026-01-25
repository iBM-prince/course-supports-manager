#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#ifndef LINKED_LIST_H 
#define LINKED_LIST_H 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* =========================
   Struct Support
   ========================= */
typedef struct {
    int id_support;
    char titre[100];
    char module[100];
    char type[50];
    char enseignant[100];
    char chemin_fichier[200];
    char date_ajout[20];
} Support;

/* =========================
   Element de la liste chaînée
   ========================= */
typedef struct Element {
    Support data;
    struct Element *suivant;
} Element;

/* =========================
   Liste chaînée
   ========================= */
typedef struct {
    Element *premier;
} Liste;

/* =========================
   Fonctions liste chaînée
   ========================= */
Liste* initialisation();
void addBack(Liste *l, Support s);
Element* findById(Liste *l, int id);
int removeById(Liste *l, int id);

#endif
