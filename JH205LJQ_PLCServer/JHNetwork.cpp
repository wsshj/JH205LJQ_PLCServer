#include "JHNetwork.h"

JHNetwork::JHNetwork() {}

JHNetwork::JHNetwork(ConfigFile cfg)
{
	m_cfg = cfg;

	m_recvOperate = connectUDP(m_cfg.getvalue<string>("recv_host").c_str(), m_cfg.getvalue<int>("recv_port"));

	//string s = m_cfg.getvalue<string>("ctc_opcua_host").c_str();
	m_recvCTCStatus = connectOPCUA(m_cfg.getvalue<string>("ctc_opcua_host").c_str());

	m_recvCTCUI = connectOPCUA(m_cfg.getvalue<string>("ctc_opcua_host").c_str());

	getSocketUDPArray(m_sendCTCStatus, m_sendCTCUI, "ctc");

	m_recvCPMStatus = connectOPCUA(m_cfg.getvalue<string>("cpm_opcua_host").c_str());

	m_recvCPMUI = connectOPCUA(m_cfg.getvalue<string>("cpm_opcua_host").c_str());

	getSocketUDPArray(m_sendCPMStatus, m_sendCPMUI, "cpm");

	m_recvSCMStatus = connectOPCUA(m_cfg.getvalue<string>("scm_opcua_host").c_str());

	m_recvSCMUI = connectOPCUA(m_cfg.getvalue<string>("scm_opcua_host").c_str());

	getSocketUDPArray(m_sendSCMStatus, m_sendSCMUI, "scm");

	m_recvCGTCStatus = connectOPCUA(m_cfg.getvalue<string>("cgtc_opcua_host").c_str());

	m_recvCGTCUI = connectOPCUA(m_cfg.getvalue<string>("cgtc_opcua_host").c_str());

	getSocketUDPArray(m_sendCGTCStatus, m_sendCGTCUI, "cgtc");

	m_recvHCBCStatus = connectOPCUA(m_cfg.getvalue<string>("hcbc_opcua_host").c_str());

	m_recvHCBCUI = connectOPCUA(m_cfg.getvalue<string>("hcbc_opcua_host").c_str());

	getSocketUDPArray(m_sendHCBCStatus, m_sendHCBCUI, "hcbc");
}

JHNetwork::~JHNetwork()
{
	for (auto socketUDP = m_SocketUDPArray.begin(); socketUDP < m_SocketUDPArray.end(); ++socketUDP)
	{
		delete* socketUDP;
		*socketUDP = nullptr;
	}

	UA_Client_delete(m_recvCTCUI);
	UA_Client_delete(m_recvCTCStatus);
	UA_Client_delete(m_recvCPMUI);
	UA_Client_delete(m_recvCPMStatus);
	UA_Client_delete(m_recvSCMUI);
	UA_Client_delete(m_recvSCMStatus);
	UA_Client_delete(m_recvCGTCUI);
	UA_Client_delete(m_recvCGTCStatus);
	UA_Client_delete(m_recvHCBCUI);
	UA_Client_delete(m_recvHCBCStatus);
}

void JHNetwork::getSocketUDPArray(vector<SocketUDP*>& status, vector<SocketUDP*>& ui, string vehicleType)
{
	for (int i = 0; i < m_cfg.getvalue<int>("send_num"); i++)
	{
		string s = m_cfg.getvalueidx<string>("send_host", i);
		int n = m_cfg.getvalue<int>(vehicleType + "_status_port");

		SocketUDP* socketStatus = connectUDP(m_cfg.getvalueidx<string>("send_host", i).c_str(), m_cfg.getvalue<int>(vehicleType + "_status_port"));
		if (socketStatus != nullptr)
		{
			status.push_back(socketStatus);
			m_SocketUDPArray.push_back(socketStatus);
		}

		SocketUDP* socketUI = connectUDP(m_cfg.getvalueidx<string>("send_host", i).c_str(), m_cfg.getvalue<int>(vehicleType + "_ui_port"));
		if (socketUI != nullptr)
		{
			ui.push_back(socketUI);
			m_SocketUDPArray.push_back(socketUI);
		}
	}
}

SocketUDP* JHNetwork::connectUDP(const char* hostUDP, int port)
{
	SocketUDP* udp = new SocketUDP();
	//初始化WSA    
	WORD sockVersion = MAKEWORD(2, 2);

	WSADATA wsaData;

	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		cout << "sockVersion error !" << endl;
		delete udp;
		return nullptr;
	}

	//绑定IP和端口    
	udp->sin.sin_family = AF_INET;
	udp->sin.sin_port = htons(port);
	inet_pton(AF_INET, hostUDP, (void*)&udp->sin.sin_addr.S_un.S_addr);

	//创建套接字    
	udp->svr = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	bool bOpt = true;

	setsockopt(udp->svr, SOL_SOCKET, SO_BROADCAST, (char*)&bOpt, sizeof(bOpt)); //设置广播

	if (udp->svr == INVALID_SOCKET)
	{
		cout << "socket error !" << endl;
		delete udp;
		return nullptr;
	}

	return udp;
}

UA_Client* JHNetwork::connectOPCUA(const char* hostUA)
{
	UA_Client* client = UA_Client_new();
	UA_ClientConfig_setDefault(UA_Client_getConfig(client));
	UA_StatusCode retval = UA_Client_connect(client, hostUA);

	if (retval != UA_STATUSCODE_GOOD)
	{
		UA_Client_delete(client);
		printf("Connect OPCUA is failed, Error code:%d.", (int)retval);
		return nullptr;
	}

	return client;
}