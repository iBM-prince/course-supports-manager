#ifndef MYSQL_HANDLER_H
#define MYSQL_HANDLER_H

#include <mariadb/mysql.h>
#include "../structures/linked_list.h"

MYSQL *db_connect(const char *host, const char *user, const char *pass,
                  const char *dbname, unsigned int port);

void db_close(MYSQL *conn);

int db_load_supports(Liste *l, MYSQL *conn);
int db_insert_support(const Support *s, MYSQL *conn);
int db_update_support(const Support *s, MYSQL *conn);
int db_delete_support(int id, MYSQL *conn);

#endif