#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double scalar_multiply(double **mat1, int col1, double **mat2, int col2, int n) {
    double res = 0;
    int i;
    for (i = 0; i < n; i++) {
        res += mat1[i][col1] * mat2[i][col2];
    }
    return res;
}

double find_norma(double **mat, int col, int n) {
    return sqrt(scalar_multiply(mat, col, mat, col, n));
}

void normalize(double **mat, int col, double norma, int n) {
    int i;
    for (i = 0; i < n; i++) {
        mat[i][col] /= norma;
    }
}
void qr_decomposition(double **a, double **q, double **r, int rows, int cols) {
    double norma, scalar;
    int i, j, k;
    for (i = 0; i < cols; i++){
        for (j = 0; j < i; j++) {
            scalar = scalar_multiply(q, j, a, i, rows);
            r[j][i] = scalar;
            for (k = 0; k < rows; k++) {
                q[k][i] -= q[k][j] * scalar;
            }
        }
        norma = find_norma(q, i, rows);
        r[i][i] = norma;
        normalize(q, i, norma, rows);
    }
}

int main(int argc, char *argv[]) {
    char *in = NULL, *out = NULL;
    int i, j, m, n;
    
    for (i = 1; i < argc; i += 2) {
        if (argv[i][0] == '-' && argv[i][1] == 'i') {
            in = argv[i + 1];
        } else if (argv[i][0] == '-' && argv[i][1] == 'o') {
            out = argv[i + 1];
        }
    }
    
    if (!in || !out) {
        printf("Входной или выходной файл не был введен.\n");
        return 1;
    }
    
    FILE *inf = fopen(in, "r"), *outf = fopen(out, "w");
    if (!inf) {
        printf("Ошибка открытия входного файла.\n");
        return 1;
    }
    if (!outf) {
        printf("Ошибка открытия выходного файла.\n");
        return 1;
    }
    
    fscanf(inf, "%d %d", &m, &n);
    double **Q = (double **)malloc(m * sizeof(double *));
    double **R = (double **)malloc(n * sizeof(double *));
    for (i = 0; i < m; i++) {
        Q[i] = (double *)malloc(n * sizeof(double));
    }
    for (i = 0; i < n; i++) {
        R[i] = (double *)malloc(n * sizeof(double));
    }
    double **Matrix = (double **)malloc(m * sizeof(double *));
    for (i = 0; i < m; i++) {
        Matrix[i] = (double *)malloc(n * sizeof(double));
        for (j = 0; j < n; j++) {
            fscanf(inf, "%lf", &Matrix[i][j]);
            Q[i][j] = Matrix[i][j];
        }
    }
    fclose(inf);

    qr_decomposition(Matrix, Q, R, m, n);
    
    fprintf(outf, "%d %d\n", m, n);
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            fprintf(outf, "%.3lf ", Q[i][j]);
        }
        fprintf(outf, "\n");
    }
    fprintf(outf, "%d %d\n", n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(outf, "%.3lf ", R[i][j]);
        }
        fprintf(outf, "\n");
    }
    fclose(outf);
    
    for (i = 0; i < m; i++) {
        free(Matrix[i]);
        free(Q[i]);
        free(R[i]);
    }
    free(Matrix);
    free(Q);
    free(R);

    return 0;
}
