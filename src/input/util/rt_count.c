/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_count.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdahlhof <cdahlhof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 17:44:22 by cdahlhof          #+#    #+#             */
/*   Updated: 2025/10/07 12:46:12 by cdahlhof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minirm.h"

/**
 * @param count [int *[3]] [A][C][L][obj]
 * @param tmp [char *] always NULL;
*/
int	count_input(t_list *lst, int *count, char *tmp)
{
	while (lst)
	{
		tmp = trm_whtsp((char *)lst->content, 2);
		if (tmp[0] == 'A' && ft_white(tmp[1]))
			count[AMBIENT]++;
		else if (tmp[0] == 'C' && ft_white(tmp[1]))
			count[CAMERA]++;
		else if (tmp[0] == 'L' && ft_white(tmp[1]))
			count[LIGHT]++;
		else if (!ft_strncmp(tmp, "sp", 2) && ft_white(tmp[2]))
			count[OBJECTS]++;
		else if (!ft_strncmp(tmp, "pl", 2) && ft_white(tmp[2]))
			count[OBJECTS]++;
		else if (!ft_strncmp(tmp, "cy", 2) && ft_white(tmp[2]))
			count[OBJECTS]++;
		else
		{
			printf("Error encountered in line >%s<\n", (char *)lst->content);
			free(tmp);
			return (printf("Error\n Line invalid\n"));
		}
		free(lst->content);
		lst->content = tmp;
		lst = lst->next;
	}
	return (0);
}

/**
 * checks if there are only one Ambient Light, one Camera
 * @param count [int[6]] counter
 * @return [int] 0 if good, 1 if error
*/
int	check_count(int *count)
{
	if (count[AMBIENT] != 1)
		perror("Error\nIncorrect number of Ambient Lights\n");
	else if (count[CAMERA] != 1)
		perror("Error\nIncorrect number of Cameras\n");
	else
		return (0);
	return (1);
}
