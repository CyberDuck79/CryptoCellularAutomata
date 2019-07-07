#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
 
typedef unsigned long long ull;
// Nombre de bits d'un octet * le nombre d'octets d'un unsigned long long.
#define N  (sizeof(ull) * CHAR_BIT)
// Masque x sur un unsigned long long.
#define B(x) (1ULL << (x))
 
void nb_evolve(ull state, int rule, int nb_gen)
{
	int     i;
	ull     st;
    char    rules[8] = {30, 135, 86, 149, 45, 101, 75, 89};

    while (nb_gen--)
	{
		st = state;
        for (i = N; i; i--)
            putchar(st & B(i) ? '#' : ' ');
		putchar('\n');
        state = 0;
		for (i = 0; i < N; i++)
			if (rules[rule] & B(7 & (st>>(i-1) | st<<(N+1-i))))
				state |= B(i);
        if (++rule > 8)
            rule = 0;
	}
}

ull evolve(ull state, int rule)
{
	int     i;
	ull     st;

	st = state;
	for (i = N; i; i--)
		putchar(st & B(i) ? '1' : '0');
	putchar('\n');
	state = 0;
	for (i = 0; i < N; i++)
		if (rule & B(7 & (st>>(i-1) | st<<(N+1-i))))
			state |= B(i);
	for (i = N; i; i--)
		putchar(state & B(i) ? '1' : '0');
	putchar('\n');
	return state;
}

void proto_evolve(ull state, int *perm_tab, ull nb_gen)
{
	int     i;
	int		rule;
	int		perm_i;
	ull     st;
    char    rules[8] = {30, 135, 86, 149, 45, 101, 75, 89};

	perm_i = 0;
	rule = perm_tab[perm_i];
    while (nb_gen--)
	{
		st = state;
        for (i = N; i; i--)
            putchar(st & B(i) ? '1' : '0');
		putchar('\n');
        state = 0;
		for (i = 0; i < N; i++)
			if (rules[rule] & B(7 & (st>>(i-1) | st<<(N+1-i))))
				state |= B(i);
        if (++perm_i > 15)
            perm_i = 0;
		if ((rule += perm_tab[perm_i]) > 7)
			rule -= 8;
	}
}

// TODO pour le chiffrement :
// - lire un fd ull par ull
// - appliquer l'automate sur des blocs de taille ull.

int main(int argc, char **argv)
{
	
	if (argc == 3)
	{
		int permutation_tab[16] = {2, 3, 2, 7, 4, 1, 6, 3, 2, 7, 4, 5, 2, 3, 7, 1};
		int *perm_ptr = permutation_tab;
		ull input = strtoull(argv[1], NULL, 10);
		ull nb_gen = strtoull(argv[2], NULL, 10);
		proto_evolve(input, perm_ptr, nb_gen);
	}
	else
	{
		puts("USAGE : 1) seed state 2) number of generation.");
	}
	// nb_evolve(2343344589778499735, 0, 600);
	// {30, 135, 86, 149, 45, 101, 75, 89}
	// ull tmp;
	// tmp = evolve(2343344589778499735, 30);
	// evolve(tmp, 135);
	return 0;
}
/*
int main(int argc, char **argv)
{
	char buffer[16];
	int permutation_tab[16] = {2, 3, 2, 7, 4, 1, 6, 3, 2, 7, 4, 5, 2, 3, 7, 1};
	int *perm_ptr = permutation_tab;
	ull input = strtoull(argv[2], NULL, 10);
	int fd = open(argv[1], O_RDONLY);
	FILE *fp;
	fp=fopen("crypt.ca","w");

	while (read(fd, buffer, 16))
	{
		input = crypt_evolve(input, perm_ptr, buffer);
		fputs(buffer, fp);
	}
	
	
	return 0;
}
*/