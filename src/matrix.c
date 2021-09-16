#include "matrix.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void chkMatrixValidity(mat *matr, size_t row, size_t col){
    NULL_PTR_CHK(matr);
    ABOVE_ZERO_CHK(row);
    ABOVE_ZERO_CHK(col);
}

void printMatrix(mat *buf, size_t row, size_t col){
    chkMatrixValidity(buf, row, col);

    for (size_t i = 0; i < row; ++i) {
        for (size_t j = 0; j < col; ++j) {
            printf("%d ", buf[(i * col) + j]);
        }
        printf("\n");
    }
    printf("\n");
}

mat *allocMatMem(size_t r, size_t c){
    ABOVE_ZERO_CHK(r);
    ABOVE_ZERO_CHK(c);

    return malloc(r * c * sizeof (mat));
}

void memsetMatrix(mat *matr, int val, size_t row, size_t col){
    chkMatrixValidity(matr, row, col);
    for (size_t i = 0; i < row; ++i) {
        for (size_t j = 0; j < col; ++j) {
            matr[(i * col) + j] = val;
        }
    }
}

void cpyMatrix(mat *dst, mat *src, size_t row, size_t col){
    chkMatrixValidity(src, row, col);
    NULL_PTR_CHK(dst);
    SAME_PTR_CHK(dst, src);
    for (size_t i = 0; i < row; ++i) {
        for (size_t j = 0; j < col; ++j) {
            dst[(i * col) + j] = src[(i * col) + j];
        }
    }
}

void reverseMatrix(mat *revH, mat *h, size_t r, size_t c){
    chkMatrixValidity(h, r, c);
    NULL_PTR_CHK(revH);
    SAME_PTR_CHK(revH, h);

    for (size_t i = 0; i < r; ++i) {
        for (size_t j = 0; j < c; ++j) {
            revH[(i * c) + j] = h[( ( (r - 1) - i ) * c ) + ( ( c - 1 ) - j )];
        }
    }
}

void sumMatrix(mat *x, mat *h, size_t r, size_t c, mat *y){
    chkMatrixValidity(x, r, c);
    NULL_PTR_CHK(h);
    NULL_PTR_CHK(y);
    SAME_PTR_CHK(x, (void *)y);
    SAME_PTR_CHK(h, (void *)y);

    for (size_t i = 0; i < r; ++i) {
        for (size_t j = 0; j < c; ++j) {
            y[((i * c) + j)] = x[(i * c) + j] + h[(i * c) + j];
        }
    }
}

void prodMatrix(mat *x, mat *h, size_t r, size_t c, mat *y){
    chkMatrixValidity(x, r, c);
    NULL_PTR_CHK(h);
    NULL_PTR_CHK(y);
    SAME_PTR_CHK(x, (void *)y);
    SAME_PTR_CHK(h, (void *)y);

    for (size_t i = 0; i < r; ++i) {
        for (size_t j = 0; j < c; ++j) {
            y[((i * c) + j)] = x[(i * c) + j] * h[(i * c) + j];
        }
    }
}

void matrixSqrt(mat *x, size_t r, size_t c, mat *y){
    chkMatrixValidity(x, r, c);
    NULL_PTR_CHK(y);
    SAME_PTR_CHK(x, (void *)y);

    for (size_t i = 0; i < r; ++i) {
        for (size_t j = 0; j < c; ++j) {
            double res = sqrt((double) x[(i * c) + j]);
            y[((i * c) + j)] = (mat) res;
        }
    }
}

void getMagnitudeMat(mat *xComp, mat *yComp, size_t row, size_t col, mat *mag){
    mat *x2Comp = allocMatMem(row, col);
    prodMatrix(xComp, xComp, row, col, x2Comp);
    mat *y2Comp = allocMatMem(row, col);
    prodMatrix(yComp, yComp, row, col, y2Comp);
    mat *sum2 = allocMatMem(row, col);
    sumMatrix(x2Comp, y2Comp, row, col, sum2);
    free(x2Comp);
    free(y2Comp);

    matrixSqrt(sum2, row, col, mag);
    free(sum2);
}

