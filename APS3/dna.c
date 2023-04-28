#include <stdio.h>

#include "dna.h"

void load_string(FILE *file, int *p, char *s, int t) {
    if (fscanf(file, "%d", p) != 1) {
        fprintf(stderr, "erro ao ler string do teste %d\n", t);
    }

    char c;
    do {
        c = getc(file);
    } while (c != '\n');

    fgets(s, *p + 1, file);
}

int mlev_w(char a[], int n, char b[], int m, int cache[MAX_SIZE + 1][MAX_SIZE + 1]) {
    if (cache[n][m] != -1) {
        return cache[n][m];
    }
    if (n == 0) {
        cache[n][m] = m;
    } else if (m == 0) {
        cache[n][m] = n;
    } else if (a[n - 1] == b[m - 1]) {
        cache[n][m] = mlev_w(a, n - 1, b, m - 1, cache);
    } else {
        int r1 = 1 + mlev_w(a, n - 1, b, m - 1, cache);
        int r2 = 1 + mlev_w(a, n - 1, b, m, cache);
        int r3 = 1 + mlev_w(a, n, b, m - 1, cache);

        int min_r = r1;
        if (r2 < min_r) {
            min_r = r2;
        }
        if (r3 < min_r) {
            min_r = r3;
        }
        cache[n][m] = min_r;
    }
    return cache[n][m];
}


int mlev(char a[], int n, char b[], int m) {
    int cache[MAX_SIZE + 1][MAX_SIZE + 1];
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            cache[i][j] = -1;
        }
    }
    return mlev_w(a, n, b, m, cache);
}

int dlev(char a[], int n, char b[], int m) {
    int cache[MAX_SIZE + 1][MAX_SIZE + 1];

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            cache[i][j] = -1;
        }
    }

    for (int i = 0; i <= n; i++) {
        cache[i][0] = i;
    }
    for (int j = 0; j <= m; j++) {
        cache[0][j] = j;
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (a[i - 1] == b[j - 1]) {
                cache[i][j] = cache[i - 1][j - 1];
            } else {
                int r1 = cache[i - 1][j - 1] + 1;
                int r2 = cache[i - 1][j] + 1;
                int r3 = cache[i][j - 1] + 1;
                cache[i][j] = r1 < r2 ? r1 : r2;
                cache[i][j] = cache[i][j] < r3 ? cache[i][j] : r3;
            }
        }
    }

    return cache[n][m];
}