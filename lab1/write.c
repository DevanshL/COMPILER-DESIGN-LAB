#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	FILE *ptr,*ptr1;
	int num;
	ptr = fopen("/home/devansh/Desktop/program/input.txt","r");
	ptr1 = fopen("/home/devansh/Desktop/program/output.txt","w");
	
	
	if(ptr == NULL)
	{
		printf("Error! page not found");
		exit(0);
	}
	num = fgetc(ptr);  // for all line characters
	while(num!=EOF)
	{
		fputc(num,ptr1);
		num = fgetc(ptr);
	}	
	fclose(ptr);
	return 0;
}
