#include "mysql_handler.h"
#include <stdio.h>
#include <string.h>



// CONNECT DATABASE

MYSQL *db_connect(const char *host, const char *user, const char *pass,
                  const char *dbname, unsigned int port) {
    MYSQL *conn = mysql_init(NULL);
    if (!conn) {
        fprintf(stderr, "mysql_init() failed\n");
        return NULL;
    }
    char mode = 0;
    mysql_options(conn, MYSQL_OPT_SSL_VERIFY_SERVER_CERT, &mode);

    if (!mysql_real_connect(conn, host, user, pass, dbname, port, NULL, 0)) {
        fprintf(stderr, "mysql_real_connect() failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return NULL;
    }
    return conn;
}

void db_close(MYSQL *conn) {
    if (conn) mysql_close(conn);
}

// FUNCTION LOADING SUPPORTS FROM THE DATABASE

int db_load_supports(Liste *l, MYSQL *conn) {
    const char *query = "SELECT id, titre, module, type, enseignant, chemin, date_ajout FROM supports ORDER BY id DESC";
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "SELECT failed: %s\n", mysql_error(conn));
        return -1;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (!result) {
        fprintf(stderr, "mysql_store_result failed: %s\n", mysql_error(conn));
        return -1;
    }

    MYSQL_ROW row;
    int num_fields = mysql_num_fields(result);
    while ((row = mysql_fetch_row(result))) {
        unsigned long *lengths = mysql_fetch_lengths(result);
        
        Support s;
        s.id_support = atoi(row[0]);
        strncpy(s.titre, row[1] ? row[1] : "", sizeof(s.titre)-1);
        s.titre[sizeof(s.titre)-1] = 0;
        strncpy(s.module, row[2] ? row[2] : "", sizeof(s.module)-1);
        s.module[sizeof(s.module)-1] = 0;
        strncpy(s.type, row[3] ? row[3] : "", sizeof(s.type)-1);
        s.type[sizeof(s.type)-1] = 0;
        strncpy(s.enseignant, row[4] ? row[4] : "", sizeof(s.enseignant)-1);
        s.enseignant[sizeof(s.enseignant)-1] = 0;
        strncpy(s.chemin_fichier, row[5] ? row[5] : "", sizeof(s.chemin_fichier)-1);
        s.chemin_fichier[sizeof(s.chemin_fichier)-1] = 0;
        strncpy(s.date_ajout, row[6] ? row[6] : "", sizeof(s.date_ajout)-1);
        s.date_ajout[sizeof(s.date_ajout)-1] = 0;
        
        addBack(l, s);
    }
    mysql_free_result(result);
    return 0;
}

// FUNCTION FOR INSERTION IN DATABASE

int db_insert_support(const Support *s, MYSQL *conn) {
    char query[1024];
    snprintf(query, sizeof(query),
             "INSERT INTO supports (titre, module, type, enseignant, chemin, date_ajout) "
             "VALUES ('%s', '%s', '%s', '%s', '%s', '%s')",
             s->titre, s->module, s->type, s->enseignant, s->chemin_fichier, s->date_ajout);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "INSERT failed: %s\n", mysql_error(conn));
        return -1;
    }
    return 0;
}
// FUNCTION FOR UPDATE A SUPPORT

int db_update_support(const Support *s, MYSQL *conn) {
    char query[2048];
    snprintf(query, sizeof(query),
             "UPDATE supports SET titre='%s', module='%s', type='%s', "
             "enseignant='%s', chemin='%s' WHERE id=%d",
             s->titre, s->module, s->type, s->enseignant, s->chemin_fichier, s->id_support);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "UPDATE failed: %s\n", mysql_error(conn));
        return -1;
    }
    return 0;
}

// FUNCTION FOR DELETE A SUPPORT

int db_delete_support(int id, MYSQL *conn) {
    char query[256];
    snprintf(query, sizeof(query), "DELETE FROM supports WHERE id=%d", id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "DELETE failed: %s\n", mysql_error(conn));
        return -1;
    }
    return 0;
}