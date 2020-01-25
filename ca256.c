/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ca256.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 16:14:09 by fhenrion          #+#    #+#             */
/*   Updated: 2020/01/25 13:17:42 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ca256.h"
#include "sha256.h"

static void write_state(ull	*state)
{
	size_t		i;
	static char	square[6] = {0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88};

	for (i = 0; i < 64; i++)
		if (*state & B(i))
			write(1, square, 6);
		else
			write(1, "  ", 2);
	write(1, "\n", 1);
}

static void encrypt_block(ull *block, ull *state, rule rule_seq[64])
{
	static rule	rules[8] = {30, 86, 90, 101, 105, 150, 153, 165};
	size_t		i;
	ull			st = *state;

	*state = 0;
	for (i = 0; i < 64; i++)
	{
		if (rule_seq[i] & B(0b111 & (st >> (i - 1) | (st << (65 - i)))))
			*state |= B(i);
		rule_seq[i] = rules[0b111 & (st >> i)];
	}
	*block ^= *state;
	//write_state(state); // -> output for tests
}

static void rule_seq_gen(ull *state, rule rule_seq[4][64])
{
	rule	rules[8] = {30, 86, 90, 101, 105, 150, 153, 165};
	size_t	i, j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 64; j++)
			rule_seq[i][j] = rules[0b111 & (state[i] >> j)];
}

static int	encryption(int fd_in, int fd_out, ull *state)
{
	ull		block[4] = {0};
	rule	rule_seq[4][64];
	size_t	i, read_size;

	rule_seq_gen(state, rule_seq);
	for (i = 0; i < 4; i++)
		encrypt_block(&block[i], &state[i], rule_seq[i]);
	while ((read_size = read(fd_in, block, 32)) > 0)
	{
		for (i = 0; i < 4; i++)
			encrypt_block(&block[i], &state[i], rule_seq[i]);
		if (write(fd_out, block, read_size) < 1)
		{
			write(1, "Write file error\n", 17);
			return (1);
		}
	}
	if (read_size == -1)
	{
		write(1, "Read file error\n", 16);
		return (1);
	}
	return (0);
}

static char	*add_extension(char *name)
{
	char	*new;

	new = (char*)malloc(strlen(name) + 4);
	strcpy(new, name);
	strlcpy(new + strlen(name), ".ca", 4);
	return (new);
}

static char	*remove_extension(char *name)
{
	char	*new;

	new = (char*)malloc(strlen(name) - 2);
	strlcpy(new, name, strlen(name) - 2);
	return (new);
}

int			main(int ac, char **av)
{
	//// USAGE : av[1] passphrase av[2] file
	uint8_t	hash[32];
	ull		state[4];
	int		fd_in;
	int		fd_out;
	char	*out_file;

	if (ac == 4)
	{
		if (!strcmp(av[1], "-e"))
		{
			out_file = add_extension(av[3]);
			fd_out = open(out_file, O_WRONLY | O_CREAT, 0644);
		}
		else if (!strcmp(av[1], "-d"))
		{
			out_file = remove_extension(av[3]);
			fd_out = open(out_file, O_WRONLY | O_CREAT, 0644);
		}
		else
		{
			write(1, "error option\n", 13);
			return (0);
		}
		hash_sha256((const uint8_t*)av[2], hash);
		memcpy(&state, hash, 32);
		fd_in = open(av[3], O_RDONLY);
		printf("%s\n", out_file);
		free(out_file);
		encryption(fd_in, fd_out, state);
		close(fd_in);
		close(fd_out);
	}
	else
		write(1, "USAGE: -e(encrypt)/-d(decrypt) \"passphrase\" \"file\"\n", 27);
	return (0);
}
