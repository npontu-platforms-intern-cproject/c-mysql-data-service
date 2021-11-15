#include <stdio.h>
#include <mysql.h>
#include "xlsxwriter.h"

void finish_with_error(MYSQL *con)
{
    /* 
    Close the mysql connection and
    print any errors
    */
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

void createDb(MYSQL *con, char *dbName)
{
    /*
    Create a database with name "dbName"
    */
    char query[300];

    sprintf(query, "CREATE DATABASE %s", dbName);
    if (mysql_query(con, query))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    mysql_close(con);
}

int createTable(MYSQL *con, char *tableName, char *dbN)
{
    /*
    Create a table called tableName in the database dbN
    */
    char query[300];
    sprintf(query, "USE %s", dbN);
    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }

    sprintf(query, "DROP TABLE IF EXISTS %s", tableName);
    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }

    sprintf(query, "CREATE TABLE %s(id INT PRIMARY KEY AUTO_INCREMENT, name VARCHAR(255), price INT)", tableName);
    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }

    return 0;
}

void retrieveDataFromTable(MYSQL *con, char *tbName, char *dbName)
{
    /*
    Retrieve data from a particular table in database
    */
    char query[300];
    MYSQL_RES *result;
    int num_fields;
    MYSQL_ROW row;
    MYSQL_FIELD *field;

    sprintf(query, "SELECT * FROM `%s`.`%s`", dbName, tbName);
    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }
    result = mysql_store_result(con);
    if (result == NULL)
    {
        finish_with_error(con);
    }
    num_fields = mysql_num_fields(result);
    while ((row = mysql_fetch_row(result)))
    {
        for (int i = 0; i < num_fields; i++)
        {
            if (i == 0)
            {
                while ((field = mysql_fetch_field(result)))
                {
                    printf("%s ", field->name);
                }

                printf("\n");
            }
            printf("%s ", row[i] ? row[i] : "NULL");
        }
    }
    printf("\n");
    mysql_free_result(result);
    mysql_close(con);
}

void retrieveDataToExcelFile(MYSQL *con, char *tbName, char *dbName)
{
    /*
    Retrieve data from a particular table in a database to excel file
    */
    char query[256];
    char filename[256];
    MYSQL_RES *result;
    lxw_workbook *workbook;
    lxw_worksheet *worksheet;
    MYSQL_ROW row;
    MYSQL_FIELD *field;
    int sheetRow = 0, sheetCol = 0, sheetRowNext = 1;
    int num_fields = 0;

    sprintf(query, "SELECT * FROM `%s`.`%s`", dbName, tbName);
    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }

    result = mysql_store_result(con);

    if (result == NULL)
    {
        finish_with_error(con);
    }

    num_fields = mysql_num_fields(result);

    sprintf(filename, "%s.xlsx", tbName);
    workbook = workbook_new(filename);
    worksheet = workbook_add_worksheet(workbook, NULL);

    while ((row = mysql_fetch_row(result)))
    {
        for (int i = 0; i < num_fields; i++)
        {
            if (sheetRow == 0)
            {
                while ((field = mysql_fetch_field(result)))
                {
                    worksheet_write_string(worksheet, 0, sheetCol, field->name, NULL);
                    sheetCol += 1;
                }
            }
            worksheet_write_string(worksheet, sheetRowNext, i, row[i], NULL);
        }
        sheetRowNext += 1;
    }
    mysql_free_result(result);
    workbook_close(workbook);
    mysql_close(con);
}

int populateTable(MYSQL *con, char *tableName, char *buff)
{
    char query[300];

    sprintf(query, buff, tableName);
    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }
    return 0;
}

int readQrsLine(MYSQL *con, char *tableName, char *dbN, char *path)
{
    FILE *file;
    char buff[255];

    file = fopen(path, "r");

    createTable(con, tableName, dbN);

    while (fgets(buff, 255, (FILE *)file) != NULL)
    {
        populateTable(con, tableName, buff);
    }
    mysql_close(con);
    fclose(file);
    return 0;
}
