#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/include/libft.h"
# include "libft/include/ft_dprintf.h"
# include "libft/include/get_next_line.h"

// readline
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

// write, close, dup, dup2, fork, pipe, access, execve, unlink
# include <unistd.h>

// FILE CONTROL
# include <fcntl.h>

// EXIT_FAILURE, EXIT_SUCCESS, NULL, malloc, free
# include <stdlib.h>

// waitpid
# include <sys/wait.h>

// sigaction
#include <signal.h>

//directory  entity
# include <dirent.h>

// macros for (error) messages
# define ERR_ARGS "invalid number of arguments"
# define USAGE "ex. usage: ./minishell"
# define ERR_WRITE "write error"
# define ERR_PRINTF "printf error"
# define ERR_MALLOC "malloc error"
# define ERR_PIPE "pipe error"
# define ERR_FORK "fork error"
# define ERR_WAITPID "waitpid error"
# define ERR_OPEN "open error"
# define ERR_CLOSE "close error"
# define ERR_DUP2 "dup2 error"
# define ERR_DUP "dup error"
# define ERR_HEREDOC "heredoc error"
# define ERR_ID "not a valid identifier"
# define PMT "\033[0;31mLiteShell: \033[0m"
# define PMT_ERR_WRITE "\033[0;31mLiteShell: \033[0mwrite error"
# define PMT_ERR_PRINTF "\033[0;31mLiteShell: \033[0mprintf error"
# define PMT_ERR_GETCWD "\033[0;31mLiteShell: \033[0mgetcwd error"
# define EXIT_CMD_PERM_ERR 126
# define EXIT_CMD_NOT_FOUND 127
# define PMT "\033[0;31mLiteShell: \033[0m"

// macros for processes
# define PARENT_PROC 0
# define CHILD_PROC 1

// macros for 2d arraylist
#define INITIAL_CAPACITY 5
#define GROWTH_FACTOR 2

// macros for parsing 
#define TESTMODE 0 
#define MAXARGS 4
#define WHITESPACE  " \t\r\n\v"
#define SYMBOLS "<>|&()"
#define ASCII_SEPARATOR 31
#define ASCII_WILD 32
#define ERR_SYNTAX_UNEXP "syntax error near unexpected token" 
#define ERR_REDIR_AMBIG  "ambiguous redirect"
#define ERR_CODE_SYNTAX 258
#define ENOMEM 12


typedef void (*sig_t) (int);

typedef struct s_arrlist
{
	 char		**data;
	 size_t	size;
	 size_t	capacity;
} t_arrlist;

// AST's node types
typedef enum e_node_type
{
	EXEC = 1,
	REDIR,
	PIPE,
	AND_CMD,
	OR_CMD,
	ARG_NODE,
	STR_NODE,
	STR_NODE_VAR,
	STR_NODE_VAR_P,
	STR_EXIT_CODE,
	STR_STAR,
	// to be remove?
	LIST
}   t_node_type;

// types of tockens
typedef enum e_token
{
	UNDEFINED_TOK,
	NEWLINE_TOK,
	STR_TOK,
	RED_IN,
	RED_OUT,
	HEREDOC,
	RED_OUT_APP,
	PIPE_TOK,
	OR_TOK,
	AND_TOK,
	//unused tok
	LPAR,
	RPAR
}   t_token_type;

typedef enum e_parse_error
{
	SYNTAX_ERR_UNDEFTOK = 0x01,
	SYNTAX_ERR_UNEXPTOK = 0x02,
	SYNTAX_ERR_UNCLOSED = 0x04,
	SYNTAX_ERROR = 0x08,
	MALLOC_ERROR = 0x10,
	DIR_OPEN_ERR = 0x20
}	t_parse_error;

typedef enum e_builtin
{
	ECHO = 1,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
}   t_builtin;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_data
{
	char		*buf;
	char		**envp;
	t_env		*env_lst;
	char		**env_paths;
	char		*cmd_path;
	t_builtin	builtin;
	int			proc;
	int			status;
	char		*stat_str;
	int			fd_stdin;
	int			fd_stdout;
}	t_data;

typedef struct s_wildcard
{
	t_arrlist	*list;
	t_arrlist	**pnt;
	char		**argv;
	int			argc;
}	t_wildcard;

typedef struct s_strstate
{
	char	*start;
	char	*pos;
	char	*finish;
	char	*beg;
	char	*end;
	int		d_quotes;
	int		s_quotes;
	int		flag;
} t_strstate;

typedef struct s_cmd
{
	int	type;
	int		flag;
}	t_cmd;

typedef struct s_strcmd
{
	int		type;
	int		flag;
	char	*start;
	char	*end;
	struct s_strcmd	*next;
}	t_strcmd;

typedef struct s_argcmd
{
	int		type;
	int		flag;
	t_strcmd	*left;
	struct s_argcmd	*right;
	char			*start;
	char			*end;
}	t_argcmd;

typedef struct s_execcmd
{
	int		type;
	int		flag;
	char	*sargv[MAXARGS];
	char	*eargv[MAXARGS];
	char	**argv;
	int		argc;
	t_argcmd	*args;
	t_arrlist	*list;
}	t_execcmd;

