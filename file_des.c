#include "header.h"

FILE *fp_write(char *filename)
{
	FILE *fp;

	if(!(fp = fopen("positive-words.txt", "a")))
	{
		perror(filename);
		return NULL;
	}
	return fp;
}

FILE *fp_read(char *filename)
{
	FILE *fp;

	if(!(fp = fopen("positive-words.txt", "r")))
	{
		perror(filename);
		return NULL;
	}
	return fp;
}
