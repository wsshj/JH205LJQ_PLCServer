#include "JHStateSync.h"

extern float fCTCThreshold;
extern float fCPMThreshold;
extern float fSCMThreshold;
extern float fCGTCThreshold;
extern float fHCBCThreshold;

JHStateSync::JHStateSync()
{
	m_ctcLastData = new CTCStateData();
	m_cpmLastData = new CPMStateData();
	m_scmLastData = new SCMStateData();
	m_cgtcLastData = new CGTCStateData();
	m_hcbcLastData = new HCBCStateData();

	m_sendToUE = nullptr;
	m_recvFromPLC = nullptr;
}

JHStateSync::JHStateSync(UA_Client* recvFromPLC, SocketUDP* sendToUE, int vehicleType)
{
	m_ctcLastData = new CTCStateData();
	m_cpmLastData = new CPMStateData();
	m_scmLastData = new SCMStateData();
	m_cgtcLastData = new CGTCStateData();
	m_hcbcLastData = new HCBCStateData();

	m_sendToUE = sendToUE;
	m_recvFromPLC = recvFromPLC;
	m_vehicleType = vehicleType;
}

JHStateSync::~JHStateSync()
{
	delete m_ctcLastData;
	delete m_cpmLastData;
	delete m_scmLastData;
	delete m_cgtcLastData;
	delete m_hcbcLastData;
}

void JHStateSync::operator()()
{
	startStateSync();
}

template <typename T>
T* JHStateSync::recvData(const char* strNodeId)
{
	UA_NodeId nodeId = UA_NODEID_STRING(3, const_cast<char*>(strNodeId));

	UA_Variant value; /* Variants can hold scalar values and arrays of any type */
	UA_Variant_init(&value);

	// 客户端接收"Testt"传来的数据
	UA_StatusCode retval = UA_Client_readValueAttribute(m_recvFromPLC, nodeId, &value);

	// 判断接收状态和数据类型是否符合
	if (retval == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]))
	{
		// 将接受到的数据转化为相应的数据类型
		UA_ExtensionObject* object = (UA_ExtensionObject*)value.data;

		// 获取扩展对象中的数据体
		UA_ByteString strBody = object->content.encoded.body;

		if (strBody.length > 0)
		{
			T data = *(T*)strBody.data;

			UA_Variant_clear(&value);

			return &data;
		}
	}

	UA_Variant_clear(&value);
	return nullptr;
}

bool JHStateSync::recvBoolData(const char* strNodeId)
{
	UA_NodeId nodeId = UA_NODEID_STRING(3, const_cast<char*>(strNodeId));

	UA_Variant value; /* Variants can hold scalar values and arrays of any type */
	UA_Variant_init(&value);

	// 客户端接收"Testt"传来的数据
	UA_StatusCode retval = UA_Client_readValueAttribute(m_recvFromPLC, nodeId, &value);

	// 判断接收状态和数据类型是否符合
	if (retval == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_BOOLEAN]))
	{
		bool data = *(bool*)value.data;

		UA_Variant_clear(&value);

		return data;
	}
	UA_Variant_clear(&value);
	return 0;
}

void JHStateSync::startStateSync()
{
	// 将数据转成相应的数据结构,然后处理
	switch (m_vehicleType)
	{
	case VEHICLETYPE::CTC:
		while (true){ comparisonData(recvData<CTCStateData>("\"VirtualCTC1ToUE\"")); }
		break;
	case VEHICLETYPE::CPM:
		while (true){ comparisonData(recvData<CPMStateData>("\"VirtualCPM1ToUE\"")); }
		break;
	case VEHICLETYPE::SCM:
		while (true){ comparisonData(recvData<SCMStateData>("\"VirtualSCM1ToUE\"")); }
		break;
	case VEHICLETYPE::CGTC:
		while (true){ comparisonData(recvData<CGTCStateData>("\"VirtualCGTC1ToUE\"")); }
		break;
	case VEHICLETYPE::HCBC:
		while (true){ comparisonData(recvData<HCBCStateData>("\"VirtualHCBC1ToUE\"")); }
		break;
	default:
		break;
	}
}

