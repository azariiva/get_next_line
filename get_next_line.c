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

int get_next_line(const int fd, char **line)
{
	static t_list *str_list;
	t_list *ptr;
	char *endlloc;
	char buff[BUFF_SIZE + 1];
	ssize_t len;

	if (!str_list)
	{
		if ((len = read(fd, buff, BUFF_SIZE)) <= 0)
		{
			return (len == 0 ? END : ERR);
		}
		buff[len] = 0;
		if ((endlloc = strchr(buff, '\n')))
		{
			*endlloc++ = 0;
			if (!(*line = ft_strdup(buff)) || (*endlloc && !(str_list = ft_lstnew(endlloc, sizeof(char) * (len - (endlloc - buff) + 1)))))
			{
				return (ERR);
			}
			return (OK);
		}
		if (!(str_list = ft_lstnew(buff, sizeof(char) * (len + 1))))
		{
			return (ERR);
		}
	}
	else if ((endlloc = ft_strchr(str_list->content, '\n')))
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
		return (OK);
	}
	ptr = str_list;

	while ((len = read(fd, buff, BUFF_SIZE)))
	{
		buff[len] = 0;
		if ((endlloc = strchr(buff, '\n')))
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
	{
		return (ERR);
	}
	return (OK);
}