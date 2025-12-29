#include "support.h"

// Variable globale pour l'auto-incrémentation de l'ID
int dernier_id = 0;

// ============ FONCTION D'AFFICHAGE D'UN SUPPORT ============
void afficher(Support s) {
    printf("\n+------------------------------------------------------------+\n");
    printf("| ID          : %-45d |\n", s.id_support);
    printf("| Titre       : %-45s |\n", s.titre);
    printf("| Module      : %-45s |\n", s.module);
    printf("| Enseignant  : %-45s |\n", s.enseignant);
    printf("| Type        : %-45s |\n", s.type);
    printf("| Chemin      : %-45s |\n", s.chemin_fichier);
    printf("| Date ajout  : %-45s |\n", s.date_ajout);
    printf("+------------------------------------------------------------+\n");
}

// ============ FONCTION COMPTER LES ÉLÉMENTS ============
int compter_elements(Liste *l) {
    int count = 0;
    Element *temp = l->premier;
    while(temp != NULL) {
        count++;
        temp = temp->suivant;
    }
    return count;
}

// ============ FONCTION AJOUTER UN SUPPORT ============
void ajouter(Liste *l) {
    Support nouveau;
    
    // ID auto-incrémenté
    nouveau.id_support = ++dernier_id;
    
    printf("\n========== AJOUTER UN SUPPORT ==========\n");
    
    // Saisie du titre
    printf("\nTitre du support: ");
    fgets(nouveau.titre, 100, stdin);
    nouveau.titre[strcspn(nouveau.titre, "\n")] = 0;
    
    // Saisie du module
    printf("Module: ");
    fgets(nouveau.module, 100, stdin);
    nouveau.module[strcspn(nouveau.module, "\n")] = 0;
    
    // Saisie de l'enseignant
    printf("Enseignant: ");
    fgets(nouveau.enseignant, 100, stdin);
    nouveau.enseignant[strcspn(nouveau.enseignant, "\n")] = 0;
    
    // Saisie du type avec validation
    int type_valide = 0;
    while(!type_valide) {
        printf("Type (PDF/PPT/Video/Note/Livre): ");
        fgets(nouveau.type, 50, stdin);
        nouveau.type[strcspn(nouveau.type, "\n")] = 0;
        
        // Convertir en majuscules pour comparaison
        char type_upper[50];
        strcpy(type_upper, nouveau.type);
        for(int i = 0; type_upper[i]; i++) {
            type_upper[i] = toupper(type_upper[i]);
        }
        
        if(strcmp(type_upper, "PDF") == 0 || 
           strcmp(type_upper, "PPT") == 0 || 
           strcmp(type_upper, "VIDEO") == 0 || 
           strcmp(type_upper, "NOTE") == 0 || 
           strcmp(type_upper, "LIVRE") == 0) {
            strcpy(nouveau.type, type_upper);
            type_valide = 1;
        } else {
            printf("[ERREUR] Type invalide! Choisissez: PDF, PPT, Video, Note ou Livre\n");
        }
    }
    
    // Chemin du fichier
    printf("Chemin du fichier: ");
    fgets(nouveau.chemin_fichier, 200, stdin);
    nouveau.chemin_fichier[strcspn(nouveau.chemin_fichier, "\n")] = 0;
    
    // Date d'ajout automatique
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(nouveau.date_ajout, 20, "%d/%m/%Y", tm_info);
    
    // Ajouter à la liste (à la fin)
    addBack(l, nouveau);
    
    printf("\n[SUCCESS] Support ajoute avec l'ID: %d\n", nouveau.id_support);
}

