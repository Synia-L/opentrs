#include <iostream>
#include <ctime>
#include <algorithm>
#include <cstring>
#include <vector>
#include <thread>
//#include "spaAI.h"
#include "./inc/Trace.h"
using namespace std;
#define TEMPLATE_NUM 5
#define SCALE 5

//是否采用平均值取点压缩模式
// #define AVE_SCALE
//是否开启多线程
#define TEST_THREAD
#define THREAD_NUM 5 //最大为模板数量5

int main() {
    std::cout << "Hello, World!" << std::endl;
    //压缩曲线
#if 0
    //读取模板文件
    string fileName_temp[TEMPLATE_NUM+s1] = {"./template/cov1_temp.trs","./template/cov2_temp.trs","./template/relu3_temp.trs","./template/pool3_temp.trs","./template/conn_temp.trs","./file/50-all-1.trs"};
    #ifdef AVE_SCALE
    string outfileName_temp[TEMPLATE_NUM+1] = { "./template/ave_scale"+to_string(SCALE)+"_cov1_temp.trs",
                                                "./template/ave_scale"+to_string(SCALE)+"_cov2_temp.trs",
                                                "./template/ave_scale"+to_string(SCALE)+"_relu3_temp.trs",
                                                "./template/ave_scale"+to_string(SCALE)+"_pool3_temp.trs",
                                                "./template/ave_scale"+to_string(SCALE)+"_conn_temp.trs",
                                                "./file/scale"+to_string(SCALE)+"_50-all-1.trs"};
    #else
    string outfileName_temp[TEMPLATE_NUM+1] = {"./template/scale"+to_string(SCALE)+"_cov1_temp.trs",
                                            "./template/scale"+to_string(SCALE)+"_cov2_temp.trs",
                                            "./template/scale"+to_string(SCALE)+"_relu3_temp.trs",
                                            "./template/scale"+to_string(SCALE)+"_pool3_temp.trs",
                                            "./template/scale"+to_string(SCALE)+"_conn_temp.trs",
                                            "./file/scale"+to_string(SCALE)+"_50-all-1.trs"};
    #endif
    TrsData trsData_temp[TEMPLATE_NUM+1],outTrsData_temp[TEMPLATE_NUM+1];
    Trace* trace_temp[TEMPLATE_NUM+1];
    for (int i = 0; i < TEMPLATE_NUM+1; i++)
    {
        trace_temp[i] = new Trace(fileName_temp[i]);
        trace_temp[i]->readNext(&trsData_temp[i]);
        outTrsData_temp[i].samples = new float[trace_temp[i]->trsHead.NS/SCALE];
    }

    for (int i = 0; i < TEMPLATE_NUM+1; i++)
    {
        for (int j = 0; j < trace_temp[i]->trsHead.NS/SCALE; j++)
        {
            #ifdef AVE_SCALE
            float sum = 0;
            for (int k = 0; k < SCALE; k++)
            {
                sum += trsData_temp[i].samples[j*SCALE+k];
            }
            outTrsData_temp[i].samples[j] = sum/SCALE;
            #else
            outTrsData_temp[i].samples[j] = trsData_temp[i].samples[j*SCALE];;
            #endif
        }
        //保存结果
        TrsHead trsHead_result = trace_temp[i]->trsHead;
        Trace *trace_result = new Trace();
        /*********头部信息*********/
        trsHead_result.NT = 1;
        trsHead_result.NS = trsHead_result.NS/SCALE;
        trsHead_result.DS = 0;
        trsHead_result.YS = 1;
        trsHead_result.SC = 0x14;//float存储类型
        trsHead_result.GT_length = 0;
        trsHead_result.DC_length = 0;
        trsHead_result.XL_length = 0;
        trsHead_result.YL_length = 0;
        trsHead_result.TS = 0;
        trace_result->createTrace(outfileName_temp[i].data(), &trsHead_result, &outTrsData_temp[i]);
        delete trace_result;
    }
#endif

//分析曲线
#if 0
    clock_t start,readEnd[2],end[TEMPLATE_NUM];
    start = clock();
    //读取待攻击文件
    string fileName_attack = "./file/scale"+to_string(SCALE)+"_50-all-1.trs";
    TrsData trsData_attack[TEMPLATE_NUM];
    Trace *trace_attack = new Trace(fileName_attack);
    trace_attack->readNext(&trsData_attack[0]);
    for (int i = 1; i < TEMPLATE_NUM; i++)
    {
        trsData_attack[i].samples = new float[trace_attack->trsHead.NS];
        memcpy(trsData_attack[i].samples,trsData_attack[0].samples,sizeof(float)*trace_attack->trsHead.NS);
    }
    readEnd[0] = clock();
    //读取模板文件
    // string fileName_temp[TEMPLATE_NUM] = {"./template/cov1_temp.trs","./template/cov2_temp.trs","./template/relu3_temp.trs","./template/pool3_temp.trs","./template/conn_temp.trs",};
    // string fileName_temp[TEMPLATE_NUM] = {"./template/scale_cov1_temp.trs","./template/scale_cov2_temp.trs","./template/scale_relu3_temp.trs","./template/scale_pool3_temp.trs","./template/scale_conn_temp.trs"};
    string fileName_temp[TEMPLATE_NUM] = {"./template/scale"+to_string(SCALE)+"_cov1_temp.trs",
                                          "./template/scale"+to_string(SCALE)+"_cov2_temp.trs",
                                          "./template/scale"+to_string(SCALE)+"_relu3_temp.trs",
                                          "./template/scale"+to_string(SCALE)+"_pool3_temp.trs",
                                          "./template/scale"+to_string(SCALE)+"_conn_temp.trs"};
    TrsData trsData_temp[TEMPLATE_NUM];
    Trace* trace_temp[TEMPLATE_NUM];
    for (int i = 0; i < TEMPLATE_NUM; i++)
    {
        trace_temp[i] = new Trace(fileName_temp[i]);
        trace_temp[i]->readNext(&trsData_temp[i]);
    }
    //结果缓存
    TrsData* trsData_result[TEMPLATE_NUM];
    for (int i = 0; i < TEMPLATE_NUM; i++)
    {
        trsData_result[i] = new TrsData;
        trsData_result[i]->samples = new float[trace_attack->trsHead.NS];
    }
    int length[TEMPLATE_NUM] = {(COV1_TEMPLATE_LEN / SCALE),
                                (COV2_TEMPLATE_LEN / SCALE),
                                (RELU_TEMPLATE_LEN / SCALE),
                                (POOL_TEMPLATE_LEN / SCALE),
                                (CONN_TEMPLATE_LEN / SCALE)};
    //保存结果
    TrsHead trsHead_result = trace_attack->trsHead;
    // Trace *trace_result = new Trace();
    Trace trace_result;
    /*********头部信息*********/
    trsHead_result.NT = 1;
    trsHead_result.DS = 0;
    trsHead_result.YS = 1;
    trsHead_result.SC = 0x14;//float存储类型
    trsHead_result.GT_length = 0;
    trsHead_result.DC_length = 0;
    trsHead_result.XL_length = 0;
    trsHead_result.YL_length = 0;
    trsHead_result.TS = 0;
    readEnd[1] = clock();
    SpaAI sapAI;
    float max[TEMPLATE_NUM] = {0};
    // string outfileName[TEMPLATE_NUM] = {"./Outfile/result_cov1.trs","./Outfile/result_cov2.trs","./Outfile/result_relu.trs","./Outfile/result_pool.trs","./Outfile/result_conn.trs"};
#ifdef AVE_SCALE
    string outfileName[TEMPLATE_NUM] = {"./Outfile/result_ave_scale"+to_string(SCALE)+"_cov1.trs",
                                        "./Outfile/result_ave_scale"+to_string(SCALE)+"_cov2.trs",
                                        "./Outfile/result_ave_scale"+to_string(SCALE)+"_relu.trs",
                                        "./Outfile/result_ave_scale"+to_string(SCALE)+"_pool.trs",
                                        "./Outfile/result_ave_scale"+to_string(SCALE)+"_conn.trs"};
#else
    string outfileName[TEMPLATE_NUM] = {"./Outfile/result_scale"+to_string(SCALE)+"_cov1.trs",
                                        "./Outfile/result_scale"+to_string(SCALE)+"_cov2.trs",
                                        "./Outfile/result_scale"+to_string(SCALE)+"_relu.trs",
                                        "./Outfile/result_scale"+to_string(SCALE)+"_pool.trs",
                                        "./Outfile/result_scale"+to_string(SCALE)+"_conn.trs"};
#endif
    vector<std::thread> mythreads;
    // for (int i = 0; i < THREAD_NUM; i++)
    for (int i = 0; i < THREAD_NUM; i++)
    {
#ifdef TEST_THREAD
        mythreads.push_back(std::thread(&SpaAI::matchTemplate_thread, &sapAI, trsData_result[i],trsData_attack[i],0,trace_attack->trsHead.NS,trsData_temp[i],0,length[i], std::ref(max[i])));
#else
        sapAI.matchTemplate(trsData_result[i],trsData_attack[i],0,trace_attack->trsHead.NS,trsData_temp[i],0,length[i], max[i]);
        // cout << "max[i] = " << max[i] <<endl;
        // for (int j = 0; j < trace_attack->trsHead.NS-length[i]; j++)
        // {
        //     if (max[i] *0.75 > trsData_result[i]->samples[j])
        //     {
        //         trsData_result[i]->samples[j] = 0;
        //     }
        // }
        trsHead_result.NS = trace_attack->trsHead.NS - length[i];
        trace_result.createTrace(outfileName[i].data(), &trsHead_result, trsData_result[i]);
        end[i] = clock();
#endif
    }
#ifdef TEST_THREAD
    for (auto iter = mythreads.begin(); iter < mythreads.end(); iter++)
    {
        iter->join();
    }
    for (int i = THREAD_NUM; i < TEMPLATE_NUM; i++)
    {
        sapAI.matchTemplate_thread(trsData_result[i],trsData_attack[i],0,trace_attack->trsHead.NS,trsData_temp[i],0,length[i],std::ref(max[i]));
    }
    for (int i = 0; i < TEMPLATE_NUM; i++)
    {
        trace_result.createTrace(outfileName[i].data(), &trsHead_result, trsData_result[i]);
    }
    end[TEMPLATE_NUM-1] = clock();
#else
    for (int i = THREAD_NUM; i < TEMPLATE_NUM; i++)
    {
        sapAI.matchTemplate(trsData_result[i],trsData_attack[i],0,trace_attack->trsHead.NS,trsData_temp[i],0,length[i], max[i]);
    }
    for (int i = 0; i < TEMPLATE_NUM; i++)
    {
        trace_result.createTrace(outfileName[i].data(), &trsHead_result, trsData_result[i]);
    }
    end[TEMPLATE_NUM-1] = clock();
#endif
    //输出程序中各个子操作的耗时
    std::cout << "Executing the program takes " << 1000*(end[TEMPLATE_NUM-1]-start)/CLOCKS_PER_SEC << "ms." << endl;
    // std::cout << "Executing the program takes " << 1000*(end[TEMPLATE_NUM-1]-start)/CLOCKS_PER_SEC << "ms. Where : " << endl;
    // std::cout << "Reading the attack file take " << 1000*(readEnd[0]-start)/CLOCKS_PER_SEC << "ms." << endl;
    // std::cout << "Reading the template file take " << 1000*(readEnd[1]-readEnd[0])/CLOCKS_PER_SEC << "ms." << endl;
    // std::cout << "Matching COV1 program take " << 1000*(end[0]-readEnd[1])/CLOCKS_PER_SEC << "ms." << endl;
    // std::cout << "Matching COV2 program take " << 1000*(end[1]-end[0])/CLOCKS_PER_SEC << "ms." << endl;
    // std::cout << "Matching RELU program take " << 1000*(end[2]-end[1])/CLOCKS_PER_SEC << "ms." << endl;
    // std::cout << "Matching POOL program take " << 1000*(end[3]-end[2])/CLOCKS_PER_SEC << "ms." << endl;
    // std::cout << "Matching CONN program take " << 1000*(end[4]-end[3])/CLOCKS_PER_SEC << "ms." << endl;
#endif
    return 0;
}
