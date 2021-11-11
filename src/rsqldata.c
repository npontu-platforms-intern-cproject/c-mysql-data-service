#include <mysql.h>
#include <curses.h>
#include "xlsxwriter.h"
#include <string.h>

#include "fun.h"
#include <curses.h>

void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

char pwd[50];
char user[50];
char host[50];
char ch;
int unsigned port = 3306;
int i = 0;

int main(int argc, char *argv[])

{
    // Enter curses mode
    initscr();
    echo();
    // Get database user
    printw("\nMySQL User: ");
    scanw("%s", &user);
    printw("\nuser: %s", user);

    // Get user's password
    noecho();
    printw("\nUser's password: ");
    scanw("%s", &pwd);
    printw("\npassword: %s\n", pwd);

    // Get host
    echo();
    printw("\nHost (Defaults to 'localhost' if omitted): ");
    while (1)
    {
        ch = getch();
        if (ch == '\n' || ch == '\t')
        {
            host[i] = '\0';
            break;
        }
        else
        {
            host[i] = ch;
            i += 1;
        }
    }
    if (host[0] == '\0')
    {
        strcpy(host, "localhost");
    }
    printw("\nhost: %s", host);

    // Get port number
    printw("\nPort (Leave blank to use default (3306): ");
    scanw("%d", &port);

    printw("\nPort: %d", port);
    getch();

    // Leave curses mode
    endwin();

    // Initialize and connect to database
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }

    if (mysql_real_connect(con, host, user, pwd, NULL, port, NULL, 0) == NULL)
    {
        finish_with_error(con);
    }

    // Process argument variables
    if (argc == 3)
    {
        if (strcmp(argv[1], "create-db") == 0)
        {
            createDb(argv[2]);
        }
    }
    if (argc == 5)
    {
        createTable(argv[2], argv[4]);
    }
    if (argc == 6)
    {
        if ((strcmp(argv[1], "create-table") == 0) && (strcmp(argv[3], "-d") == 0))
        {
            readQrsLine(argv[2], argv[4], argv[5]);
        }

        if ((strcmp(argv[1], "retrieve-data") == 0) && (strcmp(argv[2], "-d") == 0) && (strcmp(argv[4], "-t") == 0))
        {
            retrieveDataToExcelFile(argv[5], argv[3]);
        }
    }
}
