/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 12:06:40 by fhenrion          #+#    #+#             */
/*   Updated: 2020/01/20 22:56:45 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHA256_H
# define SHA256_H

# include <stdint.h>
# include <stdlib.h>
# include <memory.h>

# define SHA256_BLOCK_SIZE 32

# define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
# define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

// SHA-256 Functions
# define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
# define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
# define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
# define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
# define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
# define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

typedef unsigned long long t_ull;

typedef struct	s_sha256_ctx
{
	uint8_t		data[64];
	size_t		datalen;
	t_ull		bitlen;
	uint32_t	state[8];
}				t_sha256_ctx;

void			hash_sha256(const uint8_t *data, uint8_t hash[32]);

#endif
