#ifndef OPENTRS_TRACETOOLS_H
#define OPENTRS_TRACETOOLS_H

#pragma once
#include "Trace.h"
class TraceTools
{
public:
    static Trs addTrace(Trs trs1, Trs trs2);
    static Trs subTrace(Trs trs1, Trs trs2);
    static TrsData meanTrace(Trs trs);
    static void meanTrace(TrsData *outTrsData, TrsHead trsHead, TrsData *TrsData);
    static void trs2txt(const char *trsFileName);
    static void trs2txt(char *trsFileName, int txtMessageLength, int txtCipherLength);

    static void staticAlignment(TrsData *outTrsData, TrsData *baseTrsData, TrsData *srcTrsData, int samplePointNum, int startPoint, int length, int maxAlignLength = 10);
    static void cal_corr(float *baseTrsData, TrsData *srcTrsData, int samplePointNum, int length, TrsHead trsHead);
    static void corr(TrsData *baseTrsData, TrsData *srcTrsData, int samplePointNum, int length, int immp, TrsHead trsHead, const char *fileout);
};


#endif //OPENTRS_TRACETOOLS_H
