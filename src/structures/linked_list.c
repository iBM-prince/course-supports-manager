#include "linked_list.h"

Liste* initialisation() {
    Liste *l = malloc(sizeof(Liste));
    l->premier = NULL;
    return l;
}

void addBack(Liste *l, Support s) {
    Element *e = malloc(sizeof(Element));
    e->data = s;
    e->suivant = NULL;

    if (!l->premier) {
        l->premier = e;
    } else {
        Element *temp = l->premier;
        while (temp->suivant) temp = temp->suivant;
        temp->suivant = e;
    }
}

Element* findById(Liste *l, int id) {
    Element *temp = l->premier;
    while (temp) {
        if (temp->data.id_support == id) return temp;
        temp = temp->suivant;
    }
    return NULL;
}

int removeById(Liste *l, int id) {
    if (!l->premier) return 0;
    Element *temp = l->premier;
    Element *prev = NULL;

    while (temp) {
        if (temp->data.id_support == id) {
            if (prev) prev->suivant = temp->suivant;
            else l->premier = temp->suivant;
            free(temp);
            return 1;
        }
        prev = temp;
        temp = temp->suivant;
    }
    return 0;
}
