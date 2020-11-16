#ifndef HEADER_
#define HEADER_

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
FILE *fp_read(char *filename);
FILE *fp_write(char *filename);

typedef struct node_
{
	char *str;
	int count;
	struct node_ *link;
} node;

typedef struct list_
{
	node *head;
	node *tail;
} list;

extern list *p_list;
extern list *n_list;

#endif