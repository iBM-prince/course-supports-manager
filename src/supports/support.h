#ifndef support_h
#define support_h

#include "linked_list.h"
#include <ctype.h>

// Variables globales
extern int dernier_id;

// Déclaration des fonctions adaptées pour liste chaînée
void ajouter(Liste *l);
void modifier(Liste *l);
void supprimer(Liste *l);
void afficher(Support s);
void afficher_liste(Liste *l);
void filtrer_par_module(Liste *l, char* module);
void filtrer_par_enseignant(Liste *l, char* enseignant);
void filtrer_par_type(Liste *l, char* type);
void rechercher_par_motcle(Liste *l, char* motcle);
void sauvegarder(Liste *l);
void charger(Liste *l);
int compter_elements(Liste *l);

#endif