void JHStateSync::sendData(float& lastValue, float newValue, INT32 key, float min, float max)
{
	StateSyncProtocol data;

	data.key = key;
	data.value = newValue;

	if (newValue >= max)
	{
		data.signal = 4;
		data.value = max;
	}
	else if (newValue <= min)
	{
		data.signal = 1;
		data.value = min;
	}
	else if (lastValue > newValue)
	{
		data.signal = 3;
	}
	else if (lastValue < newValue)
	{
		data.signal = 2;
	}

	int send_len = sendto(m_sendToUE->svr, (char*)&data, sizeof(data), 0, (const SOCKADDR*)&m_sendToUE->sin, sizeof(m_sendToUE->sin));
	if (SOCKET_ERROR == send_len)
	{
		cout << "sendto error !" << endl;
	}

	lastValue = newValue;
}

void JHStateSync::sendData(bool& lastValue, bool newValue, INT32 key)
{
	StateSyncProtocol data;

	data.key = key;
	data.state = newValue;

	int send_len = sendto(m_sendToUE->svr, (char*)&data, sizeof(data), 0, (const SOCKADDR*)&m_sendToUE->sin, sizeof(m_sendToUE->sin));
	if (SOCKET_ERROR == send_len)
	{
		cout << "sendto error !" << endl;
	}

	lastValue = newValue;
}

