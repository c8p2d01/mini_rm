/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_input.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdahlhof <cdahlhof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 17:44:28 by cdahlhof          #+#    #+#             */
/*   Updated: 2025/10/25 20:05:17 by cdahlhof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minirm.h"

void	init_mrt(t_mrt *mrt, int *count)
{
	mrt->al = ft_calloc(sizeof(t_al), 1);
	mrt->l = ft_calloc(sizeof(t_light *), count[LIGHT] + 1);
	mrt->cam = ft_calloc(sizeof(t_cam), 1);
	mrt->obj = ft_calloc(sizeof(t_obj *), count[OBJECTS] + 1);
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
			flag = init_light(mrt->l, tmp, --count[LIGHT]);
		else if (((char *)lst->content)[0] == 's')
			flag = init_sph(mrt->obj, tmp, --count[OBJECTS]);
		else if (((char *)lst->content)[0] == 'p')
			flag = init_pl(mrt->obj, tmp, --count[OBJECTS]);
		else if (((char *)lst->content)[0] == 'c')
			flag = init_cyl(mrt->obj, tmp, --count[OBJECTS]);
		free_2dstr(tmp);
		if (flag)
			return ((printf("[%d]{%s}\n", count[OBJECTS], (char *)lst->content)) * 0 + count[OBJECTS] + 1);
		lst = lst->next;
	}
	return (0);
}

bool	is_line_a_comment(char *line)
{
	bool	ret;
	int		i;

	ret = false;
	i = 0;
	while (line && i[line])
	{
		if (i[line] == 32 || (9 <= i[line] && i[line] <= 13))
		{
			i++;
			continue;
		}
		if (i[line] == '#')
		{
			ret = true;
		}
		break;
	}
	return (ret);
}

t_list	*import_data(char *file, t_list *lst)
{
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
	while (true)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		
		if (ft_strlen(line) > 0 && !is_line_a_comment(line) && \
				(size_t)is_whspace(line, 1) < ft_strlen(line))
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
		rt_er_exit(mrt, good - 1, count[OBJECTS]);
	return (good);
}
