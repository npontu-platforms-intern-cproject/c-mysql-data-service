#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
char *argv[1600];
char command[MAX_LIMIT];
char *token;
char helpMessage[] = "\nC-MySQL-Data-Service\n \
Usage:\n \
    create-db [database] --creates a new database\n \
    create-table [table] -d [database] --creates a new table in database\n \
    create-table [table] -d [database] [filepath] --creates a new table in database with data from the given file\n \
    retrieve-data -d [database] -t  --retrieves the table from the database\n \
    retrieve-data -d [database] [filepath] --retrieves the table from the database and writes it to the given file\n \
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

    // Set options
    // 1. Enable load data local infile in current directory
    mysql_options(con, MYSQL_OPT_LOCAL_INFILE, &k);
    mysql_options(con, MYSQL_OPT_LOAD_DATA_LOCAL_DIR, ".");

    // Connect to MySQL
    if (mysql_real_connect(con, host, user, pwd, NULL, port, NULL, 0) == NULL)
    {
        fprintf(stderr, "Failed to connect to database: Error: %s\n",
                mysql_error(con));
        exit(1);
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

        // Process arguments
        if (argc >= 2)
        {
            if (strcmp(argv[1], "create-db") == 0)
                ;
            else if (strcmp(argv[1], "create-table") == 0)
                ;
            else if (strcmp(argv[1], "retrieve-database") == 0)
                ;
            else if (strcmp(argv[1], "load-data") == 0)
                ;
            else if (strcmp(argv[1], "help") == 0)
            {
                printf("%s", helpMessage);
            }
            else if (strcmp(argv[1], "quit") == 0)
            {
                printf("Exiting...\n");
                exit(0);
            }
            else
            {
                // Unknown arguments
                printf("Unknown command: %s\n", argv[1]);
                printf("%s", helpMessage);
            }
        }

        if (argc == 3)
        {
            if (strcmp(argv[1], "create-db") == 0)
            {
                create_db(con, argv[2]);
            }
        }

        if (argc == 5)
        {
            if ((strcmp(argv[1], "create-table") == 0) && (strcmp(argv[3], "-d") == 0))
            {
                create_table(con, argv[2], argv[4]);
            }
            else if (strcmp(argv[1], "create-table") == 0 && (strcmp(argv[2], "-d") == 0))
            {
                printf("It's working");
                tb_from_single_line_schema_file(con, argv[3], argv[4]);
            }
        }

        if (argc == 6)
        {
            if ((strcmp(argv[1], "create-table") == 0) && (strcmp(argv[3], "-d") == 0))
            {
                read_qrs_line(con, argv[2], argv[4], argv[5]);
            }
            else if ((strcmp(argv[1], "retrieve-data") == 0) && (strcmp(argv[2], "-d") == 0) && (strcmp(argv[4], "-t") == 0))
            {
                retieve_data_to_excel_file(con, argv[5], argv[3]);
            }
        }

        if (argc == 7)
        {
            if ((strcmp(argv[1], "load-data") == 0) && (strcmp(argv[2], "-d") == 0) && (strcmp(argv[4], "-t") == 0))
            {
                populate_table_with_file(con, argv[5], argv[3], argv[6]);
            }
        }
    } while (1);

    return 0;
}
