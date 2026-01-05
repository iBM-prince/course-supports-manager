Gestion des supports de cours
Petit projet en langage C réalisé dans le cadre du module Algorithmique et Programmation 3.
Objectif : gérer des supports de cours (PDF, PPT, vidéos, etc.) en mode console, en utilisant une liste chaînée.
​

1. Ce que fait le programme
Version mi‑parcours (présentation du 8 janvier) – fonctionnalités de la section 2 :
​

Ajouter un support.

Modifier un support.

Supprimer un support.

Afficher tous les supports.

Filtrer par module / enseignant / type.

Rechercher un support par mot‑clé.

Chaque support contient : id, titre, module, type, enseignant, chemin du fichier, date d’ajout.
​

2. Comment compiler et exécuter
Depuis la racine du projet :

bash
gcc -Istructures -Isupports main.c supports/support.c structures/linked_list.c -o app
./app.exe


3. Organisation des fichiers
structures/linked_list.h / structures/linked_list.c
Implémentation de la liste chaînée des supports (ajout, suppression, affichage, recherche).

supports/support.h / supports/support.c
Définition de la structure Support et fonctions de gestion (CRUD, filtres, recherche).

sql/create_table.sql
Script SQL pour créer la base et la table supports (id, titre, module, type, enseignant, chemin, date_ajout).
​

docs/
Diaporama de la présentation mi‑parcours.

4. Équipe
Ibrahima : structures de données (liste chaînée).

Ange : gestion des supports + interface console (main.c).

Abdoulaye : script SQL (sql/create_table.sql) et future intégration MySQL.