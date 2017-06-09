/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elee <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/23 21:56:25 by elee              #+#    #+#             */
/*   Updated: 2017/06/02 18:56:18 by elee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static void	set_color(t_env *env, int index, int i)
{
	t_hsl	hsl;
	t_rgb	rgb;

	hsl.h = ((int)(sin(M_PI / 2 * i / env->iter) * 360) + env->c_rot) % 360;
	hsl.s = 0.5;
	hsl.l = 0.75 - i / 2.0 / env->iter;
	rgb = hsl_to_rgb(hsl);
	env->data[index + 0] = rgb.r;
	env->data[index + 1] = rgb.g;
	env->data[index + 2] = rgb.b;
}

static void	draw_point(t_env *env, int x, int y, int i)
{
	int		index;

	if (x < 0 || x >= XSIZE || y < 0 || y >= YSIZE)
		return ;
	index = env->sl * y + env->bpp * x;
	if (i == -1 || i == env->iter)
	{
		env->data[index + 0] = 0;
		env->data[index + 1] = 0;
		env->data[index + 2] = 0;
	}
	else
		set_color(env, index, i);
}

static void	draw_string(t_env *env)
{
	char	*num;

	mlx_string_put(env->mlx, env->win, 0, 0, 0xAA6699, "Iteration: ");
	num = ft_itoa(env->iter);
	mlx_string_put(env->mlx, env->win, 100, 0, 0xAA6699, num);
	free(num);
}

static void	fill_img(t_env *env, int iter_map[XSIZE][YSIZE])
{
	int	x;
	int	y;
	
	y = -1;
	while (++y < YSIZE)
	{
		x = -1;
		while (++x < XSIZE)
			draw_point(env, x, y, iter_map[x][y]);
	}
}

void		draw_fractal(t_env *env)
{
	size_t		global;
	int			err;
	int			h_iter_map[XSIZE][YSIZE];
	cl_mem		d_iter_map;
	cl_event	compute;
	int			width;

	width = XSIZE;
	mlx_clear_window(env->mlx, env->win);
	env->img = mlx_new_image(env->mlx, XSIZE, YSIZE);
	env->data = mlx_get_data_addr(env->img, &(env->bpp), &(env->sl), &env->end);
	env->bpp /= 8;
	d_iter_map = clCreateBuffer(env->context->context,
			CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
			sizeof(int) * XSIZE * YSIZE, h_iter_map[0], NULL);
	clSetKernelArg(env->fractal, 0, sizeof(float), &(env->re_min));
	clSetKernelArg(env->fractal, 1, sizeof(float), &(env->re_offset));
	clSetKernelArg(env->fractal, 2, sizeof(float), &(env->re_factor));
	clSetKernelArg(env->fractal, 3, sizeof(float), &(env->im_min));
	clSetKernelArg(env->fractal, 4, sizeof(float), &(env->im_offset));
	clSetKernelArg(env->fractal, 5, sizeof(float), &(env->im_factor));
	clSetKernelArg(env->fractal, 6, sizeof(float), &(env->scale));
	clSetKernelArg(env->fractal, 7, sizeof(float), &(env->re_c));
	clSetKernelArg(env->fractal, 8, sizeof(float), &(env->im_c));
	clSetKernelArg(env->fractal, 9, sizeof(int), &(env->iter));
	clSetKernelArg(env->fractal, 10, sizeof(cl_mem), &d_iter_map);
	clSetKernelArg(env->fractal, 11, sizeof(int), &width);
	global = XSIZE * YSIZE;
	err = clEnqueueNDRangeKernel(env->context->commands, env->fractal, 1, NULL,
			&global, NULL, 0, NULL, &compute);
	clEnqueueReadBuffer(env->context->commands, d_iter_map, CL_TRUE, 0, 
			sizeof(int) * XSIZE * YSIZE, h_iter_map, 1, &compute, NULL);
	fill_img(env, h_iter_map);
	clFinish(env->context->commands);
	clReleaseMemObject(d_iter_map);
	mlx_put_image_to_window(env->mlx, env->win, env->img, 0, 0);
	draw_string(env);
	mlx_destroy_image(env->mlx, env->img);
}
