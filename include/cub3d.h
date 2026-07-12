#ifndef CUB3D_H
# define CUB3D_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <math.h>
# include "../MLX42/include/MLX42/MLX42.h" // Incluye tu librería MLX correspondiente

# include "ft_printf.h" // Added to use printf
# include "libft.h" // Added to use strlen and strnstr

// --- CONSTANTES ---
# define SCREEN_W 1024
# define SCREEN_H 768
# define TILE_SIZE 64 // Útil si hacéis un minimapa, aunque para el motor 3D a veces no se usa directamente.

// --- 1. ESTRUCTURA DE TEXTURAS Y COLORES (Responsabilidad Persona A) ---
typedef struct s_map_info
{
    char    *no_path;    // Ruta a la textura Norte
    char    *so_path;    // Ruta a la textura Sur
    char    *we_path;    // Ruta a la textura Oeste
    char    *ea_path;    // Ruta a la textura Este
    
    // Los colores del techo (Ceiling) y suelo (Floor) en formato RGB
    // Se suelen guardar como un int (ARGB) para que la MLX los entienda rápido
    int     floor_color; 
    int     ceiling_color;

    // El mapa en sí
    char    **grid;      // La matriz del mapa (el laberinto)
    int     width;       // Anchura máxima del mapa (para no salirnos)
    int     height;      // Altura del mapa
} t_map_info;

// --- 2. ESTRUCTURA DEL JUGADOR Y RAYCASTING (Responsabilidad Persona B) ---
typedef struct s_player
{
    double  x;           // Posición X exacta en el mapa (ej. 5.5)
    double  y;           // Posición Y exacta en el mapa (ej. 3.2)
    
    // Vectores direccionales (Hacia dónde mira)
    double  dir_x;       
    double  dir_y;
    
    // Plano de la cámara (FOV - Campo de visión)
    double  plane_x;
    double  plane_y;
    
    double  move_speed;  // Velocidad de movimiento
    double  rot_speed;   // Velocidad de giro
} t_player;

// --- 3. ESTRUCTURA DE LA LIBRERÍA GRÁFICA (Responsabilidad Persona B) ---
typedef struct s_graphics
{
    void    *mlx_ptr;    // Puntero de conexión a la librería
    void    *win_ptr;    // Puntero a la ventana
    
    // La imagen principal donde dibujáis el frame antes de volcarlo a la pantalla
    void    *img_ptr;
    char    *img_data;
    int     bpp;
    int     line_length;
    int     endian;
    
    // Aquí podríais añadir luego un array de imágenes para las 4 texturas cargadas
} t_graphics;

// --- ESTRUCTURA PRINCIPAL (El "Contrato") ---
typedef struct s_game
{
    t_map_info  map;
    t_player    player;
    t_graphics  gfx;
} t_game;

#endif
