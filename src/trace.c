/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdahlhof <cdahlhof@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 19:17:48 by cdahlhof          #+#    #+#             */
/*   Updated: 2022/06/16 16:50:28 by cdahlhof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirm.h"

void	reorient(t_mrt *mrt, char dir)
{
	if (dir == 'u')
		mrt->cam->v_o = v_unit(tilt(mrt->cam->v_o, true));
	else if (dir == 'd')
		mrt->cam->v_o = v_unit(tilt(mrt->cam->v_o, false));
	else if (dir == 'l')
		mrt->cam->v_o = v_unit(rotate_Z(mrt->cam->v_o, 10));
	else if (dir == 'r')
		mrt->cam->v_o = v_unit(rotate_Z(mrt->cam->v_o, -10));
	render(mrt);
}

void	retrace(t_mrt *mrt)
{
	int	x,y;

	mlx_get_mouse_pos(mrt->mlx, &x, &y);
	t_vec	*scr = screen(mrt->cam);
	printf("\e[1;1H\e[2Jx: %i	y: %i \n", x - (WDTH / 2), y - (HGHT / 2));
	ray(mrt, x - (WDTH / 2), y - (HGHT / 2), scr, true);
	sleep(1);
}

int	create_rgbt(unsigned char t, unsigned char r, unsigned char g, unsigned char b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

double	march(t_mrt *mrt, t_vec *ray, t_vec point, t_obj **hit, int *depth)
{
	double pd = (min_dst(mrt->obj, point, hit));
	if (pd >= RENDER_DISTANCE || (pd >= 0 && pd < 0.01) || *depth > 100)
		return (pd);
	t_vec new_point = v_sum(point, v_product(v_unit(*ray), pd));
	// printf("m, single %lf\n", pd);
	*depth += 1;
	pd += march(mrt, ray, new_point, hit, depth);
	// printf("m, added %lf\n", pd);
	return(pd);
}

int		color(t_mrt *mrt, t_obj *hit, t_vec *ray, double dst, bool print)
{
	t_vec	b = init_vec(hit->r, hit->g, hit->b);
	t_vec	to_l;
	t_obj	*cast = &mrt->tmp;
	int		cdepth = 0;
	t_vec	norm;
	double	diffuse;
	switch (hit->id)
	{
		case 'S':norm = connect(hit->cor, v_sum(mrt->cam->cor, v_product(*ray ,dst)));break;
		case 'P':norm = v_invert(hit->v_o);break;
		case 'Z':norm = connect(v_sum(hit->cor, v_product(hit->v_o, cos(angle(connect(mrt->cam->cor, hit->cor), hit->v_o)) * veclen(connect(mrt->cam->cor, hit->cor)))), mrt->cam->cor);break;
		default: norm = init_vec(1, 0 ,0);break;
	}
	double	ld;
	float	f = 0;
	for (int i = 0; mrt->l[i]; i++)
	{
		to_l = v_unit(connect(v_sum(mrt->cam->cor, v_product(*ray ,dst)), (mrt->l[i])->cor));
		diffuse = 1 - (2 * angle(norm, to_l) / PI);
		ld = march(mrt, &to_l, v_product(*ray ,dst), &cast, &cdepth);
		if (cast != &mrt->tmp)
		{
			b = v_sum(b, v_product(v_product(init_vec(mrt->l[i]->r, mrt->l[i]->g, mrt->l[i]->b), mrt->l[i]->lr), diffuse));
			f++;
			if (print)
				printf("%i	total raw light r %lf g %lf b %lf\n",i, b.x, b.y, b.z);
		}
	}
	b  = v_product(b, (1/f));
	if (b.x < 0)
		b.x = 0;
	if (b.y < 0)
		b.y = 0;
	if (b.z < 0)
		b.z = 0;
	if (print)
		printf("light r %f g %f b %f\n", b.x, b.y, b.z);
	return(create_rgbt(b.x, b.y, b.z, 255));
}

void	ray(t_mrt *mrt, int x, int y, t_vec *scr, bool print)
{
	t_vec	ray = v_unit(single_ray(x, y, mrt->cam, scr));

	t_obj	*hit = &mrt->tmp;
	int		depth = 0;
	double	dst = march(mrt, &ray, mrt->cam->cor, &hit, &depth);
	if (((int)dst) < 20000)
		mlx_put_pixel(mrt->img, x + (WDTH / 2), y + (HGHT / 2), color(mrt, hit, &ray, dst, print));
	else
		mlx_put_pixel(mrt->img, x + (WDTH / 2), y + (HGHT / 2), create_rgbt(0, 0, 0, 255));
	
	if (print)
	{
		if (hit){
			printf("object kind hit: %c\n", hit->id);
			printf("0	%p\n", mrt->obj[0]);
			printf("1	%p\n", mrt->obj[1]);
			printf("tmp	%p\n", &mrt->tmp);
			printf("hit	%p\n", hit);
		}
		printf("distance to object: %lf\n", dst);
	}
}
