/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaunevik <vaunevik@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 11:16:00 by vaunevik          #+#    #+#             */
/*   Updated: 2024/03/04 08:57:42 by vaunevik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line_bonus.h"

char	*get_next_line(int fd)
{
	char			*line;
	static t_list	*head[1024];
	t_list			*list;

	line = NULL;
	if (fd < 0 || fd > 1024)
		return (NULL);
	list = head[fd];
	if (BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		head[fd] = NULL;
		return (free_list(list), NULL);
	}
	create_list(&list, fd);
	if (!list)
		return (NULL);
	list_to_line(list, &line);
	if (check_line(&line, list))
	{
		head[fd] = NULL;
		return (NULL);
	}
	clean_list(&list);
	head[fd] = list;
	return (line);
}

t_list	*read_to_node(int fd, int *read_count)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	*read_count = read(fd, node->content, BUFFER_SIZE);
	if (*read_count < 0)
	{
		free(node);
		return (NULL);
	}
	node->content[*read_count] = '\0';
	node->next = NULL;
	return (node);
}

void	create_list(t_list **list, int fd)
{
	t_list	*node;
	t_list	*head;
	int		read_count;

	read_count = 1;
	if (!*list)
		*list = read_to_node(fd, &read_count);
	if (*list == NULL)
		return ;
	head = *list;
	while (read_count > 0 && !find_newline(*list))
	{
		node = read_to_node(fd, &read_count);
		if (!node)
		{
			free_list(head);
			*list = NULL;
			return ;
		}
		(*list)->next = node;
		*list = (*list)->next;
	}
	*list = head;
}

void	list_to_line(t_list *list, char **line)
{
	int	i;
	int	j;

	i = 0;
	if (!list)
		return ;
	create_line(&list, line, 0);
	if (!*line)
		return ;
	while (list && *line)
	{
		j = 0;
		while (list->content[j] && list->content[j] != '\n')
		{
			(*line)[i] = list->content[j];
			i++;
			j++;
		}
		if (list->content[j] == '\n')
			(*line)[i++] = list->content[j];
		list = list->next;
	}
}

void	clean_list(t_list **list)
{
	t_list	*pass_over;
	t_list	*last_node;

	last_node = *list;
	if (!last_node)
		return ;
	while (last_node && last_node->next)
		last_node = last_node->next;
	pass_over = malloc(sizeof(t_list));
	if (!pass_over)
	{
		free_list(*list);
		return ;
	}
	pass_over->next = NULL;
	pass_over_node(last_node, &pass_over);
	free_list(*list);
	*list = pass_over;
}
