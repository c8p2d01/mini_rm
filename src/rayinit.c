/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rayinit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdahlhof <cdahlhof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 17:44:32 by cdahlhof          #+#    #+#             */
/*   Updated: 2025/10/16 13:46:45 by cdahlhof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirm.h"

/**
 * @brief to initialize i use an imaginary screen plane, to which i point my rays
 * @param	cam [t_cam*] the camera in the scene
 * @return	[double**] [0]: vector on the screenplane, orthogonal to the g_z axis
 * 					[1]: vector on the screenplane, orthogonal to the vector in [0]
 * 					[2]: direction vector of the correct length
 * distance = acos (FOV/2) * ("WDTH")
*/
t_vec3d	*screen(t_cam *cam)
{
	t_vec3d	*res;

	res = ft_malloc(3 * sizeof(t_vec3d));
	if (cam->v_o.x == 0 && cam->v_o.y == 0)
		res[0] = new_vec3d(1, 0, 0);
	else
	{
		res[0] = cross3d(cam->v_o, new_vec3d(0, 0, 1));
		normalise3d(&res[0]);
	}
	res[1] = cross3d(cam->v_o, res[0]);
	normalise3d(&res[1]);
	res[2] = new_vec3d(cam->v_o.x, cam->v_o.y, cam->v_o.z);
	resize3d(&res[2], cos(cam->fov / 2) * (((WDTH * DIVERGENCE) / 2) / cos(PI / \
	2 - (cam->fov / 2))));
	return (res);
}

/**
 * NOTE: the offset is from the middle ray to a projection screen
 * meaning that to represent all pixels x and y need to start at -(max / 2)
 * @param x [int] horizontal PIxelcoordinate
 * @param y [int] vertical PIxelcoordinate
 * @param cam [t_cam*] originating viewpoint
 * @param scr [double**] defining parameters of the projection screen
 * @brief return a ray representing a PIxel with the xy offset given
*/
t_vec3d	single_ray(int x, int y, t_cam *cam, t_vec3d scr[3])
{
	t_vec3d	res;
	t_vec3d	tmp;

	res = new_vec3d(cam->location.x, cam->location.y, cam->location.z);
	tmp = product3d(scr[0], (double)x / (WDTH / 2) * ((WDTH * DIVERGENCE) / 2));
	addto3d(&res, tmp);
	tmp = product3d(scr[1], (double)y / (HGHT / 2) * ((HGHT * DIVERGENCE) / 2));
	addto3d(&res, tmp);
	addto3d(&res, scr[2]);
	res = connect3d(cam->location, res);
	normalise3d(&res);
	return (res);
}
