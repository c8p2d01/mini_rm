/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdahlhof <cdahlhof@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 17:43:51 by cdahlhof          #+#    #+#             */
/*   Updated: 2022/06/15 12:21:04 by cdahlhof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minirm.h"

/**
 * Error Exit, goes through mrt and frees all inputs
 * @param mrt [t_mrt *] mrt struct
 * @param good [int] first position filled
 * @param count [int] max number of objects
 * @return [void] exits at end of function with (1)
*/
void	rt_er_exit(t_mrt *mrt, int good, int count)
{
	if (mrt->al)
		free(mrt->al);
	if (mrt->cam)
		free(mrt->cam);
	if (mrt->l)
		free(mrt->l);
	while (good < count)
	{
		if (mrt->obj[good])
			free(mrt->obj[good]);
		good ++;
	}
	if (mrt->obj)
		free(mrt->obj);
	perror("Error\n Invalid input\n");
	exit(1);
}

/**
 * Exit, goes through mrt and frees all inputs
 * @param mrt [t_mrt *] mrt struct
 * @return [int] exits at end of function with (0)
*/
int	rt_exit(t_mrt *mrt)
{
	int	x;

	if (mrt->al)
		free(mrt->al);
	if (mrt->cam)
		free(mrt->cam);
	x = 0;
	while (mrt->l[x])
		free(mrt->l[x ++]);
	if (mrt->l)
		free(mrt->l);
	x = 0;
	while (mrt->obj[x])
		free(mrt->obj[x ++]);
	if (mrt->obj)
		free(mrt->obj);
	// mlx_ima(mrt->mlx, mrt->img.img);
	mlx_close_window(mrt->mlx);
	// mlx_destroy_display(mrt->mlx);
	if (mrt->mlx)
		free(mrt->mlx);
	exit(0);
	return (0);
}
