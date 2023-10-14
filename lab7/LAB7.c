#include <stdio.h>
#include <string.h>
#define TSIZE 128
#define STACK_SIZE 1000

int table[100][TSIZE];
int parse_table[26][TSIZE];
char terminal[TSIZE];
char nonterminal[26];

struct product
{
    char str[100];
    int len;
} pro[20];

int no_pro;
char first[26][TSIZE];
char follow[26][TSIZE];
char first_rhs[100][TSIZE];

int isNT(char c)
{
    return c >= 'A' && c <= 'Z';
}

void initStack(char stack[], int *top)
{
    *top = -1;
}

void push(char stack[], int *top, char symbol)
{
    if (*top < STACK_SIZE - 1)
    {
        stack[++(*top)] = symbol;
    }
    else
    {
        printf("Stack overflow\n");
    }
}

char pop(char stack[], int *top)
{
    if (*top >= 0)
    {
        return stack[(*top)--];
    }
    else
    {
        printf("Stack underflow\n");
        return '\0'; // Return a sentinel value for error handling
    }
}

void readFromFile(char *filename)
{
    FILE *fptr;
    fptr = fopen(filename, "r");
    char buffer[255];
    int i;
    int j;
    while (fgets(buffer, sizeof(buffer), fptr))
    {
        j = 0;
        nonterminal[buffer[0] - 'A'] = 1;
        for (i = 0; i < strlen(buffer) - 1; ++i)
        {
            pro[no_pro].str[j] = buffer[i];
            ++j;
            if (!isNT(buffer[i]) && buffer[i] != ' ' && buffer[i] != '=')
            {
                terminal[buffer[i]] = 1;
            }
        }
        pro[no_pro].len = j;
        ++no_pro;
    }
    fclose(fptr);
}

void add_FIRST_A_to_FOLLOW_B(char A, char B)
{
    int i;
    for (i = 0; i < TSIZE; ++i)
    {
        if (i != '^')
            follow[B - 'A'][i] = follow[B - 'A'][i] || first[A - 'A'][i];
    }
}

void add_FOLLOW_A_to_FOLLOW_B(char A, char B)
{
    int i;
    for (i = 0; i < TSIZE; ++i)
    {
        if (i != '^')
            follow[B - 'A'][i] = follow[B - 'A'][i] || follow[A - 'A'][i];
    }
}

void FOLLOW()
{
    int t = 0;
    int i, j, k, x;
    while (t++ < no_pro)
    {
        for (k = 0; k < 26; ++k)
        {
            if (!nonterminal[k])
                continue;
            char nt = k + 'A';
            for (i = 0; i < no_pro; ++i)
            {
                for (j = 4; j < pro[i].len; ++j)
                {
                    if (nt == pro[i].str[j])
                    {
                        for (x = j + 1; x < pro[i].len; ++x)
                        {
                            char sc = pro[i].str[x];
                            if (isNT(sc))
                            {
                                add_FIRST_A_to_FOLLOW_B(sc, nt);
                                if (first[sc - 'A']['^'])
                                    continue;
                            }
                            else
                            {
                                follow[nt - 'A'][sc] = 1;
                            }
                            break;
                        }
                        if (x == pro[i].len)
                            add_FOLLOW_A_to_FOLLOW_B(pro[i].str[0], nt);
                    }
                }
            }
        }
    }
}

void add_FIRST_A_to_FIRST_B(char A, char B)
{
    int i;
    for (i = 0; i < TSIZE; ++i)
    {
        if (i != '^')
        {
            first[B - 'A'][i] = first[A - 'A'][i] || first[B - 'A'][i];
        }
    }
}

void FIRST()
{
    int i, j;
    int t = 0;
    while (t < no_pro)
    {
        for (i = 0; i < no_pro; ++i)
        {
            for (j = 4; j < pro[i].len; ++j)
            {
                char sc = pro[i].str[j];
                if (isNT(sc))
                {
                    add_FIRST_A_to_FIRST_B(sc, pro[i].str[0]);
                    if (first[sc - 'A']['^'])
                        continue;
                }
                else
                {
                    first[pro[i].str[0] - 'A'][sc] = 1;
                }
                break;
            }
            if (j == pro[i].len)
                first[pro[i].str[0] - 'A']['^'] = 1;
        }
        ++t;
    }
}

void add_FIRST_A_to_FIRST_RHS__B(char A, int B)
{
    int i;
    for (i = 0; i < TSIZE; ++i)
    {
        if (i != '^')
            first_rhs[B][i] = first[A - 'A'][i] || first_rhs[B][i];
    }
}

