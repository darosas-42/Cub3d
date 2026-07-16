/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreix <darosas-@student.42malaga.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 16:07:04 by dreix             #+#    #+#             */
/*   Updated: 2026/07/16 02:09:17 by dreix            ###   ########.fr       */
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

void	ft_free_map(t_game *game)
{
/* 	int	i;

	if (!game->map.grid)
		return;
	i = game->map.height;
	while (--i > -1)
		free(game->map.grid[i]); */
	free(game->map.grid);
	game->map.grid = NULL;
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
	int	mapX;
	int	mapY;
	double	deltaDistX;
	double	deltaDistY;
	int	stepX;
	int	stepY;
	double	sideDistX;
	double	sideDistY;
	int	x;
	int	hit;
	int	side;
	double	perpWallDist;
	int	lineHeight;
	int	drawStart;
	int	drawEnd;
	int y;

	x = 0;
	while (x < SCREEN_W)
	{
		y = 0;
		while (y < SCREEN_H)
		{
			if (y > (SCREEN_H / 2))
				mlx_put_pixel(game->gfx.img_ptr, x, y, get_rgba(137, 81, 41, 255));
			else
				mlx_put_pixel(game->gfx.img_ptr, x, y, get_rgba(130, 200, 229, 255));
			y++;
		}
		x++;
	}
	x = 0;
	while (x < SCREEN_W)
	{
		mapX = (int)game->player.x;
		mapY = (int)game->player.y;
		cameraX = 2 * x / (double)SCREEN_W - 1;
		rayDirX = game->player.dir_x + game->player.plane_x * cameraX;
		rayDirY = game->player.dir_y + game->player.plane_y * cameraX;
		if (rayDirX == 0)
			deltaDistX = 1e30;
		else
			deltaDistX = fabs(1 / rayDirX);
		if (rayDirY == 0)
			deltaDistY = 1e30;
		else
			deltaDistY = fabs(1 / rayDirY);
		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (game->player.x - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1 - game->player.x) * deltaDistX;
		}
		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (game->player.y - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1 - game->player.y) * deltaDistY;
		}
		hit = 0;
		while (hit == 0)
		{
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			if (mapX >= 0 && mapX < game->map.height && mapY >= 0 && mapY < game->map.width)
			{
				if (game->map.grid[mapX][mapY] == '1')
					hit = 1;
			}
			else
				hit = 1;
		}
		if (side == 0)
			perpWallDist = (sideDistX - deltaDistX);
		else
			perpWallDist = (sideDistY - deltaDistY);
		lineHeight = (int)(SCREEN_H / perpWallDist);
		drawStart = -lineHeight / 2 + SCREEN_H / 2;
		if (drawStart < 0)
			drawStart = 0;
		drawEnd = lineHeight / 2 + SCREEN_H / 2;
		if (drawEnd >= SCREEN_H)
			drawEnd = SCREEN_H - 1;
		y = drawStart;
		while (y <= drawEnd)
		{
			if (side == 0)
				mlx_put_pixel(game->gfx.img_ptr, x, y, get_rgba(255, 222, 33, 255));
			else
				mlx_put_pixel(game->gfx.img_ptr, x, y, get_rgba(180, 222, 33, 255));
			y++;
		}
		x++;
	}
}


