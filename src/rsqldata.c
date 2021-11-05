#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include "xlsxwriter.h"

#include "fun.h"

int main(int argc, char *argv[])
{
    if (argc == 3)
    {
        if (strcmp(argv[1], "create-db") == 0)
        {
            createDb(argv[2]);
        }
    }
    if (argc == 5)
    {
        if ((strcmp(argv[1], "create-table") == 0) && (strcmp(argv[1], "-d") == 0))
        {
            createAndPopulateTable(argv[2], argv[4]);
        }
    }
    if (argc == 6)
    {
        if ((strcmp(argv[1], "retrieve-data") == 0) && (strcmp(argv[2], "-d") == 0) && (strcmp(argv[4], "-t") == 0))
        {
            retrieveDataFromTable(argv[5], argv[3]);
            retrieveDataToExcelFile(argv[5], argv[3]);
        }
    }
}