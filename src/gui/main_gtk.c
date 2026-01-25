#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include "../structures/linked_list.h"
#include "../structures/stack.h"
#include "../structures/queue.h"
#include "../supports/support.h"
#include "../database/mysql_handler.h"

/* les variables globales */
void supprimer_element(Liste *l, int id); 

// Variables globales
static Liste *main_list = NULL;
static MYSQL *db_conn = NULL; 
static GtkWidget *entry_title, *entry_module, *dropdown_type, *entry_teacher, *entry_path;
static GtkWidget *listbox;
static Element *current_edit = NULL;   
static GtkWidget *btn_add_action = NULL; 

static Queue *notif_queue = NULL;
static GtkWidget *notif_list = NULL;
static Stack *history_stack = NULL;
// Prototypes des fonctions statiques
static void refresh_management_list(void);
static void show_notifications_page(void);
static void clear_form(void);

/* Applique le thème Sup de Co */
static void apply_supdeco_theme() {
    GtkCssProvider *provider = gtk_css_provider_new();
    const char *css = 
        "window { background-color: #f4f7f9; }"
        ".sidebar { background-color: #1e3a8a; }"
        ".sidebar-logo-box { background-color: #ffffff; padding: 20px; border-bottom: 3px solid #fbbf24; }"
      
        ".sidebar button { background: none; border: none; color: #ffffff; padding: 15px 20px; font-size: 14px; }"
        ".sidebar button:hover { background-color: #2563eb; }"
        ".active-btn { background-color: #162d6d; color: #fbbf24; font-weight: bold; border-left: 5px solid #fbbf24; }"
        ".main-header { font-size: 19px; font-weight: bold; color: #1e3a8a; margin-bottom: 15px; }"
        ".btn-add { background-color: #ffffff; color: #333; border: 1px solid #1e3a8a; padding: 8px 30px; border-radius: 4px; font-weight: bold; }"
        ".table-header { background-color: #1e3a8a; border-radius: 4px 4px 0 0; }"
        ".table-header label { color: #fbbf24; font-weight: bold; padding: 10px 5px; }"
        ".list-row { background: white; border-bottom: 1px solid #e0e0e0; }"
      
        ".action-btn { background-color: white; border: 1px solid #1e3a8a; color: #1e3a8a; padding: 8px; border-radius: 4px; font-weight: bold; font-size: 12px; margin-bottom: 8px; min-width: 100px; }"
        
        "listbox row:selected { background-color: #2563eb; }"
        "listbox row:selected label { color: #ffffff; }";

    gtk_css_provider_load_from_string(provider, css);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(), 
        GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
}

/* Gestion des notifications */
static void add_notif(const char *msg) {
    enqueue(notif_queue, (char*)msg);
    show_notifications_page();
}

/* Affiche les notifications dans la liste */
static void show_notifications_page() {
    if (!notif_list) return;
    gtk_list_box_remove_all(GTK_LIST_BOX(notif_list));
    QueueNode *node = notif_queue->front;
    while (node) {
        GtkWidget *row = gtk_label_new(node->message);
        gtk_label_set_xalign(GTK_LABEL(row), 0.0);
        gtk_list_box_append(GTK_LIST_BOX(notif_list), row);
        node = node->next;
    }
}

/* Efface le formulaire d'ajout/modification */
static void clear_form() {
    gtk_editable_set_text(GTK_EDITABLE(entry_title), "");
    gtk_editable_set_text(GTK_EDITABLE(entry_module), "");
    gtk_editable_set_text(GTK_EDITABLE(entry_teacher), "");
    gtk_editable_set_text(GTK_EDITABLE(entry_path), "");
}
/* Callback bouton Quitter */
static void cb_quit(GtkButton *btn, gpointer data) {
    GApplication *app = G_APPLICATION(data);
    g_application_quit(app);
}

