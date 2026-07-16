/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacortes <cacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 09:31:07 by cacortes          #+#    #+#             */
/*   Updated: 2026/07/16 19:19:46 by cacortes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

int	file_walls(char *file, char *compass)
{
	char *line;
	char **choosen;
	int i;
	char *filename;
	int fd_file;
	int fd_choosen;

	i = 0;
	fd_file = open(file, O_RDONLY);
	if (fd_file == -1)
		return (1);
	line = get_next_line(fd_file);
	while (line)
	{
		printf("LINEA LEIDA: [%s]\n", line);
		if (ft_strncmp(line, compass, ft_strlen(compass)) == 0)
		{
			choosen = ft_split(line, ' ');
			if (!choosen)
			{
				free(line);
				close(fd_file);
				return (1);
			}
			i = 0;
			while (choosen[i])
			{
				if (choosen[i][0] == '.' && choosen[i][1] == '/')
				{
					filename = ft_strtrim(choosen[i], "\n");
					//printf("Indicador %s intenta abrir [%s]\n", compass, filename);
					fd_choosen = open(filename, O_RDONLY);
					free(filename);
					if (fd_choosen == -1)
					{
						free_split(choosen);
						free(line);
						close(fd_file);
						return (1);
					}
					close(fd_choosen);
					free_split(choosen);
					free(line);
					close(fd_file);
					return (0);
				}
				i++;
			}
			free_split(choosen);
		}
		free(line);
		line = get_next_line(fd_file);
	}
	close(fd_file);
	return (1);
}

int	parser_file_content(char *file)
{
	if (file_walls(file, "NO") != 0)
		printf("Error\nThere is an error in your .cub file.\n");
	else if (file_walls(file, "SO") != 0)
		printf("Error\nThere is an error in your .cub fileA.\n");
	else if (file_walls(file, "WE") != 0)
		printf("Error\nThere is an error in your .cub file.\n");
	else if (file_walls(file, "EA") != 0)
		printf("Error\nThere is an error in your .cub file.\n");
	else
		printf("GOOD\n");

	return 0;
}

int	parser_file_extension(char *file)
{
	char	*start;

	start = file + ft_strlen(file) - 4;
	if (!ft_strnstr(start, ".cub", 4))
	{
		printf("Error\nThe file extension you are trying to enter is not valid.\n");
		return (1);
	}
	return (0);
}
/*
int	main(int argc, char **argv)
{
	(void)argv;
	if (argc != 2)
	{
		printf("Error\nThe number of arguments entered is not compatible with the program.\n");
		return (1);
	}
	parser_file_extension(argv[1]);
	parser_file_content(argv[1]);
	return (0);
}*/
