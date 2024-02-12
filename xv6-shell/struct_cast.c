
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

typedef struct s_cmd_pipe
{
	int type;
	char c;
} p_cmd_pipe;

typedef struct s_cmd
{
	int type;
} p_cmd;

int	main(void)
{
	p_cmd *pcmd;
	p_cmd_pipe *ppipe;
	p_cmd_pipe *ppipe2;

	ppipe = malloc(sizeof(p_cmd_pipe));
	ppipe->type = 1;
	ppipe->c = '|';

	pcmd = (p_cmd *)ppipe;
	printf("type=%d\n", pcmd->type);
	ppipe2 = (p_cmd_pipe *)pcmd;
	printf("type=%c\n", ppipe2->c);
//xargs --show-limits
	//getconf ARG_MAX
//	printf("ARG_MAX=%d\n",ARG_MAX);
//	printf("ARG_MAX=%d\n",MAX_ARGS);
//	printf("ARG_MAX=%d\n",MAX_PARAMETERS);

}
