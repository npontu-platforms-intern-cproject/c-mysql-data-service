#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>

#include "xlsxwriter.h"

void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

int main()
{
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }

    if (mysql_real_connect(con, "localhost", "ayarma", "/secretKey/12345",
                           "testdb", 0, NULL, 0) == NULL)
    {
        finish_with_error(con);
    }

    if (mysql_query(con, "SELECT * FROM cars"))
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

    while ((row = mysql_fetch_row(result)))
    {
        for (int i = 0; i < num_fields; i++)
        {
            if (i == 0)
            {
                while (field = mysql_fetch_field(result))
                {
                    // printf("%s ", field->name);
                    for (int k = 0; k < num_fields; k++)
                    {
                        worksheet_write_string(worksheet, 0, k, row[k], NULL);
                    }
                }

                printf("\n");
            }

            // printf("%s ", row[i] ? row[i] : "NULL");
            worksheet_write_string(worksheet, 0, i, row[i], NULL);
        }
    }
    printf("\n");

    mysql_free_result(result);
    workbook_close(workbook);
    mysql_close(con);

    exit(0);
}