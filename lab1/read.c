#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	FILE *ptr;
	int num;
	ptr = fopen("/home/devansh/Desktop/program/input.txt","r");
	
	if(ptr == NULL)
	{
		printf("Error! page not found");
		exit(0);
	}
	num = fgetc(ptr);  // for all line characters
	while(num!=EOF)
	{
		printf("%c",num);
		num = fgetc(ptr);
	}	
	fclose(ptr);
	return 0;
}
