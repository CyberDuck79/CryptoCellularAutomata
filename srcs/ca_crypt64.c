/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ca_crypt64.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 16:14:09 by fhenrion          #+#    #+#             */
/*   Updated: 2021/04/11 15:02:56 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/ca64.h"
#include "../hdrs/args.h"
#include "../hdrs/progress_bar.h"

static int	decryption(file_t *file, uint64_t state, int rule_i) {
	uint64_t block[BLOCK_SIZE];
	uint64_t prev_state;
	ssize_t	read_size;
	uint64_t prev_block;

	for (size_t i = 0; i < 64; ++i) {
		prev_state = state;
		state = generate(state, rule_i);
		rule_i = prev_state % 5;
	}
	while ((read_size = read(file->read, block, BUFFER_SIZE)) > 0) {
		for (ssize_t i = 0; i < read_size / 8; ++i) {
			prev_state = state;
			prev_block = block[i];
			block[i] ^= state;
			state = generate(prev_block, rule_i);
			rule_i = prev_state % 5;
		}
		if (write(file->write, block, read_size) != read_size) {
			read_size = -1;
			break;
		}
		ft_progress(file->size);
	}
	ft_progress_end();
	close(file->read);
	close(file->write);
	return read_size < 0 ? 1 : 0;
}

static int	encryption(file_t *file, uint64_t state, int rule_i) {
	uint64_t block[BLOCK_SIZE];
	uint64_t prev_state;
	ssize_t	read_size;

	for (size_t i = 0; i < 64; ++i) {
		prev_state = state;
		state = generate(state, rule_i);
		rule_i = prev_state % 5;
	}
	while ((read_size = read(file->read, block, BUFFER_SIZE)) > 0) {
		for (ssize_t i = 0; i < read_size / 8; ++i) {
			prev_state = state;
			block[i] ^= state;
			state = generate(block[i], rule_i);
			rule_i = prev_state % 5;
		}
		if (write(file->write, block, read_size) != read_size) {
			read_size = -1;
			break;
		}
		ft_progress(file->size);
	}
	ft_progress_end();
	close(file->read);
	close(file->write);
	return read_size < 0 ? 1 : 0;
}

static void	open_files(file_t *file, const char *input_name, const char *output_name) {
	file->read = open(input_name, O_RDONLY);
	if (file->read == ERROR) {
		perror(input_name);
		close(file->read);
		exit(1);
	}

	file->write = open(output_name, O_WRONLY | O_CREAT, 0644);
	if (file->write == ERROR) {
		perror(output_name);
		close(file->read);
		close(file->write);
		exit(1);
	}
	
	file->size = lseek(file->read, 0L, SEEK_END) / BUFFER_SIZE;
	if (!file->size) {
		file->size++;
	}
	lseek(file->read, 0L, SEEK_SET);
}

int main(int ac, char **av) {
	if (ac != 5) {
		write(1, "USAGE: -e(encrypt)/-d(decrypt) \"passphrase\" file nonce\n", 55);
		return 1;
	}

	const int option_flag = parse_option(av[1]);
	if (option_flag < 0) {
		write(2, "Error: bad option flag\n", 23);
		return 1;
	} else if (!option_flag) {
		if (strncmp(".ca", av[3] + strlen(av[3]) - 3, 3)) {
			write(2, "Error: no .ca extension\n", 24);
			return 1;
		}
	}
	
	const char *output_name = output_filename(option_flag, av[3]);
	if (!output_name) {
		write(2, "Error: memory allocation\n", 25);
		return 1;
	}
	
	if (check_nonce(av[4])) {
		write(2, "Error: invalid nonce\n", 21);
		return 1;
	}
	const uint64_t seed_state = generate_hash(av[2]) ^ get_nonce(av[4]);

	file_t file;
	open_files(&file, av[3], output_name);
	printf("%s -> %s\n", av[3], output_name);
	free((void*)output_name);

	if (option_flag) {
		if (encryption(&file, seed_state, strlen(av[2]) % 5)) {
			write(2, "Error: io files error\n", 22);
			close(file.read);
			close(file.write);
			return 1;
		}
	} else {
		if (decryption(&file, seed_state, strlen(av[2]) % 5)) {
			write(2, "Error: io files error\n", 22);
			close(file.read);
			close(file.write);
			return 1;
		}
	}
	
	return (0);
}
