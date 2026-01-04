#include "support.h"

// Global variable for auto-incrementing the ID
int last_id = 0;

// ============ FUNCTION TO DISPLAY A SUPPORT ============
void display_support(Support s) {
    printf("\n+------------------------------------------------------------+\n");
    printf("| ID          : %-45d |\n", s.id_support);
    printf("| Title       : %-45s |\n", s.titre);
    printf("| Module      : %-45s |\n", s.module);
    printf("| Teacher     : %-45s |\n", s.enseignant);
    printf("| Type        : %-45s |\n", s.type);
    printf("| File path   : %-45s |\n", s.chemin_fichier);
    printf("| Date added  : %-45s |\n", s.date_ajout);
    printf("+------------------------------------------------------------+\n");
}

// ============ FUNCTION TO COUNT ELEMENTS ============
int count_elements(Liste *l) {
    int count = 0;
    Element *temp = l->premier;
    while (temp != NULL) {
        count++;
        temp = temp->suivant;
    }
    return count;
}

// ============ FUNCTION TO ADD A SUPPORT ============
void add_support(Liste *l) {
    Support nouveau;

    // Auto-incremented ID
    nouveau.id_support = ++last_id;

    printf("\n========== ADD A SUPPORT ==========\n");

    printf("\nSupport title: ");
    fgets(nouveau.titre, 100, stdin);
    nouveau.titre[strcspn(nouveau.titre, "\n")] = 0;

    printf("Module: ");
    fgets(nouveau.module, 100, stdin);
    nouveau.module[strcspn(nouveau.module, "\n")] = 0;

    printf("Teacher: ");
    fgets(nouveau.enseignant, 100, stdin);
    nouveau.enseignant[strcspn(nouveau.enseignant, "\n")] = 0;

    int valid_type = 0;
    while (!valid_type) {
        printf("Type (PDF/PPT/VIDEO/NOTE/BOOK): ");
        fgets(nouveau.type, 50, stdin);
        nouveau.type[strcspn(nouveau.type, "\n")] = 0;

        for (int i = 0; nouveau.type[i]; i++) {
            nouveau.type[i] = toupper(nouveau.type[i]);
        }

        if (strcmp(nouveau.type, "PDF") == 0 ||
            strcmp(nouveau.type, "PPT") == 0 ||
            strcmp(nouveau.type, "VIDEO") == 0 ||
            strcmp(nouveau.type, "NOTE") == 0 ||
            strcmp(nouveau.type, "BOOK") == 0) {
            valid_type = 1;
        } else {
            printf("[ERROR] Invalid type!\n");
        }
    }

    printf("File path: ");
    fgets(nouveau.chemin_fichier, 200, stdin);
    nouveau.chemin_fichier[strcspn(nouveau.chemin_fichier, "\n")] = 0;

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
void removesupport(Liste *l) {
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
    }

    fclose(f);
}
