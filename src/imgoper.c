#include "imgoper.h"

#include <stdio.h>
#include <string.h>

#include "convol.h"
#include "median.h"
#include "pgmfil.h"
#include "matrix.h"
#include "filter.h"
#include "hist.h"
#include "edge.h"

void mkOpName(char *opNam, const char *opStr, const char *ipNam, size_t strSiz){
    const char *iPtr = ipNam;
    char *oPtr = opNam;
    size_t cnt = 0;
    while( ((*iPtr != '\0') && (*iPtr != '.')) && (cnt < strSiz - 5)){
        *oPtr = *iPtr;
        ++iPtr;
        ++oPtr;
        ++cnt;
    }
    const char *kPtr = opStr;
    while( (*kPtr != '\0') && (cnt < strSiz - 5)){
        *oPtr = *kPtr;
        ++kPtr;
        ++oPtr;
        ++cnt;
    }
    *oPtr = FILTER_SIZ + '0';
    ++oPtr;
    *oPtr = '.';
    ++oPtr;
    *oPtr = 'p';
    ++oPtr;
    *oPtr = 'g';
    ++oPtr;
    *oPtr = 'm';
    ++oPtr;
    *oPtr = 0;
}

void doBinConv(const char* ipFilNam, size_t n){
    mat *imgMat;
    size_t row, col, maxVal;
    PGMType type;

    pgmToImgMat(ipFilNam, &imgMat, &row, &col, &maxVal, &type);

    mat *biConv = allocMatMem(row, col);
    binomFilConvolN(biConv, imgMat, row, col, n);

    char convOpFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(convOpFilNam, "BiConv", ipFilNam, sizeof (convOpFilNam));
    imgMatToPgm(convOpFilNam, biConv, row, col, maxVal, type);
    free(imgMat);
    free(biConv);
}

void doScharrEdgeDet(const char* ipFilNam, size_t gradTh){
    mat *imgMat;
    size_t row, col, maxVal;
    PGMType type;

    pgmToImgMat(ipFilNam, &imgMat, &row, &col, &maxVal, &type);

    mat *xComp = allocMatMem(row, col);
    mat *yComp = allocMatMem(row, col);
    mat *grad = allocMatMem(row, col);

    scharrEdgeDet(imgMat, xComp, yComp, grad, gradTh, row, col);

    char convXOpFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(convXOpFilNam, "ScharrX", ipFilNam, sizeof (convXOpFilNam));
    imgMatToPgm(convXOpFilNam, xComp, row, col, maxVal, type);

    char convOpYFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(convOpYFilNam, "ScharrY", ipFilNam, sizeof (convOpYFilNam));
    imgMatToPgm(convOpYFilNam, yComp, row, col, maxVal, type);

    char convOpGFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(convOpGFilNam, "ScharrG", ipFilNam, sizeof (convOpGFilNam));
    imgMatToPgm(convOpGFilNam, grad, row, col, maxVal, type);

    free(imgMat);
    free(xComp);
    free(yComp);
}

void doSobelEdgeDet(const char* ipFilNam, size_t gradTh){
    mat *imgMat;
    size_t row, col, maxVal;
    PGMType type;

    pgmToImgMat(ipFilNam, &imgMat, &row, &col, &maxVal, &type);

    mat *xComp = allocMatMem(row, col);
    mat *yComp = allocMatMem(row, col);
    mat *grad = allocMatMem(row, col);

    sobelEdgeDet(imgMat, xComp, yComp, grad, gradTh, row, col);
    free(imgMat);

    char convXOpFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(convXOpFilNam, "SobelX", ipFilNam, sizeof (convXOpFilNam));
    imgMatToPgm(convXOpFilNam, xComp, row, col, maxVal, type);
    free(xComp);

    char convOpYFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(convOpYFilNam, "SobelY", ipFilNam, sizeof (convOpYFilNam));
    imgMatToPgm(convOpYFilNam, yComp, row, col, maxVal, type);
    free(yComp);

    char convOpGFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(convOpGFilNam, "SobelG", ipFilNam, sizeof (convOpGFilNam));
    imgMatToPgm(convOpGFilNam, grad, row, col, maxVal, type);
    free(grad);
}

