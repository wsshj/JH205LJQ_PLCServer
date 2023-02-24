// JH205LJQ_PLCServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include <fstream>  
#include <thread>

#include "JHNetwork.h"
#include "JHOperateSync.h"
#include "JHStateSync.h"
#include "JHUIDataSync.h"
#include "configfile.h"

float fCTCThreshold = 0;
float fCPMThreshold = 0;
float fSCMThreshold = 0;
float fCGTCThreshold = 0;
float fHCBCThreshold = 0;

#define ALL

int main()
{
    ConfigFile cfg("config.cfg");

    JHNetwork np(cfg);

    JHOperateSync operateSync(np.m_recvOperate);

    std::thread threadOperateSync(operateSync);

#if defined(CTCTEST) || defined(ALL)
    if (np.m_recvCTCUI == nullptr || np.m_recvCTCStatus == nullptr)
    {
        cout << "Error:By PLCUA connect CTC From PLC failed." << endl;
        return 0;
    }

    // 拦焦车创建状态同步线程
    JHStateSync ctcStateSync(np.m_recvCTCStatus, np.m_sendCTCStatus, VEHICLETYPE::CTC);

    std::thread threadCTCStateSync(ctcStateSync);

    // 拦焦车创建UI同步线程
    JHUIDataSync ctcUIDataSync(np.m_recvCTCUI, np.m_sendCTCUI, VEHICLETYPE::CTC);

    std::thread threadCTCUIDataSync(ctcUIDataSync);
#endif

#if defined(CPMTEST) || defined(ALL)
    // 推焦车
    if (np.m_recvCPMUI == nullptr || np.m_recvCPMStatus == nullptr)
    {
        cout << "Error:By PLCUA connect CPM From PLC failed." << endl;
        return 0;
    }

    // 拦焦车创建状态同步线程
    JHStateSync cpmStateSync(np.m_recvCPMStatus, np.m_sendCPMStatus, VEHICLETYPE::CPM);

    std::thread threadCPMStateSync(cpmStateSync);

    // 拦焦车创建UI同步线程
    JHUIDataSync cpmUIDataSync(np.m_recvCPMUI, np.m_sendCPMUI, VEHICLETYPE::CPM);

    std::thread threadCPMUIDataSync(cpmUIDataSync);
#endif

#if defined(SCMTEST) || defined(ALL)
    // 装煤车
    if (np.m_recvSCMUI == nullptr || np.m_recvSCMStatus == nullptr)
    {
        cout << "Error:By PLCUA connect SCM From PLC failed." << endl;
        return 0;
    }

    // 拦焦车创建状态同步线程
    JHStateSync scmStateSync(np.m_recvSCMStatus, np.m_sendSCMStatus, VEHICLETYPE::SCM);

    std::thread threadSCMStateSync(scmStateSync);

    // 拦焦车创建UI同步线程
    JHUIDataSync scmUIDataSync(np.m_recvSCMUI, np.m_sendSCMUI, VEHICLETYPE::SCM);

    std::thread threadSCMUIDataSync(scmUIDataSync);
#endif

#if defined(CGTCTEST) || defined(ALL)
    // 导烟车
    if (np.m_recvCGTCUI == nullptr || np.m_recvCGTCStatus == nullptr)
    {
        cout << "Error:By PLCUA connect CGTC From PLC failed." << endl;
        return 0;
    }

    // 拦焦车创建状态同步线程
    JHStateSync cgtcStateSync(np.m_recvCGTCStatus, np.m_sendCGTCStatus, VEHICLETYPE::CGTC);

    std::thread threadCGTCStateSync(cgtcStateSync);

    // 拦焦车创建UI同步线程
    JHUIDataSync cgtcUIDataSync(np.m_recvCGTCUI, np.m_sendCGTCUI, VEHICLETYPE::CGTC);

    std::thread threadCGTCUIDataSync(cgtcUIDataSync);
#endif

#if defined(HCBCTEST) || defined(ALL)
    // 熄焦车
    if (np.m_recvHCBCUI == nullptr || np.m_recvHCBCStatus == nullptr)
    {
        cout << "Error:By PLCUA connect HCBC From PLC failed." << endl;
        return 0;
    }

    // 拦焦车创建状态同步线程
    JHStateSync hcbcStateSync(np.m_recvHCBCStatus, np.m_sendHCBCStatus, VEHICLETYPE::HCBC);

    std::thread threadHCBCStateSync(hcbcStateSync);

    // 拦焦车创建UI同步线程
    JHUIDataSync hcbcUIDataSync(np.m_recvHCBCUI, np.m_sendHCBCUI, VEHICLETYPE::HCBC);

    std::thread threadHCBCUIDataSync(hcbcUIDataSync);
#endif

    threadOperateSync.join();

#if defined(CTCTEST) || defined(ALL)
    threadCTCStateSync.join();
    threadCTCUIDataSync.join();
#endif

#if defined(CPMTEST) || defined(ALL)
    threadCPMStateSync.join();
    threadCPMUIDataSync.join();
#endif

#if defined(SCMTEST) || defined(ALL)
    threadSCMStateSync.join();
    threadSCMUIDataSync.join();
#endif

#if defined(CGTCTEST) || defined(ALL)
    threadCGTCStateSync.join();
    threadCGTCUIDataSync.join();
#endif

#if defined(HCBCTEST) || defined(ALL)
    threadHCBCStateSync.join();
    threadHCBCUIDataSync.join();
#endif

    return 0;
}