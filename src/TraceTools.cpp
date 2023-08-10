#include "../inc/TraceTools.h"
#include "../inc/BaseTools.h"
#include <cstring>
using namespace std;

Trs TraceTools::addTrace(Trs trs1, Trs trs2)
{
    Trs resultTrsData;
    resultTrsData.trsHead = trs1.trsHead;
    resultTrsData.trsData = new TrsData[resultTrsData.trsHead.NT];
    for (int i = 0; i < trs1.trsHead.NT; i++)
    {
        for (int j = 0; j < trs1.trsHead.DS; j++)
        {
            resultTrsData.trsData[i].data[j] = trs1.trsData[i].data[j] + trs2.trsData[i].data[j];
        }
        for (int j = 0; j < trs1.trsHead.NS; j++)
        {
            resultTrsData.trsData[i].samples[j] = (float)(trs1.trsData[i].samples[j] + trs2.trsData[i].samples[j]);
        }
    }
    return resultTrsData;
}

Trs TraceTools::subTrace(Trs trs1, Trs trs2)
{
    Trs resultTrsData;
    resultTrsData.trsHead = trs1.trsHead;
    resultTrsData.trsData = new TrsData[resultTrsData.trsHead.NT];
    for (int i = 0; i < trs1.trsHead.NT; i++)
    {
        for (int j = 0; j < trs1.trsHead.DS; j++)
        {
            resultTrsData.trsData[i].data[j] = trs1.trsData[i].data[j] - trs2.trsData[i].data[j];
        }
        for (int j = 0; j < trs1.trsHead.NS; j++)
        {
            resultTrsData.trsData[i].samples[j] = trs1.trsData[i].samples[j] - trs2.trsData[i].samples[j];
        }
    }
    return resultTrsData;
}

TrsData TraceTools::meanTrace(Trs trs)
{
    float sum = 0;
    TrsData trsData;
    trsData.data = new uint8_t[trs.trsHead.DS];
    trsData.samples = new float[trs.trsHead.NS];
    double sumSample = 0;
    int sumData = 0;
    for (int j = 0; j < trs.trsHead.DS; j++)
    {
        for (int i = 0; i < trs.trsHead.NT; i++)
        {
            sumData += trs.trsData[i].data[j];
        }
        trsData.data[j] = sumData / trs.trsHead.NT;
    }
    for (int j = 0; j < trs.trsHead.NS; j++)
    {
        for (int i = 0; i < trs.trsHead.NT; i++)
        {
            sumSample += trs.trsData[i].samples[j];
        }
        trsData.samples[j] = sumSample / trs.trsHead.NT;
    }
    return trsData;
}

void TraceTools::meanTrace(TrsData *outTrsData, TrsHead trsHead, TrsData *inTrsData)
{
    outTrsData->samples = new float[trsHead.NS];
    for (int j = 0; j < trsHead.NS; j++)
    {
        double sumSample = 0;
        for (int i = 0; i < trsHead.NT; i++)
        {
            sumSample += inTrsData[i].samples[j];
        }
        outTrsData->samples[j] = sumSample / trsHead.NT;
    }
}

void TraceTools::trs2txt(const char *trsFileName)
{
    Trace *trace = new Trace(trsFileName);
    ofstream outWave("./outfile/wave.txt", ios::out | ios::trunc);
    ofstream outMessage("./outfile/in.txt", ios::out | ios::trunc);
    ofstream outCipher("./outfile/out.txt", ios::out | ios::trunc);
    if (!outWave.is_open() || !outMessage.is_open() || !outCipher.is_open())
    {
        cerr << "Failed to open the file!";
        exit(0);
    }
    TrsData *trsData = new TrsData;
    while (trace->currentTrace < trace->trsHead.NT)
    {
        trace->readNext(trsData);
        for (int i = 0; i < trace->trsHead.NS; i++)
        {
            outWave << trsData->samples[i] << " ";
        }
        outWave << endl;
        for (int i = 0; i < 648; i++)
        {
            outMessage << trsData->data[i] << " ";
        }
        outMessage << endl;
        for (int i = 0; i < 16; i++)
        {
            outCipher << trsData->data[i + 648] << " ";
        }
        outCipher << endl;
        cout << trace->currentTrace << "/" << trace->trsHead.NT << endl;
    }
    outCipher.close();
    outMessage.close();
    outWave.close();
}

