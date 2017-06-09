/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elee <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/23 21:10:45 by elee              #+#    #+#             */
/*   Updated: 2017/06/02 19:04:23 by elee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

t_env		*env_init(void)
{
	t_env	*env;

	env = (t_env*)malloc(sizeof(t_env));
	env->mlx = mlx_init();
	env->win = mlx_new_window(env->mlx, XSIZE, YSIZE, "Fract'ol");
	env->paused = 1;
	env->move = 0;
	env->iter = 50;
	env->c_rot = 0;
	env->scale = 1.0;
	env->re_offset = 0.0;
	env->im_offset = 0.0;
	env->re_min = -2.0;
	env->re_max = 2.0;
	env->im_min = -2.0;
	env->im_max = env->im_min + (env->re_max - env->re_min) * YSIZE / XSIZE;
	env->re_factor = (env->re_max - env->re_min) / (XSIZE - 1);
	env->im_factor = (env->im_max - env->im_min) / (YSIZE - 1);
	env->re_c = 0.0;
	env->im_c = 0.0;
	env->context = setup_context();
	return (env);
}

void	choose_fractal(t_env *env, char *fractal)
{
	if (!strcmp("M", fractal))
		env->fractal = make_kernel(env->context, "srcs/mandelbrot.cl", "mandelbrot");
	else if (!strcmp("J", fractal))
		env->fractal = make_kernel(env->context, "srcs/julia.cl", "julia");
	else if (!strcmp("BS", fractal))
		env->fractal = make_kernel(env->context, "srcs/burningship.cl", "burningship");
	else if (!strcmp("AJ", fractal))
		env->fractal = make_kernel(env->context, "srcs/absolutejulia.cl", "absolutejulia");
	else
		env->fractal = NULL;
}

int				main(int argc, char **argv)
{
	t_env		*env;

	env = NULL;
	if (argc != 2)
		display_usage(env);
	env = env_init();
	choose_fractal(env, argv[1]);
	if (env->fractal == NULL)
	{
		display_usage(env);
		free_exit(env);
	}
	display_control();
	mlx_expose_hook(env->win, &expose_hook, env);
	mlx_mouse_hook(env->win, &mouse_hook, env);
	mlx_hook(env->win, 2, 0, &key_hook, env);
	mlx_hook(env->win, 5, 0, &mouse_release_hook, env);
	mlx_hook(env->win, 6, 0, &motion_hook, env);
	mlx_loop(env->mlx);
	return (0);
}
