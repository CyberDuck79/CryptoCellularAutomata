/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ca256.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 16:14:09 by fhenrion          #+#    #+#             */
/*   Updated: 2020/01/20 23:28:41 by fhenrion         ###   ########.fr       */
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
#include "sha256.h"

static void write_state(ull	*state)
{
	size_t			i;

	for (i = 0; i < 255; i++)
		write(1, *state & B(i) ? "#" : " ", 1);
	write(1, "\n", 1);
}

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
	write_state(state); // -> output for tests
}

static int	encryption(int fd_in, int fd_out, ull *hash)
{
	size_t	read_size;
	ull		block = 0;
	rule	rules[4] = {RULE0, RULE1, RULE2, RULE3};
	rule	rule_seq[127];
	size_t	index;

	if (read(fd_in, &block, 0) < 0)
		return (1);
	if (write(fd_out, &block, 0) < 0)
		return (1);
	for (index = 0; index < 127; index++)
		rule_seq[index] = rules[(*hash >> (index * 2)) & 0b11];
	for (index = 0; index < 127; index++)
		encrypt_block(&block, hash, rule_seq);
	while ((read_size = read(fd_in, &block, 64)))
	{
		encrypt_block(&block, hash, rule_seq);
		write(fd_out, &block, read_size);
	}
	return (0);
}

int			main(int ac, char **av)
{
	//// USAGE : av[1] passphrase av[2] file
	ull		hash;
	int		fd_in;
	int		fd_out;
	char	*enc_file;

	if (ac == 3)
	{
		hash_sha256((const uint8_t*)av[1], (uint8_t*)&hash);
		write_state(&hash);
		fd_in = open(av[2], O_RDONLY); // error here ???
		enc_file = (char*)malloc(strlen(av[2]) + 4);
		strlcat(enc_file, ".ca", 4);
		fd_out = open(enc_file, O_WRONLY, 0644);
		free(enc_file);
		encryption(fd_in, fd_out, &hash);
		close(fd_in);
		close(fd_out);
	}
	else
		write(1, "USAGE: \"passphrase\" \"file\"\n", 27);
	return (0);
}