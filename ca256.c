/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ca256.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 16:14:09 by fhenrion          #+#    #+#             */
/*   Updated: 2020/02/15 00:00:21 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ca256.h"
#include "sha256.h"
#include "progress_bar.h"

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

static void encrypt_block(ull *block, ull *state, rule rule_seq[64])
{
	static const rule	rules[8] = {30, 86, 90, 101, 105, 150, 153, 165};
	ull					st = *state;
	size_t				i;
	

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

static void rule_seq_gen(ull *states, rule rule_seq[4][64])
{
	static const rule	rules[8] = {30, 86, 90, 101, 105, 150, 153, 165};
	size_t				i, j;

	for (i = 0; i < 16; i++)
		for (j = 0; j < 64; j++)
			rule_seq[i][j] = rules[0b111 & (states[i] >> j)];
}

static int	encryption(int fd_in, int fd_out, ull *states, size_t file_size)
{
	ull		block[BLOCK_SIZE] = {0};
	rule	rule_seq[16][64];
	size_t	i, read_size;

	rule_seq_gen(states, rule_seq);
	for (i = 0; i < 16; i++)
		encrypt_block(&block[i], &states[i], rule_seq[i]);
	ft_progress(file_size);
	while ((read_size = read(fd_in, block, BUFFER_SIZE)) > 0)
	{
		for (i = 0; i < BLOCK_SIZE; i++)
			encrypt_block(&block[i], &states[i % 16], rule_seq[i % 16]);
		if (write(fd_out, block, read_size) < 1)
		{
			write(1, "Write file error\n", 17);
			return (1);
		}
		ft_progress(file_size);
	}
	write(1, "\n", 1);
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
	size_t	len;

	len = strlen(name) - 2;
	new = (char*)malloc(len);
	strlcpy(new, name, len);
	return (new);
}


static char	*parse_option(char *option, char *input_file)
{
	char *output_file;

	if (!strcmp(option, "-e"))
		output_file = add_extension(input_file);
	else if (!strcmp(option, "-d"))
		output_file = remove_extension(input_file);
	else
	{
		write(1, "error option\n", 13);
		return (NULL);
	}
	return (output_file);
}

static void	gen_state(char *passphrase, ull states[16])
{
	uint8_t	hash[32];

	hash_sha256((const uint8_t*)passphrase, hash);
	memcpy(&states[0], hash, 32);
	hash_sha256(hash, hash);
	memcpy(&states[4], hash, 32);
	hash_sha256(hash, hash);
	memcpy(&states[8], hash, 32);
	hash_sha256(hash, hash);
	memcpy(&states[12], hash, 32);
}

int			main(int ac, char **av)
{
	ull		states[16];
	int		fd_in;
	int		fd_out;
	size_t	file_size;
	char	*output_file;

	if (ac == 4)
	{
		fd_in = open(av[3], O_RDONLY);
		file_size = lseek(fd_in, 0L, SEEK_END) + 1;
		lseek(fd_in, 0L, SEEK_SET);
		if (!(output_file = parse_option(av[1], av[3])))
			return (1);
		fd_out = open(output_file, O_WRONLY | O_CREAT, 0644);
		gen_state(av[2], states);
		printf("%s -> %s\n", av[3], output_file);
		free(output_file);
		encryption(fd_in, fd_out, states, file_size / BUFFER_SIZE);
		close(fd_in);
		close(fd_out);
	}
	else
		write(1, "USAGE: -e(encrypt)/-d(decrypt) \"passphrase\" \"file\"\n", 51);
	return (0);
}
