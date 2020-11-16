#include "header.h"

list *p_list;
list *n_list;

//list에 filename에 해당하는 내용 저장
void make_list(list *lst, char *filename)
{
	FILE *fp;
	char buf[20];

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
	p_list = (list*)malloc(sizeof(list));
	n_list = (list*)malloc(sizeof(list));
	make_list(p_list, PWDS);
	make_list(n_list, NWDS);
}

void search_in_files(FILE *fp, bool positive)
{
	char buf[512];
	char *token;
	bool is_first;

	while(!feof(fp))
	{
		is_first = true;
		fgets(buf, 512, fp);
		while(1)
		{
			if(is_first)
				token = strtok(buf, SEP);
			else
				token = strtok(NULL, SEP);
			if(token == NULL)
				break;
			// printf("%s| ", token);
			//단어 찾음
			count_in_list(token, positive);
			is_first = false;
		}
	}
}
// mode == 0 pos, mode == 1 neg
void search_files(char *filename, bool positive)
{
	FILE *fp;
	char path[30];

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

void count_words(FILE *fp, bool positive)
{
	char filename[20];

	while(!feof(fp))
	{
		fscanf(fp, "%s\n" ,filename);
		search_files(filename, positive);
	}
	fclose(fp);
}

int main(void)
{
	init();
	// print_list(p_list);
	// print_list(n_list);
	FILE *fp_pos_fl;
	FILE *fp_neg_fl;
	fp_pos_fl = fp_read(PFLIST);
	fp_neg_fl = fp_read(NFLIST);
	count_words(fp_pos_fl, true);
	count_words(fp_neg_fl, false);
	print_list(p_list);
	// print_list(n_list);

	//메모리 할당해제
	free_list(p_list);
	free_list(n_list);
	return 0;
}
