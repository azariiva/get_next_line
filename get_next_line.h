/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blinnea <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 15:28:36 by blinnea           #+#    #+#             */
/*   Updated: 2019/09/21 17:23:38 by blinnea          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE

# define GET_NEXT_LINE

# include "libft.h"

# define BUFF_SIZE 100

enum
{
	ERR = -1,
	OK = 1,
	END = 0
};

int	get_next_line(const int fd, char **line);

#endif
