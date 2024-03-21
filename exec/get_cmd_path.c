/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:17:03 by ixu               #+#    #+#             */
/*   Updated: 2024/03/21 15:17:04 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* verify the validity of the executable/cmd, 
if the provided name is already with a path */

static char	*verify_path(char *cmd, t_data *data)
{
	struct stat	path_stat;

	if (ft_strchr(cmd, '/') != NULL)
	{
		if (access(cmd, F_OK) != 0)
			panic(cmd, data, 127);
		if (access(cmd, X_OK) != 0)
			panic(cmd, data, 126);
		if (stat(cmd, &path_stat) == 0) 
		{
			if (S_ISDIR(path_stat.st_mode))
				panic_is_a_dir(cmd, data);
		}
		else
			panic("stat error", data, 1);
		return (ft_strdup(cmd));
	}
	return (NULL);
}

char	*get_cmd_path(char **argv, t_data *data)
{
	char	*cmd_path;
	char	*tmp;
	int		i;
	int		status;

	if (argv == NULL || argv[0] == NULL)
		return (NULL);
	// dprintf(2, "debug0.1\n");
	cmd_path = verify_path(argv[0], data);
	if (cmd_path != NULL)
		return (cmd_path);
	if ((data->env_paths == NULL || ft_strlen(data->env_paths[0]) == 0)
		&& access(argv[0], F_OK | X_OK) == 0)
		return (ft_strdup(argv[0]));
	i = 0;
	while (data->env_paths != NULL && data->env_paths[i] != NULL)
	{
		tmp = ft_strjoin(data->env_paths[i], "/");
		cmd_path = ft_strjoin(tmp, argv[0]);
		free(tmp);
		if (access(cmd_path, F_OK) == 0)
			break ;
		free(cmd_path);
		cmd_path = NULL;
		i++;
	}
	return (cmd_path);
}
