#include "support.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../database/mysql_handler.h" // Indispensable pour l'accès BDD

// Note: last_id n'est plus utilisé car MySQL gère l'auto-incrément

void display_support(Support s) {
    printf("ID: %d | %s | %s | %s | %s | %s\n",
           s.id_support, s.titre, s.module, s.type, s.enseignant, s.date_ajout);
}

int count_elements(Liste *l) {
    int count = 0;
    Element *temp = l->premier;
    while (temp) {
        count++;
        temp = temp->suivant;
    }
    return count;
}

// Main function called by the "Add" button
void ajouter_support_gui(Liste *l, MYSQL *conn,
                         const char *titre,
                         const char *module,
                         const char *type,
                         const char *enseignant,
                         const char *chemin) {
    Support s;

    // 1. Filling in text data
    strncpy(s.titre, titre, sizeof(s.titre)-1); s.titre[sizeof(s.titre)-1]=0;
    strncpy(s.module, module, sizeof(s.module)-1); s.module[sizeof(s.module)-1]=0;
    strncpy(s.type, type, sizeof(s.type)-1); s.type[sizeof(s.type)-1]=0;
    strncpy(s.enseignant, enseignant, sizeof(s.enseignant)-1); s.enseignant[sizeof(s.enseignant)-1]=0;
    strncpy(s.chemin_fichier, chemin, sizeof(s.chemin_fichier)-1); s.chemin_fichier[sizeof(s.chemin_fichier)-1]=0;

    // 2. Date management (SQL format YYYY-MM-DD)
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(nouveau.date_ajout, 20, "%d/%m/%Y", tm_info);

    addBack(l, nouveau);

    printf("\n[SUCCESS] Support added with ID: %d\n", nouveau.id_support);
}

// ============ FUNCTION TO EDIT A SUPPORT ============
void edit_support(Liste *l) {
    if (l->premier == NULL) {
        printf("\n[ERROR] No support available!\n");
        return;
    }

    int id;
    printf("\nEnter support ID to edit: ");
    scanf("%d", &id);
    getchar();

    Element *element = findById(l, id);
    if (element == NULL) {
        printf("\n[ERROR] Support not found!\n");
        return;
    }

    display_support(element->data);

    char buffer[200];

    printf("New title [%s]: ", element->data.titre);
    fgets(buffer, 100, stdin);
    if (strlen(buffer) > 1) {
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(element->data.titre, buffer);
    }

    printf("New module [%s]: ", element->data.module);
    fgets(buffer, 100, stdin);
    if (strlen(buffer) > 1) {
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(element->data.module, buffer);
    }

    printf("New teacher [%s]: ", element->data.enseignant);
    fgets(buffer, 100, stdin);
    if (strlen(buffer) > 1) {
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(element->data.enseignant, buffer);
    }

    printf("\n[SUCCESS] Support updated successfully!\n");
}

// ============ FUNCTION TO DELETE A SUPPORT ============
void remove_support(Liste *l) {
    if (l->premier == NULL) {
        printf("\n[ERROR] No support available!\n");
        return;
    }

    int id;
    printf("\nEnter support ID to delete: ");
    scanf("%d", &id);
    getchar();

    if (removeById(l, id)) {
        printf("\n[SUCCESS] Support deleted!\n");
    } else {
        printf("\n[ERROR] Support not found!\n");
    }
}

// ============ FUNCTION TO DISPLAY ALL SUPPORTS ============
void printList(Liste *l) {
    if (l->premier == NULL) {
        printf("\n[INFO] No support available!\n");
        return;
    }

    Element *temp = l->premier;
    while (temp != NULL) {
        display_support(temp->data);
        temp = temp->suivant;
    }
}

// ============ FUNCTION TO FILTER BY MODULE ============
void findBymodule(Liste *l, char *module) {
    Element *temp = l->premier;
    while (temp != NULL) {
        if (strstr(temp->data.module, module)) {
            display_support(temp->data);
        }
        temp = temp->suivant;
    }
}

// ============ FUNCTION TO FILTER BY TEACHER ============
void findByteacher(Liste *l, char *teacher) {
    Element *temp = l->premier;
    while (temp != NULL) {
        if (strstr(temp->data.enseignant, teacher)) {
            display_support(temp->data);
        }
        temp = temp->suivant;
    }
}

// ============ FUNCTION TO FILTER BY TYPE ============
void findBytype(Liste *l, char *type) {
    for (int i = 0; type[i]; i++) type[i] = toupper(type[i]);

    Element *temp = l->premier;
    while (temp != NULL) {
        if (strcmp(temp->data.type, type) == 0) {
            display_support(temp->data);
        }
        temp = temp->suivant;
    }
}

// ============ FUNCTION TO SEARCH BY KEYWORD ============
void findBykeyword(Liste *l, char *keyword) {
    Element *temp = l->premier;
    while (temp != NULL) {
        if (strstr(temp->data.titre, keyword) ||
            strstr(temp->data.module, keyword) ||
            strstr(temp->data.enseignant, keyword)) {
            display_support(temp->data);
        }
        temp = temp->suivant;
    }
}

// ============ FUNCTION TO SAVE DATA ============
void save_data(Liste *l) {
    FILE *f = fopen("supports.dat", "w");
    if (!f) return;

    fprintf(f, "%d\n", last_id);

    Element *temp = l->premier;
    while (temp) {
        fprintf(f, "%d|%s|%s|%s|%s|%s|%s\n",
                temp->data.id_support,
                temp->data.titre,
                temp->data.module,
                temp->data.type,
                temp->data.enseignant,
                temp->data.chemin_fichier,
                temp->data.date_ajout);
        temp = temp->suivant;
    }

    fclose(f);
}

// ============ FUNCTION TO LOAD DATA ============
void load_data(Liste *l) {
    FILE *f = fopen("supports.dat", "r");
    if (!f) return;

    fscanf(f, "%d\n", &last_id);

    Support s;
    while (fscanf(f, "%d|%99[^|]|%99[^|]|%49[^|]|%99[^|]|%199[^|]|%19[^\n]\n",
                  &s.id_support,
                  s.titre,
                  s.module,
                  s.type,
                  s.enseignant,
                  s.chemin_fichier,
                  s.date_ajout) == 7) {
        addBack(l, s);
        
        printf("Succès : Support ajouté avec l'ID MySQL %d\n", s.id_support);
    } else {
        fprintf(stderr, "Erreur : Impossible d'insérer le support dans la base.\n");
    }

    fclose(f);
}

/* French API wrappers for compatibility with main.c */
int compter_elements(Liste *l) {
    return count_elements(l);
}

void ajouter(Liste *l) {
    add_support(l);
}

void modifier(Liste *l) {
    edit_support(l);
}

void supprimer(Liste *l) {
    remove_support(l);
}

void afficher_liste(Liste *l) {
    printList(l);
}

void filtrer_par_module(Liste *l, char* module) {
    findBymodule(l, module);
}

void filtrer_par_enseignant(Liste *l, char* teacher) {
    findByteacher(l, teacher);
}

void filtrer_par_type(Liste *l, char* type) {
    findBytype(l, type);
}

void rechercher_par_motcle(Liste *l, char* keyword) {
    findBykeyword(l, keyword);
}

void sauvegarder(Liste *l) {
    save_data(l);
}

void charger(Liste *l) {
    load_data(l);
}
