#include "linked_list.h"

// Initialize the list
Liste* initialisation() {
    Liste *l = (Liste*)malloc(sizeof(Liste));
    if (l == NULL) {
/* List initialization */
Liste* initialisation() {
    Liste *l = (Liste*)malloc(sizeof(Liste));
    if(l == NULL) {
        printf("Memory allocation error\n");
        return NULL;
    }
    l->premier = NULL;
    return l;
}

// Add an element to the back of the list
void addBack(Liste *l, Support s) {
    Element *nouveau = (Element*)malloc(sizeof(Element));
    if (nouveau == NULL) {
/* Add at front */
void addFront(Liste *l, Support s) {
    Element *nouveau = (Element*)malloc(sizeof(Element));
    if(nouveau == NULL) {
        printf("Memory allocation error\n");
        return;
    }
    nouveau->data = s;
    nouveau->suivant = l->premier;
    l->premier = nouveau;
}

/* Add at end */
void addBack(Liste *l, Support s) {
    Element *nouveau = (Element*)malloc(sizeof(Element));
    if(nouveau == NULL) {
        printf("Memory allocation error\n");
        return;
    }
    nouveau->data = s;
    nouveau->suivant = NULL;

    if (l->premier == NULL) {
        l->premier = nouveau;
    } else {
        Element *temp = l->premier;
        while (temp->suivant != NULL) {
    if(l->premier == NULL) {
        l->premier = nouveau;
    } else {
        Element *temp = l->premier;
        while(temp->suivant != NULL) {
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
/* Remove from front */
void removeFront(Liste *l) {
    if(l->premier == NULL) {
        printf("The list is empty\n");
        return;
    }
    Element *toDelete = l->premier;
    l->premier = l->premier->suivant;
    free(toDelete);
}

/* Remove from end */
void removeBack(Liste *l) {
    if(l->premier == NULL) {
        printf("The list is empty\n");
        return;
    }
    if(l->premier->suivant == NULL) {
        free(l->premier);
        l->premier = NULL;
        return;
    }
    Element *temp = l->premier;
    while(temp->suivant->suivant != NULL) {
        temp = temp->suivant;
    }
    free(temp->suivant);
    temp->suivant = NULL;
}

/* Remove by ID */
int removeById(Liste *l, int id) {
    if(l->premier == NULL) return 0;

    Element *temp = l->premier;
    Element *prec = NULL;

    while(temp != NULL && temp->data.id_support != id) {
        prec = temp;
        temp = temp->suivant;
    }

    if(temp == NULL) {
        return 0;
    }

    if(prec == NULL) {
        l->premier = temp->suivant;
    } else {
        prec->suivant = temp->suivant;
    }
    free(temp);
    return 1;
}

/* Search by ID */
Element* findById(Liste *l, int id) {
    Element *temp = l->premier;
    while(temp != NULL) {
        if(temp->data.id_support == id)
            return temp;
        temp = temp->suivant;
    }
    return NULL;
}

/* Search by keyword */
Element* findByKeyword(Liste *l, const char* mot) {
    Element *temp = l->premier;
    while(temp != NULL) {
        if(strstr(temp->data.titre, mot) ||
           strstr(temp->data.module, mot) ||
           strstr(temp->data.type, mot) ||
           strstr(temp->data.enseignant, mot)) {
            return temp;
        }
        temp = temp->suivant;
    }
    return NULL;
}
