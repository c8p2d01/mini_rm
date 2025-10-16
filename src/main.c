/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdahlhof <cdahlhof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 17:18:34 by cdahlhof          #+#    #+#             */
/*   Updated: 2025/10/16 16:38:11 by cdahlhof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirm.h"

void	keyhook(void *param)
{
	t_mrt *mrt;
	
	mrt = (t_mrt*)param;
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

void	*render_image(void *stuff)
{
	t_mrt	*mrt;
	t_vec3d	*scr;
	int		image;
	int		color;

	mrt = ((t_thread *)stuff)->mrt;
	image = ((t_thread *)stuff)->id;
	scr = screen(mrt->cam);
	for(int y = 0; y < HGHT / THREADS; y++)
	{
		for(int x = 0; x < WDTH; x++)
		{
			color = ray(mrt, x - (WDTH / 2), y - ((HGHT) / 2) + (HGHT / THREADS) * image, scr, false);
			mlx_put_pixel(mrt->img[image], x, y, color);
		}
	}
	pthread_exit(0);
	return (NULL);
}

void	render(t_mrt *mrt)
{
	int	i;

	i = 0;
	while (i < THREADS)
	{
		mrt->threads[i].id = i;
		mrt->threads[i].mrt = mrt;
		pthread_create(&mrt->threads[i].thread, NULL, render_image, &mrt->threads[i]);
		i++;
	}
	//i = 0;
	//while (i < THREADS)
	//{
	//	pthread_join(&mrt->threads[i].thread, NULL);
	//	i++;
	//}
}

//still need to change input for multi light
int main(int argc, char **argv)
{
	t_mrt	mrt;
	int		i;

	if (argc != 2)
	{
		printf("Error\nIncorrect number of Args\n");
		return (1);
	}
	if (input(&mrt, argv[1]))
		return (printf("Error\n INPUT\n"));
	mrt.mlx = mlx_init(WDTH, HGHT, "March goes in may?", true);
	i = -1;
	while (++i < THREADS)
	{
		mrt.img[i] = mlx_new_image(mrt.mlx, WDTH, HGHT / THREADS);
		mlx_image_to_window(mrt.mlx, mrt.img[i], 0, (HGHT / THREADS) * i);
	}

	render(&mrt);
	printf("DEBUG\n");
	mlx_loop_hook(mrt.mlx, keyhook, &mrt);
	mlx_loop(mrt.mlx);
	return(0);
}