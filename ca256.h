/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ca256.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 16:09:21 by fhenrion          #+#    #+#             */
/*   Updated: 2020/03/01 18:59:16 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CA256_H
# define CA256_H

# include <stddef.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>

typedef unsigned long long	ull;
typedef unsigned char		rule;

# define ERROR			-1
# define BUFFER_SIZE	1048576
# define BLOCK_SIZE		BUFFER_SIZE / 8
# define STATE_NB		16
# define HASH_SIZE		32
# define ULL_SIZE		sizeof(ull) * 8

# define B(x) (1ULL << (x))

typedef struct	s_file
{
	int			read;
	int			write;
	size_t		size;
	char		*output_name;
}				t_file;

char			*parse_option(char *option, char *input_file);

#endif
