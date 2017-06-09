/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elee <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/29 20:27:14 by elee              #+#    #+#             */
/*   Updated: 2017/06/02 18:37:43 by elee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include "mlx.h"
# include <math.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <OpenCL/opencl.h>

# define DEVICE CL_DEVICE_TYPE_DEFAULT
# define MAX(a, b) (((a) > (b)) ? (a) : (b))

# define XSIZE 900
# define YSIZE 900

# define KEY_ESC 53

# define KEY_UP 126
# define KEY_DOWN 125
# define KEY_LEFT 123
# define KEY_RIGHT 124

# define KEY_PAUSE 35

# define KEY_PLUS 24
# define KEY_MINUS 27

# define KEY_ZOOMIN 69
# define KEY_ZOOMOUT 78

# define KEY_ROTM 43
# define KEY_ROTP 47

# define SCROLL_UP 4
# define SCROLL_DOWN 5

# define L_CLICK 1
# define R_CLICK 2

typedef struct			s_context
{
	cl_device_id		device_id;
	cl_context			context;
	cl_command_queue	commands;
}						t_context;

typedef struct	s_env
{
	void		*mlx;
	void		*win;
	void		*img;
	char		*data;
	int			bpp;
	int			sl;
	int			end;
	int			paused;
	int			move;
	int			iter;
	int			c_rot;
	int			start[2];
	float		scale;
	float		re_offset;
	float		im_offset;
	float		re_min;
	float		im_min;
	float		re_max;
	float		im_max;
	float		re_factor;
	float		im_factor;
	float		re_c;
	float		im_c;
	t_context	*context;
	cl_kernel	fractal;
}				t_env;

typedef struct	s_hsl
{
	int			h;
	float		s;
	float		l;
}				t_hsl;

typedef struct	s_rgb
{
	int			r;
	int			g;
	int			b;
}				t_rgb;

void			free_exit(t_env *env);
void			display_usage(t_env *env);
void			display_control(void);

int				key_hook(int key, t_env *env);
int				mouse_hook(int but, int x, int y, t_env *env);
int				mouse_release_hook(int but, int x, int y, t_env *env);
int				motion_hook(int x, int y, t_env *env);
int				expose_hook(t_env *env);

int				mandelbrot(t_env *env, int x, int y);
int				julia(t_env *env, int x, int y);
int				burning_ship(t_env *env, int x, int y);
int				absolute_julia(t_env *env, int x, int y);

t_rgb			hsl_to_rgb(t_hsl hsl);
void			draw_fractal(t_env *env);

char			*ft_itoa(int n);

char			*getKernelName(cl_kernel kernel);
char			*load_cl_file(char *filename);
cl_kernel		make_kernel(t_context *c, char *sourcefile, char *name);
t_context		*setup_context(void);
#endif
