#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    FILE *ptr;
    int count = 0; // Line count
    char c;
    char data[1000];
    

    	// For no of lines
    
    
    ptr = fopen("input.c", "r");

    if (ptr == NULL) {
        printf("Could not open file ");
        return 0;
    }

    while ((c = getc(ptr)) != EOF) {
        if (c == '\n') {
            count++;
        }
    }
    printf("The file has %d lines\n", count);
    
    

    	// For no of characters
    					
    					
    int totalCharacters = 0;
    ptr = fopen("input.c", "r");

    while ((c = getc(ptr)) != EOF) {
        totalCharacters++;
    }
    printf("The file has %d characters\n", totalCharacters);
    

   	// For no of words
    
    int totalWords = 0;
    ptr = fopen("input.c", "r");

    while (1) {
        fgets(data, 1000, ptr);
        if (feof(ptr)) {
            break;
        }
        char* tokens;
        int wordCount = 0;
        tokens = strtok(data, " \t\n");

        while (tokens != NULL) {
            wordCount++;
            tokens = strtok(NULL, " \t\n");
        }
        totalWords += wordCount;
    }
    printf("The file has %d words\n", totalWords);
    

    	// For no of vowels
   
    ptr = fopen("input.c", "r");

    int vowelCount = 0;

    while ((c = getc(ptr)) != EOF) {
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
            vowelCount++;
        }
    }
    printf("The file has %d vowels\n", vowelCount);

    int consonantCount = 0;
    

  	  // For no of consonants
    
    ptr = fopen("input.c", "r");

    while ((c = getc(ptr)) != EOF) {
        if (isalpha(c)) {
            if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u' &&
                c != 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U') {
                consonantCount++;
            }
        }
    }
    printf("The file has %d consonants\n", consonantCount);

    int digitCount = 0;

   	 // For no of numbers
    
    
    ptr = fopen("input.c", "r");

    while ((c = getc(ptr)) != EOF) {
        if (isdigit(c)) {
            digitCount++;
        }
    }
    printf("The file has %d numbers\n", digitCount);
    
   

   	 // For other characters
    
    
    int otherCount = totalCharacters - (vowelCount + consonantCount + digitCount);
    printf("The file has %d other characters\n", otherCount);

    return 0;
}

