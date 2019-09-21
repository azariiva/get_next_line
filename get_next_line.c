/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_linec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blinnea <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 15:30:40 by blinnea           #+#    #+#             */
/*   Updated: 2019/09/21 17:23:37 by blinnea          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

t_list	*find_file(t_list *head, int fd)
{
	while (head)
	{
		if ((int)head->content_size == fd)
			return (head);
		head = head->next;
	}
	return (NULL);
}

char	*ft_strextend(char **s1, const char *s2)
{
	char	*res;

	if (!(res = ft_strjoin(*s1, s2)))
		return (NULL);
	ft_strdel(s1);
	return (res);
}

int		get_next_line(const int fd, char **line)
{
	char			buff[BUFF_SIZE + 1];
	static t_list	*flist;
	t_list			*curr;
	int				len;
	char			*crloc;

	crloc = NULL;
	if (fd < 0 || !line || read(fd, buff, 0) < 0 || !(*line = ft_strnew(1)))
		return (ERR);
	if (!(curr = find_file(flist, fd)))
	{
		if (!(curr = ft_lstnew("\0", (size_t)fd)))
			return (ERR);
		ft_lstadd(&flist, curr);
	}
	printf("%s\n", curr->content);
	*line = ft_strextend(line, curr->content);
	free(curr->content);
	while ((len = read(fd, buff, BUFF_SIZE)))
	{
		buff[len] = 0;
		if ((crloc = ft_strchr(buff, '\n')))
			break;
		if (!(*line = ft_strextend(line, buff)))
			return (ERR);
	}
	if (len < BUFF_SIZE && !ft_strlen(*line))
		return (END);
	if (crloc)
	{
		*crloc++ = 0;
		*line = ft_strextend(line, buff);
	}
	curr->content = ft_strdup(crloc);
	return (OK);
}
