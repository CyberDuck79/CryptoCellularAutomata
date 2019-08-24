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

#include "CAcrypto.h"

// TODO :
// - tests
// - dechiffrement
// - static / extern
// - evolution

// EVOLUTIONS :
// - 2 states simultaneously computed / successively swap
// - 1 mask / 1 hidden -> hidden / mask states result choose states permutation
// - key of state 2 derived from original key :
//		- for each nb of key -> if (nb % 2) nb++ else nb--
// - reverse reading key and reverse permute rules ?

ull 	encrypt_block(ull block, char *key, cypher *cphr, char gen_seed)
{
	ull				st;
	unsigned long	i;
	char			rules[8] = {30, 135, 86, 149, 45, 101, 75, 89};
	ull				start[8] = {ST1, ST2, ST3, ST4, ST5, ST6, ST7, ST8};

	cphr->rule_i += key[cphr->key_i];
	if (cphr->rule_i >= 8)
		cphr->rule_i -= 8;
	if (!cphr->state)
		cphr->state = start[cphr->rule_i];
	st = cphr->state;
	cphr->state = 0;
	for (i = 0; i < N; i++)
		if (rules[cphr->rule_i] & B(7 & (st>>(i-1) | st<<(N+1-i))))
			cphr->state |= B(i);
	// logging
	for (i = N; i; i--)
			write(1, cphr->state & B(i) ? "#" : " ", 1);
	write(1, "\n", 1);
	if (!gen_seed)
		block = block ^ cphr->state;
	if (++(cphr->key_i) == cphr->key_len)
		cphr->key_i = 0;
	return (block);
}

int		encryption(data	*data)
{
	ull		block;
	size_t	read_size;
	cypher	cphr = {0, 0, data->key_len, 0};;

	for (int i = 0; i < data->key_len; i++)
		encrypt_block(0, data->key, &cphr, 1);
	while ((read_size = read(data->fd[0], &block, U)))
	{
		block = encrypt_block(block, data->key, &cphr, 0);
		write(data->fd[1], &block, read_size);
	}
	// TODO : check errors ?
	return (0);
}

int		check_option(char *str)
{
	if (!strcmp("-e", str))
		return (1);
	else if (!strcmp("-d", str))
		return (1);
	return (0);
}

char	*read_key(char *str, int key_len)
{
	int		i = 0;
	char	*key;
	char	c[2] = {0};

	if (!(key = (char*)malloc(sizeof(char) * key_len)))
	{
		perror("memory allocation failed");
		exit(1);
	}
	while((c[0] = str[i]))
	{
		if(!(key[i] = (char)atoi(c)))
		{
			perror("invalid key");
			exit(1);
		}
		i++;
	}
	return (key);
}

char	*add_extension(char *str)
{
	int		i = 0;
	size_t	len = 0;
	char	*output;

	while (str[i++])
		len++;
	output = (char*)malloc(sizeof(char) * (len + 4));
	i = -1;
	while (str[++i])
		output[i] = str[i];
	output[i++] = '.';
	output[i++] = 'c';
	output[i++] = 'a';
	output[i++] = '\0';
	return (output);
}

char	*remove_extension(char *str)
{
	size_t	i = 0;
	size_t	len = 0;
	char	*output;

	while (str[i++])
		len++;
	output = (char*)malloc(sizeof(char) * (len - 2));
	i = 0;
	while (i < (len - 3))
	{
		output[i] = str[i];
		i++;
	}
	output[i] = '\0';
	return (output);
}

int		check_extension(char *str)
{
	size_t	i = 0;

	while (str[i])
		i++;
	if (str[--i] != 'a')
		return (0);
	if (str[--i] != 'c')
		return (0);
	if (str[--i] != '.')
		return (0);
	return (1);
}

void	open_error(void)
{
	perror("open failed");
	exit(1);
}

int		main(int argc, char **argv)
{
	char	*file;
	data	data = {0, 0, {0, 0}};

	if (argc == 4 && check_option(argv[1]))
	{
		while (argv[3][data.key_len])
			data.key_len++;
		data.key = read_key(argv[3], data.key_len);
		if (!strcmp("-e", argv[1]))
		{
			file = add_extension(argv[2]);
			if ((data.fd[0] = open(argv[2], O_RDONLY)) == -1 || \
			(data.fd[1] = open(file, O_WRONLY | O_CREAT, 0644)) == -1)
			open_error();
		}
		else
		{
			if (!check_extension(argv[2]))
				open_error();
			file = remove_extension(argv[2]);
			if ((data.fd[0] = open(argv[2], O_RDONLY)) == -1 || \
			(data.fd[1] = open(file, O_WRONLY | O_CREAT, 0644)) == -1)
				open_error();
		}
		encryption(&data);
	}
	else
	{
		write(1, USAGE, sizeof(USAGE));
	}
	return(0);
}