void JHStateSync::comparisonData(CTCStateData* pd)
{ 
	if (pd == nullptr)
	{
		cout << "StateData is null" << endl;
		return;
	}

	if (abs(m_ctcLastData->CTC_Location - pd->CTC_Location) > fCTCThreshold || abs(m_ctcLastData->CTC_MoveSpeed - pd->CTC_MoveSpeed) > fCTCThreshold)
	{
		sendData(m_ctcLastData->CTC_Location, pd->CTC_Location, 10001, 0, 152000);
		sendData(m_ctcLastData->CTC_MoveSpeed, pd->CTC_MoveSpeed, 10002, 0, 99999);
		//cout << "Location:" << pd->CTC_Location << endl;
		//cout << "CTC_MoveSpeed:" << pd->CTC_MoveSpeed << endl;
	}

	/*if (abs(m_ctcLastData->CTC_MoveSpeed - pd->CTC_MoveSpeed) > fCTCThreshold)
	{
		sendData(m_ctcLastData->CTC_MoveSpeed, pd->CTC_MoveSpeed, 10002, 0, 99999);
		cout << "CTC_MoveSpeed:" << pd->CTC_MoveSpeed << endl;
	}*/

	if (abs(m_ctcLastData->CTC_GetDoorTrolly_Location - pd->CTC_GetDoorTrolly_Location) > fCTCThreshold)
	{
		sendData(m_ctcLastData->CTC_GetDoorTrolly_Location, pd->CTC_GetDoorTrolly_Location, 10101, 0, 2838);
	}

	if (abs(m_ctcLastData->CTC_LatchPressIn_Location - pd->CTC_LatchPressIn_Location) > fCTCThreshold)
	{
		sendData(m_ctcLastData->CTC_LatchPressIn_Location, pd->CTC_LatchPressIn_Location, 10102, 0, 40);
	}

	if (abs(m_ctcLastData->CTC_LatchTurn_Location - pd->CTC_LatchTurn_Location) > fCTCThreshold)
	{
		sendData(m_ctcLastData->CTC_LatchTurn_Location, pd->CTC_LatchTurn_Location, 10103, 0, 150);
	}

	if (abs(m_ctcLastData->CTC_LiftDoor_Location - pd->CTC_LiftDoor_Location) > fCTCThreshold)
	{
		sendData(m_ctcLastData->CTC_LiftDoor_Location, pd->CTC_LiftDoor_Location, 10104, 0, 530);
	}

	if (abs(m_ctcLastData->CTC_CokeTrayUpDown_Location - pd->CTC_CokeTrayUpDown_Location) > fCTCThreshold)
	{
		sendData(m_ctcLastData->CTC_CokeTrayUpDown_Location, pd->CTC_CokeTrayUpDown_Location, 10201, 0, 500);
	}

	if (abs(m_ctcLastData->CTC_CokeTrayFrontRear_Location - pd->CTC_CokeTrayFrontRear_Location) > fCTCThreshold)
	{
		sendData(m_ctcLastData->CTC_CokeTrayFrontRear_Location, pd->CTC_CokeTrayFrontRear_Location, 10202, 0, 250);
	}

	if (abs(m_ctcLastData->CTC_DoorCleanerTrolly_Location - pd->CTC_DoorCleanerTrolly_Location) > fCTCThreshold)
	{
		sendData(m_ctcLastData->CTC_DoorCleanerTrolly_Location, pd->CTC_DoorCleanerTrolly_Location, 10301, 0, 700);
	}

	if (abs(m_ctcLastData->CTC_SideCutter_Location - pd->CTC_SideCutter_Location) > fCTCThreshold)
	{
		sendData(m_ctcLastData->CTC_SideCutter_Location, pd->CTC_SideCutter_Location, 10302, 0, 500);
	}

	if (abs(m_ctcLastData->CTC_UpperCutter_Location - pd->CTC_UpperCutter_Location) > fCTCThreshold)
	{
		sendData(m_ctcLastData->CTC_UpperCutter_Location, pd->CTC_UpperCutter_Location, 10303, 0, 250);
	}

	if (abs(m_ctcLastData->CTC_LowerCutter_Location - pd->CTC_LowerCutter_Location) > fCTCThreshold)
	{
		sendData(m_ctcLastData->CTC_LowerCutter_Location, pd->CTC_LowerCutter_Location, 10304, 0, 250);
	}

	if (abs(m_ctcLastData->CTC_FrameCleanerTrolly_Location - pd->CTC_FrameCleanerTrolly_Location) > fCTCThreshold)
	{
		sendData(m_ctcLastData->CTC_FrameCleanerTrolly_Location, pd->CTC_FrameCleanerTrolly_Location, 10401, 0, 2500);
	}

	if (abs(m_ctcLastData->CTC_FrameCleanMachine_Location - pd->CTC_FrameCleanMachine_Location) > fCTCThreshold)
	{
		sendData(m_ctcLastData->CTC_FrameCleanMachine_Location, pd->CTC_FrameCleanMachine_Location, 10402, 0, 500);
	}

	if (abs(m_ctcLastData->CTC_CokeGrid_Location - pd->CTC_CokeGrid_Location) > fCTCThreshold)
	{
		sendData(m_ctcLastData->CTC_CokeGrid_Location, pd->CTC_CokeGrid_Location, 10501, 0, 2950);
	}
	if (fCTCThreshold < 0)
	{	
		cout << "CTC_Threshold:" << fCTCThreshold << endl;
		fCTCThreshold++;
	}
}

