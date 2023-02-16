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
    JHNetwork np;

    ConfigFile cfg("config.cfg");

    string strRecv = cfg.getvalue<string>("recv_host");
    const char* recvHost = strRecv.c_str();

    string strSend = cfg.getvalue<string>("send_host");
    const char* sendHost = strSend.c_str();

    // 接收客户端操作
    SocketUDP* recvFromUE = np.connectUDP(recvHost, 9001);
    if (recvFromUE == nullptr)
    {
        cout << "Error:By UDP connect" << recvHost << "failed." << endl;

        return 0;
    }

    JHOperateSync operateSync(recvFromUE, VEHICLETYPE::CTC);

    std::thread threadOperateSync(operateSync);

#if defined(CTCTEST) || defined(ALL)
    // 拦焦车发送状态同步给客户端
    SocketUDP* sendCTCToUE = np.connectUDP(sendHost, 8001);
    SocketUDP* sendCTCUIToUE = np.connectUDP(sendHost, 8101);
    if (sendCTCToUE == nullptr)
    {
        cout << "Error:By UDP connect CTC To UE failed." << endl;

        return 0;
    }

    // 拦焦车从PLC接收数据
    UA_Client* recvCTCFromPLC = np.connectOPCUA("opc.tcp://192.168.0.7:4840");
    // 拦焦车从PLC接收UI数据
    UA_Client* recvCTCUIFromPLC = np.connectOPCUA("opc.tcp://192.168.0.7:4840");
    if (recvCTCFromPLC == nullptr || recvCTCUIFromPLC == nullptr)
    {
        cout << "Error:By PLCUA connect CTC From PLC failed." << endl;
        return 0;
    }

    // 拦焦车创建状态同步线程
    JHStateSync ctcStateSync(recvCTCFromPLC, sendCTCToUE, VEHICLETYPE::CTC);

    std::thread threadCTCStateSync(ctcStateSync);

    // 拦焦车创建UI同步线程
    JHUIDataSync ctcUIDataSync(recvCTCUIFromPLC, sendCTCUIToUE, VEHICLETYPE::CTC);

    std::thread threadCTCUIDataSync(ctcUIDataSync);
#endif

#if defined(CPMTEST) || defined(ALL)
    // 推焦车
    SocketUDP* sendCPMToUE = np.connectUDP(sendHost, 8002);
    SocketUDP* sendCPMUIToUE = np.connectUDP(sendHost, 8102);
    if (sendCPMToUE == nullptr || sendCPMUIToUE == nullptr)
    {
        cout << "Error:By UDP connect CPM To UE failed." << endl;

        return 0;
    }

    UA_Client* recvCPMFromPLC = np.connectOPCUA("opc.tcp://192.168.0.5:4840");
    UA_Client* recvCPMUIFromPLC = np.connectOPCUA("opc.tcp://192.168.0.5:4840");
    if (recvCPMFromPLC == nullptr || recvCPMUIFromPLC == nullptr)
    {
        cout << "Error:By PLCUA connect CTC From PLC failed." << endl;
        return 0;
    }

    JHStateSync cpmStateSync(recvCPMFromPLC, sendCPMToUE, VEHICLETYPE::CPM);

    std::thread threadCPMStateSync(cpmStateSync);

    JHUIDataSync cpmUIDataSync(recvCPMUIFromPLC, sendCPMUIToUE, VEHICLETYPE::CPM);

    std::thread threadCPMUIDataSync(cpmUIDataSync);
#endif

#if defined(SCMTEST) || defined(ALL)
    // 装煤车
    SocketUDP* sendSCMToUE = np.connectUDP(sendHost, 8003);
    SocketUDP* sendSCMUIToUE = np.connectUDP(sendHost, 8103);
    if (sendSCMToUE == nullptr || sendSCMUIToUE == nullptr)
    {
        cout << "Error:By UDP connect SCM To UE failed." << endl;

        return 0;
    }

    UA_Client* recvSCMFromPLC = np.connectOPCUA("opc.tcp://192.168.0.6:4840");
    UA_Client* recvSCMUIFromPLC = np.connectOPCUA("opc.tcp://192.168.0.6:4840");
    if (recvSCMFromPLC == nullptr || recvSCMUIFromPLC == nullptr)
    {
        cout << "Error:By PLCUA connect SCM From PLC failed." << endl;
        return 0;
    }

    JHStateSync scmStateSync(recvSCMFromPLC, sendSCMToUE, VEHICLETYPE::SCM);

    std::thread threadSCMStateSync(scmStateSync);

    JHUIDataSync scmUIDataSync(recvSCMUIFromPLC, sendSCMUIToUE, VEHICLETYPE::SCM);

    std::thread threadSCMUIDataSync(scmUIDataSync);
#endif

#if defined(CGTCTEST) || defined(ALL)
    // 导烟车
    SocketUDP* sendCGTCToUE = np.connectUDP(sendHost, 8004);
    SocketUDP* sendCGTCUIToUE = np.connectUDP(sendHost, 8104);
    if (sendCGTCToUE == nullptr || sendCGTCUIToUE == nullptr)
    {
        cout << "Error:By UDP connect CGTC To UE failed." << endl;

        return 0;
    }

    UA_Client* recvCGTCFromPLC = np.connectOPCUA("opc.tcp://192.168.0.4:4840");
    UA_Client* recvCGTCUIFromPLC = np.connectOPCUA("opc.tcp://192.168.0.4:4840");
    if (recvCGTCFromPLC == nullptr || recvCGTCUIFromPLC == nullptr)
    {
        cout << "Error:By PLCUA connect CGTC From PLC failed." << endl;
        return 0;
    }

    JHStateSync cgtcStateSync(recvCGTCFromPLC, sendCGTCToUE, VEHICLETYPE::CGTC);

    std::thread threadCGTCStateSync(cgtcStateSync);

    JHUIDataSync cgtcUIDataSync(recvCGTCUIFromPLC, sendCGTCUIToUE, VEHICLETYPE::CGTC);

    std::thread threadCGTCUIDataSync(cgtcUIDataSync);
#endif

#if defined(HCBCTEST) || defined(ALL)
    // 熄焦车
    SocketUDP* sendHCBCToUE = np.connectUDP(sendHost, 8005);
    SocketUDP* sendHCBCUIToUE = np.connectUDP(sendHost, 8105);
    if (sendHCBCToUE == nullptr || sendHCBCUIToUE == nullptr)
    {
        cout << "Error:By UDP connect HCBC To UE failed." << endl;

        return 0;
    }

    UA_Client* recvHCBCFromPLC = np.connectOPCUA("opc.tcp://192.168.0.8:4840");
    UA_Client* recvHCBCUIFromPLC = np.connectOPCUA("opc.tcp://192.168.0.8:4840");
    if (recvHCBCFromPLC == nullptr || recvHCBCUIFromPLC == nullptr)
    {
        cout << "Error:By PLCUA connect HCBC From PLC failed." << endl;
        return 0;
    }

    JHStateSync hcbcStateSync(recvHCBCFromPLC, sendHCBCToUE, VEHICLETYPE::HCBC);

    std::thread threadHCBCStateSync(hcbcStateSync);
   
    JHUIDataSync hcbcUIDataSync(recvHCBCUIFromPLC, sendHCBCUIToUE, VEHICLETYPE::HCBC);

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