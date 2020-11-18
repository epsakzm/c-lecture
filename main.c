
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

#define LINE_SIZE 512
#define BUF_SIZE 30

#define SEP " .,()<>!:_`?\""
#define PWDS "positive-words.txt"
#define NWDS "negative-words.txt"
#define PFLIST "positive_file_list.txt"
#define NFLIST "negative_file_list.txt"
#define PSMNT "positive-sentiment.txt"
#define NSMNT "negative-sentiment.txt"
#define RES "result.txt"

#define PREVDIR "positive_reviews/"
#define NREVDIR "negative_reviews/"
#define TESTDIR "test/"

FILE *fp_read(char *filename);
FILE *fp_write(char *filename);
DIR *dp_open(char *dirname);

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

typedef enum STATE_
{
	POSITIVE,
	NEGATIVE,
	DONTKNOW
} STATE;

list *p_list;
list *n_list;

node *create_node(char *str)
{
	node *new_node;
	int len;

	if (!(new_node = (node *)malloc(sizeof(node))))
		return NULL;
	len = strlen(str);

	new_node->str = (char *)malloc(sizeof(char) * (len + 1));
	strncpy(new_node->str, str, len);
	(new_node->str)[len] = '\0';

	new_node->count = 0;
	new_node->link = NULL;

	return new_node;
}

void push_node(list *lst, node *new_node)
{
	if (lst->head == NULL)
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
	while (temp1)
	{
		temp2 = temp1;
		temp1 = temp2->link;
		free(temp2->str);
		free(temp2);
	}
	free(lst);
}

void write_list(bool positive)
{
	FILE *fp;
	node *temp;

	fp = positive ? fp_write(PSMNT) : fp_write(NSMNT);
	temp = positive ? p_list->head : n_list->head;
	while (temp)
	{
		if (temp->count)
			fprintf(fp, "%s\t%d\n", temp->str, temp->count);
		temp = temp->link;
	}
	fclose(fp);
}

void count_in_list(char *str, bool positive)
{
	node *temp;

	temp = positive ? p_list->head : n_list->head;
	while (temp)
	{
		if (!strcmp(temp->str, str))
		{
			temp->count += 1;
			break;
		}
		else if (strcmp(temp->str, str) > 0)
			break;
		temp = temp->link;
	}
}

int check_count(char *str, bool positive)
{
	node *temp;

	temp = positive ? p_list->head : n_list->head;
	while (temp)
	{
		if (!strcmp(temp->str, str))
			return temp->count;
		else if (strcmp(temp->str, str) > 0)
			return 0;
		temp = temp->link;
	}
	return 0;
}

//list에 filename에 해당하는 내용 저장
void make_list(list *lst, char *filename)
{
	FILE *fp;
	char buf[BUF_SIZE];

	fp = fp_read(filename);
	while (!feof(fp))
	{
		fscanf(fp, "%s\n", buf);
		push_node(lst, create_node(buf));
	}
	fclose(fp);
}

void init(void)
{
	p_list = (list *)malloc(sizeof(list));
	n_list = (list *)malloc(sizeof(list));
	make_list(p_list, PWDS);
	make_list(n_list, NWDS);
}

void search_in_files(FILE *fp, bool positive)
{
	char buf[LINE_SIZE];
	char *token;
	bool is_first;

	while (!feof(fp))
	{
		is_first = true;
		fgets(buf, LINE_SIZE, fp);
		while (1)
		{
			if (is_first)
				token = strtok(buf, SEP);
			else
				token = strtok(NULL, SEP);
			if (token == NULL)
				break;
			// printf("%s| ", token);
			count_in_list(token, positive);
			is_first = false;
		}
	}
}

// mode == 0 pos, mode == 1 neg
void search_files(char *filename, bool positive)
{
	FILE *fp;
	char path[BUF_SIZE];

	if (positive)
		strcpy(path, PREVDIR);
	else
		strcpy(path, NREVDIR);
	strcat(path, filename);
	fp = fp_read(path);
	// printf("%s|\n", path);
	search_in_files(fp, positive);
	fclose(fp);
}

void search_file_list(bool positive)
{
	FILE *fp;
	char filename[BUF_SIZE];

	fp = positive ? fp_read(PFLIST) : fp_read(NFLIST);
	while (!feof(fp))
	{
		fscanf(fp, "%s\n", filename);
		search_files(filename, positive);
	}
	fclose(fp);
}

STATE check_file(char *filename)
{
	FILE *fp;
	char buf[LINE_SIZE];
	char *token;
	bool is_first;
	int p_cnt = 0, n_cnt = 0;

	fp = fp_read(filename);
	while (!feof(fp))
	{
		is_first = true;
		fgets(buf, LINE_SIZE, fp);
		while (1)
		{
			if (is_first)
				token = strtok(buf, SEP);
			else
				token = strtok(NULL, SEP);
			if (token == NULL)
				break;
			is_first = false;
			p_cnt += check_count(token, true);
			n_cnt += check_count(token, false);
			// printf("p_cnt : %d, n_cnt: %d\n", p_cnt, n_cnt);
		}
	}
	fclose(fp);
	if (p_cnt > n_cnt)
		return POSITIVE;
	else if (p_cnt < n_cnt)
		return NEGATIVE;
	return DONTKNOW;
}

void write_file_state(STATE state, FILE *fp, char *filename)
{
	fprintf(fp, "%s : ", filename);
	if (state == POSITIVE)
		fprintf(fp, "positive\n");
	else if (state == NEGATIVE)
		fprintf(fp, "negative\n");
	else
		fprintf(fp, "don't konw\n");
}

void check_dir(void)
{
	DIR *dir;
	FILE *fp;
	struct dirent *ent;
	char filename[BUF_SIZE];

	fp = fp_write(RES);
	dir = dp_open(TESTDIR);
	while ((ent = readdir(dir)))
	{
		strcpy(filename, TESTDIR);
		strcat(filename, ent->d_name);
		if (strlen(ent->d_name) <= 2)
			continue;
		write_file_state(check_file(filename), fp, filename);
	}
	fclose(fp);
	closedir(dir);
}

int main(void)
{
	init();
	search_file_list(true);
	search_file_list(false);
	write_list(true);
	write_list(false);
	printf("count done\n");
	check_dir();
	printf("check done\n");
	free_list(p_list);
	free_list(n_list);
	return 0;
}

FILE *fp_write(char *filename)
{
	FILE *fp;

	if (!(fp = fopen(filename, "w")))
	{
		perror(filename);
		exit(1);
	}
	return fp;
}

FILE *fp_read(char *filename)
{
	FILE *fp;

	if (!(fp = fopen(filename, "r")))
	{
		perror(filename);
		exit(1);
	}
	return fp;
}

DIR *dp_open(char *dirname)
{
	DIR *dir;

	if (!(dir = opendir(dirname)))
	{
		perror(dirname);
		exit(1);
	}
	return dir;
}