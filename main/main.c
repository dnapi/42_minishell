/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 20:29:30 by ixu               #+#    #+#             */
/*   Updated: 2024/03/26 12:18:29 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile sig_atomic_t	g_last_sig;

static int	process_buf(t_data *data, int *status)
{
	if (data->buf == NULL)
	{
		printf("exit\n");
		*status = 0;
		return (1);
	}
	if (is_valid_buf(data->buf))
	{
		add_history(data->buf);
		*status = make_ast(&(data->tree), data->buf, &(data->num_heredoc));
		if (*status == 0)
			runcmd(data->tree, data);
		else if (*status == ERR_CODE_TERMINATE)
		{
			*status = 1;
			return (1);
		}
		else if (*status == SIGNAL_CTRL_C_MSG)
			data->status = 1;
		else
			data->status = *status;
	}
	return (0);
}

static void	cleanup_n_reset(t_data *data)
{
	int		i;
	char	*filename;

	g_last_sig = 0;
	data->under_pipe = 0;
	data->under_redir = 0;
	free(data->buf);
	freecmd_null(&(data->tree));
	i = -1;
	while (++i < data->num_heredoc)
	{
		filename = ft_strjoin(".heredoc", ft_itoa(i));
		if (filename == NULL)
			print_error_n_exit(ERR_MALLOC);
		if (unlink(filename) == -1)
			print_error_n_exit(ERR_UNLINK);
		free(filename);
	}
}

static void	cleanup_before_exit(t_data *data)
{
	if (close(data->fd_stdin) == -1)
		print_error_n_exit(ERR_CLOSE);
	if (close(data->fd_stdout) == -1)
		print_error_n_exit(ERR_CLOSE);
	free_data(data);
	data = NULL;
	rl_clear_history();
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	int		status;

	(void)argv;
	validate_args(argc);
	data_init(&data, envp);
	status = 1;
	while (data.status >= 0)
	{
		if (set_signals_interactive() == 1)
			break ;
		data.buf = readline("LiteShell$ ");
		if (g_last_sig)
			data.status = 1;
		if (set_default_signals() == 1)
			break ;
		if (process_buf(&data, &status) == 1)
			break ;
		cleanup_n_reset(&data);
	}
	cleanup_before_exit(&data);
	exit(status);
}
