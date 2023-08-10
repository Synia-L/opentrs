
#include "../inc/BaseTools.h"
#include <cmath>
#include <algorithm>
using namespace std;

int BaseTools::hanmingWeight(uint8_t data)
{
    int n = 0;
    for (int j = 0; j < 8; j++)
        n += (data >> (7 - j)) & 0x1;
    return n;
}

int BaseTools::hanmingWeight(uint16_t data)
{
    int n = 0;
    for (int j = 0; j < 16; j++)
        n += (data >> (15 - j)) & 0x1;
    return n;
}

int BaseTools::hanmingWeight(uint32_t data)
{
    int n = 0;
    for (int j = 0; j < 32; j++)
        n += (data >> (31 - j)) & 0x1;
    return n;
}


int BaseTools::hanmingWeight(uint64_t data)
{
    int n = 0;
    for (int j = 0; j < 64; j++)
        n += (data >> (63 - j)) & 0x1;
    return n;
}

int BaseTools::hanmingWeight(int32_t data)
{
    int n = 0;
    for (int j = 0; j < 32; j++)
        n += (data >> (31 - j)) & 0x1;
    return n;
}


double BaseTools::correlate(double* src1, double* src2, int len)
{
    double sum_src1 = 0;
    double sum_src2 = 0;
    double ave_src1 = 0;
    double ave_src2 = 0;
    double deviation_src1 = 0;
    double deviation_src2 = 0;
    double cov_xy = 0;
    for (int i = 0; i < len; i++)
    {
        sum_src1 += src1[i];
        sum_src2 += src2[i];
    }
    ave_src1 = sum_src1 / len;
    ave_src2 = sum_src2 / len;
    // ����src1�ķ������src2�ķ���
    for (int i = 0; i < len; i++) {
        deviation_src2 += (src2[i] - ave_src2) * (src2[i] - ave_src2);
        deviation_src1 += (src1[i] - ave_src1) * (src1[i] - ave_src1);
        cov_xy += (src2[i] - ave_src2) * (src1[i] - ave_src1);
    }
    deviation_src1 = (std::sqrt(deviation_src1));
    deviation_src2 = (std::sqrt(deviation_src2));
    if (0 == deviation_src1 * deviation_src2)
    {
        return 0;
    }
    else
    {
        return cov_xy / (deviation_src1 * deviation_src2);
    }
}

double BaseTools::correlate(int* src1, float* src2, int len)
{
    double sum_src1 = 0;
    double sum_src2 = 0;
    double ave_src1 = 0;
    double ave_src2 = 0;
    double deviation_src1 = 0;
    double deviation_src2 = 0;
    double cov_xy = 0;
    for (int i = 0; i < len; i++)
    {
        sum_src1 += src1[i];
        sum_src2 += src2[i];
    }
    ave_src1 = sum_src1 / len;
    ave_src2 = sum_src2 / len;
    // ����src1�ķ������src2�ķ���
    for (int i = 0; i < len; i++) {
        deviation_src2 += (src2[i] - ave_src2) * (src2[i] - ave_src2);
        deviation_src1 += (src1[i] - ave_src1) * (src1[i] - ave_src1);
        cov_xy += (src2[i] - ave_src2) * (src1[i] - ave_src1);
    }
    deviation_src1 = (std::sqrt(deviation_src1));
    deviation_src2 = (std::sqrt(deviation_src2));
    if (0 == (deviation_src1 * deviation_src2))
    {
        return 0;
    }
    else
    {
        return cov_xy / (deviation_src1 * deviation_src2);
    }
}

double BaseTools::correlate(float* src1, float* src2, int len)
{
    double sum_src1 = 0;
    double sum_src2 = 0;
    double ave_src1 = 0;
    double ave_src2 = 0;
    double deviation_src1 = 0;
    double deviation_src2 = 0;
    double cov_xy = 0;
    for (int i = 0; i < len; i++)
    {
        sum_src1 += src1[i];
        sum_src2 += src2[i];
    }
    ave_src1 = sum_src1 / len;
    ave_src2 = sum_src2 / len;
    // ����src1�ķ������src2�ķ���
    for (int i = 0; i < len; i++) {
        deviation_src2 += (src2[i] - ave_src2) * (src2[i] - ave_src2);
        deviation_src1 += (src1[i] - ave_src1) * (src1[i] - ave_src1);
        cov_xy += (src2[i] - ave_src2) * (src1[i] - ave_src1);
    }
    deviation_src1 = (std::sqrt(deviation_src1));
    deviation_src2 = (std::sqrt(deviation_src2));
    if (0 == deviation_src1 * deviation_src2)
    {
        return 0;
    }
    else
    {
        return cov_xy / (deviation_src1 * deviation_src2);
    }
}

// void BaseTools::corrSort(MaxCorrStrcut* maxCorr, COMPARE_MODE sortMode, int size)
// {
// 	if (sortMode == DESCENDING_SORT)
// 	{
// 		//排序
// 		sort(maxCorr, maxCorr + size,descendingOrder);//降序
// 	}
// 	else
// 	{
// 		//排序
// 		sort(maxCorr, maxCorr + size,ascendingOrder);//降序
// 	}

// }