void JHStateSync::comparisonData(CPMStateData* pd)
{
	if (pd == nullptr)
	{
		cout << "StateData is null" << endl;
		return;
	}
	if (abs(m_cpmLastData->CPM_Location - pd->CPM_Location) > fCPMThreshold || abs(m_cpmLastData->CPM_MoveSpeed - pd->CPM_MoveSpeed) > fCPMThreshold)
	{
		sendData(m_cpmLastData->CPM_Location, pd->CPM_Location, 20001, 0, 152000);
		sendData(m_cpmLastData->CPM_MoveSpeed, pd->CPM_MoveSpeed, 20002, 0, 99999);
	}

	if (abs(m_cpmLastData->CPM_GetDoorTrolly_Location - pd->CPM_GetDoorTrolly_Location) > fCPMThreshold)
	{
		sendData(m_cpmLastData->CPM_GetDoorTrolly_Location, pd->CPM_GetDoorTrolly_Location, 20101, 0, 2812);
	}

	if (abs(m_cpmLastData->CPM_LatchPressIn_Location - pd->CPM_LatchPressIn_Location) > fCPMThreshold)
	{
		sendData(m_cpmLastData->CPM_LatchPressIn_Location, pd->CPM_LatchPressIn_Location, 20102, 0, 40);
	}

	if (abs(m_cpmLastData->CPM_LatchTurn_Location - pd->CPM_LatchTurn_Location) > fCPMThreshold)
	{
		sendData(m_cpmLastData->CPM_LatchTurn_Location, pd->CPM_LatchTurn_Location, 20103, 0, 150);
	}

	if (abs(m_cpmLastData->CPM_LiftDoor_Location - pd->CPM_LiftDoor_Location) > fCPMThreshold)
	{
		sendData(m_cpmLastData->CPM_LiftDoor_Location, pd->CPM_LiftDoor_Location, 20104, 0, 342);
	}

	if (abs(m_cpmLastData->CPM_DoorCleanerTrolly_Location - pd->CPM_DoorCleanerTrolly_Location) > fCPMThreshold)
	{
		sendData(m_cpmLastData->CPM_DoorCleanerTrolly_Location, pd->CPM_DoorCleanerTrolly_Location, 20201, 0, 700);
	}

	if (abs(m_cpmLastData->CPM_SideCutter_Location - pd->CPM_SideCutter_Location) > fCPMThreshold)
	{
		sendData(m_cpmLastData->CPM_SideCutter_Location, pd->CPM_SideCutter_Location, 20202, 0, 500);
	}

	if (abs(m_cpmLastData->CPM_UpperCutter_Location - pd->CPM_UpperCutter_Location) > fCPMThreshold)
	{
		sendData(m_cpmLastData->CPM_UpperCutter_Location, pd->CPM_UpperCutter_Location, 20203, 0, 250);
	}

	if (abs(m_cpmLastData->CPM_LowerCutter_Location - pd->CPM_LowerCutter_Location) > fCPMThreshold)
	{
		sendData(m_cpmLastData->CPM_LowerCutter_Location, pd->CPM_LowerCutter_Location, 20204, 0, 250);
	}

	if (abs(m_cpmLastData->CPM_FrameCleanerTrolly_Location - pd->CPM_FrameCleanerTrolly_Location) > fCPMThreshold)
	{
		sendData(m_cpmLastData->CPM_FrameCleanerTrolly_Location, pd->CPM_FrameCleanerTrolly_Location, 20301, 0, 2000);
	}

	if (abs(m_cpmLastData->CPM_FrameCleanMachine_Location - pd->CPM_FrameCleanMachine_Location) > fCPMThreshold)
	{
		sendData(m_cpmLastData->CPM_FrameCleanMachine_Location, pd->CPM_FrameCleanMachine_Location, 20302, 0, 500);
	}

	if (abs(m_cpmLastData->CPM_CokePushRod_Location - pd->CPM_CokePushRod_Location) > fCPMThreshold)
	{
		sendData(m_cpmLastData->CPM_CokePushRod_Location, pd->CPM_CokePushRod_Location, 20401, 0, 25000);
	}

	if (abs(m_cpmLastData->CPM_CoalClearMachine_Location - pd->CPM_CoalClearMachine_Location) > fCPMThreshold)
	{
		sendData(m_cpmLastData->CPM_CoalClearMachine_Location, pd->CPM_CoalClearMachine_Location, 20501, 0, 1500);
	}
	if (fCPMThreshold < 0)
	{
		cout << "CPM_Threshold:" << fCPMThreshold << endl;
		fCPMThreshold++;
	}
}