void TraceTools::trs2txt(char *trsFileName, int txtMessageLength, int txtCipherLength)
{
    Trace *trace = new Trace(trsFileName);
    ofstream outWave("./outtxt/wave.txt", ios::out | ios::trunc);
    ofstream outMessage("./outtxt/in.txt", ios::out | ios::trunc);
    ofstream outCipher("./outtxt/out.txt", ios::out | ios::trunc);
    if (!outWave.is_open() || !outMessage.is_open() || !outCipher.is_open())
    {
        cerr << "Failed to open the file!";
        exit(0);
    }
    TrsData *trsData = new TrsData;
    while (trace->currentTrace < trace->trsHead.NT)
    {
        trace->readNext(trsData);
        for (int i = 0; i < trace->trsHead.NS; i++)
        {
            outWave << trsData->samples[i] << " ";
        }
        outWave << endl;
        for (int i = 0; i < txtMessageLength; i++)
        {
            outMessage << trsData->data[i] << " ";
        }
        outMessage << endl;
        for (int i = 0; i < txtCipherLength; i++)
        {
            outCipher << trsData->data[i + txtMessageLength] << " ";
        }
        outCipher << endl;
        cout << trace->currentTrace << "/" << trace->trsHead.NT << endl;
    }
    outCipher.close();
    outMessage.close();
    outWave.close();
}

void TraceTools::staticAlignment(TrsData *outTrsData, TrsData *baseTrsData, TrsData *srcTrsData, int samplePointNum, int startPoint, int length, int maxAlignLength)
{
    if (samplePointNum < startPoint + length || startPoint + length < 0)
    {
        std::cerr << "out of baseTrsData sample number!" << endl;
        exit(0);
    }
    double result;
    double maxCorr = 0;
    int maxPoint = 0;
    for (int i = 0; i < maxAlignLength; i++)
    {
        int alignStart = startPoint - maxAlignLength / 2 + i;
        if (alignStart < 0)
        {
            result = 0;
        }
        else
        {
            result = BaseTools::correlate(baseTrsData->samples + startPoint, srcTrsData->samples + alignStart, length);
        }
        if (maxCorr < result)
        {
            maxCorr = result;
            maxPoint = alignStart;
        }
    }

    if (startPoint < maxPoint)
    {
        int offset = maxPoint - startPoint;
        int normalLength = samplePointNum - offset;
        std::memcpy(outTrsData->samples, srcTrsData->samples + offset, normalLength);
        std::memcpy(outTrsData->samples + normalLength, baseTrsData->samples + normalLength, offset);
    }
    else
    {
        int offset = startPoint - maxPoint;
        int normalLength = samplePointNum - offset;
        std::memcpy(outTrsData->samples, baseTrsData->samples, offset);
        std::memcpy(outTrsData->samples + offset, srcTrsData->samples, normalLength);
    }
}

void TraceTools::corr(TrsData *baseTrsData, TrsData *srcTrsData, int samplePointNum, int length, int immp, TrsHead trsHead, const char *fileout)
{
    TrsData trsData_result;
    trsData_result.samples = new float[samplePointNum - length];
    for (int i = 0; i < samplePointNum - length; i++)
    {
        trsData_result.samples[i] = BaseTools::correlate(baseTrsData->samples, srcTrsData->samples + i + immp, length);
    }

    TrsHead trsHead_result = trsHead;
    trsHead_result.NT = 1;
    trsHead_result.NS = samplePointNum - length;
    trsHead_result.DS = 0;
    trsHead_result.YS = 1;
    trsHead_result.SC = 0x14;
    trsHead_result.GT_length = 0;
    trsHead_result.DC_length = 0;
    trsHead_result.XL_length = 0;
    trsHead_result.YL_length = 0;
    trsHead_result.TS = 0;
    Trace trace;

    trace.createTrace(fileout, &trsHead_result, &trsData_result);
}

void TraceTools::cal_corr(float *baseTrsData, TrsData *srcTrsData, int samplePointNum, int length, TrsHead trsHead)
{
    double *result = new double[samplePointNum - length];
    TrsData trsData_result;
    trsData_result.samples = new float[samplePointNum - length];
    for (int i = 0; i < samplePointNum - length; i++)
    {
        trsData_result.samples[i] = BaseTools::correlate(baseTrsData, srcTrsData->samples + i, length);
        result[i] = trsData_result.samples[i];
    }

    TrsHead trsHead_result = trsHead;
    trsHead_result.NT = 1;
    trsHead_result.NS = samplePointNum - length;
    trsHead_result.DS = 0;
    trsHead_result.YS = 1;
    trsHead_result.SC = 0x14;
    trsHead_result.GT_length = 0;
    trsHead_result.DC_length = 0;
    trsHead_result.XL_length = 0;
    trsHead_result.YL_length = 0;
    trsHead_result.TS = 0;
    Trace trace;

    trace.createTrace("outfile_cov1.trs", &trsHead_result, &trsData_result);
}