// ============ FONCTION MODIFIER UN SUPPORT ============
void modifier(Liste *l) {
    if(l->premier == NULL) {
        printf("\n[ERREUR] Aucun support dans la liste!\n");
        return;
    }
    
    int id;
    printf("\n========== MODIFIER UN SUPPORT ==========\n");
    printf("\nEntrer l'ID du support a modifier: ");
    scanf("%d", &id);
    getchar();
    
    Element *element = findById(l, id);
    
    if(element == NULL) {
        printf("\n[ERREUR] Support avec ID %d introuvable!\n", id);
        return;
    }
    
    printf("\nSupport trouve:\n");
    afficher(element->data);
    
    printf("\n--- Modification (laisser vide pour conserver) ---\n");
    
    char buffer[200];
    
    // Modifier titre
    printf("\nNouveau titre [%s]: ", element->data.titre);
    fgets(buffer, 100, stdin);
    if(strlen(buffer) > 1) {
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(element->data.titre, buffer);
    }
    
    // Modifier module
    printf("Nouveau module [%s]: ", element->data.module);
    fgets(buffer, 100, stdin);
    if(strlen(buffer) > 1) {
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(element->data.module, buffer);
    }
    
    // Modifier enseignant
    printf("Nouvel enseignant [%s]: ", element->data.enseignant);
    fgets(buffer, 100, stdin);
    if(strlen(buffer) > 1) {
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(element->data.enseignant, buffer);
    }
    
    // Modifier type
    printf("Nouveau type [%s] (PDF/PPT/Video/Note/Livre): ", element->data.type);
    fgets(buffer, 50, stdin);
    if(strlen(buffer) > 1) {
        buffer[strcspn(buffer, "\n")] = 0;
        for(int i = 0; buffer[i]; i++) {
            buffer[i] = toupper(buffer[i]);
        }
        strcpy(element->data.type, buffer);
    }
    
    // Modifier chemin fichier
    printf("Nouveau chemin [%s]: ", element->data.chemin_fichier);
    fgets(buffer, 200, stdin);
    if(strlen(buffer) > 1) {
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(element->data.chemin_fichier, buffer);
    }
    
    printf("\n[SUCCESS] Support modifie avec succes!\n");
}

// ============ FONCTION SUPPRIMER UN SUPPORT ============
void supprimer(Liste *l) {
    if(l->premier == NULL) {
        printf("\n[ERREUR] Aucun support dans la liste!\n");
        return;
    }
    
    int id;
    printf("\n========== SUPPRIMER UN SUPPORT ==========\n");
    printf("\nEntrer l'ID du support a supprimer: ");
    scanf("%d", &id);
    getchar();
    
    Element *element = findById(l, id);
    
    if(element == NULL) {
        printf("\n[ERREUR] Support avec ID %d introuvable!\n", id);
        return;
    }
    
    printf("\nSupport a supprimer:\n");
    afficher(element->data);
    
    char confirmation;
    printf("\nConfirmer la suppression? (o/n): ");
    scanf("%c", &confirmation);
    getchar();
    
    if(confirmation == 'o' || confirmation == 'O') {
        removeById(l, id);
        printf("\n[SUCCESS] Support supprime avec succes!\n");
    } else {
        printf("\n[ANNULE] Suppression annulee.\n");
    }
}

// ============ FONCTION AFFICHER TOUS LES SUPPORTS ============
void afficher_liste(Liste *l) {
    if(l->premier == NULL) {
        printf("\n[INFO] Aucun support dans la liste!\n");
        return;
    }
    
    int count = compter_elements(l);
    printf("\n========== LISTE DES SUPPORTS (%d) ==========\n", count);
    
    Element *temp = l->premier;
    while(temp != NULL) {
        afficher(temp->data);
        temp = temp->suivant;
    }
}

// ============ FONCTION FILTRER PAR MODULE ============
void filtrer_par_module(Liste *l, char* module) {
    if(l->premier == NULL) {
        printf("\n[INFO] Aucun support dans la liste!\n");
        return;
    }
    
    int trouve = 0;
    printf("\n========== SUPPORTS DU MODULE: %s ==========\n", module);
    
    Element *temp = l->premier;
    while(temp != NULL) {
        if(strstr(temp->data.module, module) != NULL) {
            afficher(temp->data);
            trouve++;
        }
        temp = temp->suivant;
    }
    
    if(trouve == 0) {
        printf("\n[INFO] Aucun support trouve pour ce module.\n");
    } else {
        printf("\n[INFO] %d support(s) trouve(s).\n", trouve);
    }
}

// ============ FONCTION FILTRER PAR ENSEIGNANT ============
void filtrer_par_enseignant(Liste *l, char* enseignant) {
    if(l->premier == NULL) {
        printf("\n[INFO] Aucun support dans la liste!\n");
        return;
    }
    
    int trouve = 0;
    printf("\n========== SUPPORTS DE L'ENSEIGNANT: %s ==========\n", enseignant);
    
    Element *temp = l->premier;
    while(temp != NULL) {
        if(strstr(temp->data.enseignant, enseignant) != NULL) {
            afficher(temp->data);
            trouve++;
        }
        temp = temp->suivant;
    }
    
    if(trouve == 0) {
        printf("\n[INFO] Aucun support trouve pour cet enseignant.\n");
    } else {
        printf("\n[INFO] %d support(s) trouve(s).\n", trouve);
    }
}

