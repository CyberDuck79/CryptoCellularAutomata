/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ca256.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 16:14:09 by fhenrion          #+#    #+#             */
/*   Updated: 2020/11/06 15:28:02 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ca256.h"
#include "sha256.h"
#include "progress_bar.h"

/*
** TODO :
** - Ways to improve randomness ?
*/

/*
static void write_state(ull	*state)
{
	static const char	square[6] = {0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88};

	for (int i = 0; i < 64; i++)
		if (*state & B(i))
			write(1, square, 6);
		else
			write(1, "  ", 2);
	write(1, "\n", 1);
}
*/

const uint8_t	rules[8] = {0x1E, 0x54, 0x5A, 0x65, 0x69, 0x96, 0x99, 0xA5};

static void encrypt_block(ull *block, ull *state, uint8_t rule_seq[ULL_SIZE])
{
	ull	st = *state;

	*state = 0;
	for (ull i = 0; i < ULL_SIZE; i++)
	{
		if (rule_seq[i] & B(0b111 & (st >> (i - 1) | (st << (65 - i)))))
			*state |= B(i);
		rule_seq[i] = rules[0b111 & (st >> i)];
	}
	*block ^= *state;
	//write_state(state); // -> output for tests
}

static void rule_seq_gen(ull states[STATE_NB], uint8_t rule_seq[STATE_NB][ULL_SIZE])
{
	for (int i = 0; i < STATE_NB; i++)
		for (ull j = 0; j < ULL_SIZE; j++)
			rule_seq[i][j] = rules[0b111 & (states[i] >> j)];
}

static int	encryption(t_file *file, ull states[STATE_NB])
{
	ull		block[BLOCK_SIZE] = {0};
	uint8_t	rule_seq[STATE_NB][ULL_SIZE];
	ssize_t	read_size;

	rule_seq_gen(states, rule_seq);
	for (int i = 0; i < STATE_NB; i++)
		encrypt_block(&block[i], &states[i], rule_seq[i]);
	while ((read_size = read(file->read, block, BUFFER_SIZE)) > 0)
	{
		for (int i = 0; i < BLOCK_SIZE; i++)
			encrypt_block(&block[i], &states[i % STATE_NB], rule_seq[i % STATE_NB]);
		if (write(file->write, block, read_size) != read_size)
		{
			read_size = -1;
			break;
		}
		ft_progress(file->size);
	}
	ft_progress_end();
	close(file->read);
	close(file->write);
	return (read_size < 0 ? 1 : 0);
}

static void	gen_state(char *passphrase, ull states[STATE_NB])
{
	hash_sha256((const uint8_t*)passphrase, (uint8_t*)&states[0]);
	for (int i = 4; i < STATE_NB; i += 4)
		hash_sha256((const uint8_t*)&states[i - 4], (uint8_t*)&states[i]);
}

static void	open_files(t_file *file, char **av, char *output_name)
{
	if ((file->read = open(av[3], O_RDONLY)) == ERROR)
	{
		perror(av[3]);
		close(file->read);
		exit(errno);
	}
	if ((file->write = open(output_name, O_WRONLY | O_CREAT, 0644)) == ERROR)
	{
		perror(output_name);
		close(file->read);
		close(file->write);
		exit(errno);
	}
	file->size = lseek(file->read, 0L, SEEK_END) / BUFFER_SIZE;
	if (!file->size) {
		file->size++;
	}
	lseek(file->read, 0L, SEEK_SET);
}

int			main(int ac, char **av)
{
	ull		states[STATE_NB] = {0};
	t_file	file;
	char	*output_name;

	if (ac != 4)
	{
		write(2, "USAGE: -e(encrypt)/-d(decrypt) \"passphrase\" \"file\"\n", 51);
		return (1);
	}
	output_name = parse_option(av[1], av[3]);
	if (!output_name)
	{
		write(2, "Error: bad option or file extension for decryption\n", 51);
		return (2);
	}
	open_files(&file, av, output_name);
	printf("%s -> %s\n", av[3], output_name);
	free(output_name);
	gen_state(av[2], states);
	if (encryption(&file, states))
	{
		write(2, "Error: reading or writing files error\n", 38);
		return (3);
	}
	return (0);
}
