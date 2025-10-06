/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdahlhof <cdahlhof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 19:17:48 by cdahlhof          #+#    #+#             */
/*   Updated: 2025/10/06 21:51:08 by cdahlhof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirm.h"

void	reorient(t_mrt *mrt, char dir)
{
	//if (dir == 'u')
	//	mrt->cam->v_o = v_unit(tilt(mrt->cam->v_o, true));
	//else if (dir == 'd')
	//	mrt->cam->v_o = v_unit(tilt(mrt->cam->v_o, false));
	//else if (dir == 'l')
	//	mrt->cam->v_o = v_unit(rotate_Z(mrt->cam->v_o, 10));
	//else if (dir == 'r')
	//	mrt->cam->v_o = v_unit(rotate_Z(mrt->cam->v_o, -10));
	//render(mrt);
}

void	retrace(t_mrt *mrt)
{
	int	x,y;

	mlx_get_mouse_pos(mrt->mlx, &x, &y);
	t_vec3d	*scr = screen(mrt->cam);
	//x = 42;
	//y = 42;
	x = x - (WDTH / 2);
	y = y - (HGHT / 2);
	printf("\e[1;1H\e[2Jx: %i	y: %i \n", x, y );
	ray(mrt, x, y , scr, true);
	sleep(1);
}

int	create_rgbt(unsigned char t, unsigned char r, unsigned char g, unsigned char b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

double	march(t_mrt *mrt, t_ray *ray, bool print)
{
	double	step_dst;

	step_dst = min_dst(mrt->obj, ray);
	if ((0 <= step_dst && step_dst < 0.01) || ray->depth > 100)
		return (step_dst);
	if (step_dst > RENDER_DISTANCE - 42)
	{
		ray->hit = NULL;
		return (step_dst * 2);
	}
	normalise3d(&ray->direction);
	ray->origin = sum3d(ray->origin, product3d(&ray->direction, step_dst));
	ray->depth += 1;
	if (step_dst < ray->lowest_step)
		ray->lowest_step = step_dst;
	if (print)
		printf("step %i: lowest so far %lf\tcurrent %lf\n", ray->depth, ray->lowest_step, step_dst);
	//ray->dst += step_dst;
	return(step_dst + march(mrt, ray, print));
}

int		color(t_mrt *mrt, t_ray *ray, bool print, int divisor)
{
	t_obj	*hit;

	hit = ray->hit;
	if (hit == NULL)
	{
		if (print)
			printf("no hit %lf\n", ray->lowest_step);
		if (ray->lowest_step < GLOW)
			return (create_rgbt(240, 240, 240, 255));
		return (create_rgbt(0, 0, 0, 255));
	}
	if (hit->id == 'S')
	{
		double factor = (ray->depth / 25.f);
		//if (print)
		//	printf("hit sphere at depth %i , div %i color %i %i %i\n", ray->depth, divisor, hit->r * factor, hit->g * factor, hit->b * factor);
		return (create_rgbt(hit->r * factor, hit->g * factor, hit->b * factor, 255));
	}
	return (create_rgbt(0, 255, 0, 255));
}

void	ray(t_mrt *mrt, int x, int y, t_vec3d *scr, bool print)
{
	t_ray	ray;
	int		depth;
	static int divisor = 1;
	
	ray.origin = mrt->cam->location;
	ray.direction = single_ray(x, y, mrt->cam, scr);
	ray.hit = NULL;
	ray.depth = 0;
	ray.lowest_step = RENDER_DISTANCE;
	ray.dst = 0;
	ray.dst = march(mrt, &ray, print);
	if (ray.dst > RENDER_DISTANCE)
		ray.hit = NULL;
	mlx_put_pixel(mrt->img, x + (WDTH / 2), y + (HGHT / 2), color(mrt, &ray, print, divisor));
	
	if (print)
	{
		if (ray.hit){
			printf("object kind hit: %c\n", ray.hit->id);
			printf("object position: %lf %lf %lf\n", ray.hit->cor.x, ray.hit->cor.y, ray.hit->cor.z);
			printf("0	%p\n", mrt->obj[0]);
			printf("tmp	%p\n", &mrt->tmp);
			printf("hit	%p\n", ray.hit);
		}
		printf("distance to object: %lf\n", ray.dst);
	}
	if (ray.depth > divisor && ray.hit)
		divisor = ray.depth;
}
