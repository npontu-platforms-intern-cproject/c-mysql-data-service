#include <mysql.h>
#include <curses.h>
#include "xlsxwriter.h"
#include <string.h>
#include "fun.h"
#include <curses.h>

#define MAX_LIMIT 1024

char pwd[50];
char user[50];
char host[50];
char ch;
int unsigned port = 3306;
int j, i = 0;
int argc = 0;
char *argv[1600];
char command[MAX_LIMIT];
char *token;
char helpMessage[] = "\nC-MySQL-Data-Service\n \
Usage:\n \
    create-db [database] --creates a new database\n \
    create-table [table] -d [database] --creates a new table in database\n \
    help -- show this help menu\n \
    quit -- quit the program\n";

int main()

{
    // Enter curses mode
    initscr();
    echo();

    // Get database user
    printw("MySQL User: ");
    scanw("%s", &user);

    // Get user's password
    noecho();
    printw("Enter password: ");
    scanw("%s", &pwd);
    echo();

    // Get host
    printw("Host (Defaults to 'localhost' if omitted): ");
    scanw("%s", &host);
    if (strlen(host) == 0)
    {
        strcpy(host, "localhost");
    }

    // Get port number
    printw("Port (Leave blank to use default (3306): ");
    scanw("%d", &port);
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

    // if (mysql_real_connect(con, host, user, pwd, NULL, port, NULL, CLIENT_MULTI_STATEMENTS) == NULL)
    if (mysql_real_connect(con, host, user, pwd, NULL, port, NULL, 0) == NULL)
    {
        finish_with_error(con);
    }

    // Process argument variables
    printf("%s", helpMessage);
    do
    {
        // Read the command line arguments
        if (fgets(command, MAX_LIMIT, stdin) != NULL)
        {
            // Remove trailing newline character from fgets() input
            size_t len = strlen(command);
            if (len > 0 && command[len - 1] == '\n')
            {
                command[len - 1] = '\0';
            }
        }
        // Parse arguments
        token = strtok(command, " ");
        j = 1;
        while (token != NULL)
        {
            argv[j] = token;
            token = strtok(NULL, " ");
            j++;
        }
        argc = j;
        if (argc == 2)
        {
            if (strcmp(argv[1], "help") == 0)
            {
                printf("%s", helpMessage);
            }
            else if (strcmp(argv[1], "quit") == 0)
            {
                printf("Exiting...\n");
                exit(0);
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

        // TODO: Check for unknown arguments

        // Process arguments
        if (argc == 3)
        {
            if (strcmp(argv[1], "create-db") == 0)
            {
                createDb(con, argv[2]);
            }
        }
        if (argc == 5)
        {
            if ((strcmp(argv[1], "create-table") == 0) && (strcmp(argv[3], "-d") == 0))
            {
                createTable(con, argv[2], argv[4]);
            }
        }

        if (argc == 6)
        {
            if ((strcmp(argv[1], "create-table") == 0) && (strcmp(argv[3], "-d") == 0))
            {
                readQrsLine(con, argv[2], argv[4], argv[5]);
            }

            if ((strcmp(argv[1], "retrieve-data") == 0) && (strcmp(argv[2], "-d") == 0) && (strcmp(argv[4], "-t") == 0))
            {
                retrieveDataToExcelFile(con, argv[5], argv[3]);
            }
        }
    } while (1);

    return 0;
}
