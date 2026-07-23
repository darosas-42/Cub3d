/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacortes <cacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 09:31:07 by cacortes          #+#    #+#             */
/*   Updated: 2026/07/23 12:47:18 by cacortes         ###   ########.fr       */
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

int	texture_extension(char *texture)
{
	int	start;

	start = ft_strlen(texture) - 4;
	if (start < 0 || ft_strncmp(&texture[start], ".xpm", 4) != 0)
	{
		printf("Error\n"
			"The texture you are trying to enter"
			" is not valid or doesn't exists.\n");
		return (1);
	}
	return (0);
}

int	struct_saver(char *compass, t_map_info *map, char *choosen)
{
	if (ft_strncmp(compass, "NO", 3) == 0)
	{
		map->no_path = ft_strdup(choosen);
		map->no_ind++;
	}
	else if (ft_strncmp(compass, "SO", 3) == 0)
	{
		map->so_path = ft_strdup(choosen);
		map->so_ind++;
	}
	else if (ft_strncmp(compass, "WE", 3) == 0)
	{
		map->we_path = ft_strdup(choosen);
		map->we_ind++;
	}
	else if (ft_strncmp(compass, "EA", 3) == 0)
	{
		map->ea_path = ft_strdup(choosen);
		map->ea_ind++;
	}
	return (0);
}

void	color_saver(char *compass, t_map_info *map, int color)
{
	if (ft_strncmp(compass, "F", 2) == 0)
	{
		map->floor_color = color;
		map->floor_ind++;
	}
	else if (ft_strncmp(compass, "C", 2) == 0)
	{
		map->ceiling_color = color;
		map->ceiling_ind++;
	}
/*	printf("EL COLOR SUELO ES: %d\n", map->floor_color);
	printf("EL COLOR CIELO ES: %d\n", map->ceiling_color);*/
}


int file_walls(char *file, char *compass, t_map_info *map)
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
					if (texture_extension(filename) != 0)
						return (1);
                    fd_choosen = open(filename, O_RDONLY);
                    free(filename);
                    if (fd_choosen != -1)
                    {
                        close(fd_choosen);
                        found = 1;
                    }
				//	printf("LA VARIABLE ES %s\n\n", choosen[i]);
					if (choosen[2] != NULL)
					{
    					free_split(choosen);
    					free(line);
    					close(fd_file);
    					return (1);
					}
					if (struct_saver(compass, map, choosen[i]))
						return (1);
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

int	rgb_parser(int r, int g, int b)
{
	return ((r << 16) | (g << 8) | b);
}

int file_color(char *file, char *compass, t_map_info *map)
{
    char    *line;
    char    **choosen;
	char	**rgb;
	int		j;
	int		k;
    int     fd_file;
    int     found;
    int     i;
	int		r;
	int		g;
	int		b;
	int		color;

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
							if (j == 0)
								r = value;
							else if (j == 1)
								g = value;
							else if (j == 2)
								b = value;

							j++;
						}
						color = rgb_parser(r, g, b);
						color_saver(compass, map, color);
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
	if (*line == '\n' || *line == '\0')
		return (0);
	while (*line)
	{
		if (!ft_strchr(" 01NSEW\n", *line))
			return (0);
		line++;
	}
	return (1);
}
int	is_empty_line(char *line)
{
	while (*line == ' ')
		line++;
	if (*line == '\n' || *line == '\0')
		return (1);
	return (0);
}

