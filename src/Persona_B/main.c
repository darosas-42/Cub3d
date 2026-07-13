/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacortes <cacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 16:07:04 by dreix             #+#    #+#             */
/*   Updated: 2026/07/12 11:53:54 by cacortes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "cub3d.h"

static void	conststr(const char *s)
{
	int	i;

	i = -1;
	if (!s)
		return ;
	while (s[++i])
		write(1, &s[i], 1);
	write(1, "\n", 1);
}

void	ft_hook(void* param)
{
	t_game *game;

	game = param;
	if (mlx_is_key_down(game->gfx.mlx_ptr, MLX_KEY_ESCAPE))
		mlx_close_window(game->gfx.mlx_ptr);
}

void init_test_game(t_game *game)
{
    // 1. EL MAPA FALSO (Respetando el char **)
    game->map.grid = malloc(sizeof(char *) * 6);
    game->map.grid[0] = "11111";
    game->map.grid[1] = "10001";
    game->map.grid[2] = "10001";
    game->map.grid[3] = "10001";
    game->map.grid[4] = "11111";
    game->map.grid[5] = NULL;
    game->map.width = 5;
    game->map.height = 5;

    // 2. EL JUGADOR (Los valores de Lodev)
    game->player.x = 2.5;        // En el centro del mapa
    game->player.y = 2.5;
    
    game->player.dir_x = -1.0;   // Mirando hacia el Oeste
    game->player.dir_y = 0.0;
    
    game->player.plane_x = 0.0;  // El plano de la cámara (FOV de 66 grados)
    game->player.plane_y = 0.66; 
}

int get_rgba(int r, int g, int b, int a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

void	render_frame(t_game *game)
{
	double	cameraX;
	double	rayDirX;
	double	rayDirY;
	//int	mapX;
	//int	mapY;
	int	x;

	x = 0;
	//mapX = (int)game->player.x;
	//mapY = (int)game->player.y;
	while (x < SCREEN_W)
	{
		cameraX = 2 * x / (double)SCREEN_W - 1;
		rayDirX = game->player.dir_x + game->player.plane_x * cameraX;
		rayDirY = game->player.dir_y + game->player.plane_y * cameraX;
		if (x == 0 || x == (SCREEN_W / 2) || x == (SCREEN_W - 1))
			printf("X = %f, Y = %f \n", rayDirX, rayDirY);
		x++;
	}

}

int	main(void)
{
	t_game	game;

	init_test_game(&game);
	if (!(game.gfx.mlx_ptr = mlx_init(SCREEN_W, SCREEN_H, "Cub3d", false)))
	{
		conststr(mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	if (!(game.gfx.img_ptr = mlx_new_image(game.gfx.mlx_ptr, SCREEN_W, SCREEN_H)))
	{
		mlx_close_window(game.gfx.mlx_ptr);
		conststr(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}

	int	x = 0;
	while (x < SCREEN_W)
	{
		int	y = 0;
		while (y < SCREEN_H)
		{
			if (y > (SCREEN_H / 2))
				mlx_put_pixel(game.gfx.img_ptr, x, y, get_rgba(137, 81, 41, 255));
			else
				mlx_put_pixel(game.gfx.img_ptr, x, y, get_rgba(130, 200, 229, 255));
			y++;
		}
		x++;
	}
	if (mlx_image_to_window(game.gfx.mlx_ptr, game.gfx.img_ptr, 0, 0) == -1)
	{
		mlx_close_window(game.gfx.mlx_ptr);
		conststr(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}

	mlx_loop_hook(game.gfx.mlx_ptr, ft_hook, &game);

	render_frame(&game);

	mlx_loop(game.gfx.mlx_ptr);
	mlx_terminate(game.gfx.mlx_ptr);
	return (EXIT_SUCCESS);
}