// ============ FONCTION FILTRER PAR TYPE ============
void filtrer_par_type(Liste *l, char* type) {
    if(l->premier == NULL) {
        printf("\n[INFO] Aucun support dans la liste!\n");
        return;
    }
    
    // Convertir en majuscules
    char type_upper[50];
    strcpy(type_upper, type);
    for(int i = 0; type_upper[i]; i++) {
        type_upper[i] = toupper(type_upper[i]);
    }
    
    int trouve = 0;
    printf("\n========== SUPPORTS DE TYPE: %s ==========\n", type_upper);
    
    Element *temp = l->premier;
    while(temp != NULL) {
        if(strcmp(temp->data.type, type_upper) == 0) {
            afficher(temp->data);
            trouve++;
        }
        temp = temp->suivant;
    }
    
    if(trouve == 0) {
        printf("\n[INFO] Aucun support trouve pour ce type.\n");
    } else {
        printf("\n[INFO] %d support(s) trouve(s).\n", trouve);
    }
}

// ============ FONCTION RECHERCHER PAR MOT-CLE ============
void rechercher_par_motcle(Liste *l, char* motcle) {
    if(l->premier == NULL) {
        printf("\n[INFO] Aucun support dans la liste!\n");
        return;
    }
    
    int trouve = 0;
    printf("\n========== RECHERCHE: \"%s\" ==========\n", motcle);
    
    // Convertir mot-clé en minuscules
    char motcle_lower[100];
    strcpy(motcle_lower, motcle);
    for(int i = 0; motcle_lower[i]; i++) {
        motcle_lower[i] = tolower(motcle_lower[i]);
    }
    
    Element *temp = l->premier;
    while(temp != NULL) {
        // Créer versions minuscules des champs à rechercher
        char titre_lower[100], module_lower[100], enseignant_lower[100];
        
        strcpy(titre_lower, temp->data.titre);
        strcpy(module_lower, temp->data.module);
        strcpy(enseignant_lower, temp->data.enseignant);
        
        for(int j = 0; titre_lower[j]; j++) titre_lower[j] = tolower(titre_lower[j]);
        for(int j = 0; module_lower[j]; j++) module_lower[j] = tolower(module_lower[j]);
        for(int j = 0; enseignant_lower[j]; j++) enseignant_lower[j] = tolower(enseignant_lower[j]);
        
        // Rechercher dans titre, module ou enseignant
        if(strstr(titre_lower, motcle_lower) != NULL ||
           strstr(module_lower, motcle_lower) != NULL ||
           strstr(enseignant_lower, motcle_lower) != NULL) {
            afficher(temp->data);
            trouve++;
        }
        
        temp = temp->suivant;
    }
    
    if(trouve == 0) {
        printf("\n[INFO] Aucun support trouve contenant \"%s\".\n", motcle);
    } else {
        printf("\n[INFO] %d support(s) trouve(s).\n", trouve);
    }
}

// ============ FONCTION SAUVEGARDER ============
void sauvegarder(Liste *l) {
    FILE *fichier = fopen("supports.dat", "w");
    if(fichier == NULL) {
        printf("\n[ERREUR] Impossible d'ouvrir le fichier!\n");
        return;
    }
    
    fprintf(fichier, "%d\n", dernier_id);
    
    Element *temp = l->premier;
    int count = 0;
    while(temp != NULL) {
        fprintf(fichier, "%d|%s|%s|%s|%s|%s|%s\n",
                temp->data.id_support,
                temp->data.titre,
                temp->data.module,
                temp->data.type,
                temp->data.enseignant,
                temp->data.chemin_fichier,
                temp->data.date_ajout);
        count++;
        temp = temp->suivant;
    }
    
    fclose(fichier);
    printf("\n[SUCCESS] %d support(s) sauvegarde(s)!\n", count);
}

// ============ FONCTION CHARGER ============
void charger(Liste *l) {
    FILE *fichier = fopen("supports.dat", "r");
    if(fichier == NULL) {
        printf("\n[INFO] Aucune donnee a charger (premier lancement).\n");
        return;
    }
    
    fscanf(fichier, "%d\n", &dernier_id);
    
    Support s;
    int count = 0;
    
    while(fscanf(fichier, "%d|%99[^|]|%99[^|]|%49[^|]|%99[^|]|%199[^|]|%19[^\n]\n",
                 &s.id_support,
                 s.titre,
                 s.module,
                 s.type,
                 s.enseignant,
                 s.chemin_fichier,
                 s.date_ajout) == 7) {
        addBack(l, s);
        count++;
    }
    
    fclose(fichier);
    printf("\n[SUCCESS] %d support(s) charge(s)!\n", count);
}