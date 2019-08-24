/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CAcrypto.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 20:21:07 by fhenrion          #+#    #+#             */
/*   Updated: 2019/08/22 20:21:07 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

// TODO :
// - tests
// - evolution

// EVOLUTIONS :
// - 2 states simultaneously computed / successively swap
// - 1 mask / 1 hidden -> hidden / mask states result choose states permutation
// - key of state 2 derived from original key :
//		- for each nb of key -> if (nb % 2) nb++ else nb--
// - reverse reading key and reverse permute rules ?

static void	logging(int rule, ull state)
{
	for (size_t i = N; i; i--)
				write(1, state & B(i) ? "#" : " ", 1);
	printf(" rule %d\n", rule);
}

static ull 	encrypt_block(ull block, int *key, cypher *cphr, char gen_seed)
{
	ull				st;
	int				rules[16] = {RULES};
	ull				start[9] = {START_STATES};

	cphr->rule_i += key[cphr->key_i++];
	cphr->rule_i = cphr->rule_i % 16;
	if (!cphr->state)
		cphr->state = start[(cphr->rule_i - 1)];
	st = cphr->state;
	cphr->state = 0;
	for (size_t i = 0; i < N; i++)
		if (rules[cphr->rule_i] & B(7 & (st>>(i-1) | st<<(N+1-i))))
			cphr->state |= B(i);
	if (LOG)
		logging(rules[cphr->rule_i], cphr->state);
	if (!gen_seed)
		block = block ^ cphr->state;
	cphr->key_i = cphr->key_i % cphr->key_len;
	return (block);
}

static void	encryption(int fd[2], int *key, size_t keylen)
{
	ull		block;
	ull		tmp;
	size_t	read_size;
	cypher	cphr = {0, 0, keylen, 0};

	for (size_t i = 0; i < keylen; i++)
		encrypt_block(0, key, &cphr, 1);
	while ((read_size = read(fd[0], &block, U)))
	{
		tmp = block;
		while (tmp == block)
			block = encrypt_block(block, key, &cphr, 0);
		write(fd[1], &block, read_size);
	}
}

static int	*read_key(char *str)
{
	int		i = 0;
	int		*key;
	char	c[2] = {0};

	if (!(key = (int*)malloc(sizeof(int) * strlen(str))))
	{
		perror("memory allocation failed");
		exit(EXIT_FAILURE);
	}
	while((c[0] = str[i]))
	{
		if(!(key[i] = atoi(c)))
		{
			write(2, "invalid key\n", 12);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	return (key);
}

int		main(int argc, char **argv)
{
	char	*file;
	data	data = {0, {0, 0}};

	if (argc == 4 && check_option(argv[1]))
	{
		data.key = read_key(argv[3]);
		if (!strcmp("-e", argv[1]))
		{
			file = add_extension(argv[2]);
			if ((data.fd[0] = open(argv[2], O_RDONLY)) == -1 || \
			(data.fd[1] = open(file, O_WRONLY | O_CREAT, 0644)) == -1)
				open_error(1);
		}
		else
		{
			if (!check_extension(argv[2]))
				open_error(0);
			file = remove_extension(argv[2]);
			if ((data.fd[0] = open(argv[2], O_RDONLY)) == -1 || \
			(data.fd[1] = open(file, O_WRONLY | O_CREAT, 0644)) == -1)
				open_error(1);
		}
		encryption(data.fd, data.key, strlen(argv[3]));
		remove(argv[2]);
	}
	else
	{
		write(1, USAGE, sizeof(USAGE));
	}
	return(0);
}
