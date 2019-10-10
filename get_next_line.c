/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_linec.c                                   :+:      :+:    :+:   */
/*                                  x                  +:+ +:+         +:+     */
/*   By: blinnea <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 15:30:40 by blinnea           #+#    #+#             */
/*   Updated: 2019/09/28 17:18:25 by blinnea          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void ft_delone(void *str, size_t len)
{
	if (str)
	{
		free(str);
		len = 0;
	}
}

char *list_to_str(t_list **list)
{
	size_t len;
	t_list *ptr;
	char *str;

	ptr = *list;
	len = 0;
	while (ptr)
	{
		len += ptr->content_size;
		ptr = ptr->next;
	}
	if ((str = ft_strnew(len + 1)))
	{
		ptr = *list;
		while (ptr)
		{
			ft_strncat(str, ptr->content, ptr->content_size);
			ptr = ptr->next;
		}
	}
	ft_lstdel(list, &ft_delone);
	return (str);
}

int proc_rem(t_list *fd_list, char **line)
{
	t_list *str_list;
	t_list *ptr;
	char *endlloc;
	char buff[BUFF_SIZE + 1];
	ssize_t len;
	
	if (!(str_list = (t_list *) fd_list->content))
	{
		if ((len = read(fd_list->content_size, buff, BUFF_SIZE)) <= 0)
			return (len == 0 ? END : ERR);
		buff[len] = 0;
		if (!(str_list = ft_lstnew(buff, sizeof(char) * (len + 1))))
			return (ERR);
	}
	if ((endlloc = ft_strchr(str_list->content, '\n')))
	{
		*endlloc++ = 0;
		ptr = (*endlloc ? ft_lstnew(endlloc, sizeof(char) * (str_list->content_size - (endlloc - (char *)str_list->content))) : NULL);
		str_list->content_size = endlloc - (char *)str_list->content;
		if (!(*line = list_to_str(&str_list)))
		{
			ft_lstdelone(&ptr, ft_delone);
			return (ERR);
		}
		str_list = ptr;
		fd_list->content = str_list;
		return (OK);
	}
	ptr = str_list;

	while ((len = read(fd_list->content_size, buff, BUFF_SIZE)))
	{
		buff[len] = 0;
		if ((endlloc = ft_strchr(buff, '\n')))
		{
			*endlloc++ = 0;
			if (!(ptr->next = ft_lstnew(buff, sizeof(char) * (endlloc - buff + 1))))
			{
				ft_lstdel(&str_list, &ft_delone);
				return (ERR);
			}
			break;
		}
		if (!(ptr->next = ft_lstnew(buff, sizeof(char) * (len + 1))))
		{
			ft_lstdel(&str_list, &ft_delone);
			return (ERR);
		}
		ptr = ptr->next;
	}
	if (!(*line = list_to_str(&str_list)) || (endlloc && *endlloc && !(str_list = ft_lstnew(endlloc, sizeof(char) * (len - (endlloc - buff) + 1)))))
		return (ERR);
	fd_list->content = str_list;
	return (OK);
}

int get_next_line(const int fd, char **line)
{
	static t_list *fd_list;
	t_list *ptr;
	int res;

	ptr = fd_list;
	while (ptr)
	{
		if (ptr->content_size == (size_t) fd)
		{
			if ((res = proc_rem(ptr, line)) == ERR)
				ptr->content = NULL;
			return (res);
		}
		ptr = ptr->next;
	}
	if (!(ptr = ft_lstnew(NULL, 0)))
		return (ERR);
	ptr->content_size = fd;
	ft_lstadd(&fd_list, ptr);
	if ((res = proc_rem(fd_list, line)) == ERR)
		fd_list->content = NULL;
	return (res);
}