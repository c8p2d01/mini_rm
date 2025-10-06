/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_sphere.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdahlhof <cdahlhof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 17:44:10 by cdahlhof          #+#    #+#             */
/*   Updated: 2025/10/06 19:50:56 by cdahlhof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minirm.h"

/**
 * @brief Return the the distance to the given sphere
 * if it hitting as a double else it return -1
 * @param sph_org Pointer of type double that save the coordinate
 * 				of the sphere point[3]
 * @param sph_rad Radius of the sphere as double
 * @param ray_or Ray Vector as Pointer of type double ray[3]
 * @param ray_dir
 * 
 * @return Distance to object on this ray vector
 */
double	hit_sphere(t_vec3d sph_org, double sph_rad, t_vec3d ray_or, t_vec3d ray_dir)
{
	t_vec3d	oc;
	t_vec3d	tmp;
	double	disc;

	oc = connect3d(sph_org, ray_or);
	tmp.x = dot_product3d(&ray_dir, NULL);
	tmp.y = 2.0 * dot_product3d(&oc, &ray_dir);
	tmp.z = dot_product3d(&oc, NULL) - sph_rad * sph_rad;
	disc = tmp.y * tmp.y - 4 * tmp.x * tmp.z;
	if (disc < 0)
		return (0);
	else
		return (-((tmp.y) + sqrt(disc)) / tmp.x);
}
