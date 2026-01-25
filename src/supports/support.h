#ifndef SUPPORT_H
#define SUPPORT_H

#include "../structures/linked_list.h"
#include <mysql/mysql.h> // Necessary for the MYSQL type

// Utility functions
void display_support(Support s);
int count_elements(Liste *l);

    // New signature with MYSQL *conn
void ajouter_support_gui(Liste *l, MYSQL *conn,
                         const char *titre,
                         const char *module,
                         const char *type,
                         const char *enseignant,
                         const char *chemin);

// Note: save_data and load_data have been removed because they were replaced by the database

#endif
