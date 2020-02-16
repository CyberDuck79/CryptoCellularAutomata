/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ca256.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 16:14:09 by fhenrion          #+#    #+#             */
/*   Updated: 2020/02/16 14:03:32 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ca256.h"
#include "sha256.h"
#include "progress_bar.h"

/*
static void write_state(ull	*state)
{
	static const char	square[6] = {0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88};
	size_t				i;

	for (i = 0; i < 64; i++)
		if (*state & B(i))
			write(1, square, 6);
		else
			write(1, "  ", 2);
	write(1, "\n", 1);
}
*/

static void encrypt_block(ull *block, ull *state, rule rule_seq[ULL_SIZE])
{
	static const rule	rules[8] = {30, 86, 90, 101, 105, 150, 153, 165};
	ull					st = *state;
	size_t				i;
	

	*state = 0;
	for (i = 0; i < ULL_SIZE; i++)
	{
		if (rule_seq[i] & B(0b111 & (st >> (i - 1) | (st << (65 - i)))))
			*state |= B(i);
		rule_seq[i] = rules[0b111 & (st >> i)];
	}
	*block ^= *state;
	//write_state(state); // -> output for tests
}

static void rule_seq_gen(ull states[STATE_NB], rule rule_seq[STATE_NB][ULL_SIZE])
{
	static const rule	rules[8] = {30, 86, 90, 101, 105, 150, 153, 165};
	size_t				i, j;

	for (i = 0; i < STATE_NB; i++)
		for (j = 0; j < ULL_SIZE; j++)
			rule_seq[i][j] = rules[0b111 & (states[i] >> j)];
}

static int	encryption(t_file *file, ull states[STATE_NB])
{
	ull			block[BLOCK_SIZE] = {0};
	rule		rule_seq[STATE_NB][ULL_SIZE];
	size_t		i;
	long long	read_size;

	rule_seq_gen(states, rule_seq);
	for (i = 0; i < STATE_NB; i++)
		encrypt_block(&block[i], &states[i], rule_seq[i]);
	ft_progress(file->size);
	while ((read_size = read(file->fd_in, block, BUFFER_SIZE)) > 0)
	{
		for (i = 0; i < BLOCK_SIZE; i++)
			encrypt_block(&block[i], &states[i % STATE_NB], rule_seq[i % STATE_NB]);
		if (write(file->fd_out, block, read_size) < 1)
			return (1);
		ft_progress(file->size);
	}
	write(1, "\n", 1);
	if (read_size < 0)
		return (1);
	return (0);
}

static void	gen_state(char *passphrase, ull states[STATE_NB])
{
	uint8_t	hash[HASH_SIZE];

	hash_sha256((const uint8_t*)passphrase, hash);
	memcpy(&states[0], hash, HASH_SIZE);
	hash_sha256((uint8_t*)&states[0], hash);
	memcpy(&states[4], hash, HASH_SIZE);
	hash_sha256((uint8_t*)&states[4], hash);
	memcpy(&states[8], hash, HASH_SIZE);
	hash_sha256((uint8_t*)&states[8], hash);
	memcpy(&states[12], hash, HASH_SIZE);
}

int			main(int ac, char **av)
{
	ull		states[STATE_NB] = {0};
	t_file	file;

	if (ac == 4 && (file.output_name = parse_option(av[1], av[3])))
	{
		file.fd_in = open(av[3], O_RDONLY);
		file.size = lseek(file.fd_in, 0L, SEEK_END) / BUFFER_SIZE;
		lseek(file.fd_in, 0L, SEEK_SET);
		file.fd_out = open(file.output_name, O_WRONLY | O_CREAT, 0644);
		gen_state(av[2], states);
		printf("%s -> %s\n", av[3], file.output_name);
		if (encryption(&file, states))
			write(1, "Error while reading or writing files\n", 1);
		free(file.output_name);
		close(file.fd_in);
		close(file.fd_out);
	}
	else
		write(1, "USAGE: -e(encrypt)/-d(decrypt) \"passphrase\" \"file\"\n", 51);
	return (0);
}
