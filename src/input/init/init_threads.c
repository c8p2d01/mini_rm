/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdahlhof <cdahlhof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 17:44:19 by cdahlhof          #+#    #+#             */
/*   Updated: 2025/10/07 13:08:52 by cdahlhof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minirm.h"

void	init_mutexes(t_mrt *mrt)
{
	int	i;

	pthread_mutex_init(&mrt->image_lock, NULL);
	i = 0;
	while (i < THREADS)
	{
		mrt->threads[i].status = PAUSED;
		mrt->threads[i].number = i;
		mrt->threads[i].mrt = mrt;
		pthread_mutex_init(&mrt->threads[i].m_ray, NULL);
		pthread_mutex_init(&mrt->threads[i].m_status, NULL);
		i++;
	}
}

void	*routine(void *v_renthread)
{
	t_renthread	*t_thrd;

	t_thrd = (t_renthread*)v_renthread;
	while (1)
	{
		pthread_mutex_lock(&t_thrd->m_status);
		if (t_thrd->status == ERROR)
			break ;
		if (t_thrd->status == PAUSED)
		{
			pthread_mutex_unlock(&t_thrd->m_status);
			continue ;
		}
		pthread_mutex_unlock(&t_thrd->m_status);
		ray(t_thrd->mrt, t_thrd->ray);
		pthread_mutex_lock(&t_thrd->m_status);
		t_thrd->status = PAUSED;
		pthread_mutex_unlock(&t_thrd->m_status);
	}
}

void	init_threads(t_mrt *mrt)
{
	int	i;

	i = 0;
	while (i < THREADS)
	{
		pthread_create(&mrt->threads[i].id, NULL, routine, (void*)&mrt->threads[i]);
		i++;
	}
}

void	destroy_threads(t_mrt *mrt)
{
	int	i;

	for (i = 0; i < THREADS; i++)
		pthread_mutex_unlock(&mrt->threads[i].m_ray);
	i = 0;
	while (i < THREADS)
	{
		pthread_mutex_lock(&mrt->threads[i].m_status);
		mrt->threads[i].status = ERROR;
		pthread_mutex_unlock(&mrt->threads[i].m_status);
		pthread_join(mrt->threads[i].id, NULL);
		i++;
	}
}