int	check_map_last(char *file)
{
	int		fd;
	int		in_map;
	int		after_map;
	char	*line;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (1);

	in_map = 0;
	after_map = 0;
	line = get_next_line(fd);

	while (line)
	{
		if (!in_map)
		{
			if (is_map_line(line))
				in_map = 1;
		}
		else
		{
			if (after_map)
			{
				if (!is_empty_line(line))
				{
					free(line);
					close(fd);
					return (1);
				}
			}
			else
			{
				if (!is_map_line(line))
				{
					if (is_empty_line(line))
						after_map = 1;
					else
					{
						free(line);
						close(fd);
						return (1);
					}
				}
			}
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
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

char	**save_map(char *file, t_map_info *map)
{
	int		num_lines;
	int		fd_file;
	char	*line;
	char	**map_save;
	int		i;

	num_lines = count_map_lines(file);
	i = 0;
	(void)map;
	map_save = malloc(sizeof(char *) * (num_lines + 1));
	if (!map_save)
		return (NULL);
	fd_file = open(file, O_RDONLY);
	line = get_next_line(fd_file);
	while (line)
	{
		if (is_map_line(line))
		{
			map_save[i++] = ft_strtrim(line, "\n");
			free(line);
		}
		else
			free(line);
		line = get_next_line(fd_file);
	}
	map_save[i] = NULL;
	return (map_save);
}

int	map_height(t_map_info *map)
{
	int	i;

	i = 0;
	while (map->grid[i])
		i++;
	return (i);
}

int	map_width(t_map_info *map)
{
	int	i;
	int	max;

	i = 0;
	max = 0;
	while (map->grid[i])
	{
		if ((int)ft_strlen(map->grid[i]) > max)
			max = ft_strlen(map->grid[i]);
		i++;
	}
	return (max);
}

char	**expand_map(t_map_info *map)
{
	char	**new_map;
	int 	i;
	int		x;
	int		y;
	int		len;

	i = 0;
	len = 0;
	map->height = map_height(map);
	map->width = map_width(map);
	new_map = malloc(sizeof(char *) * (map->height + 3));
	while (i < map->height + 3)
	{
		new_map[i] = malloc(sizeof(char) * (map->width + 3));
		i++;
	}
	y = 0;
	while (y < map->height + 3)
	{
		x = 0;
		while (x < map->width + 2)
			new_map[y][x++] = ' ';
		new_map[y][map->width + 2] = '\0';
		y++;
	}
	y = 0;
	while (y < map->height)
	{
		len = ft_strlen(map->grid[y]);
		x = 0;
		while (x < len)
		{
			new_map[y + 1][x + 1] = map->grid[y][x];
			x++;
		}
		y++;
	}
	return (new_map);
}

static int	flood_fill(char **map_exp, int x, int y, t_map_info *map)
{
	char	c;

	if (x < 0 || x >= map->width + 2 || y < 0 || y >= map->height + 2)
	{
		//printf("FUERA (%d,%d)\n", x, y);
		return (0);
	}
	c = map_exp[y][x];
	//printf("(%2d,%2d) -> '%c'\n", x, y, c ? c : '#');
	if (c == 'V' || c == '1')
		return (0);
	if (ft_strchr("0NSEW", c))
	{
	//	printf("TOCADO '%c' EN (%d,%d)\n", c, x, y);
		return (1);
	}
	map_exp[y][x] = 'V';
	if (flood_fill(map_exp, x + 1, y, map))
		return (1);
	if (flood_fill(map_exp, x - 1, y, map))
		return (1);
	if (flood_fill(map_exp, x, y + 1, map))
		return (1);
	if (flood_fill(map_exp, x, y - 1, map))
		return (1);
	return (0);
}

int	check_player_pos(char **map_save, t_map_info *map)
{
	int	i;
	int	j;

	i = 0;
	while (map_save[i])
	{
		j = 0;
		while (map_save[i][j])
		{
			if (ft_strchr("NSEW", map_save[i][j]))
				map->player_ind++;
			j++;
		}
		i++;
	}
	if (map->player_ind != 1)
		return (1);
	return (0);
}

int	file_map(char *file, t_map_info *map)
{
	//char **map;
	//t_map_info map;
	char **map_exp;

	if (check_map_last(file))
		return (1);
	map->grid = save_map(file, map);
	if (check_player_pos(map->grid, map) != 0)
		return (1);
	map_exp = expand_map(map);
	(void)map_exp;
	/*printf("map.width = %d\n", map->width);
	for (int y = 0; y < map->height + 2; y++)
	{
		 printf("fila %2d: strlen=%zu\n", y, ft_strlen(map_exp[y]));
		//printf("%2d |%s|\n", y, map_exp[y]);
	}*/
	if (flood_fill(map_exp, 0, 0, map) != 0)
		return (1);
	return (0);
}

int	copycat_detector(t_map_info *map)
{
	if (map->no_ind > 1)
		return (1);
	if (map->so_ind > 1)
		return (1);
	if (map->we_ind > 1)
		return (1);
	if (map->ea_ind > 1)
		return (1);
	if (map->floor_ind > 1)
		return (1);
	if (map->ceiling_ind > 1)
		return (1);
	return (0);
}

int	parser_file_content(char *file, t_map_info *map)
{
	if (file_walls(file, "NO", map) != 0)
		printf("Error\nThere is an error in your .cub file"
			" regarding north texture.\n");
	else if (file_walls(file, "SO", map) != 0)
		printf("Error\nThere is an error in your .cub file"
			" regarding south texture.\n");
	else if (file_walls(file, "WE", map) != 0)
		printf("Error\nThere is an error in your .cub file"
			" regarding west texture.\n");
	else if (file_walls(file, "EA", map) != 0)
		printf("Error\nThere is an error in your .cub file"
			" regarding east texture.\n");
	else if (file_color(file, "F", map) != 0)
		printf("Error\nThere is an error in your .cub file"
			" regarding floor color.\n");
	else if (file_color(file, "C", map) != 0)
		printf("Error\nThere is an error in your .cub file"
			" regarding ceiling color.\n");
	else if (file_map(file, map) != 0)
		printf("Error\nThere is an error in your .cub file"
			" regarding the map.\n");
	else if (copycat_detector(map) != 0)
		printf("Error\nThere is a duplicate indicator in your .cub file.\n");
	else // Estas líneas
	printf("GOOD\n"); // Se van a quitar en el final
	/*printf("NO %d\n", map->no_ind);
	printf("SO %d\n", map->so_ind);
	printf("WE %d\n", map->we_ind);
	printf("EA %d\n", map->ea_ind);
	printf("F  %d\n", map->floor_ind);
	printf("C  %d\n", map->ceiling_ind);*/
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

void	init_map_info(t_map_info *map)
{
	map->no_path = NULL;
	map->so_path = NULL;
	map->we_path = NULL;
	map->ea_path = NULL;

	map->no_ind = 0;
	map->so_ind = 0;
	map->we_ind = 0;
	map->ea_ind = 0;

	map->floor_color = 0;
	map->ceiling_color = 0;

	map->floor_ind = 0;
	map->ceiling_ind = 0;

	map->player_ind = 0;

	map->grid = NULL;
	map->width = 0;
	map->height = 0;
}

int	main(int argc, char **argv)
{
	t_map_info map;

	init_map_info(&map);
	if (argc != 2)
	{
		printf("Error\nThe number of arguments entered"
			" is not compatible with the program.\n");
		return (1);
	}
	if (parser_file_extension(argv[1]) != 0)
		return (1);
	if (parser_file_content(argv[1], &map))
		return (1);
	return (0);
}