/* Callback bouton Ajouter / Sauvegarder */
static void cb_add_support(GtkButton *btn, gpointer data) {
    if (!db_conn) return;
    const char *t = gtk_editable_get_text(GTK_EDITABLE(entry_title));
    const char *m = gtk_editable_get_text(GTK_EDITABLE(entry_module));
    const char *e = gtk_editable_get_text(GTK_EDITABLE(entry_teacher));
    const char *p = gtk_editable_get_text(GTK_EDITABLE(entry_path));
    guint selected = gtk_drop_down_get_selected(GTK_DROP_DOWN(dropdown_type));
    const char *types_list[] = {"PDF", "Video", "PPT", "Note", "Livre"};
    const char *type = types_list[selected];

    if (strlen(t) == 0) return;

    if (current_edit == NULL) {
        ajouter_support_gui(main_list, db_conn, (char*)t, (char*)m, (char*)type, (char*)e, (char*)p);
        add_notif("Nouveau support ajouté.");
    } else {
        Support *s = &current_edit->data;
        strncpy(s->titre, t, sizeof(s->titre)-1);
        strncpy(s->module, m, sizeof(s->module)-1);
        strncpy(s->enseignant, e, sizeof(s->enseignant)-1);
        strncpy(s->chemin_fichier, p, sizeof(s->chemin_fichier)-1);
        strncpy(s->type, type, sizeof(s->type)-1);
        db_update_support(s, db_conn);
        add_notif("Modification enregistrée.");
        current_edit = NULL;
        gtk_button_set_label(GTK_BUTTON(btn_add_action), "Ajouter"); 
    }
    refresh_management_list();
    clear_form();
}

/* Callback bouton Modifier */
static void cb_edit_support(GtkButton *btn, gpointer data) {
    // Récupération de la ligne sélectionnée
    GtkListBoxRow *row = gtk_list_box_get_selected_row(GTK_LIST_BOX(listbox));
    if (!row) return;
    int index = gtk_list_box_row_get_index(row);
    Element *curr = main_list->premier;
    for (int i = 0; i < index && curr; i++) curr = curr->suivant;
    if (curr) {
        // Préparation du formulaire en mode édition
        current_edit = curr;
        gtk_editable_set_text(GTK_EDITABLE(entry_title), curr->data.titre);
        gtk_editable_set_text(GTK_EDITABLE(entry_module), curr->data.module);
        gtk_editable_set_text(GTK_EDITABLE(entry_teacher), curr->data.enseignant);
        gtk_editable_set_text(GTK_EDITABLE(entry_path), curr->data.chemin_fichier);
        gtk_button_set_label(GTK_BUTTON(btn_add_action), "Sauvegarder");
    }
}

