<h1> Projet Algorithmique et Programmation 3 - 2025-2026</h1>
<h3> Application complète en C avec GTK4, MySQL, liste chaînée, pile et file.</h3>

🎯 Fonctionnalités
100% du cahier des charges respecté :

✅ CRUD supports : Ajouter/Modifier/Supprimer/Visualiser

✅ Liste chaînée : Chargement/sauvegarde MySQL ↔ mémoire

✅ Pile : Historique des consultations (popup dédiée)

✅ File : Notifications (zone dédiée)

✅ Interface GTK4 : Thème SupDeCo, moderne

✅ MySQL C-API : Persistance complète (table supports)

✅ Ouverture fichiers : Double-clic → "Ouvrir fichier"

🛠 Compilation & Exécution
MSYS2/MinGW64 (Windows) :

bash
pacman -S mingw-w64-x86_64-gtk4 mingw-w64-x86_64-libmariadbclient
gcc -Isupports -Istructures -Idatabase gui/main_gtk.c supports/*.c structures/*.c database/*.c \
    `pkg-config --cflags --libs gtk4 libmariadb` -o app_gtk.exe
./app_gtk.exe
📁 Organisation modulaire (.c/.h)

src/
├── gui/           # Interface GTK4 (main_gtk.c)
├── supports/      # Structure Support + CRUD (support.c/h)
├── structures/    # Liste chaînée, Pile, File (linked_list.c/h, stack.c/h, queue.c/h)
├── database/      # MySQL C-API (mysql_handler.c/h)
└── sql/           # Schema BDD (create_table.sql)
<h1>🗄 Base de données MySQL</h1>
Script sql/create_table.sql :

sql
CREATE DATABASE IF NOT EXISTS gestion_supports_db;
USE gestion_supports_db;
CREATE TABLE supports (
    id INTEGER PRIMARY KEY AUTO_INCREMENT,
    titre TEXT NOT NULL,
    module TEXT,
    type TEXT,
    enseignant TEXT,
    chemin TEXT,
    date_ajout TEXT
);
<h1>👥 Répartition des tâches (Équipe 3)</h1>
Membre	          Tâches principales
Abdoulaye	      database/mysql_handler.c/.h (MySQL C-API), stack.h
Ange	          support.c/support.h, stack.c
Ibrahima	      linked_list.c/.h, queue.c/.h, main_gtk.c

<h1>🎨 Fonctionnalités avancées</h1>

Thème SupDeCo : Bleu/Or institutionnel

Mode édition : "Ajouter" → "Sauvegarder Modif."

Synchronisation temps réel : Liste ↔ MySQL

Gestion erreurs : Connexion BDD, champs vides

Ouverture fichiers : Windows start, Linux xdg-open

<h1>📚 Comment utiliser</h1>
Lancer : ./app_gtk.exe

Ajouter : Remplir formulaire → "Ajouter"

Modifier : Sélectionner → "Modifier" → changer → "Sauvegarder"

Supprimer : Sélectionner → "Supprimer"

Historique : Bouton "Historique" (pile des consultations)

🔧 <h1> Dépendances </h1>
GTK4 : Interface graphique

libmariadbclient : MySQL C-API

MSYS2/MinGW : Compilation Windows

📈 <h2>État du projet</h2>

Critère sujet	            Statut
Liste chaînée	            ✅ linked_list.c/.h
Pile (historique)	        ✅ stack.c/.h
File (notifications)	    ✅ queue.c/.h
Interface GTK4	            ✅ main_gtk.c
MySQL C-API	                ✅ mysql_handler.c/.h
Gestion mémoire	            ✅ malloc/free
Modularité	                ✅ .c/.h propres

Présentation finale 22 janvier 2026 → Projet COMPLET ! 🎉