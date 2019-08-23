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

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

typedef unsigned long long ull;
#define N (sizeof(ull) * CHAR_BIT)
#define U sizeof(ull)
#define B(x) (1ULL << (x))

// TO TEST : start seed select -> by first number of key ?
// start state to test :
// 12297829382473034410 -> 10
// 6148914691236517205 -> 01
// 9838263505978427528 -> 1000
// 4919131752989213764- > 0100
// 2459565876494606882 -> 0010
// 1229782938247303441 -> 0001
// 9259542123273814144 -> 10000000
// 4629771061636907072 -> 01000000
// 2314885530818453536 -> 00100000
// 1157442765409226768 -> 00010000
// 578721382704613384 -> 00001000
// 289360691352306692 -> 00000100
// 144680345676153346 -> 00000010
// 72340172838076673 -> 00000001

ull 	encrypt_block(ull block, char *key, int key_len, char gen_seed)
{
	ull			st;
	static int	key_i = 0;
	static int	rule_i = 0;
	static ull	state = 2147483648;
	char		rules[8] = {30, 135, 86, 149, 45, 101, 75, 89};

	rule_i += key[key_i]; // get rule index by adding permutation value at key index
	if (rule_i >= 8) // if rule index > 8, loop
		rule_i -= 8;
	st = state;
	state = 0;
	for (unsigned long i = 0; i < N; i++)
		if (rules[rule_i] & B(7 & (st>>(i-1) | st<<(N+1-i))))
			state |= B(i);
	// logging
	write(1, "===> State :\n", 13);
	for (i = N; i; i--)
			write(1, state & B(i) ? "1" : "0", 1);
	write(1, "\n", 1);
	if (!gen_seed) // if seed gen, do not apply XOR mask
	{
		// logging
		write(1, "===> block pre encryption :\n", 29);
		for (i = N; i; i--)
			write(1, block & B(i) ? "1" : "0", 1);
		write(1, "\n", 1);
		block = block ^ state;
		// logging
		write(1, "===> block post encryption :\n", 30);
		for (i = N; i; i--)
			write(1, block & B(i) ? "1" : "0", 1);
		write(1, "\n", 1);
	}
	if (++key_i == key_len) // if at key end return to start
		key_i = 0;
	return (block);
}

int		encryption(int fd[2], char *key, int key_len)
{
	ull		block;
	size_t	read_size;

	write(1, "========== SEED GENERATION ==========\n", 38); // logging
	for (int i = 0; i < 64; i++) // gen seed state
		encrypt_block(0, key, key_len, 1);
	write(1, "==========   ENCRYPTION    ==========\n", 38); // logging
	while ((read_size = read(fd[0], &block, U))) // encryption
	{
		block = encrypt_block(block, key, key_len, 0);
		write(fd[1], &block, read_size);
	}
	// TODO : check errors ?
	// TODO : end gestion for decryption ? -> write only read_size ?
	return (0);
}

int		main(int argc, char **argv)
{
	char	*key;
	char	c[2] = {0};
	int		fd[2];
	int		i = 0;
	int		key_len = 0;

	if (argc == 4)
	{
		// TODO :
		// - prendre le nom du fichier et ajouter / enlever extension
		// - gerer options sur argv[1]
		if ((fd[0] = open(argv[2], O_RDONLY)) == -1 || \
		(fd[1] = open("output.ca",  O_WRONLY | O_CREAT, 0644)) == -1)
		{
			perror("open failed");
			exit(1);
		}
		while (argv[3][key_len])
			key_len++;
		if (!(key = (char*)malloc(sizeof(char) * key_len)))
		{
			perror("memory allocation failed");
			exit(1);
		}
		while((c[0] = argv[3][i]))
		{
			if(!(key[i] = (char)atoi(c)))
			{
				perror("invalid key");
				exit(1);
			}
			i++;
		}
		encryption(fd, key, key_len);
		// TODO : add decryption gestion
	}
	else
	{
		write(1, "USAGE : (-e : encryption / -d : decryption) filename key :\n", 59);
	}
	return(0);
}
