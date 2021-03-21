/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ca_hash64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 14:27:19 by fhenrion          #+#    #+#             */
/*   Updated: 2021/03/21 09:33:46 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define B(x) (1ULL << (x))

static int shift_rule(uint64_t state, int rule_i) {
	if (state % 2) {
		++rule_i;
		if (rule_i == 5) {
			rule_i = 0;
		}
	} else {
		--rule_i;
		if (rule_i == -1) {
			rule_i = 4;
		}
	}
	return rule_i;
}

static uint64_t generate(uint64_t state, int rule_i) {
	static uint16_t rules[5] = {39318, 42390, 43350, 51510, 57630};
	uint64_t new_state = 0;
	
	for (uint64_t i = 0; i < 64; ++i) {
		if (rules[rule_i] & B(0b1111 & ((state >> (i - 1)) | (state << (63 - i)) | (state << (64 - i))))) {
			new_state |= B(i);
		}
	}
	return new_state;
}

static uint64_t to_block_padding(const char *str_block, size_t padding) {
	uint64_t block = 0;
	size_t i = 0;
	
	while (i < padding) {
		block |= (uint64_t)str_block[i] << (i * 8);
		++i;
	}
	while (i < 8) {
		block |= (uint64_t)'#' << (i * 8);
		++i;
	}
	return block;
}

static uint64_t to_block(const char *str_block) {
	uint64_t block = 0;
	
	for (size_t i = 0; i < 8; ++i) {
		block |= (uint64_t)str_block[i] << (i * 8);
	}
	return block;
}

uint64_t generate_hash(const char *passphrase) {
	const size_t len = strlen(passphrase);
	const size_t nb_blocks = len / 8;
	const size_t padding = len % 8;
	
	uint64_t block;
	uint64_t hash = 0;
	int rule_i = nb_blocks % 5;
	
	for (size_t i = 0; i < nb_blocks; ++i) {
		block = to_block(passphrase);
		passphrase += 8;
		hash ^= generate(block, rule_i);
		for (size_t j = 0; j < 8; ++j) {
			hash ^= generate(hash, rule_i);
		}
		rule_i = shift_rule(block, rule_i);
	}
	if (padding) {
		block = to_block_padding(passphrase, padding);
		hash ^= generate(block, rule_i);
		for (size_t j = 0; j < 8; ++j) {
			hash ^= generate(hash, rule_i);
		}
	}
	return hash;
}

#include <unistd.h>
static void write_state(uint64_t state) {
	static const char square[6] = {0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88};

	for (int i = 0; i < 64; i++) {
		if (state & B(i)) {
			write(1, square, 6);
		} else {
			write(1, "  ", 2);
		}
	}	
	write(1, "\n", 1);
}

int main (int argc, char *argv[]) {
	if (argc != 2) {
		return 1;
	}
	write_state(generate_hash(argv[1]));
}