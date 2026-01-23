#ifndef SUPPORT_H
#define SUPPORT_H

#include "../structures/linked_list.h"
#include <mysql/mysql.h> // Nécessaire pour le type MYSQL

// Fonctions utilitaires
void display_support(Support s);
int count_elements(Liste *l);

// Nouvelle signature avec MYSQL *conn
void ajouter_support_gui(Liste *l, MYSQL *conn,
                         const char *titre,
                         const char *module,
                         const char *type,
                         const char *enseignant,
                         const char *chemin);

// Note : save_data et load_data sont supprimés car remplacés par la BDD

#endif