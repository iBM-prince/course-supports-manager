#include "linked_list.h"

// Initialize the list
Liste* initialisation() {
    Liste *l = (Liste*)malloc(sizeof(Liste));
    if (l == NULL) {
        return NULL;
    }
    l->premier = NULL;
    return l;
}

// Add an element to the back of the list
void addBack(Liste *l, Support s) {
    Element *nouveau = (Element*)malloc(sizeof(Element));
    if (nouveau == NULL) {
        return;
    }
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

// Find an element by ID
Element* findById(Liste *l, int id) {
    Element *temp = l->premier;
    while (temp != NULL) {
        if (temp->data.id_support == id) {
            return temp;
        }
        temp = temp->suivant;
    }
    return NULL;
}

// Remove an element by ID
int removeById(Liste *l, int id) {
    if (l->premier == NULL) {
        return 0;
    }

    if (l->premier->data.id_support == id) {
        removeFront(l);
        return 1;
    }

    Element *temp = l->premier;
    while (temp->suivant != NULL) {
        if (temp->suivant->data.id_support == id) {
            Element *toDelete = temp->suivant;
            temp->suivant = toDelete->suivant;
            free(toDelete);
            return 1;
        }
        temp = temp->suivant;
    }
    return 0;
}

// Remove the front element
void removeFront(Liste *l) {
    if (l->premier != NULL) {
        Element *temp = l->premier;
        l->premier = l->premier->suivant;
        free(temp);
    }
}
