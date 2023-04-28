#include <math.h>
#include <stdio.h>

#include "geometry.h"

#define ERROR_CONST 0.000001

int abs(int n) {
    if (n >= 0) {
        return n;
    } else {
        return -n;
    }
}

int compare(double a, double b) { // 0->equal; 1->a is greater; 2->a is smaller
    double diff = a - b;
    if (diff > ERROR_CONST) {
        return 1;
    } else if (diff < -ERROR_CONST) {
        return 2;
    } else {
        return 0;
    }
}
int verify(point p, point a, point b) {

    double cx, cy; // ponto sobre a reta com o y de p

    int biggest_y;
    int smallest_y;
    int biggest_x;
    int smallest_x;
    int delta_x;
    int delta_y;
    double m; // coeficiente angular
    double n; // coeficiente linear

    if (a.y > b.y) {
        biggest_y = a.y;
        smallest_y = b.y;
    } else {
        smallest_y = a.y;
        biggest_y = b.y;
    }

    if (a.x > b.x) {
        biggest_x = a.x;
        smallest_x = b.x;
    } else {
        smallest_x = a.x;
        biggest_x = b.x;
    }

    delta_y = a.y - b.y;
    delta_x = a.x - b.x;

    cy = p.y;

    if (delta_x == 0) { // se a.x==b.x (para evitar divisão por 0)
        cx = a.x;
    } else if (delta_y == 0) {
        if ((cy == biggest_y) || (cy == smallest_y)) {
            if (p.x > biggest_x) {
                return 1;
            } else if (p.x < smallest_x) {
                return 0;
            } else {
                return 2;
            }
        }
    } else {
        m = (double)delta_y / delta_x;
        n = a.y - m * a.x;

        cx = (cy - n) / m;
    }

    if (a.x == b.x && a.y == b.y && p.x == a.x && p.y == a.y) {
        return 2;
    } // caso p==a==b

    if ((delta_y == 0) && ((p.x == a.x) || (p.x == b.x))) { // perturbação para reta horizontal com p no mesmo y da reta
        return 0;
    }

    if (((compare(cx, p.x) == 2) && ((cy <= biggest_y) && (cy > smallest_y)))) { // p a direita da reta e cy esta entre a e b
        return 1;
    } else if ((compare(cx, p.x) == 0 && (compare(cx, smallest_x) == 1 || compare(cx, smallest_x) == 0) && (compare(cx, biggest_x) == 2 || compare(cx, biggest_x) == 0) && (compare(cy, smallest_y) == 1 || compare(cy, smallest_y) == 0) && (compare(cy, biggest_y) == 2 || compare(cy, biggest_y) == 0))) { // c==p
        return 2;
    } else {
        return 0;
    }
}

int inside(point p, point poly[], int n) {
    int ray_count = 0;

    for (int i = 0; i < n; i++) {
        point a = poly[n];
        point b;
        if (i == n - 1) {
            b = poly[0];
        } else {
            b = poly[n + 1];
        }

        int result = verify(p, a, b);

        if (result == 2) {
            return 1;
        } else {
            ray_count += result;
        }
    }

    if (ray_count % 2 == 0) {
        return 0;
    } else {
        return 1;
    }
}