void JHStateSync::comparisonData(SCMStateData* pd)
{
	if (pd == nullptr)
	{
		cout << "StateData is null" << endl;
		return;
	}
	if (abs(m_scmLastData->SCM_Location - pd->SCM_Location) > fSCMThreshold || abs(m_scmLastData->SCM_MoveSpeed - pd->SCM_MoveSpeed) > fSCMThreshold)
	{
		sendData(m_scmLastData->SCM_Location, pd->SCM_Location, 30001, 0, 180100);
		sendData(m_scmLastData->SCM_MoveSpeed, pd->SCM_MoveSpeed, 30002, 0, 99999);
	}

	if (abs(m_scmLastData->SCM_FrontArmor_Location - pd->SCM_FrontArmor_Location) > fSCMThreshold)
	{
		sendData(m_scmLastData->SCM_FrontArmor_Location, pd->SCM_FrontArmor_Location, 30101, 0, 700);
	}

	if (abs(m_scmLastData->SCM_RearArmor_Location - pd->SCM_RearArmor_Location) > fSCMThreshold)
	{
		sendData(m_scmLastData->SCM_RearArmor_Location, pd->SCM_RearArmor_Location, 30102, 0, 20000);
	}

	if (abs(m_scmLastData->SCM_CoalLoadBaseBoard_Location - pd->SCM_CoalLoadBaseBoard_Location) > fSCMThreshold)
	{
		sendData(m_scmLastData->SCM_CoalLoadBaseBoard_Location, pd->SCM_CoalLoadBaseBoard_Location, 30103, 0, 20000);
	}

	if (abs(m_scmLastData->SCM_RearArmorLock_Location - pd->SCM_RearArmorLock_Location) > fSCMThreshold)
	{
		sendData(m_scmLastData->SCM_RearArmorLock_Location, pd->SCM_RearArmorLock_Location, 30104, 0, 155);
	}             	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         

 	if ( abs(m_scmLastData->SCM_PackFrame_Location - pd->SCM_PackFrame_Location) > fSCMThreshold)
	{
		sendData(m_scmLastData->SCM_PackFrame_Location, pd->SCM_PackFrame_Location, 30201, 0, 150);
	}

	if (abs(m_scmLastData->SCM_CoalPressMachine_Location - pd->SCM_CoalPressMachine_Location) > fSCMThreshold)
	{
		sendData(m_scmLastData->SCM_CoalPressMachine_Location, pd->SCM_CoalPressMachine_Location, 30301, 0, 18000);
	}

	if (abs(m_scmLastData->SCM_CoalWall_Location - pd->SCM_CoalWall_Location) > fSCMThreshold)
	{
		sendData(m_scmLastData->SCM_CoalWall_Location, pd->SCM_CoalWall_Location, 30401, 0, 500);
	}
	if (fSCMThreshold < 0)
	{
		cout << "SCM_Threshold:" << fSCMThreshold << endl;
		fSCMThreshold++;
	}
}

