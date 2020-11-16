#include "header.h"

node *create_node(char *str)
{
	node *new_node;
	int len;

	if(!(new_node = (node*)malloc(sizeof(node))))
		return NULL;
	len = strlen(str);

	new_node->str = (char*)malloc(sizeof(len) + 1);
	strncpy(new_node->str, str, len);
	(new_node->str)[len] = '\0';

	new_node->count = 0;
	new_node->link = NULL;

	return new_node;
}

void push_node(list *lst, node *new_node)
{
	if(lst->head == NULL)
	{
		lst->head = new_node;
		lst->tail = new_node;
	}
	else
	{
		lst->tail->link = new_node;
		lst->tail = new_node;
	}
	
}
