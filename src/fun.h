int createDb(char *dbN)
{
    char query[300];
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }

    if (mysql_real_connect(con, "localhost", "ayarma", "/secretKey/12345", NULL, 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    sprintf(query, "CREATE DATABASE %s", dbN);
    if (mysql_query(con, query))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    mysql_close(con);
    exit(0);
}

int retrieveDataToExcelFile(char *tbname, char *dbname)
{
    void finish_with_error(MYSQL * con)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }

    if (mysql_real_connect(con, "localhost", "ayarma", "/secretKey/12345", dbname, 0, NULL, 0) == NULL)
    {
        finish_with_error(con);
    }
    char query[300];
    sprintf(query, "SELECT * FROM %s", tbname);
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
    lxw_workbook *workbook = workbook_new("data-book.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    int sheetRow = 0, sheetCol = 0, sheetRowNext = 1;

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

    exit(0);
}

void retrieveDataFromTable(char *tableName, char *dbN)
{
    char query[300];
    void finish_with_error(MYSQL * con)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }

    if (mysql_real_connect(con, "localhost", "ayarma", "/secretKey/12345",
                           dbN, 0, NULL, 0) == NULL)
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

int populateTable(char *tableName, char *dbN, char *buff)
{
    char query[300];
    void finish_with_error(MYSQL * con)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }

    if (mysql_real_connect(con, "localhost", "ayarma", "/secretKey/12345",
                           dbN, 0, NULL, 0) == NULL)
    {
        finish_with_error(con);
    }

    sprintf(query, buff, tableName);
    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }

    mysql_close(con);
    return 0;
}

int readQrsLine(char *tableName, char *dbN, char *path)
{
    FILE *file;
    char buff[255];

    file = fopen(path, "r");

    createTable(tableName, dbN);

    while (fgets(buff, 255, (FILE *)file) != NULL)
    {
        populateTable(tableName, dbN, buff);
    }

    fclose(file);
    return 0;
}
