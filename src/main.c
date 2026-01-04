#include <stdio.h>
#include <stdlib.h>
#include "support.h"
#include "linked_list.h"
#include <string.h>
#include <ctype.h>  

int main() 
{
    // Initialiser la liste chaînée
    Liste *liste = initialisation();
    if(liste == NULL) {
        printf("Erreur d'initialisation!\n");
        return 1;
    }
    
    // Charger les données sauvegardées
    charger(liste);
    
    int choix;
    
    do {
        #ifdef _WIN32
            system("cls");
            system("title Gestion des Supports de Cours - SUPDECO");
        #else
            system("clear");
        #endif
        
        printf("\n");
        printf("  ╔═══════════════════════════════════════════════════╗\n");
        printf("  ║   GESTION DES SUPPORTS DE COURS - SUPDECO        ║\n");
        printf("  ╚═══════════════════════════════════════════════════╝\n");
        printf("\n");
        printf("  ┌───────────────────────────────────────────────────┐\n");
        printf("  │  1. Ajouter un support                            │\n");
        printf("  │  2. Modifier un support                           │\n");
        printf("  │  3. Supprimer un support                          │\n");
        printf("  │  4. Visualiser tous les supports                  │\n");
        printf("  ├───────────────────────────────────────────────────┤\n");
        printf("  │  5. Filtrer par module                            │\n");
        printf("  │  6. Filtrer par enseignant                        │\n");
        printf("  │  7. Filtrer par type                              │\n");
        printf("  │  8. Rechercher par mot-cle                        │\n");
        printf("  ├───────────────────────────────────────────────────┤\n");
        printf("  │  9. Sauvegarder les donnees                       │\n");
        printf("  │  0. Quitter                                       │\n");
        printf("  └───────────────────────────────────────────────────┘\n");
        printf("\n  Supports actuels: %d\n", compter_elements(liste));
        printf("\n  => Votre choix: ");
        
        scanf("%d", &choix);
        getchar(); // Vider le buffer
        
        switch(choix) {
            case 1:
                ajouter(liste);
                break;
                
            case 2:
                modifier(liste);
                break;
                
            case 3:
                supprimer(liste);
                break;
                
            case 4:
                afficher_liste(liste);
                break;
                
            case 5: {
                char module[100];
                printf("\n  Entrer le nom du module: ");
                fgets(module, 100, stdin);
                module[strcspn(module, "\n")] = 0;
                filtrer_par_module(liste, module);
                break;
            }
            
            case 6: {
                char enseignant[100];
                printf("\n  Entrer le nom de l'enseignant: ");
                fgets(enseignant, 100, stdin);
                enseignant[strcspn(enseignant, "\n")] = 0;
                filtrer_par_enseignant(liste, enseignant);
                break;
            }
            
            case 7: {
                char type[50];
                printf("\n  Types: PDF, PPT, Video, Note, Livre\n");
                printf("  Entrer le type: ");
                fgets(type, 50, stdin);
                type[strcspn(type, "\n")] = 0;
                filtrer_par_type(liste, type);
                break;
            }
            
            case 8: {
                char motcle[100];
                printf("\n  Entrer le mot-cle: ");
                fgets(motcle, 100, stdin);
                motcle[strcspn(motcle, "\n")] = 0;
                rechercher_par_motcle(liste, motcle);
                break;
            }
            
            case 9:
                sauvegarder(liste);
                break;
                
            case 0:
                printf("\n  Sauvegarder avant de quitter? (o/n): ");
                char reponse;
                scanf("%c", &reponse);
                if(reponse == 'o' || reponse == 'O') {
                    sauvegarder(liste);
                }
                printf("\n  Merci d'avoir utilise l'application!\n\n");
                break;
                
            default:
                printf("\n  [ERREUR] Choix invalide!\n");
        }
        
        if(choix != 0) {
            printf("\n  Appuyez sur Entree pour continuer...");
            getchar();
        }
        
    } while(choix != 0);
    
    // Libérer la mémoire avant de quitter
    while(liste->premier != NULL) {
        removeFront(liste);
    }
    free(liste);
    
    return 0;
}