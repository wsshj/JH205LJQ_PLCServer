#pragma once

#include <vector>

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "ws2_32.lib")

#include "open62541.h"
#include "configfile.h"

//extern float m_fThreshold;
using namespace std;

// 车辆类型
enum VEHICLETYPE
{
	CTC,
	CPM,
	SCM,
	CGTC,
	HCBC
};

struct SocketUDP
{
	SOCKET svr;
	sockaddr_in sin;
};

class JHNetwork
{
private:
	//UA_Client* m_client;
	//UA_StatusCode m_retval;

	//SOCKET m_svr;
	//sockaddr_in m_sin;

public:
	ConfigFile m_cfg;

	// 利用 UDP 从 UE 接收五大车操作数据
	SocketUDP* m_recvOperate = nullptr;

	// 利用 OPCUA 从 PLC 接收拦焦车数据
	UA_Client* m_recvCTCUI = nullptr;
	UA_Client* m_recvCTCStatus = nullptr;

	// 利用 UDP 向 UE 发送拦焦车数据
	vector<SocketUDP*> m_sendCTCUI;
	vector<SocketUDP*> m_sendCTCStatus;

	// 利用 OPCUA 从 PLC 接收推焦车数据
	UA_Client* m_recvCPMUI = nullptr;
	UA_Client* m_recvCPMStatus = nullptr;

	// 利用 UDP 向 UE 发送推焦车数据
	vector<SocketUDP*> m_sendCPMUI;
	vector<SocketUDP*> m_sendCPMStatus;

	// 利用 OPCUA 从 PLC 接收装煤车数据
	UA_Client* m_recvSCMUI = nullptr;
	UA_Client* m_recvSCMStatus = nullptr;

	// 利用 UDP 向 UE 发送装煤车数据
	vector<SocketUDP*> m_sendSCMUI;
	vector<SocketUDP*> m_sendSCMStatus;

	// 利用 OPCUA 从 PLC 接收导烟车数据
	UA_Client* m_recvCGTCUI = nullptr;
	UA_Client* m_recvCGTCStatus = nullptr;

	// 利用 UDP 向 UE 发送导烟车数据
	vector<SocketUDP*> m_sendCGTCUI;
	vector<SocketUDP*> m_sendCGTCStatus;

	// 利用 OPCUA 从 PLC 接收熄焦车数据
	UA_Client* m_recvHCBCUI = nullptr;
	UA_Client* m_recvHCBCStatus = nullptr;

	// 利用 UDP 向 UE 发送熄焦车数据
	vector<SocketUDP*> m_sendHCBCUI;
	vector<SocketUDP*> m_sendHCBCStatus;

	vector<SocketUDP*> m_SocketUDPArray;

	JHNetwork();

	JHNetwork(ConfigFile cfg);

	~JHNetwork();

	void getSocketUDPArray(vector<SocketUDP*>& statusSync, vector<SocketUDP*>& uiSync, string vehicleType);

	SocketUDP* connectUDP(const char* hostUDP, int port);

	UA_Client* connectOPCUA(const char* hostUA);
};