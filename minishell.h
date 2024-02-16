#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"

// readline
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

/* write, close, dup2, fork, pipe, access, execve, unlink
STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO */
# include <unistd.h>

//FILE CONTROL
# include <fcntl.h>

// EXIT_FAILURE, EXIT_SUCCESS, NULL
# include <stdlib.h>

// wait
# include <sys/wait.h>

// macros for (error) messages
# define ERR_WRITE "write error"
# define ERR_ARGS "invalid number of arguments"
# define USAGE "ex. usage: ./minishell"


#define MAXARGS 10

# define ERR_MALLOC "malloc error"
# define ERR_PIPE "pipe error"
# define ERR_FORK "fork error"
# define ERR_WAITPID "waitpid error"
// # define EXIT_CMD_PERM_ERR 126
# define EXIT_CMD_NOT_FOUND 127

// Parsed command representation
#define EXEC  1
#define REDIR 2
#define PIPE  3
#define LIST  4
#define BACK  5
#define AND_CMD 6
#define OR_CMD 7

typedef enum e_token
{
    WORD,
    RED_IN,
    HEREDOC,
    RED_OUT,
    RED_OUT_APP,
    PIPE_TOK,
    OR_TOK,
    AND_TOK,
    LPAR,
    RPAR
}   t_token_type;

typedef struct s_data
{
	char	*buf;
	char	**envp;
}	t_data;

typedef struct s_cmd
{
	int	type;
}	t_cmd;

typedef struct s_execcmd
{
	int		type;
	char	*argv[MAXARGS];
	char	*eargv[MAXARGS];
}	t_execcmd;

typedef struct s_redircmd
{
	int		type;
	t_cmd	*cmd;
	char	*file;
	char	*efile;
	int		mode;
	int		fd;
}	t_redircmd;

typedef struct s_pipecmd
{
	int		type;
	t_cmd	*left;
	t_cmd	*right;
}	t_pipecmd;

typedef struct s_listcmd
{
	int		type;
	t_cmd	*left;
	t_cmd	*right;
}	t_listcmd;

typedef struct s_backcmd
{
        int type;
        t_cmd *cmd;
}       t_backcmd;

void	runcmd(t_cmd *cmd, t_data *data);
char	**copy_env(char **envp);

#endif
