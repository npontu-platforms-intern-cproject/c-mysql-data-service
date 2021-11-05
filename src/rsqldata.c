#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include "xlsxwriter.h"
#include "fun.h"
// #include "conio.h"
#include <ncurses.h>

int main(int argc, char *argv[])
{
    int i = 0;
    char ch;
    char password[256];
    printf("Enter password: ");
    while ((ch = getch()) != 13)
    {
        password[i] = ch;
        i += 1;
        printf("*");
    }
    password[i] = '\0';

    char username[256];
    printf("Enter Username: ");
    scanf("%s", username);
    printf("Username is: %s", username);
    printf("Password is: %s", password);

    if (argc == 1)
    {
        retrieveDataToExcelFile();
    }
    else if (argc == 2)
    {
        if (atoi(argv[1]) == 1)
        {
            createDb();
        }
        else if (atoi(argv[1]) == 2)
        {
            createAndPopulateTable();
        }
        else if (atoi(argv[1]) == 3)
        {
            retrieveDataFromTable();
        }
    }
}
