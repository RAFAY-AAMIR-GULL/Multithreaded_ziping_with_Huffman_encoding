/*
read data
divide it into chunks
	structure 
	threads
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <assert.h>


typedef struct mydat{
	int c, count[256];
	char* data;
} mydat;


int main(int argc, char* argv[])
{
    // char * word;
    // word= (char*) malloc(sizeof(char) * 320);
    // word[0] = 'L';
    // printf("%s\n", word);
    // strcat((word),"lol");
    // printf("%s\n", word);
    // free(word);

    // mydat* myv = (mydat*) malloc(sizeof(mydat) * 10);
    // myv[0].data =  (char*) malloc(sizeof(char) * 320);

    // myv[0].data[0] = 'L';
    // printf("%s\n", myv[0].data);
    // strcat(myv[0].data,"lol");
    // printf("%s\n", myv[0].data);
    // free(myv[0].data);
    // free(myv);  
    FILE* file;
	file = fopen("anothertextfile.txt", "r");
	if (file == NULL)
	{
		printf("Error opening a file\n");
	}

    return 0;
}