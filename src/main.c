/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdahlhof <cdahlhof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 17:18:34 by cdahlhof          #+#    #+#             */
/*   Updated: 2025/10/07 13:05:08 by cdahlhof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirm.h"

void	keyhook(void *param)
{
	t_mrt *mrt = (t_mrt*)param;

	if (mlx_is_key_down(mrt->mlx, MLX_KEY_D))
		retrace(mrt);
	if (mlx_is_key_down(mrt->mlx, MLX_KEY_ESCAPE))
		rt_exit(mrt);
	//if (mlx_is_key_down(mrt->mlx, MLX_KEY_UP))
	//	reorient(mrt, 'u');
	//if (mlx_is_key_down(mrt->mlx, MLX_KEY_DOWN))
	//	reorient(mrt, 'd');
	//if (mlx_is_key_down(mrt->mlx, MLX_KEY_LEFT))
	//	reorient(mrt, 'l');
	//if (mlx_is_key_down(mrt->mlx, MLX_KEY_RIGHT))
	//	reorient(mrt, 'r');
}

void	ray_starter(t_mrt *mrt, t_vec3d *scr, int x, int y, bool print)
{
	int	i;

	i = 0;
	while (true)
	{
		i %= THREADS;
		pthread_mutex_lock(&mrt->threads[i].m_status);
		if (mrt->threads[i].status == DONE)
		{
			mrt->threads[i].status = READY;
			pthread_mutex_unlock(&mrt->threads[i].m_status);
			continue ;
		}
		if (mrt->threads[i].status == BUSY)
		{
			pthread_mutex_unlock(&mrt->threads[i].m_status);
			continue ;
		}
		if (mrt->threads[i].status == PAUSED || mrt->threads[i].status == READY)
		{
			pthread_mutex_lock(&mrt->threads[i].m_ray);
			mrt->threads[i].status = BUSY;
			mrt->threads[i].ray.origin = mrt->cam->location;
			mrt->threads[i].ray.direction = single_ray(x - (WDTH / 2), y - (HGHT / 2), mrt->cam, scr);
			mrt->threads[i].ray.depth = 0;
			mrt->threads[i].ray.hit = NULL;
			mrt->threads[i].ray.lowest_step = RENDER_DISTANCE;
			mrt->threads[i].ray.dst = 0;
			mrt->threads[i].ray.x = x;
			mrt->threads[i].ray.y = y;
			mrt->threads[i].ray.print = print;
			mrt->threads[i].mrt = mrt;
			pthread_mutex_unlock(&mrt->threads[i].m_ray);
			pthread_mutex_unlock(&mrt->threads[i].m_status);
			break ;
		}
		i++;
	}
}

void	render(t_mrt *mrt)
{
	t_vec3d	*scr = screen(mrt->cam);
	for(int y = 0; y < HGHT; y++)
	{
		for(int x = 0; x < WDTH; x++)
		{
			ray_starter(mrt, scr, x, y, false);
		}
	}
	for(int i = 0; i < THREADS; i++)
	{
		pthread_mutex_lock(&mrt->threads[i].m_status);
		mrt->threads[i].status = PAUSED;
		pthread_mutex_unlock(&mrt->threads[i].m_status);
	}
	mlx_image_to_window(mrt->mlx, mrt->img, 0, 0);
}

//still need to change input for multi light
int main(int argc, char **argv)
{
	t_mrt	mrt;

	if (argc != 2)
	{
		printf("Error\nIncorrect number of Args\n");
		return (1);
	}
	if (input(&mrt, argv[1]))
		return (printf("Error\n INPUT\n"));
	init_mutexes(&mrt);
	sleep(1);
	init_threads(&mrt);
	mrt.mlx = mlx_init(WDTH, HGHT, "March goes in may?", true);
	mrt.img = mlx_new_image(mrt.mlx, WDTH, HGHT);

	render(&mrt);
	mlx_loop_hook(mrt.mlx, keyhook, &mrt);
	mlx_loop(mrt.mlx);
	return(0);
}