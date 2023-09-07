#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

char input[100];
int token = 0;   // Current token count

// Function to get the next token from user input
void get_token() {
    if (input[token] != '\0') {
        token++;
    }
}

// Error function
void error() {
    printf("Error: Invalid input\n");
    exit(1);
}

// Function for nonterminal S
bool fS();

// Function for nonterminal A
bool fA();

// Function for nonterminal B
bool fB();

int main() {
    printf("Enter a string: ");
    scanf("%s", input);

    // Start parsing from S
    if (fS() && input[token] == '$' && input[token + 1] == '\0') {
        printf("Accepted\n");
    } else {
        printf("Rejected\n");
    }

    return 0;
}

bool fS() {
    switch (input[token]) {
        case 'a':
            get_token();
            if (fA()) {
                if (fS()) {
                    return true; // 'a' followed by fA and fS
                }
            }
            break;
        case 'c':
            get_token();
            return true; // 'c'
        default:
            error();
    }
    return false; // Invalid input
}

bool fA() {
    switch (input[token]) {
        case 'b':
            get_token();
            if (input[token] == 'a') {
                get_token();
                return true; // 'ba'
            } else {
                error();
            }
            break;
        case 'a':
        case 'c':
            if (fS()) {
                if (fB()) {
                    return true; // fS followed by fB
                }
            }
            break;
        default:
            error();
    }
    return false; // Invalid input
}

bool fB() {
    switch (input[token]) {
        case 'b':
            get_token();
            if (fA()) {
                return true; // fA
            }
            break;
        case 'a':
        case 'c':
            if (fS()) {
                return true; // fS
            }
            break;
        default:
            error();
    }
    return false; // Invalid input
}
