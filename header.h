/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 10:46:59 by fhenrion          #+#    #+#             */
/*   Updated: 2019/08/24 16:04:01 by fhenrion         ###   ########.fr       */
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
#define LOG 0
#define N (sizeof(ull) * CHAR_BIT)
#define U sizeof(ull)
#define B(x) (1ULL << (x))
#define RULES 30,135,86,149,45,101,75,89,105,121,145,107,109,125,131,73
#define ST1 9259542123273814144ULL
#define ST2 4629771061636907072ULL
#define ST3 2314885530818453536ULL
#define ST4 1157442765409226768ULL
#define ST5 578721382704613384ULL
#define ST6 289360691352306692ULL
#define ST7 144680345676153346ULL
#define ST8 72340172838076673ULL
#define ST9 6148914691236517205ULL
#define START_STATES ST1,ST2,ST3,ST4,ST5,ST6,ST7,ST8,ST9

typedef unsigned long long ull;

typedef struct
{
	int		key_i;
	int		rule_i;
	size_t	key_len;
	ull		state;
}			cypher;

int		check_option(char *str);
void	open_error(int type);
int		check_extension(char *str);
char	*remove_extension(char *str);
char	*add_extension(char *str);
