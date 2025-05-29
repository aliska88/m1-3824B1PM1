#include <stdio.h>
#include <math.h>
#include <malloc.h>

float sin_(float x, int n, float x_n) {
    if (n == 0)
        return x;
    return -x_n * (x * x) / (4 * n * n + 2 * n);
}
float cos_(float x, int n, float x_n) {
    if (n == 0)
        return 1.0;
    return -x_n * (x * x) / (4 * n * n - 2 * n);
}
float exp_(float x, int n, float x_n) {
    if (n == 0)
        return 1.0;
    return x_n * (x / n);
}
float ln_(float x, int n, float x_n) {
    if (n == 0)
        return x;
    return -x_n * (n * x) / (n + 1);
}

float forvardSum(float x, int n, float(*fptr)(float, int, float)) {
    float sum = 0, prev = 0;
    for (int i = 0; i <= n; i++) {
        prev = fptr(x, i, prev);
        sum += prev;
    }
    return sum;
}
float reverseSum(float x, int n, float(*fptr)(float, int, float)) {
    float sum = 0, prev = 0;
    float* t_arr = (float*)calloc(n + 1, sizeof(float));
    for (int i = 0; i <= n; i++)
        t_arr[i] = prev = fptr(x, i, prev);
    for (int i = n; i != -1; i--)
        sum += t_arr[i];
    free(t_arr);
    return sum;
}
float twinsSum(float x, int n, float(*fptr)(float, int, float)) {
    float sum = 0, prev = 0;
    float* t_arr = (float*)calloc(n + 1, sizeof(float));
    for (int i = 0; i <= n; i++)
        t_arr[i] = prev = fptr(x, i, prev);
    for (int i = 0; i < n / 2 * 2; i += 2)
        sum += t_arr[i] + t_arr[i + 1];
    if (n % 2 != 0)
        sum += t_arr[n];
    return sum;
}

void main() {
    printf("1) Sin(x) \n2) Cos(x) \n3) Exp(x) \n4) Ln(x + 1) \n");
    int func_c = 0;
    float(*fptr)(float, int, float) = NULL;
    double(*eFptr)(double) = NULL;
    scanf_s("%d", &func_c);
    switch (func_c) {
    case 1:
        fptr = sin_;
        eFptr = sin;
        break;
    case 2:
        fptr = cos_;
        eFptr = cos;
        break;
    case 3:
        fptr = exp_;
        eFptr = exp;
        break;
    case 4:
        fptr = ln_;
        eFptr = log1p;
        break;

    default:
        break;
    }

    printf("1) Forvard \n2) Reverce \n3) Twins \n");
    int sum_c = 0;
    scanf_s("%d", &sum_c);

    printf("Enter x: \n");
    float x = 0.0;
    scanf_s("%f", &x);

    printf("Enter max n: \n");
    int n = 0;
    scanf_s("%d", &n);

    for (int i = 1; i <= n; ++i) {
        switch (sum_c) {
        case 1:
            printf("n = %d, err = %f \n", i, fabs(eFptr(x) - forvardSum(x, i, fptr)));
            break;
        case 2:
            printf("n = %d, err = %f \n", i, fabs(eFptr(x) - reverseSum(x, i, fptr)));
            break;
        case 3:
            printf("n = %d, err = %f \n", i, fabs(eFptr(x) - twinsSum(x, i, fptr)));
            break;

        default:
            break;
        }
    }
}