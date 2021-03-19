/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CAemulator.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 13:31:09 by fhenrion          #+#    #+#             */
/*   Updated: 2021/03/19 13:23:27 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define B(x) (1ULL << (x))

static void write_state(uint64_t state) {
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

static uint64_t generate(uint64_t state, uint16_t rule) {
	uint64_t new_state = 0;
	(void)rule;
	static uint16_t rules[5] = {39318, 42390, 43350, 51510, 57630};
	static int rule_i = 0;
	for (uint64_t i = 0; i < 64; ++i) {
		if (rules[rule_i] & B(0b1111 & ((state >> (i - 1)) | (state << (63 - i)) | (state << (64 - i))))) {
			new_state |= B(i);
		}
	}
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
	return new_state;
}

uint64_t rand_uint64(void) {
	srand(time(NULL));
	uint64_t r = 0;
	for (int i = 0; i < 64; i += 15) {
		r = r * ((uint64_t)RAND_MAX + 1) + rand();
	}
	return r;
}

int main(int argc, char *argv[]) {
	uint16_t rule;
	long long iterations_nb;
	if (argc < 3) {
		printf("USAGE: rule(0-65535) nb_of_iterations\n");
		return 0;
	} else {
		int input = atoi(argv[1]);
		if (input < 0 || input > 65535) {
			printf("Error: rule must be in range 0-65535.\n");
			return 1;
		}
		rule = (uint16_t)input;
		iterations_nb = atoll(argv[2]);
		if (iterations_nb < 0) {
			printf("Error: iterations must be positive.\n");
			return 1;
		}
	}
	uint64_t seed = rand_uint64();
	write_state(seed);
	for (long long i = 0; i < iterations_nb; ++i) {
		seed = generate(seed, rule);
		write_state(seed);
	}
	return 0;
}
