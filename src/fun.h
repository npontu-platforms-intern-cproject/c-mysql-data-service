#include <stdio.h>

void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

void createDb(MYSQL *con, char *dbName)
{
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

void createAndPopulateTable(MYSQL *con, char *tbName, char *dbName)
{
    char query[300];
    sprintf(query, "USE %s", dbName);
    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }

    sprintf(query, "DROP TABLE IF EXISTS %s", tbName);
    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }

    sprintf(query, "CREATE TABLE %s (id INT PRIMARY KEY AUTO_INCREMENT, name VARCHAR(255), price INT)", tbName);
    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }

    sprintf(query, "INSERT INTO %s VALUES(1,'Audi',52642)", tbName);
    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }

    sprintf(query, "INSERT INTO %s VALUES(2,'Mercedes',57127)", tbName);
    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }

    sprintf(query, "INSERT INTO %s VALUES(3,'Skoda',9000)", tbName);
    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }

    sprintf(query, "INSERT INTO %s VALUES(4,'Volvo',29000)", tbName);
    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }

    sprintf(query, "INSERT INTO %s VALUES(5,'Bentley',350000)", tbName);
    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }

    sprintf(query, "INSERT INTO %s VALUES(6,'Citroen',21000)", tbName);
    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }

    sprintf(query, "INSERT INTO %s VALUES(7,'Hummer',41400)", tbName);
    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }

    sprintf(query, "INSERT INTO %s VALUES(8,'Volkswagen',21600)", tbName);
    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }

    mysql_close(con);
}

void retrieveDataFromTable(MYSQL *con, char *tbName, char *dbName)
{
    char query[300];

    sprintf(query, "SELECT * FROM `%s`.`%s`", dbName, tbName);
    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        finish_with_error(con);
    }

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;

    MYSQL_FIELD *field;

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

    mysql_close(con);
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

    fclose(file);
    return 0;
}
