/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hsl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elee <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/30 17:47:27 by elee              #+#    #+#             */
/*   Updated: 2017/06/02 17:46:18 by elee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static float	hue_to_rgb(float v1, float v2, float hue)
{
	if (hue < 0)
		hue += 1;
	if (hue > 1)
		hue -= 1;
	if (6 * hue < 1)
		return (v1 + (v2 - v1) * 6 * hue);
	if (2 * hue < 1)
		return (v2);
	if (3 * hue < 2)
		return (v1 + (v2 - v1) * (2.0 / 3 - hue) * 6);
	return (v1);
}

t_rgb			hsl_to_rgb(t_hsl hsl)
{
	t_rgb	rgb;
	float	hue;
	float	v1;
	float	v2;

	rgb.r = 0;
	rgb.g = 0;
	rgb.b = 0;
	if (hsl.s == 0.0)
	{
		rgb.r = hsl.l * 255;
		rgb.g = hsl.l * 255;
		rgb.b = hsl.l * 255;
	}
	else
	{
		hue = (float)hsl.h / 360;
		v2 = (hsl.l < 0.5) ? (hsl.l * (1 + hsl.s)) :
			((hsl.l + hsl.s) - (hsl.l * hsl.s));
		v1 = 2 * hsl.l - v2;
		rgb.r = (255 * hue_to_rgb(v1, v2, hue + 1.0 / 3));
		rgb.g = (255 * hue_to_rgb(v1, v2, hue));
		rgb.b = (255 * hue_to_rgb(v1, v2, hue - 1.0 / 3));
	}
	return (rgb);
}