void matrixThresholding(mat *x, size_t r, size_t c, size_t th, mat *y){
    chkMatrixValidity(x, r, c);
    NULL_PTR_CHK(y);
    SAME_PTR_CHK(x, (void *)y);
    assert(th <= (unsigned)MAX_PIX_VAL);
    for (size_t i = 0; i < r; ++i) {
        for (size_t j = 0; j < c; ++j) {
            if( x[(i * c) + j] < (signed)th){
                y[(i * c) + j] = 0;
            } else {
                y[(i * c) + j] = x[(i * c) + j];
            }
        }
    }
}

#define PI 3.14

void matrixArcTan(mat *y, mat *x, size_t r, size_t c, mat *theta){
    chkMatrixValidity(x, r, c);
    NULL_PTR_CHK(y);
    NULL_PTR_CHK(theta);
    SAME_PTR_CHK(x, (void *)theta);
    SAME_PTR_CHK(y, (void *)theta);

    for (size_t i = 0; i < r; ++i) {
        for (size_t j = 0; j < c; ++j) {
            double atanVal = atan2( y[(i * c) + j], x[(i * c) + j] );
            atanVal *= 180;
            atanVal /= PI;
            if(atanVal < 0){
                atanVal += 180;
            }
            theta[((i * c) + j)] = (mat) atanVal;
        }
    }
}

void getDirecMat(mat *xComp, mat *yComp, size_t row, size_t col, mat *direc){
    matrixArcTan(xComp, yComp, row, col, direc);
}

void getMatAEle(mat *matr, size_t col, size_t rowNo, mat *arr){
    for (size_t j = 0; j < col; ++j) {
        arr[j] = matr[ (rowNo * col) + j];
    }
}

void getMatBEle(mat *matr, size_t nbRow, size_t nbCol, size_t colNo, mat *arr){
    for (size_t i = 0; i < nbRow; ++i) {
        arr[i] = matr[ (i * nbCol) + colNo];
    }
}

int multEle(mat *matA, mat *matB, size_t nbEle){
    int sum = 0;
    for (size_t i = 0; i < nbEle; ++i) {
        sum += (matA[i] * matB[i]);
    }
    return sum;
}

void matrixMul(mat *matA, size_t rowA, size_t colA, mat *matB, size_t rowB, size_t colB, mat **matC, size_t *rowC, size_t *colC){
    chkMatrixValidity(matA, rowA, colA);
    chkMatrixValidity(matB, rowB, colB);
    NULL_PTR_CHK(matC);
    NULL_PTR_CHK(rowC);
    NULL_PTR_CHK(colC);
    SAME_PTR_CHK(*matC,(void*)matA);
    SAME_PTR_CHK(*matC,(void*)matB);
    assert(colA == rowB);

    *rowC = rowA;
    *colC = colB;
    *matC = (void *)allocMatMem(rowA, colB);

    mat *p = *matC;

    for (size_t i = 0; i < rowA; ++i) {
        for (size_t j = 0; j < colB; ++j) {
            mat *arrA = allocMatMem(1, colA);
            getMatAEle(matA, colA, i, arrA);

            mat *arrB = allocMatMem(rowB, 1);
            getMatBEle(matB, rowB, colB, j, arrB);

            p[(i * colB) + j] = multEle(arrA, arrB, colA);

            free(arrA);
            free(arrB);
        }
    }
}

mat getMatrixMax(mat *matr, size_t row, size_t col){
    chkMatrixValidity(matr, row, col);
    mat maxVal = 0;
    for (size_t i = 0; i < row; ++i) {
        for (size_t j = 0; j < col; ++j) {
            int val = matr[(i * col) + j];
            if(val > maxVal){
                maxVal = val;
            }
        }
    }
    return maxVal;
}
