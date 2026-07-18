/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacortes <cacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 09:31:07 by cacortes          #+#    #+#             */
/*   Updated: 2026/07/18 20:36:01 by cacortes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	count_words(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		i++;
	return (i);
}

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

int file_walls(char *file, char *compass)
{
    char    *line;
    char    **choosen;
    char    *filename;
    int     fd_file;
    int     fd_choosen;
    int     found;
    int     i;

    found = 0;
	//printf("LA VARIABLE ES %d\n", found);
    fd_file = open(file, O_RDONLY);
    if (fd_file == -1)
        return (1);
	
    line = get_next_line(fd_file);
    while (line)
    {
        if (ft_strncmp(line, compass, ft_strlen(compass)) == 0 && line[ft_strlen(compass)] == ' ')
        {
            choosen = ft_split(line, ' ');
            if (!choosen)
				return (free(line), close(fd_file), 1);

            i = 0;
            while (choosen[i])
            {
                if (choosen[i][0] == '.' && choosen[i][1] == '/')
                {
                    filename = ft_strtrim(choosen[i], "\n");
                    fd_choosen = open(filename, O_RDONLY);
                    free(filename);
                    if (fd_choosen != -1)
                    {
                        close(fd_choosen);
                        found = 1;
                    }
					if (choosen[2] != NULL)
					{
    					free_split(choosen);
    					free(line);
    					close(fd_file);
    					return (1);
					}
                    break;
                }
                i++;
            }
            free_split(choosen);
        }
        free(line);
        line = get_next_line(fd_file);
    }
    close(fd_file);
    return (!found);
}

int file_color(char *file, char *compass)
{
    char    *line;
    char    **choosen;
	char	**rgb;
	int		j;
	int		k;
    int     fd_file;
    int     found;
    int     i;

	j = 0;
	k = 0;
    found = 0;
    fd_file = open(file, O_RDONLY);
    if (fd_file == -1)
        return (1);

    line = get_next_line(fd_file);
    while (line)
    {
        if (ft_strncmp(line, compass, ft_strlen(compass)) == 0 && line[ft_strlen(compass)] == ' ')
        {
            choosen = ft_split(line, ' ');
            if (!choosen)
                return (free(line), close(fd_file), 1);

            i = 0;
            while (choosen[i])
            {
				//printf("LA VARIABLE ES %s", choosen[i]);
                if (ft_isdigit(choosen[i][0]))
                {
					int len = ft_strlen(choosen[i]);
					if (choosen[i][len - 1] == ',')
					{
    					free_split(choosen);
    					free(line);
    					close(fd_file);
    					return (1);
					}
					rgb = ft_split(choosen[i], ',');
					j = 0;
					while (rgb[j])
					{
						rgb[j] = ft_strtrim(rgb[j], "\n");
						j++;
					}
					j = 0;
					if (count_words(rgb) == 3)
					{
						found = 1;
						while (rgb[j])
						{
							k = 0;
							int value = ft_atoi(rgb[j]);
							if (value < 0 || value > 255)
							{
        						found = 0;
								break ;
							}
							//printf("LA VARIABLE ES %d\n", found);
							while (rgb[j][k])
							{
								if (!ft_isdigit(rgb[j][k]))
								{
									found = 0;
									break ;
								}
								k++;
							}
							if (found == 0)
								break;
							j++;
						}
					}
					if (choosen[2] != NULL)
					{
    					free_split(choosen);
    					free(line);
    					close(fd_file);
    					return (1);
					}
                    break;
				}
                i++;
            }
            free_split(choosen);
        }
        free(line);
        line = get_next_line(fd_file);
    }
    close(fd_file);
    return (!found);
}

/*Map things*/
int	is_map_line(char *line)
{
	while (*line == ' ')
		line++;

	while (*line)
	{
		if (!ft_strchr(" 01NSEW\n", *line))
			return (0);
		line++;
	}
	return (1);
}
int	count_map_lines(char *file)
{
	int		fd;
	int		count;
	char	*line;

	fd = open(file, O_RDONLY);
	count = 0;

	line = get_next_line(fd);
	while (line)
	{
		if (is_map_line(line))
			count++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (count);
}

char	**create_map(char *file)
{
	int	num_lines;
	int	fd_file;
	char *line;
	char **map;
	int	i;

	num_lines = count_map_lines(file);
	i = 0;
	map = malloc(sizeof(char *) * (num_lines + 1));
	if (!map)
		return (NULL);
	fd_file = open(file, O_RDONLY);
	line = get_next_line(fd_file);
	while (line)
	{
		if (is_map_line(line))
			map[i++] = line;
		else
			free(line);
		line = get_next_line(fd_file);
	}
	map[i] = NULL;
	return (map);
}

int	map_width(char **map)
{
	int	i;
	int	max;
	int	len;

	i = 0;
	max = 0;
	while (map[i])
	{
		len = ft_strlen(map[i]);
		if (len > max)
			max = len;
		i++;
	}
	return (max);
}

char	**expand_map(char **map)
{
	int	w;
	int h;
	char	**new_map;
	int 	i;
	int		x
	int		y;

	i = 0;
	x = 0;
	y = 0;
	h = count_words(map);
	w = map_width(map);
	new_map = malloc(sizeof(char *) * (h + 3));
	while (i < h + 2)
	{
		new_map[i] = malloc(sizeof(char) * (old_width + 3));
		i++;
	}
	while(y < h + 2)
	{
		while(x < w + 2)
		{
			new_map[y][x] = ' ';
			x++;
		}
		new_map[y][w + 2] = '\0';
		y++;
	}
	for (int y = 0; y < old_height; y++)
	{
    	for (int x = 0; x < old_width; x++)
    	{
    	    new_map[y + 1][x + 1] = map[y][x];
	    }
	}
	return (new_map);
}

static void	flood_fill(char **ctx, int x, int y)
{
	char	c;

	if (x < 0 || x >= ctx->width || y < 0 || y >= ctx->height)
		return ;
	c = ctx->map[y][x];
	if (c == '1' || c == 'V')
		return ;
	if (c == ' ')
		return ;
	if (c == 'C')
		ctx->found->collectible++;
	else if (c == 'E')
		ctx->found->exit++;
	ctx->map[y][x] = 'V';
	flood_fill(ctx, x + 1, y);
	flood_fill(ctx, x - 1, y);
	flood_fill(ctx, x, y + 1);
	flood_fill(ctx, x, y - 1);
}

int	file_map(char *file)
{
	char **map;
	char **map_exp;
	int	i;

	map = create_map(file);
	i = 0;
	while (map[i])
	{
		printf("%s\n", map[i]);
		i++;
	}
	map_exp = expand_map(map);
	flood_fill(new_map, 0, 0);
	return (0);
}

int	parser_file_content(char *file)
{
	if (file_walls(file, "NO") != 0)
		printf("Error\nThere is an error in your .cub file"
			" regarding north texture.\n");
	else if (file_walls(file, "SO") != 0)
		printf("Error\nThere is an error in your .cub file"
			" regarding south texture.\n");
	else if (file_walls(file, "WE") != 0)
		printf("Error\nThere is an error in your .cub file"
			" regarding west texture.\n");
	else if (file_walls(file, "EA") != 0)
		printf("Error\nThere is an error in your .cub file"
			" regarding east texture.\n");
	else if (file_color(file, "F") != 0)
		printf("Error\nThere is an error in your .cub file"
			" regarding floor color.\n");
	else if (file_color(file, "C") != 0)
		printf("Error\nThere is an error in your .cub file"
			" regarding ceiling color.\n");
	else if (file_map(file) != 0)
		printf("Error\nThere is an error in your .cub file"
			" regarding the map.\n");
	else
		printf("GOOD\n");
	return (0);
}

int	parser_file_extension(char *file)
{
	char	*start;
	int		fd;

	start = file + ft_strlen(file) - 4;
	if (!ft_strnstr(start, ".cub", 4))
	{
		printf("Error\n"
			"The file you are trying to enter"
			" is not valid or doesn't exists.\n");
		return (1);
	}
	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		printf("Error\n"
			"The file can not be opened\n");
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
		printf("Error\nThe number of arguments entered"
			" is not compatible with the program.\n");
		return (1);
	}
	if (parser_file_extension(argv[1]) != 0)
		return (1);
	parser_file_content(argv[1]);
	return (0);
}*/
