/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ca64.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 15:19:56 by fhenrion          #+#    #+#             */
/*   Updated: 2021/03/21 20:38:57 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/ca64.h"

void write_state(const uint64_t state) {
	static const char square[6] = {0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88};

	for (uint64_t i = 0; i < 64; ++i) {
		if (state & B(i)) {
			write(1, square, 6);
		} else {
			write(1, "  ", 2);
		}
	}
	write(1, "\n", 1);
}

int shift_rule(const uint64_t state, int rule_i) {
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

uint64_t generate(const uint64_t state, const int rule_i) {
	static uint16_t rules[5] = {39318, 42390, 43350, 51510, 57630};
	uint64_t new_state = 0;

	for (uint64_t i = 0; i < 64; ++i) {
		if (rules[rule_i] & B(0b1111 & ((state >> (i - 1)) | (state << (63 - i)) | (state << (64 - i))))) {
			new_state |= B(i);
		}
	}
	//write_state(state); // -> output for tests
	return new_state;
}