typedef struct s_redircmd
{
	int		type;
	int		flag;
	t_cmd	*cmd;
	char	*file;
	char	*sfile;
	char	*efile;
	int		mode;
	int		fd;
	t_strcmd	*str;
	t_arrlist	*list;
}	t_redircmd;

typedef struct s_pipecmd
{
	int		type;
	int		flag;
	t_cmd	*left;
	t_cmd	*right;
}	t_pipecmd;

typedef struct s_listcmd
{
	int		type;
	int		flag;
	t_cmd	*left;
	t_cmd	*right;
}	t_listcmd;

// readline
void	rl_clear_history(void);
void	rl_replace_line(const char *text, int clear_undo);

// data init
t_env	*copy_env_arr_to_lst(char **envp);
char	**get_env_paths(char **envp, t_data *data);
void	data_init(t_data *data, char **envp);

// data init utils
t_env	*get_node_in_init(char *name_value_str);
void	lst_append_in_init(t_env **env_lst, t_env *new_node);
void	print_error_partial_free(char *name, t_data *data);

// constructors.c
t_cmd		*execcmd(void);
t_cmd		*redircmd_old(t_cmd *subcmd, char *file, char *efile, int mode, int fd);
t_cmd		*redircmd(t_cmd *subcmd, t_strstate *state, int mode, int fd);
t_cmd		*pipecmd(t_cmd *left, t_cmd *right);
t_cmd		*list_cmd(t_cmd *left, t_cmd *right, int type);
t_argcmd	*argcmd(t_strcmd *str, t_argcmd *args, char *start, char *end);
t_strcmd	*strcmd(int type, char *start, char *end);
t_strstate	*make_strstate(char *pos, char *finish);

// parseexec.c
t_cmd	*parseexec(char**, char*);
t_cmd	*parseredirs(t_cmd *cmd, char **ps, char *es);

// parsecmd.c
int			make_ast(t_cmd **p_cmd, char *s);
t_cmd   *parsecmd(char *buf, int *status);
t_cmd   *parseblock(char **ps, char *es);
//t_cmd   *parseline(char**, char*);
//t_cmd   *parsepipe(char**, char*);

//parsing_utils.c
int gettoken(char **ps, char *es, char **q, char **eq);
int peek(char **ps, char *es, char *toks);
t_cmd   *nulterminate(t_cmd *cmd);
const char  *token_type_to_str(t_token_type token);
//void    panic_test(char *s);  //this is temporal function that exit(1) from parser

// string operations
char	*strlist_join(t_strcmd *str);
int		make_argv(t_execcmd *cmd, t_data *data);

// runcmd() func and its helper funcs
void	runcmd(t_cmd *cmd, t_data *data);
int		fork1(t_data *data);
void	run_exec(t_cmd *cmd, t_data *data);
void	run_redir(t_cmd *cmd, t_data *data);
void	get_input(t_data *data, char *delimiter);
void	run_and(t_cmd *cmd, t_data *data);
void	run_or(t_cmd *cmd, t_data *data);
void	run_pipe(t_cmd *cmd, t_data *data);
// to be removed at some point
void	runcmd_test(t_cmd *cmd, t_data *data);

// parsing cmd path
char	*get_cmd_path(char **argv, t_data *data);

// handling builtins
int		exec_echo(char **argv);
int		exec_exit(char **argv);
int		exec_cd(char **argv, t_env *env_lst);
int		exec_pwd(char **argv);
int		exec_export(char **argv, t_env *env_lst);
int		exec_unset(char **argv, t_env *env_lst);
int		exec_env(t_env *env_lst);
int		name_in_env_lst(t_env *env_lst, char *arg, size_t name_len, t_env **node);
char	*get_value(char *name_value_str, t_env *new_node, int *err_flag);
t_env	*get_node(char *name_value_str);
void	lst_append(t_env **env_lst, t_env *new_node);
int		is_builtin(char **argv, t_data **data);
int		run_builtin(char **argv, t_data *data);

// handling env
char	**copy_env(char **envp);
t_env	*copy_env_arr_to_lst(char **envp);
char	**copy_env_lst_to_arr(t_env *env_lst);
char	**get_env_paths(char **envp, t_data *data);

// freeing
void	free_arr(char **arr);
void	free_data(t_data *data);

// error handling
void	validate_args(int argc);
void	print_error_n_exit(char *err_msg);
t_env	*error_handler(char *err_msg, int *err_flag);
void	panic(char *err_msg, t_data *data, int status_code);
void	free_n_exit(t_data *data, int status_code);

// readline
void	rl_clear_history(void);
void	rl_replace_line (const char *text, int clear_undo);

// string operations
char	*strlist_join(t_strcmd *str);
int		make_argv(t_execcmd *cmd, t_data *data);
int		make_filename(t_redircmd *rcmd, t_data *data);
int		match(const char *pattern, const char *text);
void	ft_free_char2d(char **split);
void	heapsort_str(char **arr, int n);


// arraylist
t_arrlist	*create_arrlist(void);
int				add_string_arrlist(t_arrlist *list, const char* str);
void			free_arrlist(t_arrlist *list);
int	wildcard_star(t_execcmd *cmd);

#endif
