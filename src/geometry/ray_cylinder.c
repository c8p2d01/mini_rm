///* ************************************************************************** */
///*                                                                            */
///*                                                        :::      ::::::::   */
///*   ray_cylinder.c                                     :+:      :+:    :+:   */
///*                                                    +:+ +:+         +:+     */
///*   By: cdahlhof <cdahlhof@student.42.fr>          +#+  +:+       +#+        */
///*                                                +#+#+#+#+#+   +#+           */
///*   Created: 2022/05/10 17:44:04 by cdahlhof          #+#    #+#             */
///*   Updated: 2025/10/06 16:30:50 by cdahlhof         ###   ########.fr       */
///*                                                                            */
///* ************************************************************************** */

//#include "../../inc/minirm.h"

//#define EPSILON 0.00001

//int	cylinder_coefficient(t_obj cyl, double *x, t_vec3d *ray_or, t_vec3d *ray_dir)
//{
//	t_vec3d	v;
//	t_vec3d	u;
//	t_vec3d	normalized;
//	t_vec3d	tmp;

//	normalized = cyl.v_o;
//	normalise3d(&normalized);
//	v = resize3d(&normalized, dot_product3d(ray_dir, &normalized));
//	v = connect3d(v, *ray_dir);
//	tmp = connect3d(cyl.cor, *ray_or);
//	u = resize3d(&normalized, dot_product3d(&tmp, &normalized));
//	u = connect3d(u, connect3d(cyl.cor, *ray_or));
//	tmp.x = dot_product3d(&v, &v);
//	tmp.y = 2 * dot_product3d(&v, &u);
//	tmp.z = dot_product3d(&u, &u) - pow(cyl.rad, 2);
//	x[0] = (-tmp.y + sqrt(pow(tmp.y, 2) - 4 * tmp.x * tmp.z)) / (2 * tmp.x);
//	x[1] = (-tmp.y - sqrt(pow(tmp.y, 2) - 4 * tmp.x * tmp.z)) / (2 * tmp.x);
//	if ((x[0] != x[0] && x[1] != x[1]) || (x[0] < EPSILON && x[1] < EPSILON))
//	{
//		x[0] = INFINITY;
//		x[1] = INFINITY;
//		return (0);
//	}
//	return (1);
//}

//t_vec3d	calc_cy_normal(double *x, t_obj cyl, t_vec3d ray_or, t_vec3d ray_dir)
//{
//	double	d[2];

//	unit(&cyl.v_o);
//	if ((x[2] >= 0 && x[2] <= cyl.hght && x[0] > EPSILON) && (x[3] >= \
//		0 && x[3] <= cyl.hght && x[1] > EPSILON))
//	{
//		d[0] = (x[0] < x[1]) * x[2] + (x[0] >= x[1]) * x[3];
//		d[1] = (x[0] < x[1]) * x[0] + (x[0] >= x[1]) * x[1];
//	}
//	else if (x[2] >= 0 && x[2] <= cyl.hght && x[0] > EPSILON)
//	{
//		d[0] = x[2];
//		d[1] = x[0];
//	}
//	else
//	{
//		d[0] = x[3];
//		d[1] = x[1];
//	}
//	x[0] = d[1];
//	return (v_unit(connect(connect(ray_or, cyl.cor), connect(\
//	v_product(v_unit(cyl.v_o), d[0]), v_product(ray_dir, d[1])))));
//}

//t_vec3d	new_pos(t_vec3d *posi, t_vec3d *dir, double height, t_vec3d *norm)
//{
//	t_vec3d	pos;

//	*norm = *dir;
//	unit(norm);
//	pos.x = posi->x - (norm->x * (height / 2));
//	pos.y = posi->y - (norm->y * (height / 2));
//	pos.z = posi->z - (norm->z * (height / 2));
//	return (pos);
//}

//double	new_cylinder_intersect(t_obj cyl, t_vec3d *ray_or, t_vec3d *ray_dir)
//{
//	double	x[4];
//	t_vec3d	normalized;
//	t_vec3d	helper;

//	cyl.cor = new_pos(&cyl.cor, &cyl.v_o, cyl.hght, &normalized);
//	if (cylinder_coefficient(cyl, x, ray_or, ray_dir) == 0)
//		return (0);
//	helper = connect3d(connect3d(*ray_or, cyl.cor), resize3d(ray_dir, x[0]));
//	x[2] = dot_product3d(&normalized, &helper);
//	helper = connect3d(connect3d(*ray_or, cyl.cor), resize3d(ray_dir, x[1]));
//	x[3] = dot_product3d(&normalized, &helper);
//	if (!((x[2] >= 0 && x[2] <= cyl.hght && x[0] > EPSILON) || (x[3] >= 0 && \
//			x[3] <= cyl.hght && x[0] > EPSILON)))
//		return (0);
//	calc_cy_normal(x, cyl, *ray_or, *ray_dir);
//	return (x[0]);
//}

//double	hit_cylinder(t_obj cyl, t_vec3d ray_or, t_vec3d ray_dir)
//{
//	double	cylinder_inter;

//	cylinder_inter = new_cylinder_intersect(cyl, &ray_or, &ray_dir);
//	return (cylinder_inter);
//}