void JHStateSync::comparisonData(CGTCStateData* pd)
{
	if (pd == nullptr)
	{
		cout << "StateData is null" << endl;
		return;
	}
	if (abs(m_cgtcLastData->CGTC_Location - pd->CGTC_Location) > fCGTCThreshold || abs(m_cgtcLastData->CGTC_MoveSpeed - pd->CGTC_MoveSpeed) > fCGTCThreshold)
	{
		sendData(m_cgtcLastData->CGTC_Location, pd->CGTC_Location, 40001, 0, 107000);
		sendData(m_cgtcLastData->CGTC_MoveSpeed, pd->CGTC_MoveSpeed, 40002, 0, 99999);
	}

	if (abs(m_cgtcLastData->CGTC_OvenHeadSmoke_Location - pd->CGTC_OvenHeadSmoke_Location) > fCGTCThreshold)
	{
		sendData(m_cgtcLastData->CGTC_OvenHeadSmoke_Location, pd->CGTC_OvenHeadSmoke_Location, 40101, 0, 1055);
	}

	if (abs(m_cgtcLastData->CGTC_NO1MGuideSleeve_Location - pd->CGTC_NO1MGuideSleeve_Location) > fCGTCThreshold)
	{
		sendData(m_cgtcLastData->CGTC_NO1MGuideSleeve_Location, pd->CGTC_NO1MGuideSleeve_Location, 40201, 0, 675);
	}

	if (abs(m_cgtcLastData->CGTC_NO2MGuideSleeve_Location - pd->CGTC_NO2MGuideSleeve_Location) > fCGTCThreshold)
	{
		sendData(m_cgtcLastData->CGTC_NO2MGuideSleeve_Location, pd->CGTC_NO2MGuideSleeve_Location, 40202, 0, 675);
	}	

	if (abs(m_cgtcLastData->CGTC_NO1LidTakeOff_Location - pd->CGTC_NO1LidTakeOff_Location) > fCGTCThreshold)
	{
		sendData(m_cgtcLastData->CGTC_NO1LidTakeOff_Location, pd->CGTC_NO1LidTakeOff_Location, 40301, 0, 1300);
	}

	if (abs(m_cgtcLastData->CGTC_NO2LidTakeOff_Location - pd->CGTC_NO2LidTakeOff_Location) > fCGTCThreshold)
	{
		sendData(m_cgtcLastData->CGTC_NO2LidTakeOff_Location, pd->CGTC_NO2LidTakeOff_Location, 40302, 0, 1300);
	}

	if (abs(m_cgtcLastData->CGTC_NO3LidTakeOff_Location - pd->CGTC_NO3LidTakeOff_Location) > fCGTCThreshold)
	{
		sendData(m_cgtcLastData->CGTC_NO3LidTakeOff_Location, pd->CGTC_NO3LidTakeOff_Location, 40303, 0, 1300);
	}

	if (abs(m_cgtcLastData->CGTC_NO4LidTakeOff_Location - pd->CGTC_NO4LidTakeOff_Location) > fCGTCThreshold)
	{
		sendData(m_cgtcLastData->CGTC_NO4LidTakeOff_Location, pd->CGTC_NO4LidTakeOff_Location, 40304, 0, 1300);
	}

	if (abs(m_cgtcLastData->CGTC_NO5LidTakeOff_Location - pd->CGTC_NO5LidTakeOff_Location) > fCGTCThreshold)
	{
		sendData(m_cgtcLastData->CGTC_NO5LidTakeOff_Location, pd->CGTC_NO5LidTakeOff_Location, 40305, 0, 1300);
	}

	if (abs(m_cgtcLastData->CGTC_NO6LidTakeOff_Location - pd->CGTC_NO6LidTakeOff_Location) > fCGTCThreshold)
	{
		sendData(m_cgtcLastData->CGTC_NO6LidTakeOff_Location, pd->CGTC_NO6LidTakeOff_Location, 40306, 0, 1300);
	}	
	if (fCGTCThreshold < 0)
	{
		cout << "CGTC_Threshold:" << fCGTCThreshold << endl;
		fCGTCThreshold++;
	}
}

void JHStateSync::comparisonData(HCBCStateData* pd)
{
	if (pd == nullptr)
	{
		cout << "StateData is null" << endl;
		return;
	}
	if (abs(m_hcbcLastData->HCBC_Location - pd->HCBC_Location) > fHCBCThreshold || abs(m_hcbcLastData->HCBC_MoveSpeed - pd->HCBC_MoveSpeed) > fHCBCThreshold)
	{
		sendData(m_hcbcLastData->HCBC_Location, pd->HCBC_Location, 50001, 0, 369100);
		sendData(m_hcbcLastData->HCBC_MoveSpeed, pd->HCBC_MoveSpeed, 50002, 0, 99999);
	}

	if (abs(m_hcbcLastData->HCBC_OutsideFlipBoard_Location - pd->HCBC_OutsideFlipBoard_Location) > fHCBCThreshold)
	{
		sendData(m_hcbcLastData->HCBC_OutsideFlipBoard_Location, pd->HCBC_OutsideFlipBoard_Location, 50003, 0, 1000);
	}

	if (abs(m_hcbcLastData->HCBC_InsideFlipBoard_Location - pd->HCBC_InsideFlipBoard_Location) > fHCBCThreshold)
	{
		sendData(m_hcbcLastData->HCBC_InsideFlipBoard_Location, pd->HCBC_InsideFlipBoard_Location, 50004, 0, 350);
	}
	if (fHCBCThreshold < 0)
	{
		cout << "HCBC_Threshold:" << fHCBCThreshold << endl;
		fHCBCThreshold++;
	}
}

