/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CAcrypto.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 10:46:59 by fhenrion          #+#    #+#             */
/*   Updated: 2019/08/24 12:14:28 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

typedef unsigned long long ull;
#define USAGE "USAGE : (-e : encryption / -d : decryption) filename key\n"
#define N (sizeof(ull) * CHAR_BIT)
#define U sizeof(ull)
#define B(x) (1ULL << (x))
#define ST1 9259542123273814144ULL
#define ST2 4629771061636907072ULL
#define ST3 2314885530818453536ULL
#define ST4 1157442765409226768ULL
#define ST5 578721382704613384ULL
#define ST6 289360691352306692ULL
#define ST7 144680345676153346ULL
#define ST8 72340172838076673ULL

typedef struct
{
	int	key_i;
	int	rule_i;
	int	key_len;
	ull	state;
}		cypher;

typedef struct
{
	char	*key;
	int		key_len;
	int		fd[2];
}		data;