void	ft_hook(void* param)
{
	t_game *game;
	double oldDirX;
	double oldPlaneX;
	double  margin;
    double  next_x;
    double  next_y;

	game = param;
	game->player.move_speed = 0.02;
	game->player.rot_speed = 0.02;
	margin = 0.1;
	if (mlx_is_key_down(game->gfx.mlx_ptr, MLX_KEY_ESCAPE))
	{
		mlx_close_window(game->gfx.mlx_ptr);
		//ft_free_map(game);
	}
	if (mlx_is_key_down(game->gfx.mlx_ptr, MLX_KEY_W))
    {
        next_x = game->player.x + (game->player.dir_x * game->player.move_speed);
        next_y = game->player.y + (game->player.dir_y * game->player.move_speed);
        if (game->player.dir_x > 0)
        {
            if (game->map.grid[(int)(game->player.y)][(int)(next_x + margin)] != '1')
                game->player.x = next_x;
        }
        else
        {
            if (game->map.grid[(int)(game->player.y)][(int)(next_x - margin)] != '1')
                game->player.x = next_x;
        }
        if (game->player.dir_y > 0)
        {
            if (game->map.grid[(int)(next_y + margin)][(int)(game->player.x)] != '1')
                game->player.y = next_y;
        }
        else
        {
            if (game->map.grid[(int)(next_y - margin)][(int)(game->player.x)] != '1')
                game->player.y = next_y;
        }
    }
	if (mlx_is_key_down(game->gfx.mlx_ptr, MLX_KEY_S))
    {
        next_x = game->player.x - (game->player.dir_x * game->player.move_speed);
        next_y = game->player.y - (game->player.dir_y * game->player.move_speed);
        if (game->player.dir_x < 0)
        {
            if (game->map.grid[(int)(game->player.y)][(int)(next_x + margin)] != '1')
                game->player.x = next_x;
        }
        else
        {
            if (game->map.grid[(int)(game->player.y)][(int)(next_x - margin)] != '1')
                game->player.x = next_x;
        }

        if (game->player.dir_y < 0)
        {
            if (game->map.grid[(int)(next_y + margin)][(int)(game->player.x)] != '1')
                game->player.y = next_y;
        }
        else
        {
            if (game->map.grid[(int)(next_y - margin)][(int)(game->player.x)] != '1')
                game->player.y = next_y;
        }
    }
	if (mlx_is_key_down(game->gfx.mlx_ptr, MLX_KEY_D) || mlx_is_key_down(game->gfx.mlx_ptr, MLX_KEY_RIGHT))
	{
		oldDirX = game->player.dir_x;
		game->player.dir_x = game->player.dir_x * cos(-game->player.rot_speed) - game->player.dir_y * sin(-game->player.rot_speed);
		game->player.dir_y = oldDirX * sin(-game->player.rot_speed) + game->player.dir_y * cos(-game->player.rot_speed);

		oldPlaneX = game->player.plane_x;
		game->player.plane_x = game->player.plane_x * cos(-game->player.rot_speed) - game->player.plane_y * sin(-game->player.rot_speed);
		game->player.plane_y = oldPlaneX * sin(-game->player.rot_speed) + game->player.plane_y * cos(-game->player.rot_speed);
	}
	if (mlx_is_key_down(game->gfx.mlx_ptr, MLX_KEY_A) || mlx_is_key_down(game->gfx.mlx_ptr, MLX_KEY_LEFT))
	{
		oldDirX = game->player.dir_x;
		game->player.dir_x = game->player.dir_x * cos(game->player.rot_speed) - game->player.dir_y * sin(game->player.rot_speed);
		game->player.dir_y = oldDirX * sin(game->player.rot_speed) + game->player.dir_y * cos(game->player.rot_speed);
		oldPlaneX = game->player.plane_x;
		game->player.plane_x = game->player.plane_x * cos(game->player.rot_speed) - game->player.plane_y * sin(game->player.rot_speed);
		game->player.plane_y = oldPlaneX * sin(game->player.rot_speed) + game->player.plane_y * cos(game->player.rot_speed);
	}
	render_frame(game);
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
	if (mlx_image_to_window(game.gfx.mlx_ptr, game.gfx.img_ptr, 0, 0) == -1)
	{
		mlx_close_window(game.gfx.mlx_ptr);
		conststr(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	mlx_loop_hook(game.gfx.mlx_ptr, ft_hook, &game);

	mlx_loop(game.gfx.mlx_ptr);
	mlx_delete_image(game.gfx.mlx_ptr, game.gfx.img_ptr);
	mlx_terminate(game.gfx.mlx_ptr);
	return (EXIT_SUCCESS);
}
