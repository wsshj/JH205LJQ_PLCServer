#pragma once

#include <vector>

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "ws2_32.lib")

#include "open62541.h"
#include "configfile.h"

//extern float m_fThreshold;
using namespace std;

// ��������
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

	// ���� UDP �� UE ������󳵲�������
	SocketUDP* m_recvOperate = nullptr;

	// ���� OPCUA �� PLC ��������������
	UA_Client* m_recvCTCUI = nullptr;
	UA_Client* m_recvCTCStatus = nullptr;

	// ���� UDP �� UE ��������������
	vector<SocketUDP*> m_sendCTCUI;
	vector<SocketUDP*> m_sendCTCStatus;

	// ���� OPCUA �� PLC �����ƽ�������
	UA_Client* m_recvCPMUI = nullptr;
	UA_Client* m_recvCPMStatus = nullptr;

	// ���� UDP �� UE �����ƽ�������
	vector<SocketUDP*> m_sendCPMUI;
	vector<SocketUDP*> m_sendCPMStatus;

	// ���� OPCUA �� PLC ����װú������
	UA_Client* m_recvSCMUI = nullptr;
	UA_Client* m_recvSCMStatus = nullptr;

	// ���� UDP �� UE ����װú������
	vector<SocketUDP*> m_sendSCMUI;
	vector<SocketUDP*> m_sendSCMStatus;

	// ���� OPCUA �� PLC ���յ��̳�����
	UA_Client* m_recvCGTCUI = nullptr;
	UA_Client* m_recvCGTCStatus = nullptr;

	// ���� UDP �� UE ���͵��̳�����
	vector<SocketUDP*> m_sendCGTCUI;
	vector<SocketUDP*> m_sendCGTCStatus;

	// ���� OPCUA �� PLC ����Ϩ��������
	UA_Client* m_recvHCBCUI = nullptr;
	UA_Client* m_recvHCBCStatus = nullptr;

	// ���� UDP �� UE ����Ϩ��������
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