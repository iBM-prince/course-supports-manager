#include "linked_list.h"

/* =========================
   Initialization
   ========================= */
Liste* initialisation() {
    Liste *l = malloc(sizeof(Liste));
    if (l == NULL) {
        printf("Erreur d'allocation de la liste\n");
        return NULL;
    }
    l->premier = NULL;
    return l;
}

/* =========================
   add elements
   ========================= */
void addFront(Liste *l, Support s) {
    Element *nouveau = malloc(sizeof(Element));
    if (nouveau == NULL) return;
    
    nouveau->data = s;
    nouveau->suivant = l->premier;
    l->premier = nouveau;
}

void addBack(Liste *l, Support s) {
    Element *nouveau = malloc(sizeof(Element));
    if (nouveau == NULL) return;
    
    nouveau->data = s;
    nouveau->suivant = NULL;

    if (l->premier == NULL) {
        l->premier = nouveau;
    } else {
        Element *temp = l->premier;
        while (temp->suivant != NULL) {
            temp = temp->suivant;
        }
        temp->suivant = nouveau;
    }
}

/* =========================
   search by ID
   ========================= */
Element* findById(Liste *l, int id) {
    Element *temp = l->premier;
    while (temp != NULL) {
        if (temp->data.id_support == id) return temp;
        temp = temp->suivant;
    }
    return NULL;
}

/* =========================
    remove elements
   ========================= */
void removeFront(Liste *l) {
    if (l->premier == NULL) return;
    
    Element *toDelete = l->premier;
    l->premier = l->premier->suivant;
    free(toDelete);
}

void removeBack(Liste *l) {
    if (l->premier == NULL) return;

    if (l->premier->suivant == NULL) {
        free(l->premier);
        l->premier = NULL;
        return;
    }

    Element *temp = l->premier;
    while (temp->suivant->suivant != NULL) {
        temp = temp->suivant;
    }
    free(temp->suivant);
    temp->suivant = NULL;
}

int removeById(Liste *l, int id) {
    if (l == NULL || l->premier == NULL) return 0;

    Element *temp = l->premier;
    Element *prev = NULL;

    while (temp != NULL) {
        if (temp->data.id_support == id) {
            if (prev == NULL) {
                // L'élément est au début
                l->premier = temp->suivant;
            } else {
                // L'élément est au milieu ou à la fin
                prev->suivant = temp->suivant;
            }
            free(temp);
            return 1;
        }
        prev = temp;
        temp = temp->suivant;
    }
    return 0;
}