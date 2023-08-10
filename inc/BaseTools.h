#ifndef OPENTRS_BASETOOLS_H
#define OPENTRS_BASETOOLS_H

#pragma once
#include <stdint.h>
#include <vector>
typedef struct MaxCorrStrcut
{
    float corr = 0;
    int point = 0;
    int key = 0;
};

enum COMPARE_MODE
{
    DESCENDING_SORT = 0,
    ASCENDING_SORT = 1
};
static bool descendingOrder(const MaxCorrStrcut a, const MaxCorrStrcut b) { return a.corr < b.corr; };//降序
static bool ascendingOrder(const MaxCorrStrcut a, const MaxCorrStrcut b) { return a.corr > b.corr; };//升序

class BaseTools
{
public:

    int hanmingWeight(const uint8_t data);
    int hanmingWeight(const uint16_t data);
    int hanmingWeight(const uint32_t data);
    int hanmingWeight(const uint64_t data);
    static int hanmingWeight(const int32_t data);
    static double correlate(double* src1, double* src2, int len);
    static double correlate(float* src1, float* src2, int len);
    static double correlate(int* src1, float* src2, int len);
    // void corrSort(MaxCorrStrcut* maxCorr, COMPARE_MODE sortMode, int size);
};


#endif //OPENTRS_BASETOOLS_H
