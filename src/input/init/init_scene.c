/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdahlhof <cdahlhof@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 17:44:19 by cdahlhof          #+#    #+#             */
/*   Updated: 2022/06/15 16:47:21 by cdahlhof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minirm.h"

/**
 * initilzes a Ambient Light struct
 * @param al [t_al *] ambient light struct
 * @param info [char **] information line split by whitespace
 * @return [int] 0 if successful; 1 if error in line
*/
int	init_al(t_al *al, char **info)
{
	int	i;

	i = 1;
	if (!info || !info[0])
		return (1);
	else
		al->id = 'A';
	if (!info[i] || init_dim(info[i ++], &al->lr, 2))
		return (1);
	if (!info[i] || init_rgb(ft_split(info[i ++], ','), &al->r, &al->g, &al->b))
		return (1);
	if (info[i])
		return (1);
	return (0);
}

/**
 * initilzes a Camara struct
 * @param ca [t_cam] Camera struct
 * @param info [char **] information line split by whitespace
 * @return [int] 0 if successful; 1 if error in line
*/
int	init_cam(t_cam *ca, char **info)
{
	int	i;

	i = 1;
	if (!info || !info[0])
		return (1);
	else
		ca->id = 'K';
	if (!info[i] || init_cor(ft_split(info[i ++], ','), &ca->cor, 0))
		return (1);
	if (!info[i] || init_cor(ft_split(info[i ++], ','), &ca->v_o, 1))
		return (1);
	if (!info[i] || init_fov(info[i ++], &ca->fov))
		return (1);
	if (info[i])
		return (1);
	if (ca->v_o.x == 0.0 && ca->v_o.y == 0.0 && ca->v_o.z == 0.0)
		ca->v_o.x = 1.0;
	// limit(&ca->fov, 179.9, 0);
	return (0);
}

/**
 * initilzes a Light struct with bonus RGB
 * @param l [t_light **] light bonus struct
 * @param info [char **] information line split by whitespace
 * @return [int] 0 if successful; 1 if error in line
*/
int	init_light(t_light **l, char **info, int p)
{
	int	i;

	i = 1;
	if (!info || !info[0])
		return (1);
		l[p] = ft_calloc(sizeof(t_light), 1);
		l[p]->id = 'l';
	if (!info[i] || init_cor(ft_split(info[i ++], ','), &l[p]->cor, 0))
		return (1);
	if (!info[i] || init_dim(info[i ++], &l[p]->lr, 2))
		return (1);
	if (!info[i] || init_rgb(ft_split(info[i ++], ','), &l[p]->r, &l[p]->g, &l[p]->b))
		return (1);
	if (info[i])
		return (1);
	return (0);
}
