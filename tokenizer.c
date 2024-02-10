#include "stdio.h"
#include "libft/libft.h"


typedef enum e_bool
{
	FALSE,
	TRUE
}	t_bool;

typedef enum e_token
{
	HEAD,
	WORD,
	RED_IN,
	HEREDOC,
	RED_OUT,
	RED_OUT_APP,
	PIPE,
	OR,
	AMPERSAND,
	AND,
	LPAR,
	RPAR,
	WHITESPACE,
	EMPTY
}	t_token_type;

typedef struct s_token
{
	int				position;
	t_token_type	type;
	char			*content;
	struct s_token	*next;
}	t_token;

t_bool	is_whitespace(char c)
{
	if (c == ' ')
		return (TRUE);
	if (c >= 9 && c <= 13)
		return (TRUE);
	return (FALSE);
}

t_token_type	ind_token(char *str, size_t i)
{
	if (str[i] == '<' && str[i + 1] == '<')
		return (HEREDOC);
	else if (str[i] == '<')
		return (RED_IN);
	else if (str[i] == '>' && str[i + 1] == '>')
		return (RED_OUT_APP);
	else if (str[i] == '>')
		return (RED_OUT);
	else if (str[i] == '|' && str[i + 1] == '|')
		return (OR);
	else if (str[i] == '|')
		return (PIPE);
	else if (str[i] == '&' && str[i + 1] == '&')
		return (AND);
	else if (str[i] == '&')
		return (AMPERSAND);
	else if (str[i] == '(')
		return (LPAR);
	else if (str[i] == ')')
		return (RPAR);
	else if (is_whitespace(str[i]))
		return (WHITESPACE);
	else
		return (WORD);
}

char	*make_token(t_token_type type, char *str, size_t *start)
{
	size_t	end;
	char		*word;

	end = *start;
	if (type == WORD)
	{
		while (ind_token(str, end) == type && str[end])
			end++;
		word = (char *)malloc(sizeof(char) * (end - *start + 1));
		ft_strlcpy(word, (const char *)(str + *start), end - *start + 1);
		*start = end;
		return (word);
	}
	if (type == HEREDOC || type == RED_OUT_APP || type == OR || type == AND)
		*start +=2;
	else
		(*start)++;
	return (NULL);

}

int	main()
{
	//          012345678901234567890
	char	*str="Hi1 hi2 hi3&&hi4 & |i | and|!";
	size_t		i;
	char	*word;
	t_token_type type;
	t_list	*curr_lst;

	i = 0;
	curr_lst = NULL;
	while (str[i])
	{
		while (is_whitespace(str[i]))
			i++;
		type = ind_token(str, i);
		word = make_token(type, str, &i);
//		printf("type=%d, word = ->%s<-, i=%d\n", type, word, (int)i);
			printf("word = ->%s<, type=%d\n", word, type);
		curr_lst = ft_lstnew(word);
	}
	return (0);
}


