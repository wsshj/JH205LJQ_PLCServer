#include "JHNetwork.h"

bool JHNetwork::connectTCP()
{
	return false;
}

SocketUDP* JHNetwork::connectUDP(const char* hostUDP, int port)
{
	SocketUDP* udp = new SocketUDP();
	//初始化WSA    
	WORD sockVersion = MAKEWORD(2, 2);

	WSADATA wsaData;

	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
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
		printf("socket error !");
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