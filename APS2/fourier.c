#include <math.h>

#include "fourier.h"

void normalize(complex s[MAX_SIZE], int n) {
    for (int k = 0; k < n; k++) {
        s[k].a /= n;
        s[k].b /= n;
    }
}

void nft(complex s[MAX_SIZE], complex t[MAX_SIZE], int n, int sign) {
    for (int k = 0; k < n; k++) {
        t[k].a = 0;
        t[k].b = 0;

        for (int j = 0; j < n; j++) {
            double x = sign * 2 * PI * k * j / n;

            double cosx = cos(x);
            double sinx = sin(x);

            t[k].a += s[j].a * cosx - s[j].b * sinx;
            t[k].b += s[j].a * sinx + s[j].b * cosx;
        }
    }
}

void nft_forward(complex s[MAX_SIZE], complex t[MAX_SIZE], int n) {
    nft(s, t, n, -1);
}

void nft_inverse(complex t[MAX_SIZE], complex s[MAX_SIZE], int n) {
    nft(t, s, n, 1);
    normalize(s, n);
}

void fft(complex s[MAX_SIZE], complex t[MAX_SIZE], int n, int sign) {
    if (n == 1) {
        t[0].a = s[0].a;
        t[0].b = s[0].b;
        return;
    }

    complex sp[n / 2];
    complex si[n / 2];

    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            sp[i / 2] = s[i];
        } else {
            si[i / 2] = s[i];
        }
    }

    complex tp[n / 2];
    complex ti[n / 2];

    fft(sp, tp, n / 2, sign);
    fft(si, ti, n / 2, sign);

    for (int k = 0; k < (n / 2); k++) {
        double sinx = sin((2 * sign * PI * k) / n);
        double cosx = cos((2 * sign * PI * k) / n);

        t[k].a = tp[k].a + ti[k].a * cosx - ti[k].b * sinx;
        t[k].b = tp[k].b + ti[k].a * sinx + ti[k].b * cosx;
        t[k + (n / 2)].a = tp[k].a - (ti[k].a * cosx - ti[k].b * sinx);
        t[k + (n / 2)].b = tp[k].b - (ti[k].a * sinx + ti[k].b * cosx);
    }
}

void fft_forward(complex s[MAX_SIZE], complex t[MAX_SIZE], int n) {
    fft(s, t, n, -1);   
}

void fft_inverse(complex t[MAX_SIZE], complex s[MAX_SIZE], int n) {
    fft(t, s, n, 1);
    normalize(s, n);
}

void fft_forward_2d(complex matrix[MAX_SIZE][MAX_SIZE], int width, int height) {
    for (int l=0;l<height;l++){
        complex line[MAX_SIZE];
        for(int c=0;c<width;c++){
            line[c] = matrix[l][c];
        }
        fft_forward(line, line, width);
        for(int c=0;c<width;c++){
            matrix[l][c] = line[c];
        }
    }
    for(int c=0;c<width;c++){
        complex column[MAX_SIZE];
        for(int l=0;l<height;l++){
            column[l] = matrix[l][c];
        }
        fft_forward(column, column, height);
        for(int l=0;l<height;l++){
            matrix[l][c] = column[l];
        }
    }
}

void fft_inverse_2d(complex matrix[MAX_SIZE][MAX_SIZE], int width, int height) {
    for (int l=0;l<height;l++){
        complex line[MAX_SIZE];
        for(int c=0;c<width;c++){
            line[c] = matrix[l][c];
        }
        fft_inverse(line, line, width);
        for(int c=0;c<width;c++){
            matrix[l][c] = line[c];
        }
    }
    for(int c=0;c<width;c++){
        complex column[MAX_SIZE];
        for(int l=0;l<height;l++){
            column[l] = matrix[l][c];
        }
        fft_inverse(column, column, height);
        for(int l=0;l<height;l++){
            matrix[l][c] = column[l];
        }
    }
}

void filter(complex input[MAX_SIZE][MAX_SIZE], complex output[MAX_SIZE][MAX_SIZE], int width, int height, int flip) {
    int center_x = width / 2;
    int center_y = height / 2;
    double variance = -2 * SIGMA * SIGMA;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int dx = center_x - (x + center_x) % width;
            int dy = center_y - (y + center_y) % height;
            double d = dx * dx + dy * dy;
            double g = exp(d / variance);
            if (flip) {
                g = 1 - g;
            }
            output[y][x].a = g * input[y][x].a;
            output[y][x].b = g * input[y][x].b;
        }
    }
}

void filter_lp(complex input[MAX_SIZE][MAX_SIZE], complex output[MAX_SIZE][MAX_SIZE], int width, int height) {
    filter(input, output, width, height, 0);
}

void filter_hp(complex input[MAX_SIZE][MAX_SIZE], complex output[MAX_SIZE][MAX_SIZE], int width, int height) {
    filter(input, output, width, height, 1);
}