void FIRST_RHS()
{
    int i, j;
    int t = 0;
    while (t < no_pro)
    {
        for (i = 0; i < no_pro; ++i)
        {
            for (j = 4; j < pro[i].len; ++j)
            {
                char sc = pro[i].str[j];
                if (isNT(sc))
                {
                    add_FIRST_A_to_FIRST_RHS__B(sc, i);
                    if (first[sc - 'A']['^'])
                        continue;
                }
                else
                {
                    first_rhs[i][sc] = 1;
                }
                break;
            }
            if (j == pro[i].len)
                first_rhs[i]['^'] = 1;
        }
        ++t;
    }
}

int parseInputString(char inputString[], int inputLength)
{
    char stack[STACK_SIZE];
    int top;
    initStack(stack, &top);

    push(stack, &top, '$');           // Push the end marker onto the stack
    push(stack, &top, pro[0].str[0]); // Push the start symbol onto the stack

    int inputIndex = 0;
    char stackTop;

    while (top >= 0)
    {
        stackTop = pop(stack, &top);

        if (stackTop == inputString[inputIndex])
        {
            printf("Matched %c\n", stackTop);
            if (stackTop == '$' && inputString[inputIndex] == '$')
            {
                printf("Input string is accepted by the grammar.\n");
                return 1; // Parsing successful
            }
            inputIndex++;
        }
        else if (isNT(stackTop))
        {
            int row = stackTop - 'A';
            int col = inputString[inputIndex];
            int productionIndex = parse_table[row][col];

            if (productionIndex == 0)
            {
                printf("Error: No production in table for %c and %c\n", stackTop, inputString[inputIndex]);
                return 0; // Parsing failed
            }

            // Push the production onto the stack in reverse order
            int len = pro[productionIndex - 1].len;
            for (int i = len - 1; i >= 4; i--)
            {
                if (pro[productionIndex - 1].str[i] != '^')
                {
                    push(stack, &top, pro[productionIndex - 1].str[i]);
                }
            }
        }
        else
        {
            printf("Error: Mismatch between stack top %c and input symbol %c\n", stackTop, inputString[inputIndex]);
            return 0; // Parsing failed
        }
    }

    if (inputIndex == inputLength)
    {
        printf("Input string is accepted by the grammar.\n");
        return 1; // Parsing successful
    }
    else
    {
        printf("Input string is not accepted by the grammar.\n");
        return 0; // Parsing failed
    }
}

int main(int argc, char **argv)
{
    readFromFile(argv[1]);
    follow[pro[0].str[0] - 'A']['$'] = 1;
    FIRST();
    FOLLOW();
    FIRST_RHS();
    int i, j, k;
    terminal['$'] = 1;
    terminal['^'] = 0;
    printf("Production format \"S = a\"\n");
    printf("^ represents epsilon\n");
    printf("Each production ends with new-line\n");
    

    int p = 0;
    for (i = 0; i < no_pro; ++i)
    {
        if (i != 0 && (pro[i].str[0] != pro[i - 1].str[0]))
            p = p + 1;
        for (j = 0; j < TSIZE; ++j)
        {
            if (first_rhs[i][j] && j != '^')
            {
                if (table[p][j] == 0)
                    table[p][j] = i + 1;
                else
                {
                    printf("CFG is not LL(1)\n");
                    return 0;
                }
            }
            else if (first_rhs[i]['^'])
            {
                for (k = 0; k < TSIZE; ++k)
                {
                    if (follow[pro[i].str[0] - 'A'][k])
                    {
                        if (table[p][k] == 0)
                            table[p][k] = i + 1;
                        else
                        {
                            printf("CFG is not LL(1)\n");
                            return 0;
                        }
                    }
                }
            }
        }
    }
    
    printf("Predictive Parsing Table\n");
    printf("%-10s", "");
    for (i = 0; i < TSIZE; ++i)
    {
        if (terminal[i])
            printf("%-10c", i);
    }
    printf("\n");

    k = 0;
    for (i = 0; i < no_pro; ++i)
    {
        if (i == 0 || (pro[i - 1].str[0] != pro[i].str[0]))
        {
            printf("%-10c", pro[i].str[0]);
            for (j = 0; j < TSIZE; ++j)
            {
                if (table[k][j])
                {
                    printf("%-10s", pro[table[k][j] - 1].str);
                    parse_table[pro[i].str[0] - 'A'][j] = table[k][j];
                }
                else if (terminal[j])
                {
                    printf("%-10s", "");
                }
            }
            ++k;
            printf("\n");
        }
    }

    int inputLength;
    printf("Enter the input length including $ : ");
    scanf("%d", &inputLength);
    char inputString[inputLength];
    printf("Enter the input string including $ : ");
    scanf("%s", inputString);
    int parsingResult = parseInputString(inputString, inputLength);

    if (parsingResult)
    {
        printf("Parsing successful!\n");
    }
    else
    {
        printf("Parsing failed.\n");
    }

    return 0;
}
