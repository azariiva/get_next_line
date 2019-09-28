/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_linec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blinnea <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 15:30:40 by blinnea           #+#    #+#             */
/*   Updated: 2019/09/28 17:18:25 by blinnea          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_list	*find_fd(t_list *head, const int fd)
{
	while (head)
	{
		if ((int)head->content_size == fd)
			return (head);
		head = head->next;
	}
	return (NULL);
}

t_list	*add_fd(t_list **head, const int fd)
{
	t_list	*new;

	if (!(new = ft_lstnew("\0", (size_t)fd)))
		return (NULL);
	ft_lstadd(head, new);
	return (new);
}

void	crutch(void *s, size_t size)
{
	ft_memdel(&s);
	size = 0;
}

size_t	get_strlist(const int fd, char *content, t_list **strlist)
{
	int		len;
	char	buff[BUFF_SIZE + 1];
	t_list	*ptr;

	if (!(*strlist = ft_lstnew(content, ft_strlen(content))))
		return (0);
	ptr = *strlist;
	while ((len = read(fd, buff, BUFF_SIZE)))
	{
		buff[len] = 0;
		if (!(ptr->next = ft_lstnew(ft_strdup(buff), len)))
		{
			ft_lstdel(strlist, &crutch);
			return (0);
		}
		ptr = ptr->next;
		if (ft_strchr(buff, '\n'))
			break;
	}
	if (*strlist == ptr)
		return ((*strlist)->content_size);
	if (*strlist + 1 == ptr)
		return ((*strlist)->content_size + ptr->content_size);
	return ((*strlist - ptr - 2) * BUFF_SIZE + \
			(*strlist)->content_size + ptr->content_size);
}

t_list	*strlist_to_line(t_list *ptr, char *line, size_t l_line)
{
	char	*newline;

	while (ptr->next)
	{
		ft_strlcat(line, ptr->content, l_line);
		ptr = ptr->next;
	}
	if ((newline = ft_strchr(ptr->content, '\n')))
		*newline++ = 0;
	ft_strlcat(line, ptr->content, l_line);
	ptr->content = newline;
	return (ptr);
}

int		get_next_line(const int fd, char **line)
{
	static t_list	*flist;
	t_list			*curr;
	char			*newline;
	t_list			*strlist;
	size_t			l_strlist;

	if (!(curr = find_fd(flist, fd)) && !(curr = add_fd(&flist, fd)))
		return (ERR);
	if ((newline = ft_strchr(curr->content, '\n')))
	{
		*newline = 0;
		*line = ft_strdup(curr->content);
		curr->content = newline + 1;
		return (OK);
	}
	l_strlist = get_strlist(fd, curr->content, &strlist);
	free(curr->content);
	if (!(*line = ft_strnew(l_strlist)))
		return (ERR);
	curr->content = ft_strdup(strlist_to_line(strlist, *line, \
				l_strlist)->content);
	ft_lstdel(&strlist, &crutch);
	if (!curr->content)
		return (END);
	return (OK);
}
