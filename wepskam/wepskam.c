#include <stdio.h>
#include <unistd.h>
#include <x86intrin.h>
#include <locale.h>
#include <time.h>

static void
mat_set(const int N, float mat[][N], float value) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            mat[i][j] = value;
        }
    }
}

#define N 1000
float mul1[N][N];
float mul2[N][N];
float res[N][N];

int main(int argc, char *argv[]) {
    long l1size = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
    printf("l1size: %ldbytes\n", l1size);

    // NOTE: teach printf to use thousands separator
    setlocale(LC_NUMERIC, "");
    struct lconv *ptrLocale = localeconv();
    ptrLocale->thousands_sep = "'";

    // NOTE naive matrix multiplication

    mat_set(N, mul1, 42);
    mat_set(N, mul2, 13);

    struct timespec started, ended;
    clock_gettime(CLOCK_MONOTONIC, &started);

    long started_cycles = __rdtsc();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                res[i][j] += mul1[i][k] * mul2[k][j];
            }
        }
    }

    long ended_cycles = __rdtsc();
    clock_gettime(CLOCK_MONOTONIC, &ended);

    long elapsed_cycles = ended_cycles - started_cycles;

    printf("Cycles elapsed: %'ld\n", elapsed_cycles);
    double elapsed = (ended.tv_sec * 1e9 + ended.tv_nsec) - (started.tv_sec * 1e9 + started.tv_nsec);
    printf("Time elapsed: %'fs\n", elapsed * 1e-9);
}
