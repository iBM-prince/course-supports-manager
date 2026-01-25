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

// Fonction principale appelée par le bouton "Ajouter"
void ajouter_support_gui(Liste *l, MYSQL *conn,
                         const char *titre,
                         const char *module,
                         const char *type,
                         const char *enseignant,
                         const char *chemin) {
    Support s;

    // 1. Remplissage des données textuelles
    strncpy(s.titre, titre, sizeof(s.titre)-1); s.titre[sizeof(s.titre)-1]=0;
    strncpy(s.module, module, sizeof(s.module)-1); s.module[sizeof(s.module)-1]=0;
    strncpy(s.type, type, sizeof(s.type)-1); s.type[sizeof(s.type)-1]=0;
    strncpy(s.enseignant, enseignant, sizeof(s.enseignant)-1); s.enseignant[sizeof(s.enseignant)-1]=0;
    strncpy(s.chemin_fichier, chemin, sizeof(s.chemin_fichier)-1); s.chemin_fichier[sizeof(s.chemin_fichier)-1]=0;

    // 2. Gestion de la date (Format SQL YYYY-MM-DD)
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(s.date_ajout, sizeof(s.date_ajout), "%Y-%m-%d", tm_info);

    // 3. Insertion dans MySQL
    // On passe l'adresse de s (&s) et la connexion
    if (db_insert_support(&s, conn) == 0) {
        
        // --- TA MODIFICATION ICI ---
        // Récupération de l'ID généré par l'AUTO_INCREMENT de MySQL
        s.id_support = (int)mysql_insert_id(conn); 
        
        // 4. Ajout à la liste en mémoire (pour affichage immédiat dans GTK)
        addBack(l, s);
        
        printf("Succès : Support ajouté avec l'ID MySQL %d\n", s.id_support);
    } else {
        fprintf(stderr, "Erreur : Impossible d'insérer le support dans la base.\n");
    }
}