void doCannyEdgeDet(const char* ipFilNam){
    mat *imgMat;
    size_t row, col, maxVal;
    PGMType type;

    pgmToImgMat(ipFilNam, &imgMat, &row, &col, &maxVal, &type);
    mat *filMat = allocMatMem(row, col);

    mat *xComp = allocMatMem(row, col);
    mat *yComp = allocMatMem(row, col);
    mat *gradThComp = allocMatMem(row, col);

    mat *nmsMat = allocMatMem(row, col);

    mat *hysThMat = allocMatMem(row, col);

    mat *edgeImg = allocMatMem(row, col);

    cannyEdgeDet(imgMat, filMat, xComp, yComp, gradThComp, nmsMat, hysThMat, edgeImg, row, col);
    free(imgMat);

    char cannyFilOpFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(cannyFilOpFilNam, "CannyFil", ipFilNam, sizeof (cannyFilOpFilNam));
    imgMatToPgm(cannyFilOpFilNam, filMat, row, col, maxVal, type);
    free(filMat);

    char convXOpFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(convXOpFilNam, "CannyX", ipFilNam, sizeof (convXOpFilNam));
    imgMatToPgm(convXOpFilNam, xComp, row, col, maxVal, type);
    free(xComp);

    char convOpYFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(convOpYFilNam, "CannyY", ipFilNam, sizeof (convOpYFilNam));
    imgMatToPgm(convOpYFilNam, yComp, row, col, maxVal, type);
    free(yComp);

    char convOpGFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(convOpGFilNam, "CannyG", ipFilNam, sizeof (convOpGFilNam));
    imgMatToPgm(convOpGFilNam, gradThComp, row, col, maxVal, type);
    free(gradThComp);

    char nmsOpFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(nmsOpFilNam, "CannyNMS", ipFilNam, sizeof (nmsOpFilNam));
    imgMatToPgm(nmsOpFilNam, nmsMat, row, col, maxVal, type);
    free(nmsMat);

    char hysThOpFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(hysThOpFilNam, "CannyHysTh", ipFilNam, sizeof (hysThOpFilNam));
    imgMatToPgm(hysThOpFilNam, hysThMat, row, col, maxVal, type);
    free(hysThMat);

    char cannyOpFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(cannyOpFilNam, "Canny", ipFilNam, sizeof (cannyOpFilNam));
    imgMatToPgm(cannyOpFilNam, edgeImg, row, col, maxVal, type);
    free(edgeImg);
}

void doMedianFilter(const char* ipFilNam, size_t n){
    mat *imgMat;
    size_t row, col, maxVal;
    PGMType type;

    pgmToImgMat(ipFilNam, &imgMat, &row, &col, &maxVal, &type);

    mat *med = allocMatMem(row, col);
    medianN(med, imgMat, row, col, n);

    char medOpFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(medOpFilNam, "Med", ipFilNam, sizeof (medOpFilNam));
    imgMatToPgm(medOpFilNam, med, row, col, maxVal, type);
    free(imgMat);
    free(med);
}

void doHistStch(const char* ipFilNam){
    mat *imgMat;
    size_t row, col, maxVal;
    PGMType type;

    pgmToImgMat(ipFilNam, &imgMat, &row, &col, &maxVal, &type);

    mat *histStch = allocMatMem(row, col);
    stretchHist(histStch, imgMat, row, col);

    char histStchOpFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(histStchOpFilNam, "HistStch", ipFilNam, sizeof (histStchOpFilNam));
    imgMatToPgm(histStchOpFilNam, histStch, row, col, maxVal, type);
    free(imgMat);
    free(histStch);
}

void doHistEq(const char* ipFilNam){
    mat *imgMat;
    size_t row, col, maxVal;
    PGMType type;

    pgmToImgMat(ipFilNam, &imgMat, &row, &col, &maxVal, &type);

    mat *histEq = allocMatMem(row, col);
    eqHist(histEq, imgMat, row, col);

    char histEqOpFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(histEqOpFilNam, "HistEq", ipFilNam, sizeof (histEqOpFilNam));
    imgMatToPgm(histEqOpFilNam, histEq, row, col, maxVal, type);
    free(imgMat);
    free(histEq);
}

void doAll(const char *ipFilNam, size_t n){
    doBinConv(ipFilNam, n);
    doMedianFilter(ipFilNam, n);
    doHistStch(ipFilNam);
    doHistEq(ipFilNam);
    doScharrEdgeDet(ipFilNam, 0);
    doCannyEdgeDet(ipFilNam);
}
