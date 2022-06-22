/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_input.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdahlhof <cdahlhof@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 17:44:28 by cdahlhof          #+#    #+#             */
/*   Updated: 2022/06/15 16:34:08 by cdahlhof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minirm.h"

void	init_mrt(t_mrt *mrt, int *count)
{
	mrt->al = ft_calloc(sizeof(t_al), 1);
	mrt->l = ft_calloc(sizeof(t_light *), count[2] + 1);
	mrt->cam = ft_calloc(sizeof(t_cam), 1);
	mrt->obj = ft_calloc(sizeof(t_obj *), count[3] + 1);
	return ;
}

int	parse_input(t_mrt *mrt, t_list *lst, int *count, int flag)
{
	char	**tmp;

	init_mrt(mrt, count);
	while (lst)
	{
		tmp = split_wh(lst->content);
		if (((char *)lst->content)[0] == 'A')
			flag = init_al(mrt->al, tmp);
		else if (((char *)lst->content)[0] == 'C')
			flag = init_cam(mrt->cam, tmp);
		else if (((char *)lst->content)[0] == 'L')
			flag = init_light(mrt->l, tmp, --count[2]);
		else if (((char *)lst->content)[0] == 's')
			flag = init_sph(mrt->obj, tmp, --count[3]);
		else if (((char *)lst->content)[0] == 'p')
			flag = init_pl(mrt->obj, tmp, --count[3]);
		else if (((char *)lst->content)[0] == 'c')
			flag = init_cyl(mrt->obj, tmp, --count[3]);
		free_2dstr(tmp);
		if (flag)
			return ((printf("[%d]{%s}\n", count[3], (char *)lst->content)) * 0 + count[3] + 1);
		lst = lst->next;
	}
	return (0);
}

t_list	*import_data(char *file, t_list *lst)
{
	int		i;
	int		fd;
	char	*line;

	fd = open(file, O_RDONLY);
	if (fd == -1 || ft_strnstr(file, ".rt", ft_strlen(file)) != \
	&file[ft_strlen(file) - 3])
	{
		perror("Error\nNot a valid file");
		if (fd != -1)
			close(fd);
		exit(1);
	}
	i = 1;
	while (i > 0)
	{
		i = get_next_line(fd, &line);
		if (ft_strlen(line) > 0 && (size_t)is_whspace(line, 1) < \
		ft_strlen(line))
			ft_lstadd_back(&lst, ft_lstnew((void *)line));
		else
			free(line);
	}
	close(fd);
	return (lst);
}

int	input(t_mrt *mrt, char *file)
{
	int		count[4];
	int		good;
	t_list	*lst;

	lst = NULL;
	lst = import_data(file, lst);
	if (!lst)
	{
		perror("Error\nEmpty file\n");
		exit(1);
	}
	good = 0;
	ft_bzero(count, sizeof(int) * 4);
	if (!count_input(lst, count, NULL) && !check_count(count))
	{
		good = parse_input(mrt, lst, count, 0);
	}
	else
		good = -1;
	ft_lstclear(&lst, free);
	if (good > 0)
		rt_er_exit(mrt, good - 1, count[3]);
	return (good);
}
