#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include "JHNetwork.h"

using namespace std;

#define STATE_INIT_NUM -5

struct OperateSyncProtocol
{
	INT32 key;
	UINT8 value;
};

class JHOperateSync
{
private:
	SocketUDP* m_recvFromUE;

	int m_vehicleType;

	const char* m_host;

public:
	JHOperateSync();

	JHOperateSync(SocketUDP* recvFromUE, int vehicleType);

	void operator()();

	void sendData(UA_Boolean value, UA_NodeId nodeId);

	void sendData(UA_Int16 value, UA_NodeId nodeId);

	void comparisonData(UINT8 key, const char* str1);

	void comparisonData(UINT8 key, const char* str1, const char* str2);

	void comparisonData(UINT8 key, const char* str1, const char* str2, const char* str3);

	void startOperateSync();

	void ctcOperateSync(int key, UINT8 value);

	void cpmOperateSync(int key, UINT8 value);

	void scmOperateSync(int key, UINT8 value);

	void cgtcOperateSync(int key, UINT8 value);

	void hcbcOperateSync(int key, UINT8 value);
};

