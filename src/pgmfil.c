#include "pgmfil.h"
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "matrix.h"

// to do:
// current max pix value -> max possible pix value
// max pix value, one that's read from file
// filter size not hardcoded
// clean up menu
// option to enable or disable intermediates
// check why name not appended with ./dir/img.pgm or is it always
// check processing with p2


int fileGetC(FILE *fileH){
    int ich = getc( fileH );
    assert(ich != EOF );
    return ich;
}

// get char skipping comments
char pm_getc(FILE* file){
    int ich;
    char ch;

    ich = fileGetC(file);
    ch = (char) ich;

    if ( ch == '#' ){
        do{
            ich = fileGetC(file);
            ch = (char) ich;
        } while ( ch != '\n' && ch != '\r' ); // skip the comment and yield newline char
    }
    return ch;
}

// p1 -> p4
bit pm_getbit(FILE* file){
    char ch;

    do {
        ch = pm_getc( file );
    } while ( ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' ); // trying to find 1st non-whitespace char

    switch (ch) {
    case '0':
        return 0;
    case '1':
        return 1;
    default:
        assert(0);
    }
}


unsigned char pm_getrawbyte(FILE* file){
    return  (unsigned char) fileGetC(file);
}

int pm_getint( FILE* file){
    char ch;
    do {
        ch = pm_getc( file );
    } while ( ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' );// get 1st non-whitespace char

    if ( ch < '0' || ch > '9' ){ // check if it's valid ascii dec
        assert(0);
    }

    int i = 0;
    do {
        i = i * 10 + ch - '0';
        ch = pm_getc( file );
    } while ( ch >= '0' && ch <= '9' ); // until next whitespace convert chars in ascii decimal to numbers

    return i;
}

gray *allocImgMem(size_t r, size_t c){
    ABOVE_ZERO_CHK(r);
    ABOVE_ZERO_CHK(c);

    return malloc(r * c * sizeof (gray));
}

void imgMatToGrayMap(gray *gMap, mat *matr, size_t row, size_t col){
    for (size_t i = 0; i < row; ++i) {
        for (size_t j = 0; j < col; ++j) {
            gray gVal;
            mat mVal = matr[(i * col) + j ];
            if(mVal < 0){
                gVal = 0;
            } else {
                if(mVal > MAX_PIX_VAL){
                    gVal = (gray) MAX_PIX_VAL;
                } else {
                    gVal = (gray) mVal;
                }
            }
            gMap[(i * col) + j] = gVal;
        }
    }
}

void grayMapToImgMat(mat *matr, gray *gMap, size_t row, size_t col){
    for (size_t i = 0; i < row; ++i) {
        for (size_t j = 0; j < col; ++j) {
            matr[(i * col) + j] = (mat) gMap[(i * col) + j ];
        }
    }
}

void pgmToImgMat(const char *filNam, mat **imgMat, size_t *row, size_t *col, size_t *maxVal, PGMType *type){
    NULL_PTR_CHK(filNam);
    NULL_PTR_CHK(imgMat);
    NULL_PTR_CHK(row);
    NULL_PTR_CHK(col);
    NULL_PTR_CHK(type);

    FILE* ifp = fopen(filNam, "r");
    NULL_PTR_CHK(ifp);

    fileGetC( ifp );
    int ich2 = fileGetC( ifp );

    switch (ich2) {
    case '2':
        *type = eP2;
        break;
    case '5':
        *type = eP5;
        break;
    default:
        assert(0);
        break;
    }

    *col = (unsigned)pm_getint( ifp );
    *row = (unsigned)pm_getint( ifp );
    *maxVal = (unsigned)pm_getint( ifp );

    gray *grayMap = allocImgMem(*row, *col);

    for(size_t i=0; i < *row; ++i){
        for(size_t j=0; j < *col; ++j){
            gray val;
            if(*type == eP5){
                val = pm_getrawbyte(ifp);
            } else {
                val = (gray)pm_getint(ifp) - '0';
            }
            grayMap[(i * (*col)) + j] = val;
        }
    }
    fclose(ifp);

    *imgMat = allocMatMem(*row, *col);
    grayMapToImgMat(*imgMat, grayMap, *row, *col);
    free(grayMap);
}

void imgMatToPgm(const char *filNam, mat *imgMat, size_t row, size_t col, size_t maxVal, PGMType type){
    FILE* ifp = fopen(filNam, "w");
    NULL_PTR_CHK(ifp);

    if(type == eP5){
        fprintf(ifp, "P5\n");
    } else {
        fprintf(ifp, "P2\n");
    }
    fprintf(ifp, "%d %d\n", (signed)col, (signed)row);
    fprintf(ifp, "%d\n", (signed)maxVal);

    gray *grayMap = allocImgMem(row, col);
    imgMatToGrayMap(grayMap, imgMat, row, col);
    for(size_t i=0; i < row; ++i){
        for(size_t j=0; j < col; ++j){
            gray val = grayMap[(i * col) + j];
            if(type == eP5){
                fprintf(ifp, "%c", val);
            } else {
                fprintf(ifp, "%d ", val + '0');
            }
        }
    }
    free(grayMap);
    fclose(ifp);
}
