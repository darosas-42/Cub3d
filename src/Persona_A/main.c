/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacortes <cacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 09:31:07 by cacortes          #+#    #+#             */
/*   Updated: 2026/07/12 11:53:45 by cacortes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	parser_extension(char *file)
{
	char * start;

	start = file + ft_strlen(file) - 4;
	if (!ft_strnstr(start, ".cub", 4))
	{
		ft_printf("Error\nThe file extension you are trying to enter is not valid.\n");
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
		ft_printf("Error\nThe number of arguments entered is not compatible with the program.\n");
		return (1);
	}
	parser_extension(argv[1]);
	return (0);
}*/
