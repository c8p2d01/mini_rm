/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdahlhof <cdahlhof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 19:17:48 by cdahlhof          #+#    #+#             */
/*   Updated: 2025/10/24 02:17:08 by cdahlhof         ###   ########.fr       */
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
}

int	create_rgbt(int t, int r, int g, int b)
{
	if (r > 255)
		r = 255;
	if (g > 255)
		g = 255;
	if (b > 255)
		b = 255;
	if (r < 0)
		r = 0;
	if (g < 0)
		g = 0;
	if (b < 0)
		b = 0;
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
	ray->origin = sum3d(ray->origin, product3d(ray->direction, step_dst));
	ray->depth += 1;
	if (step_dst < ray->lowest_step)
		ray->lowest_step = step_dst;
	//if (print)
	//	printf("step %i: lowest so far %lf\tcurrent %lf\n", ray->depth, ray->lowest_step, step_dst);
	ray->dst += step_dst;
	return(step_dst + march(mrt, ray, print));
}

void	add_light(t_ray *ray, t_ray *light, t_light *l, bool print)
{
	double	impact;
	t_vec3d	normal;

	if (ray->hit->id == 'S')
		normal = connect3d(ray->hit->cor, ray->hit_point);
	if (ray->hit->id == 'P')
		normal = ray->hit->v_o;
	impact = (PI / 360) * (90 - (angle3d(normal, light->direction) * (180 / PI))); // TODO : cut this down using maths xD
	if (impact < 0)
		impact = 0;
	//if (impact > 1)
	//	impact = 1;
	ray->r += (ray->r / 256 + impact * l->r / 256) * 127;
	ray->g += (ray->g / 256 + impact * l->g / 256) * 127;
	ray->b += (ray->b / 256 + impact * l->b / 256) * 127;
	if (print)
	{
		printf("added light %i\t%i\t%i\n", impact * l->r, impact * l->g, impact * l->b);
		printf("impact angle %lf\n", impact);
		printf("light ray %lf\t%lf\t%lf\n", light->direction.x, light->direction.y, light->direction.z);
		printf("norma ray %lf\t%lf\t%lf\nnormal length? %lf\n", normal.x, normal.y, normal.z, veclen3d(normal));
	}
}

int		color(t_mrt *mrt, t_ray *ray, bool print, int divisor)
{
	t_obj	*hit;
	t_ray	light;
	int		i;

	hit = ray->hit;
	if (hit == NULL)
	{
		if (print)
			printf("no hit %lf\n", ray->lowest_step);
		if (ray->lowest_step < GLOW/100)
			return (create_rgbt(240, 240, 240, 255));
		return (create_rgbt(0, 0, 0, 255));
	}
	ray->hit_point = sum3d(mrt->cam->location, product3d(ray->direction, ray->dst));
	if (print)
		printf("cam origin %lf %lf %lf\nray origin %lf %lf %lf\n\n", \
			mrt->cam->location.x, mrt->cam->location.y, mrt->cam->location.z, \
			ray->origin.x, ray->origin.y, ray->origin.z);
	i = -1;
	while (mrt->l[++i])
	{
		if (print)
			printf("point light %i\t%i\t%i\n", ray->r, ray->g, ray->b);
		light.origin = mrt->l[i]->cor;
		light.depth = 0;
		light.dst = 0;
		light.lowest_step = RENDER_DISTANCE;
		light.direction = product3d(connect3d(light.origin, ray->hit_point), -1);
		if (print)
			printf("connecting light at %lf %lf %lf to %lf %lf %lf with %lf %lf %lf\n", mrt->l[i]->cor.x, mrt->l[i]->cor.y, mrt->l[i]->cor.z, ray->hit_point.x, ray->hit_point.y, ray->hit_point.z, light.direction.x, light.direction.y, light.direction.z);
		normalise3d(&light.direction);
		light.dst = march(mrt, &light, false);
		if (light.hit != ray->hit)
			continue ;
		add_light(ray, &light, mrt->l[i], print);
		if (print)
			printf("point light %i\t%i\t%i\n", ray->r, ray->g, ray->b);
	}
	return (create_rgbt(ray->r, ray->g, ray->b, 255));
}

int	ray(t_mrt *mrt, int x, int y, t_vec3d *scr, bool print)
{
	t_ray	ray;
	int		depth;
	

	ray.origin = mrt->cam->location;
	ray.direction = single_ray(x, y, mrt->cam, scr);
	ray.hit = NULL;
	ray.depth = 0;
	ray.lowest_step = RENDER_DISTANCE;
	ray.dst = 0;
	ray.dst = march(mrt, &ray, print);
	if (ray.dst > RENDER_DISTANCE)
		ray.hit = NULL;
	if (print)
	{
		printf("ray directions x:%.4lf\ty:%.4lf\tz:%.4lf\n", ray.direction.x, ray.direction.y, ray.direction.z);
		if (ray.hit){
			printf("object kind hit: %c\n", ray.hit->id);
			printf("object position: %lf %lf %lf\n", ray.hit->cor.x, ray.hit->cor.y, ray.hit->cor.z);
			printf("0	%p\n", mrt->obj[0]);
			printf("tmp	%p\n", &mrt->tmp);
			printf("hit	%p\n", ray.hit);
		}
		printf("distance to object: %lf\n", ray.dst);
	}
	if (ray.hit)
	{
		ray.r = ray.hit->r;
		ray.g = ray.hit->g;
		ray.b = ray.hit->b;
	}
	return (color(mrt, &ray, print, 12));
}