/* Callback bouton Supprimer */
static void cb_delete_support(GtkButton *btn, gpointer data) {
    // Récupération de la ligne sélectionnée
    GtkListBoxRow *row = gtk_list_box_get_selected_row(GTK_LIST_BOX(listbox));
    if (!row) {
        add_notif("Sélectionnez un support à supprimer.");
        return;
    }
    
    // Récupération de l'élément sélectionné
    int index = gtk_list_box_row_get_index(row);
    Element *curr = main_list->premier;
    for (int i = 0; i < index && curr; i++) curr = curr->suivant;

    if (curr) {
        int id_del = curr->data.id_support;
        
        
        db_delete_support(id_del, db_conn); 
        
        
        if (removeById(main_list, id_del)) {
            add_notif("Support supprimé avec succès.");
        } else {
            add_notif("Erreur lors de la suppression locale.");
        }

        refresh_management_list();
    }
}
/* Callback activation d'une ligne (consultation) */
static void cb_row_activated(GtkListBox *box, GtkListBoxRow *row, gpointer data) {
    int index = gtk_list_box_row_get_index(row);
    Element *curr = main_list->premier;
    for (int i = 0; i < index && curr; i++) curr = curr->suivant;

    if (curr) {
        // 1. On crée le Pop-up de consultation
        GtkWidget *dialog = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(dialog), "Détails du Support");
        gtk_window_set_default_size(GTK_WINDOW(dialog), 450, 350);
        
        // Empêche l'erreur si la fenêtre parente n'est pas trouvable
        GtkRoot *root = gtk_widget_get_root(GTK_WIDGET(box));
        if (GTK_IS_WINDOW(root)) {
            gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(root));
        }
        gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);

        GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
         
        // Marges
        gtk_widget_set_margin_start(vbox, 25);
        gtk_widget_set_margin_end(vbox, 25);
        gtk_widget_set_margin_top(vbox, 25);
        gtk_widget_set_margin_bottom(vbox, 25);
        
        gtk_window_set_child(GTK_WINDOW(dialog), vbox);

        // Affichage des informations formatées
        char info[1200];
        snprintf(info, sizeof(info), 
            "<span size='large' weight='bold' foreground='#1e3a8a'>Informations Support</span>\n\n"
            "<b>Titre :</b> %s\n"
            "<b>Module :</b> %s\n"
            "<b>Type :</b> %s\n"
            "<b>Enseignant :</b> %s\n"
            "<b>Emplacement :</b> %s\n"
            "<b>Ajouté le :</b> %s",
            curr->data.titre, curr->data.module, curr->data.type, 
            curr->data.enseignant, curr->data.chemin_fichier, curr->data.date_ajout);

        GtkWidget *label = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(label), info);
        gtk_label_set_xalign(GTK_LABEL(label), 0.0);
        gtk_label_set_wrap(GTK_LABEL(label), TRUE); 
        gtk_box_append(GTK_BOX(vbox), label);

        GtkWidget *btn_close = gtk_button_new_with_label("Fermer la consultation");
        gtk_widget_add_css_class(btn_close, "btn-add"); 
        g_signal_connect_swapped(btn_close, "clicked", G_CALLBACK(gtk_window_destroy), dialog);
        gtk_box_append(GTK_BOX(vbox), btn_close);

        gtk_window_present(GTK_WINDOW(dialog));

        // 2. On l'ajoute à la PILE (Stack) d'historique
        push(history_stack, curr->data);
        add_notif("Support consulté ");
    }
}

/* Affiche la fenêtre d'historique */
static void show_history_popup(GtkButton *btn, gpointer data) {
    GtkWidget *win = gtk_window_new();
    
    gtk_window_set_title(GTK_WINDOW(win), "Historique");
    gtk_window_set_default_size(GTK_WINDOW(win), 300, 400);
    GtkWidget *list = gtk_list_box_new();
    StackNode *sn = history_stack->top;
    while(sn) {
        gtk_list_box_append(GTK_LIST_BOX(list), gtk_label_new(sn->data.titre));
        sn = sn->next;
    }
    
    GtkWidget *scroll = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), list);
    gtk_window_set_child(GTK_WINDOW(win), scroll);
    gtk_window_present(GTK_WINDOW(win));
}

/* Rafraîchit la liste des supports dans l'interface de gestion */
static void refresh_management_list() {
    // Clear existing list
    gtk_list_box_remove_all(GTK_LIST_BOX(listbox));
    if (!main_list) return;
    
    // Header row
    Element *curr = main_list->premier;
    while (curr) {
        GtkWidget *row_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        gtk_widget_add_css_class(row_box, "list-row");
        const int w[] = {40, 150, 120, 80, 130, 120};
        char id_str[10]; snprintf(id_str, 10, "%d", curr->data.id_support);
        const char *fields[] = {id_str, curr->data.titre, curr->data.module, curr->data.type, curr->data.enseignant, curr->data.date_ajout};
        for(int i=0; i<6; i++) {
            GtkWidget *l = gtk_label_new(fields[i]);
            gtk_widget_set_size_request(l, w[i], -1);
            gtk_label_set_xalign(GTK_LABEL(l), 0.0);
            gtk_box_append(GTK_BOX(row_box), l);
        }
        gtk_list_box_append(GTK_LIST_BOX(listbox), row_box);
        curr = curr->suivant;
    }
}



