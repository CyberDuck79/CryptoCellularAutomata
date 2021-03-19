/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ca_hash64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 14:27:19 by fhenrion          #+#    #+#             */
/*   Updated: 2021/03/19 17:05:25 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint64_t to_block_padding(const char *str_block, size_t padding) {
	uint64_t block = 0;
	for (size_t i = 0; i < padding; ++i) {
		block |= str_block[i] << i;
	}
	for (size_t i = 0; i < 8 - padding; ++i) {
		block |= '#' << i;
	}
	return block;
}

uint64_t generate_seed(const char *passphrase) {
	size_t len = strlen(passphrase);
	size_t nb_blocks = len / 8;
	size_t padding = len % 8;
	uint64_t seed = 0;
	uint64_t block = 0;
	
	for (size_t i = 0; i < nb_blocks; ++i) {
		block = to_block_padding(passphrase + i, 8);
	}
}