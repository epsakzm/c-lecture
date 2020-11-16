#ifndef HEADER_
#define HEADER_

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

// #define SEP ", \"\n`()<>.!-"
#define SEP " .,()<>!:_`?\""
#define PWDS "positive-words.txt"
#define NWDS "negative-words.txt"
#define PREVDIR "positive_reviews/"
#define NREVDIR "negative_reviews/"
#define PFLIST "positive_file_list.txt"
#define NFLIST "negative_file_list.txt"

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

node *create_node(char *str);
void push_node(list *lst, node *new_node);
void free_list(list *lst);
void print_list(list *lst);
void count_in_list(char *str, bool positive);

#endif