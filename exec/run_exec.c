/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:14:11 by ixu               #+#    #+#             */
/*   Updated: 2024/03/21 16:21:49 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	update_shlvl(int argc, char **argv, t_env *env_lst) // lvl down before exit???????
{
	t_env	*tmp;
	int		new_shlvl;

	if (argc == 1 && argv != NULL && ft_strcmp(argv[0], "./minishell") == 0)
	{
		tmp = env_lst;
		while (tmp != NULL)
		{
			if (ft_strcmp(tmp->name, "SHLVL") == 0 && tmp->value != NULL)
			{
				new_shlvl = ft_atoi(tmp->value) + 1;
				free(tmp->value);
				tmp->value = ft_itoa(new_shlvl);
			}
			tmp = tmp->next;
		}
	}
}

int	run_exec(t_cmd *cmd, t_data *data)
{
	t_execcmd	*ecmd;
	pid_t		pid;
	int			status;

	ecmd = (t_execcmd *)cmd;
	make_argv(ecmd, data);
	if (ecmd->argc == 0)
	{
		if (!data->under_pipe && data->under_redir)
		{
			if (dup2(data->fd_stdin, 0) == -1)
				return (panic(ERR_DUP2, data, 1));
			if (dup2(data->fd_stdout, 1) == -1)
				return (panic(ERR_DUP2, data, 1));
		}
		return (panic(NULL, data, 0));
		// if (data->proc == CHILD_PROC)
		// 	free_n_exit(data, 0);
	 	// return (0);
	}
	if (is_builtin(ecmd->argv, &data))
	{
		data->status = run_builtin(ecmd->argv, data);
		if (!data->under_pipe && data->under_redir)
		{
			if (dup2(data->fd_stdin, 0) == -1)
				return (panic(ERR_DUP2, data, 1));
			if (dup2(data->fd_stdout, 1) == -1)
				return (panic(ERR_DUP2, data, 1));
		}
		return (panic(NULL, data, data->status));
		// if (data->proc == CHILD_PROC)
		// 	free_n_exit(data, data->status);
	}
	else
	{
		update_shlvl(ecmd->argc, ecmd->argv,data->env_lst);
		if (data->proc == CHILD_PROC)
		{
			data->cmd_path = get_cmd_path(ecmd->argv, data);
			data->envp = copy_env_lst_to_arr(data->env_lst);
			execve(data->cmd_path, ecmd->argv, data->envp);
			panic_cmd_not_found(ecmd->argv[0], data);
		}
		else
		{
			pid = fork1(data);
			if (pid == -1)
				return (1);
			if (pid == 0)
			{
				data->proc = CHILD_PROC;
				data->cmd_path = get_cmd_path(ecmd->argv, data);
				data->envp = copy_env_lst_to_arr(data->env_lst);
				execve(data->cmd_path, ecmd->argv, data->envp);
				panic_cmd_not_found(ecmd->argv[0], data);
			}
			if (ignore_signals() == 1)
				return (panic(ERR_SIGACTION, data, 1));
			if (!data->under_pipe && data->under_redir)
			{
				if (dup2(data->fd_stdin, 0) == -1)
					return (panic(ERR_DUP2, data, 1));
				if (dup2(data->fd_stdout, 1) == -1)
					return (panic(ERR_DUP2, data, 1));
			}
			if (waitpid(pid, &status, 0) == -1)
				return (panic(ERR_WAITPID, data, 1));
			if (WIFEXITED(status))
				data->status = WEXITSTATUS(status);
			if (WIFSIGNALED(status))
				data->status = 128 + WTERMSIG(status);
		}
	}
	return (0);
}
