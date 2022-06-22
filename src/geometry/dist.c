/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dist.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdahlhof <cdahlhof@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 18:44:20 by cdahlhof          #+#    #+#             */
/*   Updated: 2022/06/16 17:22:36 by cdahlhof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minirm.h"

double	s_sphere(t_obj *sph, t_vec *point)
{
	double	dist;

	dist = veclen(connect(*point, sph->cor));
	if (dist > sph->rad)
		return(dist - sph->rad);
	return (0);
}

/*first case: the point is in between the caps relative to the cylinder -> orth - rad
/*2nd   case: the point is above or underneath the caps but (would be)within the body -> planedist of cap
/*3rd   case: the point is outside bot boy and caps -> distance to cor+(hght/2)+(unit(orth)*rad)
*/
double	s_cylinder(t_obj *cyl, t_vec *point)
{
	double	dist;
	double	relative_height;
	t_vec	orth_con;

	relative_height = cos(angle(connect(*point, cyl->cor), cyl->v_o)) * veclen(connect(*point, cyl->cor));
	orth_con = connect(v_sum(cyl->cor, v_product(cyl->v_o, relative_height)), *point);
	if (relative_height < cyl->hght / 2 && relative_height > -cyl->hght / 2)
	{
		dist = veclen(orth_con);
		if (dist > cyl->rad)
			return(dist - cyl->rad);
		return (0);
	}
	else
	{
		if (relative_height < -cyl->hght / 2)
		{
			if (veclen(orth_con) < cyl->rad)
				return(cos(angle(connect(*point, v_sum(cyl->cor, v_product(cyl->v_o, -cyl->hght / 2))), cyl->v_o)) * veclen(connect(*point, cyl->cor)));
			return(cos(angle(connect(*point, v_sum(v_product(v_unit(orth_con), cyl->rad), v_sum(cyl->cor, v_product(cyl->v_o, cyl->hght / 2)))), cyl->v_o)) * veclen(connect(*point, cyl->cor)));
		}
		else
		{
			if (veclen(orth_con) < cyl->rad)
				return(cos(angle(connect(*point, v_sum(cyl->cor, v_product(cyl->v_o, cyl->hght / 2))), cyl->v_o)) * veclen(connect(*point, cyl->cor)));
			return(cos(angle(connect(*point, v_sum(v_product(v_unit(orth_con), cyl->rad), v_sum(cyl->cor, v_product(cyl->v_o, cyl->hght / 2)))), cyl->v_o)) * veclen(connect(*point, cyl->cor)));
		}
	}
}

double	plane(t_obj *pla, t_vec *point)
{
	t_vec con;

	con = connect(*point, pla->cor);
	return(cos(angle(v_unit(con), pla->v_o)) * veclen(con));
}

double	min_dst(t_obj **objs, t_vec point, t_obj **hit)
{
	double	d = RENDER_DISTANCE * 2;
	double	res = RENDER_DISTANCE;

	for(int i = 0; i[objs]; i++)
	{
		if (i[objs]->id == 'S')
			d = ((s_sphere(i[objs], &point)));
		if (i[objs]->id == 'P')
			d = ((s_cylinder(i[objs], &point)));
		if (i[objs]->id == 'Z')
			d = ((plane(i[objs], &point)));
		if (d < res)
		{
			*hit = objs[i];
			res = d;
		}
	}
	return (res);
}