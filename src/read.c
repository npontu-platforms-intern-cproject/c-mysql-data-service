#include <stdio.h>


int main()
{
    FILE *file;
    char buff[255];
    char c;

    file = fopen("test.txt", "r");
    
    while (fgets(buff, 255, (FILE*)file) != NULL)
    {
        printf("%s\n", buff);
    }

    
    fclose(file);
    return 0;
}