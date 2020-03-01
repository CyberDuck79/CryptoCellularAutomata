/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ca256.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 16:14:09 by fhenrion          #+#    #+#             */
/*   Updated: 2020/03/01 19:36:11 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ca256.h"
#include "sha256.h"
#include "progress_bar.h"

/*
** TODO :
** - error if extension is not .ca for decryption
** - Ways to improve randomness ?
*/

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
	int			i, read_size;

	rule_seq_gen(states, rule_seq);
	for (i = 0; i < STATE_NB; i++)
		encrypt_block(&block[i], &states[i], rule_seq[i]);
	ft_progress(file->size);
	while ((read_size = read(file->read, block, BUFFER_SIZE)) > 0)
	{
		for (i = 0; i < BLOCK_SIZE; i++)
			encrypt_block(&block[i], &states[i % STATE_NB], rule_seq[i % STATE_NB]);
		if (write(file->write, block, read_size) != read_size)
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
	size_t	i;

	hash_sha256((const uint8_t*)passphrase, (uint8_t*)&states[0]);
	for (i = 4; i < STATE_NB; i += 4)
		hash_sha256((const uint8_t*)&states[i - 4], (uint8_t*)&states[i]);
}

int			main(int ac, char **av)
{
	ull		states[STATE_NB] = {0};
	t_file	file;

	if (ac == 4 && (file.output_name = parse_option(av[1], av[3])))
	{
		if ((file.read = open(av[3], O_RDONLY)) == ERROR)
		{
			perror(av[3]);
			exit(errno);
		}
		file.size = lseek(file.read, 0L, SEEK_END) / BUFFER_SIZE;
		lseek(file.read, 0L, SEEK_SET);
		if ((file.write = open(file.output_name, O_WRONLY | O_CREAT, 0644)) == ERROR)
		{
			perror(file.output_name);
			exit(errno);
		}
		gen_state(av[2], states);
		printf("%s -> %s\n", av[3], file.output_name);
		if (encryption(&file, states))
			write(1, "Error while reading or writing files\n", 1);
		free(file.output_name);
		close(file.read);
		close(file.write);
	}
	else
		write(1, "USAGE: -e(encrypt)/-d(decrypt) \"passphrase\" \"file\"\n", 51);
	return (0);
}
