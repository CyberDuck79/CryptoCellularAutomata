/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ca64.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 16:09:21 by fhenrion          #+#    #+#             */
/*   Updated: 2021/03/21 15:40:27 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CA64_H
# define CA64_H

# include <stdint.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>

# define ERROR			-1
# define BUFFER_SIZE	1048576
# define BLOCK_SIZE		BUFFER_SIZE / 8

# define B(x) (1ULL << (x))

typedef struct	file_s {
	int			read;
	int			write;
	size_t		size;
}	file_t;

void		write_state(const uint64_t state);
int			shift_rule(const uint64_t state, int rule_i);
uint64_t	generate(const uint64_t state, const int rule_i);
uint64_t	generate_hash(const char *passphrase);

#endif
