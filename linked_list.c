#include "header.h"

node *create_node(char *str)
{
	node *new_node;
	int len;

	if(!(new_node = (node*)malloc(sizeof(node))))
		return NULL;
	len = strlen(str);

	new_node->str = (char*)malloc(sizeof(char) * (len + 1));
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

void free_list(list *lst)
{
	node *temp1;
	node *temp2;

	temp1 = lst->head;
	while(temp1)
	{
		temp2 = temp1;
		temp1 = temp2->link;
		free(temp2->str);
		free(temp2);
	}
	free(lst);
}

void print_list(list *lst)
{
	node *temp;

	temp = lst->head;
	while(temp)
	{
		printf("|%s| -> |%d| \t", temp->str, temp->count);
		temp = temp->link;
	}
}

void count_in_list(char *str, bool positive)
{
	node *temp;

	temp = positive ? p_list->head : n_list->head;
	while (temp)
	{
		if(!strcmp(temp->str, str))
		{
			temp->count += 1;
			break;
		}
		//단어보다 알파벳이 크면 검색중지
		else if(strcmp(temp->str, str) > 0)
			break;
		temp = temp->link;
	}
}