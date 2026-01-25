#ifndef SUPPORT_H
#define SUPPORT_H

#include "../structures/linked_list.h"
#include <mysql/mysql.h> // Necessary for the MYSQL type

// Utility functions
void display_support(Support s);
int count_elements(Liste *l);

/* French API wrappers (compatibility with main.c) */
int count_elements(Liste *l);
void add_support(Liste *l);
void edit_support(Liste *l);
void remove_support(Liste *l);
void printList(Liste *l);
void findBymodule(Liste *l, char* module);
void findByteacher(Liste *l, char* teacher);
void findBytype(Liste *l, char* type);
void findBykeyword(Liste *l, char* keyword);
void save_data(Liste *l);
void load_data(Liste *l);
#endif
