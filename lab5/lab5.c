#include <stdio.h>
#include <string.h>
#define TSIZE 128
int table[100][TSIZE];
char terminal[TSIZE], nonterminal[26];
struct product {
    char str[100];
    int len;
} pro[20];

int no_pro;
char first_rhs[100][TSIZE];

int is_non_terminal(char c) {
    return c >= 'A' && c <= 'Z';
}

void readFromFile() {
    FILE* fptr;
    fptr = fopen("cfg.txt", "r");
    char buffer[255];
    int i, j;
    while (fgets(buffer, sizeof(buffer), fptr)) {
        printf("%s", buffer);
        j = 0;
        nonterminal[buffer[0] - 'A'] = 1;
        for (i = 0; i < strlen(buffer) - 1; ++i) {
            if (buffer[i] == '|') {
                ++no_pro;
                pro[no_pro - 1].str[j] = '\0';
                pro[no_pro - 1].len = j;
                pro[no_pro].str[0] = pro[no_pro - 1].str[0];
                pro[no_pro].str[1] = pro[no_pro - 1].str[1];
                pro[no_pro].str[2] = pro[no_pro - 1].str[2];
                j = 3;
            }
            else {
                pro[no_pro].str[j] = buffer[i];
                ++j;
                if (!is_non_terminal(buffer[i]) && buffer[i] != '-' && buffer[i] != '>') {
                    terminal[buffer[i]] = 1;
                }
            }
        }
        pro[no_pro].len = j;
        ++no_pro;
    }
}

void computeParsingTable() {
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < TSIZE; ++j) {
            table[i][j] = -1; // Initialize the parsing table entries to -1
        }
    }

    for (int i = 0; i < no_pro; ++i) {
        char X = pro[i].str[0];

        // For each terminal 'a' in FIRST(X), add i to M[X, a]
        for (int j = 3; j < pro[i].len; ++j) {
            char a = pro[i].str[j];
            if (!is_non_terminal(a)) {
                table[X][a] = i;
                break;
            }
        }

        // If ε is in FIRST(X), add i to M[X, a] for all 'a' in FOLLOW(X)
        if (pro[i].str[3] == '^') {
            for (int j = 0; j < TSIZE; ++j) {
                if (pro[i].str[0] == 'S' && j == '$') {
                    table[X][j] = i;
                }
                else if (is_non_terminal(j) && table[X][j] == -1) {
                    table[X][j] = i;
                }
            }
        }
    }
}

int main() {
    readFromFile();
    computeParsingTable();
    
    printf("\n\t**************** LL(1) PARSING TABLE *******************\n");
    printf("\t--------------------------------------------------------\n");
    printf("%-10s", "");
    for (int i = 0; i < TSIZE; ++i) {
        if (terminal[i]) printf("%-10c", i);
    }
    printf("\n");

    for (int i = 0; i < no_pro; ++i) {
        if (i == 0 || (pro[i - 1].str[0] != pro[i].str[0])) {
            char c = pro[i].str[0];
            printf("%-10c", c);
            for (int j = 0; j < TSIZE; ++j) {
                if (table[c][j] != -1) {
                    printf("%-10s", pro[table[c][j]].str);
                }
                else if (terminal[j]) {
                    printf("%-10s", "");
                }
            }
            printf("\n");
        }
    }
}
