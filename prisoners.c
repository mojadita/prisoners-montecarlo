/* prisoners.c -- program to simulate the prisoners problem
 * with montecarlo method.
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: Fri Sep 16 22:14:35 EEST 2022
 * Copyright: (c) 2022 Luis Colorado.  All rights reserved.
 * License: BSD.
 */
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define FLG_TRACE (1 << 0)

int flags = 0;

int experiment(int n)
{
	int *box = calloc(n, sizeof *box),
		 n2  = n >> 1;
	uint64_t *bits = calloc((n + 63) >> 6, sizeof *bits);

	for (unsigned i = 0; i < n; i++) {
		box[i] = i;
	}
	if (flags & FLG_TRACE)
		printf("\n");

	/* random permutation */
	char *sep = "";
	for (int i = 0; i < n; i++) {
		int j = i + random() % (n - i);
		if (i != j) {
			int temp = box[i];
			box[i] = box[j];
			box[j] = temp;
		}
		if (flags & FLG_TRACE) {
			printf("%s%d->%d", sep, i+1, box[i]+1);
			sep = ", ";
		}
	}
	if (flags & FLG_TRACE)
		printf("\n");

	/* the experiment */
	int ret_val = 0;
	for (int i = 0; i < n; i++) {
		if (bits[i >> 6] & (1UL << (i & 63)))
			continue;
		int k = 1;
		if (flags & FLG_TRACE)
			printf("(%d", i+1);
		for (int j = box[i]; j != i; j = box[j], k++) {
			bits[j >> 6] |= (1UL << (j & 63));
			if (flags & FLG_TRACE)
				printf(", %d", j+1);
		}
		if (flags & FLG_TRACE)
			printf(")%s\n", k <= n2 ? "*" : "?");
		if (k <= n2)
			ret_val += k;
	}
	free(box);
	free(bits);
	return ret_val;
}

int main(int argc, char **argv)
{
	int N    = 100,
		ITER =   1,
		opt;

	while ((opt = getopt(argc, argv, "di:n:")) != EOF) {
		switch (opt) {
		case 'n': N      = atoi(optarg); break;
		case 'i': ITER   = atoi(optarg); break;
		case 'd': flags |= FLG_TRACE;    break;
		}
	} /* while */

	srandomdev();
	int N2    = N >> 1,
		num   =      0,
		denom =   ITER,
		i;
	printf("Calc. %d times\n", ITER);
	for (i = 0; i < denom; i++)
		if (experiment(N) == N)
			num++;

	printf("Estim. Montecarlo: %d/%d ~= %0.10lf\n",
			num, denom, (double)num / denom);

	/* calculation of 1 - sum(i; n2; n; 1/i) */

	double prob = 1.0;
	for (int i = N2 + 1; i <= N; i++)
		prob -= 1.0 / i;

	printf("  Calculated prob: %0.10lf\n", prob);
} /* main */
