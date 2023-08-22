// append.c
#include <stdio.h>

int main()
{
    FILE *ptr;
    char sentence[100];

    ptr = fopen("/home/devansh/Desktop/program/output.txt", "a");
    if (ptr == NULL)
    {
        printf("Error! Unable to open the file.\n");
        return 1;
    }

    printf("Enter a sentence to append to the file:\n");
    fgets(sentence, sizeof(sentence), stdin);

    fputs(sentence, ptr);

    fclose(ptr);
    return 0;
}
