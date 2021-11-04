#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/xlsxwriter.h"
#include "fun.h"




int main(int argc, char *argv[])
{
    if (argc == 1){
        retrieveDataToExcelFile();
    }  
    else if (argc == 2){
        if (atoi(argv[1]) == 1)
        {
            createDb();
        }
        else if (atoi(argv[1]) == 2){
            createAndPopulateTable();
        }
        else if (atoi(argv[1]) == 3){
            retrieveDataFromTable();
        }        
    }
}







