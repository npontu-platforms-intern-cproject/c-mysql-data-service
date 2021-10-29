#include "xlsxwriter.h"

int main()
{

    lxw_workbook *workbook = workbook_new("data-book.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_string(worksheet, 0, 0, "Hello", NULL);
    worksheet_write_number(worksheet, 1, 0, 123, NULL);

    workbook_close(workbook);

    return 0;
}
