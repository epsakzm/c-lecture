#include "header.h"

FILE *fp_write(char *filename)
{
	FILE *fp;

	if(!(fp = fopen(filename, "a")))
	{
		perror(filename);
		exit(1);
	}
	return fp;
}

FILE *fp_read(char *filename)
{
	FILE *fp;

	if(!(fp = fopen(filename, "r")))
	{
		perror(filename);
		exit(1);
	}
	return fp;
}
