/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 10:46:59 by fhenrion          #+#    #+#             */
/*   Updated: 2019/08/30 16:18:31 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <errno.h>

#define USAGE "USAGE : (-e : encryption / -d : decryption) filename key\n"
#define LOG 1
#define EXPORT 1
#define N (sizeof(ull) * CHAR_BIT)
#define U sizeof(ull)
#define B(x) (1ULL << (x))
#define RULES 30,86,73,105,150,109
#define ST1 4294967296ULL
#define ST2 268435456ULL
#define ST3 2147483648ULL
#define ST4 8589934592ULL
#define ST5 34359738368ULL
#define ST6 17179869184ULL
#define ST7 536870912ULL
#define ST8 1073741824ULL
#define START_STATES ST1,ST2,ST3,ST4,ST5,ST6,ST7,ST8

typedef unsigned long long ull;

typedef struct	s_cypher
{
	int			key_i;
	int			rule_i;
	size_t		key_len;
	ull			state;
}				cypher;

int				check_option(char *str);
void			open_error(int type);
int				check_extension(char *str);
char			*remove_extension(char *str);
char			*add_extension(char *str);
