#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <mysql.h>
#include <curses.h>
#include "fun.h"

#define MAX_LIMIT 1024

char pwd[50];
char user[50];
char host[50];
char ch;
unsigned int port = 3306;
unsigned int k = 1;
int j, i = 0;
int argc = 0;
char *argv[10];
char command[MAX_LIMIT];
char *token;
bool reconnect = 1;
char helpMessage[] = "\nC-MySQL-Data-Service\n \
Usage:\n \
    create-db [database] \n \
        -- creates a new database\n \
    create-table -d [database] [filepath]\n \
        -- creates a new table in database with schema from the given file\n \
    retrieve-data -d [database] -t [table]\n \
        -- retrieves data from table in database\n \
    retrieve-data -d [database] \n \
        -- retrieves the all data from all tables in database\n \
    load-data -d [database] -t [table] [path-to-txt-or-csv-file]\n \
        -- populate table in database with data from file\n \
    list-dbs [regex]\n \
        -- Returns a result set consisting of database names on the server\n \
            that match the simple regular expression regex.\n \
            regex may contain the wildcard characters \% or _ \n \
            Omitting regex assumes all databases \n \
    list-tables -d [database] [regex]\n \
        -- Returns a result set consisting of database names on the server\n \
            that match the simple regular expression specified by regex\n \
            regex may contain the wildcard characters \% or _ \n \
            Omitting regex assumes all tables in database\n \
    help -- show this help menu\n \
    quit -- quit the program\n";

void sytax_error();

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

    // Leave curses mode
    refresh();
    endwin();

    // Initialize and connect to database
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }

    // Set options
    // 1. Enable load data local infile in current directory
    mysql_options(con, MYSQL_OPT_LOCAL_INFILE, &k);
    mysql_options(con, MYSQL_OPT_LOAD_DATA_LOCAL_DIR, ".");

    // 1. Enable reconnection of mysql
    mysql_options(con, MYSQL_OPT_RECONNECT, &reconnect);

    // Connect to MySQL
    if (mysql_real_connect(con, host, user, pwd, NULL, port, NULL, 0) == NULL)
    {
        fprintf(stderr, "Failed to connect to database: Error: %s\n",
                mysql_error(con));
        mysql_library_end();
        exit(1);
    }

    // Process argument variables
    printf("%s\n", helpMessage);
    do
    {
        // Read the command line arguments
        printf("%s", "mysql-data-service> ");
        if (fgets(command, MAX_LIMIT, stdin) != NULL)
        {
            // Remove trailing newline character from fgets() input
            size_t len = strlen(command);
            if (len > 0 && command[len - 1] == '\n')
            {
                command[len - 1] = '\0';
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
        }

        // Check for unknown arguments
        if (argc >= 2)
        {
            if (
                (strcmp(argv[1], "create-db") == 0) || (strcmp(argv[1], "create-table") == 0) || (strcmp(argv[1], "retrieve-data") == 0) || (strcmp(argv[1], "load-data") == 0) || (strcmp(argv[1], "list-tables") == 0) || (strcmp(argv[1], "list-dbs") == 0) || (strcmp(argv[1], "help") == 0) || (strcmp(argv[1], "quit") == 0))
                ;
            else
            {
                printf("ERROR (Unknown command): %s\n", argv[1]);
            }
        }

        // Process arguments
        if (argc == 2)
        {
            if (strcmp(argv[1], "help") == 0)
            {
                printf("%s\n", helpMessage);
            }
            else if (strcmp(argv[1], "quit") == 0)
            {
                mysql_close(con);
                mysql_library_end();
                printf("%s\n", "Exiting...");
                exit(0);
            }
            else if (strcmp(argv[1], "list-dbs") == 0)
            {
                list_dbs(con, NULL);
            }
            else
            {
                sytax_error();
            }
        }
        else if (argc == 3)
        {
            if (strcmp(argv[1], "list-dbs") == 0)
            {
                list_dbs(con, argv[2]);
            }
            else if (strcmp(argv[1], "create-db") == 0)
            {
                create_db(con, argv[2]);
            }
            else
            {
                sytax_error();
            }
        }

        else if (argc == 4)
        {
            if ((strcmp(argv[1], "list-tables") == 0) && (strcmp(argv[2], "-d") == 0))
            {
                list_tables(con, argv[3], NULL);
            }
            else if ((strcmp(argv[1], "retrieve-data") == 0) && (strcmp(argv[2], "-d") == 0))
            {
                retrieve_db_data_to_excel_file(con, argv[3]);
            }
            else
            {
                sytax_error();
            }
        }

        else if (argc == 5)
        {
            if ((strcmp(argv[1], "list-tables") == 0) && (strcmp(argv[2], "-d") == 0))
            {
                list_tables(con, argv[3], argv[4]);
            }
            else if (strcmp(argv[1], "create-table") == 0 && (strcmp(argv[2], "-d") == 0))
            {
                tb_from_schema_file(con, argv[3], argv[4]);
            }
            else
            {
                sytax_error();
            }
        }

        else if (argc == 6)
        {
            if ((strcmp(argv[1], "retrieve-data") == 0) && (strcmp(argv[2], "-d") == 0) && (strcmp(argv[4], "-t") == 0))
            {
                retrieve_table_data_to_excel_file(con, argv[5], argv[3]);
            }
            else
            {
                sytax_error();
            }
        }

        else if (argc == 7)
        {
            if ((strcmp(argv[1], "load-data") == 0) && (strcmp(argv[2], "-d") == 0) && (strcmp(argv[4], "-t") == 0))
            {
                populate_table_with_file(con, argv[5], argv[3], argv[6]);
            }
            else
            {
                sytax_error();
            }
        }

    } while (1);

    return 0;
}

void sytax_error()
{
    printf("%s\n", "ERROR: Please check your syntax.");
    printf("%s\n", "Type 'help' to show available commands");
}
