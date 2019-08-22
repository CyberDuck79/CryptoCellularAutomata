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

typedef unsigned long long ull;
#define N (sizeof(ull) * CHAR_BIT)
#define U sizeof(ull)
#define B(x) (1ULL << (x))

ull encrypt_block(ull block, char *key, int key_len, char gen_seed)
{
    ull             st;
	static int		key_i = 0;
    static ull      state = 2147483648;
    int             rule_i = 0;
    char            rules[8] = {30, 135, 86, 149, 45, 101, 75, 89};

	rule_i += key[key_i]; // get rule index by adding permutation value at key index
    if (rule_i >= 8) // if rule index > 8, loop
        rule_i -= 8;
    st = state;
    state = 0;
    for (unsigned long i = 0; i < N; i++)
        if (rules[rule_i] & B(7 & (st>>(i-1) | st<<(N+1-i))))
            state |= B(i);
    if (!gen_seed) // if seed gen, do not apply XOR mask
        block = block ^ state;
    if (++key_i == key_len) // if at key end return to start
        key_i = 0;
    return (block);
}

int     encryption(int fd[2], char *key, int key_len)
{
    ull block;

    for (int i = 0; i < 64; i++) // gen seed state
        encrypt_block(0, key, key_len, 1);
    while (read(fd[0], &block, U)) // encryption
    {
        block = encrypt_block(block, key, key_len, 0);
        write(fd[1], &block, U);
    }
    // TODO : check errors ?
    // TODO : end gestion for decryption ?
    return (0);
}

// TODO : print states for test
int     main(int argc, char **argv)
{
    char    *key;
    char    c[2] = {0};
    int     fd[2];
    int     i = 0;
    int     key_len = 0;

    if (argc == 3)
    {
        if ((fd[0] = open(argv[1], O_RDONLY)) == -1 || \
        (fd[1] = open("output.ca",  O_WRONLY | O_CREAT, 0644)) == -1)
        {
			perror("open failed");
			exit(1);
        }
        while (argv[2][key_len])
            key_len++;
        if (!(key = (char*)malloc(sizeof(char) * key_len)))
        {
            perror("memory allocation failed");
			exit(1);
        }
        while((c[0] = argv[2][i]))
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
        write(1, "USAGE : filename key\n", 21);
    }
    return(0);
}