/* Fonction principale d'activation de l'application GTK */
static void activate(GtkApplication *app, gpointer data) {
    main_list = initialisation();
    notif_queue = init_queue();
    history_stack = init_stack();
    db_conn = db_connect("localhost", "root", "", "gestion_supports_db", 3306);
    
    // Création de la fenêtre principale
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Gestion des supports de cours - Sup de Co");
    gtk_window_set_default_size(GTK_WINDOW(window), 1150, 800);
    apply_supdeco_theme();

    GtkWidget *main_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_window_set_child(GTK_WINDOW(window), main_hbox);

    // --- SIDEBAR ---
    GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_add_css_class(sidebar, "sidebar");
    gtk_widget_set_size_request(sidebar, 260, -1);
    gtk_box_append(GTK_BOX(main_hbox), sidebar);
    
    // LOGO SUP DE CO
    GtkWidget *logo_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_add_css_class(logo_box, "sidebar-logo-box");
    GtkWidget *logo = gtk_picture_new_for_filename("logo_esitec.png");
    gtk_widget_set_size_request(logo, 180, 180);
    gtk_box_append(GTK_BOX(logo_box), logo);
    gtk_box_append(GTK_BOX(sidebar), logo_box);
    
    // BOUTON GESTION SUPPORTS
    GtkWidget *btn_nav1 = gtk_button_new_with_label("]   Gestion des supports");
    gtk_widget_add_css_class(btn_nav1, "active-btn");
    gtk_box_append(GTK_BOX(sidebar), btn_nav1);

    // BOUTON HISTORIQUE
    GtkWidget *btn_nav2 = gtk_button_new_with_label(")   Historique");
    g_signal_connect(btn_nav2, "clicked", G_CALLBACK(show_history_popup), NULL);
    gtk_box_append(GTK_BOX(sidebar), btn_nav2);

    // AJOUT DU BOUTON QUITTER
    GtkWidget *btn_quit = gtk_button_new_with_label(")   Quitter");
    g_signal_connect(btn_quit, "clicked", G_CALLBACK(cb_quit), app);
    gtk_box_append(GTK_BOX(sidebar), btn_quit);

    //  CONTENT AREA 
    GtkWidget *content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_widget_set_hexpand(content, TRUE);
    gtk_widget_set_margin_start(content, 40);
    gtk_widget_set_margin_end(content, 40);
    gtk_widget_set_margin_top(content, 20);
    gtk_widget_set_margin_bottom(content, 20);
    gtk_box_append(GTK_BOX(main_hbox), content);

    // AJOUT DU TITRE FORMULAIRE
    GtkWidget *lbl_add = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(lbl_add), "<span size='x-large' weight='bold' foreground='#1e3a8a'>Ajouter un support</span>");
    gtk_label_set_xalign(GTK_LABEL(lbl_add), 0.0);
    gtk_box_append(GTK_BOX(content), lbl_add);
    
    // FORMULAIRE D'AJOUT
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
    
    entry_title = gtk_entry_new(); entry_module = gtk_entry_new();
    entry_teacher = gtk_entry_new(); entry_path = gtk_entry_new();
    const char *types[] = {"PDF", "Video", "PPT", "Note", "Livre", NULL};
    dropdown_type = gtk_drop_down_new_from_strings(types);
    
    // Placement des éléments dans la grille
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Titre"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_title, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Module"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_module, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Type"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), dropdown_type, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Enseignant"), 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_teacher, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Chemin du fichier"), 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_path, 1, 4, 1, 1);
    
    // Bouton Ajouter / Sauvegarder
    btn_add_action = gtk_button_new_with_label("Ajouter");
    gtk_widget_add_css_class(btn_add_action, "btn-add");
    gtk_widget_set_halign(btn_add_action, GTK_ALIGN_START);
    g_signal_connect(btn_add_action, "clicked", G_CALLBACK(cb_add_support), NULL);
    gtk_grid_attach(GTK_GRID(grid), btn_add_action, 1, 5, 1, 1);
    gtk_box_append(GTK_BOX(content), grid);

    // AJOUT DU TITRE LISTE
    GtkWidget *lbl_list = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(lbl_list), "<span size='x-large' weight='bold' foreground='#1e3a8a'>Liste des supports</span>");
    gtk_label_set_xalign(GTK_LABEL(lbl_list), 0.0);
    gtk_box_append(GTK_BOX(content), lbl_list);
    
    // TABLEAU DE GESTION DES SUPPORTS
    GtkWidget *table_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
    gtk_box_append(GTK_BOX(content), table_hbox);
    
    // Liste des supports
    GtkWidget *list_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_hexpand(list_vbox, TRUE);
    gtk_box_append(GTK_BOX(table_hbox), list_vbox);
    
    // En-têtes du tableau
    GtkWidget *h_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_add_css_class(h_box, "table-header");
    const char *h_names[] = {"ID", "Titre", "Module", "Type", "Enseignant", "Date d'ajout"};
    for(int i=0; i<6; i++) {
        GtkWidget *l = gtk_label_new(h_names[i]);
        gtk_widget_set_size_request(l, 40 + (i>0?100:0), -1);
        gtk_box_append(GTK_BOX(h_box), l);
    }
    gtk_box_append(GTK_BOX(list_vbox), h_box);
    
    
    listbox = gtk_list_box_new();
    GtkWidget *scroll = gtk_scrolled_window_new();
    gtk_widget_set_size_request(scroll, -1, 250);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), listbox);
    gtk_box_append(GTK_BOX(list_vbox), scroll);
    g_signal_connect(listbox, "row-activated", G_CALLBACK(cb_row_activated), NULL);
    
    // Boutons d'action
    GtkWidget *action_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *btn_edit = gtk_button_new_with_label("Modifier");
    GtkWidget *btn_del = gtk_button_new_with_label("Supprimer");
    GtkWidget *btn_refresh = gtk_button_new_with_label("Actualiser");
    gtk_widget_add_css_class(btn_edit, "action-btn");
    gtk_widget_add_css_class(btn_del, "action-btn");
    gtk_widget_add_css_class(btn_refresh, "action-btn");
    g_signal_connect(btn_edit, "clicked", G_CALLBACK(cb_edit_support), NULL);
    g_signal_connect(btn_del, "clicked", G_CALLBACK(cb_delete_support), NULL);
    g_signal_connect(btn_refresh, "clicked", G_CALLBACK(refresh_management_list), NULL);
    gtk_box_append(GTK_BOX(action_vbox), btn_edit);
    gtk_box_append(GTK_BOX(action_vbox), btn_del);
    gtk_box_append(GTK_BOX(action_vbox), btn_refresh);
    gtk_box_append(GTK_BOX(table_hbox), action_vbox);

    // AJOUT DU TITRE NOTIFICATIONS
    GtkWidget *lbl_notif = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(lbl_notif), "<span size='x-large' weight='bold' foreground='#1e3a8a'>Notifications</span>");
    gtk_label_set_xalign(GTK_LABEL(lbl_notif), 0.0);
    gtk_box_append(GTK_BOX(content), lbl_notif);
    
    // Liste des notifications
    notif_list = gtk_list_box_new();
    GtkWidget *n_scroll = gtk_scrolled_window_new();
    gtk_widget_set_size_request(n_scroll, -1, 100);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(n_scroll), notif_list);
    gtk_box_append(GTK_BOX(content), n_scroll);
    
    // Chargement des supports depuis la BDD
    if(db_conn) db_load_supports(main_list, db_conn);
    refresh_management_list();
    gtk_window_present(GTK_WINDOW(window));
}
/* Fonction principale */
int main(int argc, char *argv[]) {
    
    // Création de l'application GTK
    GtkApplication *app = gtk_application_new("sn.esitec.manager", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    return g_application_run(G_APPLICATION(app), argc, argv);
}