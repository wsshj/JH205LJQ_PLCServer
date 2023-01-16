#pragma once

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "ws2_32.lib")

#include "open62541.h"

//extern float m_fThreshold;

// ≥µ¡æ¿‡–Õ
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
	JHNetwork() {}

	bool connectTCP();

	SocketUDP* connectUDP(const char* hostUDP, int port);

	UA_Client* connectOPCUA(const char* hostUA);
};