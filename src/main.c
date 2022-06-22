/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdahlhof <cdahlhof@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 17:18:34 by cdahlhof          #+#    #+#             */
/*   Updated: 2022/06/16 14:44:47 by cdahlhof         ###   ########.fr       */
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
	if (mlx_is_key_down(mrt->mlx, MLX_KEY_UP))
		reorient(mrt, 'u');
	if (mlx_is_key_down(mrt->mlx, MLX_KEY_DOWN))
		reorient(mrt, 'd');
	if (mlx_is_key_down(mrt->mlx, MLX_KEY_LEFT))
		reorient(mrt, 'l');
	if (mlx_is_key_down(mrt->mlx, MLX_KEY_RIGHT))
		reorient(mrt, 'r');
}

void	render(t_mrt *mrt)
{
	t_vec	*scr = screen(mrt->cam);
	for(int y = 0; y < HGHT; y++)
	{
		for(int x = 0; x < WDTH; x++)
		{
			ray(mrt, x - (WDTH / 2), y - (HGHT / 2), scr, false);
		}
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
	mrt.mlx = mlx_init(WDTH, HGHT, "March goes in may?", true);
	mrt.img = mlx_new_image(mrt.mlx, WDTH, HGHT);

	render(&mrt);
	mlx_loop_hook(mrt.mlx, keyhook, &mrt);
	mlx_loop(mrt.mlx);
	return(0);
}