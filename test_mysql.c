#include <stdio.h>
#include <mysql.h>

int main() {
    MYSQL *conn;
    conn = mysql_init(NULL);
    if (!conn) {
        printf("mysql_init() failed\n");
        return 1;
    }

    // Using root / Mansi@123 / mysql database
    if (!mysql_real_connect(conn, "localhost", "root", "Mansi@123", "todo_db", 0, NULL, 0)) {
        fprintf(stderr, "mysql_real_connect() failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    printf("✅ Connected to MariaDB/MySQL!\n");
    mysql_close(conn);
    return 0;
}
