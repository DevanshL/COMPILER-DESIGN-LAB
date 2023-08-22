#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void processString(FILE *ptr);
int isKeyword(char *str);

int main() {
    FILE *inputPtr, *outputPtr;
    char ch, prevCh = ' ';
    char currentStr[20];
    char currentNum[10];
    int j = 0;
    int i = 0;
    int flag = 0;
    int state = 0;

    inputPtr = fopen("input.c", "r");
    if (inputPtr == NULL) {
        printf("File can't be opened\n");
        return 1;
    }

    outputPtr = fopen("output.c", "w");
    if (outputPtr == NULL) {
        printf("File can't be opened\n");
        return 1;
    }

    while ((ch = fgetc(inputPtr)) != EOF) {
        if ((ch == '/' && prevCh == '/')) {
            // Handle single-line comments
            while (ch != '\n') {
                ch = fgetc(inputPtr);
            }
        } else if (ch == '*' && prevCh == '/') {
            // Handle multi-line comments
            while (ch != '/' || prevCh != '*') {
                prevCh = ch;
                ch = fgetc(inputPtr);
            }
        } else if (ch == '#') {
            // Handle preprocessor directives
            while (ch != '>') {
                ch = fgetc(inputPtr);
            }
        } else {
            // Copy non-commented characters to the output file
            if (ch != '\n' && ch != ' ' && ch != '/' && ch != '\t') {
                fputc(ch, outputPtr);
            }
        }

        prevCh = ch;
    }

    fclose(outputPtr);
    fclose(inputPtr);

    inputPtr = fopen("output.c", "r");
    if (inputPtr == NULL) {
        printf("File can't be opened\n");
        return 1;
    }

    while ((ch = getc(inputPtr)) != EOF) {
        switch (state) {
            case 0:
                if (isalpha(ch)) {
                    state = 1;
                    currentStr[i] = ch;
                    flag = isKeyword(currentStr);
                    if (flag == 1) {
                        printf("%s = keyword\n", currentStr);
                        memset(currentStr, '\0', sizeof(currentStr));
                        i = -1;
                    }
                    i++;
                } else {
                    if (ch == '%' || ch == '/' || ch == '*' || ch == '+' || ch == '-') {
                        printf("%c = operator\n", ch);
                    } else if (isdigit(ch)) {
                        currentNum[j] = ch;
                        j++;
                    } else if (ch == '"') {
                        printf("%c = special character\n", ch);
                        processString(inputPtr);
                    } else {
                        printf("%c is a special character\n", ch);
                    }

                    state = 0;
                }
                break;

            case 1:
                if (isalnum(ch)) {
                    state = 1;
                    currentStr[i] = ch;
                    flag = isKeyword(currentStr);
                    if (flag == 1) {
                        printf("%s = keyword\n", currentStr);
                        memset(currentStr, '\0', sizeof(currentStr));
                        i = -1;
                    }
                    i++;

                    if (strlen(currentNum) != 0) {
                        printf("%s = constant\n", currentNum);
                        memset(currentNum, '\0', sizeof(currentNum));
                        j = 0;
                    }
                } else {
                    state = 0;

                    if (strlen(currentNum) != 0) {
                        printf("%s = constant\n", currentNum);
                        memset(currentNum, '\0', sizeof(currentNum));
                        j = 0;
                    }

                    flag = isKeyword(currentStr);
                    if (flag == 1) {
                        printf("%s = keyword\n", currentStr);
                    } else {
                        printf("%s = identifier\n", currentStr);
                    }

                    if (ch == '%' || ch == '/' || ch == '*' || ch == '+' || ch == '-') {
                        printf("%c = operator\n", ch);
                    } else if (ch == '"') {
                        printf("%c = special character\n", ch);
                        processString(inputPtr);
                    } else {
                        printf("%c = special character\n", ch);
                    }

                    memset(currentStr, '\0', sizeof(currentStr));
                    i = 0;
                }
                break;
        }
    }

    fclose(inputPtr);
    return 0;
}

void processString(FILE *ptr) {
    char ch;
    while ((ch = getc(ptr)) != '"') {
        if (ch != '"') {
            printf("%c", ch);
        }
    }
    printf(" = String\n");
    printf("%c = special character\n", ch);
}

int isKeyword(char *str) {
    char keywords[32][10] = {
        "auto", "break", "case", "char", "const", "continue", "default", "do",
        "double", "else", "extern", "float", "for", "goto", "if", "int",
        "long", "register", "return", "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
    };

    for (int i = 0; i < 32; i++) {
        if (!strcmp(str, keywords[i])) {
            return 1;
        }
    }

    return 0;
}

