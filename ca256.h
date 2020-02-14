/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ca256.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 16:09:21 by fhenrion          #+#    #+#             */
/*   Updated: 2020/02/15 00:07:15 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CA256_H
# define CA256_H

# include <stddef.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <stdio.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4194304
# endif

# define BLOCK_SIZE BUFFER_SIZE / 8

# define B(x) (1ULL << (x))

typedef unsigned long long	ull;

typedef unsigned char	rule;

typedef struct	s_file
{
	int			fd_in;
	int			fd_out;
	size_t		size;
	char		*output_name;
}				t_file;

#endif
