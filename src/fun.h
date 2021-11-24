#include <stdio.h>
#include <mysql.h>
#include <string.h>
#include "xlsxwriter.h"

void finish_with_error(MYSQL *con)
{
    /* 
    Close the mysql connection and
    print any errors
    */
    fprintf(stderr, "%s\n", mysql_error(con));
}

void finish_with_success(MYSQL *con, char *message)
{
    uint64_t num_rows;
    int warnings;
    warnings = mysql_warning_count(con);
    num_rows = mysql_affected_rows(con);
    fprintf(stdout, "%s\n", message);
    fprintf(stdout, "Affected Rows: %d, Warnings: %d\n", num_rows, warnings);
}

void create_db(MYSQL *con, char *dbName)
{
    /*
    Create a database with name "dbName"
    */
    char query[300];
    char success_message[300];
    sprintf(query, "CREATE DATABASE %s", dbName);
    if (mysql_query(con, query))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
    }
    else
    {
        sprintf(success_message, "Successfully created database: %s", dbName);
        finish_with_success(con, success_message);
    }
}

void retrieve_data_to_excel_worksheet(MYSQL *con, lxw_workbook *workbook, char *dbName, char *tbName)
{
    /*
    Retrieve data from a particular table in a database to excel spreadsheet file
    */
    char query[255];
    MYSQL_RES *result;
    lxw_worksheet *worksheet;
    MYSQL_ROW row;
    MYSQL_FIELD *field;
    unsigned int sheetRow = 0;
    unsigned int sheetCol = 0;
    unsigned int sheetRowNext = 1;
    int num_fields = 0;
    sprintf(query, "SELECT * FROM `%s`.`%s`", dbName, tbName);
    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }
    result = mysql_use_result(con);
    if (result == NULL)
    {
        finish_with_error(con);
    }
    else
    {
        num_fields = mysql_num_fields(result);
        worksheet = workbook_add_worksheet(workbook, tbName);
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
    }
}

void retrieve_table_data_to_excel_file(MYSQL *con, char *tbName, char *dbName)
{
    /*
    Retrieve data from a particular table in a database to excel file
    */
    char filename[255];
    lxw_workbook *workbook;

    if (mysql_select_db(con, dbName))
    {
        finish_with_error(con);
    }
    else
    {
        sprintf(filename, "%s.xlsx", dbName);
        workbook = workbook_new(filename);
        printf("%s\n", "Spreadsheet file created...");
        retrieveDataToExcelWorksheet(con, workbook, dbName, tbName);
        printf("%s\n", "Worksheet created, populating with data...");
        workbook_close(workbook);
        finish_with_success(con, "Successfully retrieved data...");
    }
}

void retrieve_db_data_to_excel_file(MYSQL *con, char *dbName)
{

    char filename[255];
    char tbName[255];
    char query[255];
    MYSQL_RES *result;
    MYSQL_ROW row;
    lxw_workbook *workbook;

    if (mysql_select_db(con, dbName))
    {
        finish_with_error(con);
    }
    result = mysql_list_tables(con, NULL);
    if (result == NULL)
    {
        finish_with_error(con);
    }
    else
    {
        sprintf(filename, "%s.xlsx", dbName);
        workbook = workbook_new(filename);

        while ((row = mysql_fetch_row(result)))
        {
            // Result is records of tables in database
            // one column per row hence row[0]
            strcpy(tbName, row[0]);
            if (tbName != NULL)
            {
                printf("%s", tbName);
                retrieveDataToExcelWorksheet(con, workbook, dbName, tbName);
            }
        }
        workbook_close(workbook);
    }
}

void populate_table_with_txt_file(MYSQL *con, char *tbName, char *dbName, char *filePath)
{
    char query[300];

    if (mysql_select_db(con, dbName))
    {
        finish_with_error(con);
    }
    sprintf(query, "LOAD DATA LOCAL INFILE \"%s\" INTO TABLE %s", filePath, tbName);
    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }
}

void populate_table_with_csv_file(MYSQL *con, char *tbName, char *dbName, char *filePath)
{
    char query[300];
    if (mysql_select_db(con, dbName))
    {
        finish_with_error(con);
    }
    sprintf(query, "LOAD DATA LOCAL INFILE \"%s\" INTO TABLE %s \
                        FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '\"' \
                        LINES TERMINATED BY '\n'",
            filePath, tbName);
    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }
}

void populate_table_with_file(MYSQL *con, char *tbName, char *dbName, char *filePath)
{
    // Determine file extension
    char *token;
    char *fileExt;
    char filePathCopy[255];
    strcpy(filePathCopy, filePath);
    token = strtok(filePathCopy, ".");
    while (token != NULL)
    {
        token = strtok(NULL, ".");
        if (token != NULL)
        {
            fileExt = token;
        }
    }
    if (strcmp(fileExt, "txt") == 0)
    {
        //  .txt file
        populateTableWithTxtFile(con, tbName, dbName, filePath);
    }
    else if (strcmp(fileExt, "csv") == 0)
    {
        //  .csv file
        populateTableWithCsvFile(con, tbName, dbName, filePath);
    }
}

int tb_from_single_line_schema_file(MYSQL *con, char *dbName, char *path)
{
    FILE *file;
    char buff[1024];
    if (mysql_select_db(con, dbName))
    {
        finish_with_error(con);
    }
    file = fopen(path, "r");
    if (fgets(buff, 1024, (FILE *)file) != NULL)
    {
        fclose(file);
        if (mysql_query(con, buff))
        {
            finish_with_error(con);
        }
    }
    return 0;
}
