/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elee <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/28 03:45:48 by elee              #+#    #+#             */
/*   Updated: 2017/06/02 17:04:15 by elee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	free_exit(t_env *env)
{
	if (env)
	{
		if (env->win)
			mlx_destroy_window(env->mlx, env->win);
		if (env->mlx)
			free(env->mlx);
		free(env);
	}
	exit(0);
}

void	display_usage(t_env *env)
{
	printf("Usage: ./fractol [M/J/BS/AJ]\n");
	printf("EX:\n");
	printf("\tM = Mandelbrot\n");
	printf("\tJ = Julia\n");
	printf("\tBS = Burning Ship\n");
	printf("\tAJ = Absolute Julia\n");
	free_exit(env);
}

void	display_control(void)
{
	printf("Controls :\n");
	printf("\t+ - : Increase/decrease the number of max iterations\n");
	printf("\tMouse scroll up/down: zoom in/out around the mouse cursor\n");
	printf("\tMouse drag: translate\n");
	printf("\tNumpad + - : zoom in/out from the center of image\n");
	printf("\tArrow keys : translate\n");
	printf("\t< > : Rotate color palette\n");
	printf("\tp : pause the window (only for julia sets)\n");
	printf("\tesc : close the window\n");
}
