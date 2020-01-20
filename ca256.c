/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ca256.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 16:14:09 by fhenrion          #+#    #+#             */
/*   Updated: 2020/01/17 19:00:46 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
TODO
ask for a passphrase
ask for a file
calculate the hash of the passphare
put the hash on initial state
calculate the rules sequence
for 0 to 127:
	rules_seq[i] = (hash >> (i * 2)) & 0x3
start encryption
while (not en of file):
	for (i = 0, j = 0; i < 255; i += 2, j++)
		rule_i = 0
		calculate rule j at state i
		if (bits):
			apply bits
			rule_i++
		calculate rule j at state i + 1
		if (bits):
			apply bits
			rule_i += 2
		rule_seq[j] = rule_i
	calculate block with state XOR Mask
*/

#include "ca256.h"

static void encrypt_block(ull *block, ull *state, rule *rule_seq)
{
	size_t			i, j, r;

	for (i = 0, j = 0; i < 255; i += 2, j++)
	{
		r = 0;
		if (rule_seq[j] & B(0b111 & (*state>>(i-1) | *state<<(254-i))))
		{
			*state |= B(i);
			r++;
		}
		if (rule_seq[j] & B(0b111 & (*state>>(i) | *state<<(255-i))))
		{
			*state |= B(i+1);
			r += 2;
		}
		rule_seq[j] = r;
	}
	*block = *block ^ *state;
}

static void	encryption(int fd, ull *hash)
{
	size_t	read_size;
	ull		block;
	rule	rule_seq[127];
	size_t	index;

	for (index = 0; index < 127; index++)
		rule_seq[index] = (*hash >> (index * 2)) & 0b11;
	for (index = 0; index < 127; index++)
		encrypt_block(&block, hash, rule_seq);
	while ((read_size = read(fd, &block, 64)))
	{
		encrypt_block(&block, hash, rule_seq);
		write(fd, &block, read_size);
	}
	close(fd);
}