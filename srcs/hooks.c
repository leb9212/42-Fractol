/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elee <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/23 21:32:23 by elee              #+#    #+#             */
/*   Updated: 2017/06/02 17:02:22 by elee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int		key_hook(int key, t_env *env)
{
	char	*name;

	if (key != KEY_ESC && key != KEY_ZOOMIN && key != KEY_ZOOMOUT &&
		key != KEY_LEFT && key != KEY_RIGHT && key != KEY_UP && key != KEY_DOWN
		&& key != KEY_PLUS && key != KEY_MINUS && key != KEY_PAUSE
		&& key != KEY_ROTP && key != KEY_ROTM)
		return (0);
	(key == KEY_ESC) ? free_exit(env) : 0;
	(key == KEY_ZOOMIN) ? (env->scale *= 1.2) : 0;
	(key == KEY_ZOOMOUT) ? (env->scale /= 1.2) : 0;
	(key == KEY_LEFT) ? (env->re_offset -= 45.0) : 0;
	(key == KEY_RIGHT) ? (env->re_offset += 45.0) : 0;
	(key == KEY_DOWN) ? (env->im_offset -= 45.0) : 0;
	(key == KEY_UP) ? (env->im_offset += 45.0) : 0;
	(key == KEY_PLUS) ? (env->iter += 5) : 0;
	(key == KEY_MINUS) ? (env->iter = MAX(0, env->iter - 5)) : 0;
	(key == KEY_ROTP) ? (env->c_rot = (env->c_rot + 10) % 360) : 0;
	(key == KEY_ROTM) ? (env->c_rot = (env->c_rot - 10) % 360) : 0;
	if (key == KEY_PAUSE)
	{
		name = getKernelName(env->fractal);
		if (!strcmp(name, "julia") || !strcmp(name, "absolutejulia"))
			env->paused = !env->paused;
		free(name);
	}
	draw_fractal(env);
	return (0);
}

int		mouse_hook(int but, int x, int y, t_env *env)
{
	if (but == SCROLL_UP)
	{
		env->re_offset = ((env->re_min + (x + env->re_offset) * env->re_factor)
				* 1.2 - env->re_min - x * env->re_factor) / env->re_factor;
		env->im_offset = -((env->im_min + (y - env->im_offset) * env->im_factor)
				* 1.2 - env->im_min - y * env->im_factor) / env->im_factor;
		env->scale *= 1.2;
	}
	else if (but == SCROLL_DOWN)
	{
		env->re_offset = ((env->re_min + (x + env->re_offset) * env->re_factor)
			* (1 / 1.2) - env->re_min - x * env->re_factor) / env->re_factor;
		env->im_offset = -((env->im_min + (y - env->im_offset) * env->im_factor)
			* (1 / 1.2) - env->im_min - y * env->im_factor) / env->im_factor;
		env->scale /= 1.2;
	}
	else if (but == L_CLICK && env->paused && (env->move = 1))
	{
		env->start[0] = x;
		env->start[1] = y;
	}
	else
		return (0);
	draw_fractal(env);
	return (0);
}

int		mouse_release_hook(int but, int x, int y, t_env *env)
{
	(void)x;
	(void)y;
	if (but == L_CLICK && env->paused && env->move)
		env->move = 0;
	return (0);
}

int		motion_hook(int x, int y, t_env *env)
{
	if (!env->paused)
	{
		env->re_c = (float)x / (XSIZE - 1) * (env->re_max - env->re_min) - env->re_max;
		env->im_c = (float)y / (YSIZE - 1) * (env->im_max - env->im_min) - env->im_max;
	}
	else if (env->move)
	{
		env->re_offset += (env->start[0] - x);
		env->im_offset -= (env->start[1] - y);
		env->start[0] = x;
		env->start[1] = y;
	}
	else
		return (0);
	draw_fractal(env);
	return (0);
}

int		expose_hook(t_env *env)
{
	draw_fractal(env);
	return (0);
}
