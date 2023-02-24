#include "JHOperateSync.h"

extern float fCTCThreshold;
extern float fCPMThreshold;
extern float fSCMThreshold;
extern float fCGTCThreshold;
extern float fHCBCThreshold;

JHOperateSync::JHOperateSync()
{
	m_recvFromUE = nullptr;
}

JHOperateSync::JHOperateSync(SocketUDP* recvFromUE)
{
	m_recvFromUE = recvFromUE;
	m_host = "";
}

void JHOperateSync::operator()()
{
	startOperateSync();
}

void JHOperateSync::sendData(UA_Boolean value, UA_NodeId nodeId)
{
	UA_Client* client = UA_Client_new();

	UA_ClientConfig_setDefault(UA_Client_getConfig(client));

	UA_StatusCode retval = UA_Client_connect(client, m_host);

	if (retval != UA_STATUSCODE_GOOD)
	{
		UA_Client_delete(client);
		cout << "Connect OPCUA is failed, Error code:" << (int)retval << endl;
	}

	UA_Variant* myVariant = UA_Variant_new();

	UA_Variant_setScalarCopy(myVariant, &value, &UA_TYPES[UA_TYPES_BOOLEAN]);

	retval = UA_Client_writeValueAttribute(client, nodeId, myVariant);

	if (retval != UA_STATUSCODE_GOOD)
	{
		cout << (int)retval << endl;
	}

	UA_Variant_delete(myVariant);
	UA_Client_delete(client);
}

void JHOperateSync::sendData(UA_Int16 value, UA_NodeId nodeId)
{
	UA_Client* client = UA_Client_new();

	UA_ClientConfig_setDefault(UA_Client_getConfig(client));

	UA_StatusCode retval = UA_Client_connect(client, m_host);

	if (retval != UA_STATUSCODE_GOOD)
	{
		UA_Client_delete(client);
		cout << "Connect OPCUA is failed, Error code:" << (int)retval << endl;
	}

	UA_Variant* myVariant = UA_Variant_new();

	UA_Variant_setScalarCopy(myVariant, &value, &UA_TYPES[UA_TYPES_INT16]);

	retval = UA_Client_writeValueAttribute(client, nodeId, myVariant);

	if (retval != UA_STATUSCODE_GOOD)
	{
		cout << (int)retval << endl;
	}

	UA_Variant_delete(myVariant);
	UA_Client_delete(client);
}

void JHOperateSync::comparisonData(UINT8 key, const char* str1)
{
	if (key == 1)
	{
		sendData(true, UA_NODEID_STRING(3, const_cast<char*>(str1)));
	}
	else
	{
		sendData(false, UA_NODEID_STRING(3, const_cast<char*>(str1)));
	}
}

void JHOperateSync::comparisonData(UINT8 key, const char* str1, const char* str2)
{
	if(key == 1)
	{
		sendData(true, UA_NODEID_STRING(3, const_cast<char*>(str1)));
	}
	else if (key == 2)
	{
		sendData(true, UA_NODEID_STRING(3, const_cast<char*>(str2)));
	}
	else
	{
		sendData(false, UA_NODEID_STRING(3, const_cast<char*>(str1)));
		sendData(false, UA_NODEID_STRING(3, const_cast<char*>(str2)));
	}
}

//void JHOperateSync::comparisonData(UINT8 key, const char* str1, const char* str2, const char* str3)
//{
//	sendData(false, UA_NODEID_STRING(3, const_cast<char*>(str1)));
//	sendData(false, UA_NODEID_STRING(3, const_cast<char*>(str2)));
//	sendData(false, UA_NODEID_STRING(3, const_cast<char*>(str3)));
//
//	if (key == 0)
//	{
//		sendData(true, UA_NODEID_STRING(3, const_cast<char*>(str1)));
//	}
//	else if (key == 1)
//	{
//		sendData(true, UA_NODEID_STRING(3, const_cast<char*>(str2)));
//	}
//	else
//	{
//		sendData(true, UA_NODEID_STRING(3, const_cast<char*>(str3)));
//	}
//}

void JHOperateSync::startOperateSync()
{
	char revData[1024];
	memset(revData, 0, sizeof(revData));

	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);

	if (bind(m_recvFromUE->svr, (LPSOCKADDR)&m_recvFromUE->sin, sizeof(m_recvFromUE->sin)) == SOCKET_ERROR)
	{
		cout << "\nbind error !\n" << endl;
	}

	while (true)
	{
		int ret_len = recvfrom(m_recvFromUE->svr, revData, 1024, 0, (SOCKADDR*)&remoteAddr, &nAddrlen);

		if (SOCKET_ERROR == ret_len)
		{
			cout << "\nrecvfrom error !\n" << endl;
			Sleep(3000);
			break;
		}

		OperateSyncProtocol aData;
		memcpy(&aData, revData, sizeof(aData));

		if (aData.key == 1001)
		{
			fCTCThreshold = STATE_INIT_NUM;
			fCPMThreshold = STATE_INIT_NUM;
			fSCMThreshold = STATE_INIT_NUM;
			fCGTCThreshold = STATE_INIT_NUM;
			fHCBCThreshold = STATE_INIT_NUM;
			continue;
		}

		if (11000 < aData.key && aData.key < 12000)
		{
			m_host = "opc.tcp://192.168.0.7:4840";
			ctcOperateSync(aData.key, aData.value);	
		}
		else if (21000 < aData.key && aData.key < 22000)
		{
			m_host = "opc.tcp://192.168.0.5:4840";
			cpmOperateSync(aData.key, aData.value);	
		}
		else if (31000 < aData.key && aData.key < 32000)
		{
			m_host = "opc.tcp://192.168.0.6:4840";
			scmOperateSync(aData.key, aData.value);
		}
		else if (41000 < aData.key && aData.key < 42000)
		{
			m_host = "opc.tcp://192.168.0.4:4840";
			cgtcOperateSync(aData.key, aData.value);
		}
		else if (51000 < aData.key && aData.key < 52000)
		{
			m_host = "opc.tcp://192.168.0.8:4840";
			hcbcOperateSync(aData.key, aData.value);
		}
	}
}

void JHOperateSync::ctcOperateSync(int key, UINT8 value)
{
	switch (key)
	{
	case 11001:
		comparisonData(value, "\"DB200  HMI\".\"Manual-MoveLeft\"", "\"DB200  HMI\".\"Manual-MoveRight\"");
		break;
	case 11101:
		comparisonData(value, "\"DB200  HMI\".\"Manual-TakeDoorTrolleyBackward\"", "\"DB200  HMI\".\"Manual-TakeDoorTrolleyForward\"");
		break;
	case 11102:
		break;
	case 11103:	
		comparisonData(value, "\"DB200  HMI\".\"Manual-LatchBack\"", "\"DB200  HMI\".\"Manual-LatchPressIn\"");
		break;	
	case 11104:	
		comparisonData(value, "\"DB200  HMI\".\"Manual-LatchTurnOpen\"", "\"DB200  HMI\".\"Manual-LatchTurnClose\"");
		break;	
	case 11105:	
		comparisonData(value, "\"DB200  HMI\".\"Manual-LiftDoorUp\"", "\"DB200  HMI\".\"Manual-LiftDoorDown\"");
		break;	
	case 11106:
		comparisonData(value, "\"DB200  HMI\".\"Manual-LiftDoorHookUp\"", "\"DB200  HMI\".\"Manual-LiftDoorHookDown\"");
		break;
	case 11201:	
		comparisonData(value, "\"DB200  HMI\".\"Manual-CokeTrayUp\"", "\"DB200  HMI\".\"Manual-CokeTrayDown\"");
		break;	
	case 11202:	
		comparisonData(value, "\"DB200  HMI\".\"Manual-CokeTrayBack\"", "\"DB200  HMI\".\"Manual-CokeTrayExtend\"");
		break;
	case 11301:	
		comparisonData(value, "\"DB200  HMI\".\"Manual-CleanDoorTrolleyBackward\"", "\"DB200  HMI\".\"Manual-CleanDoorTrolleyForward\"");
		break;
	case 11302:	
		comparisonData(value, "\"DB200  HMI\".\"Manual-SiderCutterRunUp\"", "\"DB200  HMI\".\"Manual-SiderCutterRunDown\"");
		break;	
	case 11303:	
		comparisonData(value, "\"DB200  HMI\".\"Manual-SiderCutterClose\"", "\"DB200  HMI\".\"Manual-SiderCutterOpen\"");
		break;	
	case 11304:	
		comparisonData(value, "\"DB200  HMI\".\"Manual-UpperCutterRunLeft\"", "\"DB200  HMI\".\"Manual-UpperCutterRunRight\"");
		break;	
	case 11305:
		comparisonData(value, "\"DB200  HMI\".\"Manual-LowerCutterRunLeft\"", "\"DB200  HMI\".\"Manual-LowerCutterRunRight\"");
		break;
	case 11401:	
		comparisonData(value, "\"DB200  HMI\".\"Manual-CleanFrameTrolleyBackward\"", "\"DB200  HMI\".\"Manual-CleanFrameTrolleyForward\"");
		break;	
	case 11402:
		comparisonData(value, "\"DB200  HMI\".\"Manual-CleanFrameMachineUp\"", "\"DB200  HMI\".\"Manual-CleanFrameMachineDown\"");
		break;	
	case 11501:
		comparisonData(value, "\"DB200  HMI\".\"Manual-CokeGridBackward\"", "\"DB200  HMI\".\"Manual-CokeGridForward\"");
		break;	
	case 11502:
		comparisonData(value, "\"DB200  HMI\".\"Manual-CokeGridClose\"", "\"DB200  HMI\".\"Manual-CokeGridOpen\"");
		break;
	case 11901:
		comparisonData(value, "\"DB200  HMI\".\"Sys-ManualMode\"");
		break;
	case 11902:
		comparisonData(value, "\"DB200  HMI\".\"Sys-SemiAutoMode\"");
		break;
	case 11903:
		comparisonData(value, "\"DB200  HMI\".\"Sys-AutoMode\"");
		break;
	case 11904:
		sendData((UA_Int16)value, UA_NODEID_STRING(3, const_cast<char*>("\"DB200  HMI\".\"Sys-GoalStove\"")));
		break;
	case 11905:
		comparisonData(value, "\"DB200  HMI\".\"SemiAuto-MoveToGoalStove\"");
		break;
	case 11906:
		comparisonData(value, "\"DB200  HMI\".\"SemiAuto-GetDoor\"", "\"DB200  HMI\".\"SemiAuto-PutDoor\"");
		break;
	case 11907:
		comparisonData(value, "\"DB200  HMI\".\"SemiAuto-CokeTrayClose\"", "\"DB200  HMI\".\"SemiAuto-CokeTrayOpen\"");
		break;
	case 11908:
		comparisonData(value, "\"DB200  HMI\".\"SemiAuto-CleanDoor\"");
		break;
	case 11909:
		comparisonData(value, "\"DB200  HMI\".\"SemiAuto-CleanDoorFrame\"");
		break;
	case 11910:
		comparisonData(value, "\"DB200  HMI\".\"SemiAuto-CokeGridBackward\"", "\"DB200  HMI\".\"SemiAuto-CokeGridForward\"");
		break;
	case 11911:
		comparisonData(value, "\"DB200  HMI\".\"Sys-MaintenanceMode\"");
		break;
	default:
		break;
	}
}

void JHOperateSync::cpmOperateSync(int key, UINT8 value)
{
	switch (key)
	{
	case 21001:
		comparisonData(value, "\"DB200  HMI\".\"Manual-MoveLeft\"", "\"DB200  HMI\".\"Manual-MoveRight\"");
		break;
	case 21101:
		comparisonData(value, "\"DB200  HMI\".\"Manual-TakeDoorTrolleyBackward\"", "\"DB200  HMI\".\"Manual-TakeDoorTrolleyForward\"");
		break;
	case 21102:
		comparisonData(value, "\"DB200  HMI\".\"Manual-CokeGridForward\"", "\"DB200  HMI\".\"Manual-CokeGridForward\"");
		break;
	case 21103:
		comparisonData(value, "\"DB200  HMI\".\"Manual-LatchBack\"", "\"DB200  HMI\".\"Manual-LatchPressIn\"");
		break;
	case 21104:
		comparisonData(value, "\"DB200  HMI\".\"Manual-LatchTurnOpen\"", "\"DB200  HMI\".\"Manual-LatchTurnClose\"");
		break;
	case 21105:
		comparisonData(value, "\"DB200  HMI\".\"Manual-LiftDoorUp\"", "\"DB200  HMI\".\"Manual-LiftDoorDown\"");
		break;
	case 21106:
		comparisonData(value, "\"DB200  HMI\".\"Manual-LiftDoorHookUp\"", "\"DB200  HMI\".\"Manual-LiftDoorHookDown\"");
		break;
	case 21201:
		comparisonData(value, "\"DB200  HMI\".\"Manual-CleanDoorTrolleyBackward\"", "\"DB200  HMI\".\"Manual-CleanDoorTrolleyForward\"");
		break;
	case 21202:
		comparisonData(value, "\"DB200  HMI\".\"Manual-SiderCutterRunUp\"", "\"DB200  HMI\".\"Manual-SiderCutterRunDown\"");
		break;
	case 21203:
		comparisonData(value, "\"DB200  HMI\".\"Manual-SiderCutterClose\"", "\"DB200  HMI\".\"Manual-SiderCutterOpen\"");
		break;
	case 21204:
		comparisonData(value, "\"DB200  HMI\".\"Manual-UpperCutterRunLeft\"", "\"DB200  HMI\".\"Manual-UpperCutterRunRight\"");
		break;
	case 21205:
		comparisonData(value, "\"DB200  HMI\".\"Manual-LowerCutterRunLeft\"", "\"DB200  HMI\".\"Manual-LowerCutterRunRight\"");
		break;
	case 21301:
		comparisonData(value, "\"DB200  HMI\".\"Manual-CleanFrameTrolleyBackward\"", "\"DB200  HMI\".\"Manual-CleanFrameTrolleyForward\"");
		break;
	case 21302:
		comparisonData(value, "\"DB200  HMI\".\"Manual-CleanFrameMachineUp\"", "\"DB200  HMI\".\"Manual-CleanFrameMachineDown\"");
		break;
	case 21401:
		comparisonData(value, "\"DB200  HMI\".\"Manual-CokePushingRodBackward\"", "\"DB200  HMI\".\"Manual-CokePushingRodForward\"");
		break;
	case 21501:
		comparisonData(value, "\"DB200  HMI\".\"Manual-CleanCoalMachineBackward\"", "\"DB200  HMI\".\"Manual-CleanCoalMachineForward\"");
		break;
	case 21901:
		comparisonData(value, "\"DB200  HMI\".\"Sys-ManualMode\"");
		break;
	case 21902:
		comparisonData(value, "\"DB200  HMI\".\"Sys-SemiAutoMode\"");
		break;
	case 21903:
		comparisonData(value, "\"DB200  HMI\".\"Sys-AutoMode\"");
		break;
	case 21904:
		sendData((UA_Int16)value, UA_NODEID_STRING(3, const_cast<char*>("\"DB200  HMI\".\"Sys-GoalStove\"")));
		break;
	case 21905:
		comparisonData(value, "\"DB200  HMI\".\"SemiAuto-MoveToGoalStove\"");
		break;
	case 21906:
		comparisonData(value, "\"DB200  HMI\".\"SemiAuto-GetDoor\"", "\"DB200  HMI\".\"SemiAuto-PutDoor\"");
		break;
	case 21907:
		comparisonData(value, "\"DB200  HMI\".\"SemiAuto-CleanDoor\"");
		break;
	case 21908:
		comparisonData(value, "\"DB200  HMI\".\"SemiAuto-CleanDoorFrame\"");
		break;
	case 21909:
		comparisonData(value, "\"DB200  HMI\".\"SemiAuto-CokePushRodToDoor\"");
		break;
	case 21910:
		comparisonData(value, "\"DB200  HMI\".\"SemiAuto-CokePush\"");
		break;
	case 21911:
		comparisonData(value, "\"DB200  HMI\".\"SemiAuto-CokePushRodBack\"");
		break;
	case 21912:
		comparisonData(value, "\"DB200  HMI\".\"SemiAuto-MoveToCoalCleanLoc\"");
		break;
	case 21913:
		comparisonData(value, "\"DB200  HMI\".\"SemiAuto-CoalClean\"");
		break;
	default:
		break;
	}
}

void JHOperateSync::scmOperateSync(int key, UINT8 value)
{
	switch (key)
	{
	case 31001:
		comparisonData(value, "\"DB200  HMI\".\"Manual-MoveLeft\"", "\"DB200  HMI\".\"Manual-MoveRight\"");
		break;
	case 31101:
		comparisonData(value, "\"DB200  HMI\".\"Manual-FrontArmorClose\"", "\"DB200  HMI\".\"Manual-FrontArmorOpen\"");
		break;
	case 31102:
		comparisonData(value, "\"DB200  HMI\".\"Manual-RearArmorBackward\"", "\"DB200  HMI\".\"Manual-RearArmorForward\"");
		break;
	case 31103:
		comparisonData(value, "\"DB200  HMI\".\"Manual-RearArmorLockClose\"", "\"DB200  HMI\".\"Manual-RearArmorLockOpen\"");
		break;
	case 31104:
		comparisonData(value, "\"DB200  HMI\".\"Manual-CoalLoadBaseBoardBackward\"", "\"DB200  HMI\".\"Manual-CoalLoadBaseBoardForward\"");
		break;
	case 31201:
		comparisonData(value, "\"DB200  HMI\".\"Manual-PackFrameBackward\"", "\"DB200  HMI\".\"Manual-PackFrameForward\"");
		break;
	case 31301:
		comparisonData(value, "\"DB200  HMI\".\"Manual-CoalPressMachineBackward\"", "\"DB200  HMI\".\"Manual-CoalPressMachineForward\"");
		break;
	case 31401:
		comparisonData(value, "\"DB200  HMI\".\"Manual-CoalWallClose\"", "\"DB200  HMI\".\"Manual-CoalWallOpen\"");
		break;
	case 31501:
		comparisonData(value, "\"DB200  HMI\".\"Manual-DustCleanBoardClose\"", "\"DB200  HMI\".\"Manual-DustCleanBoardOpen\"");
		break;
	case 31601:
		comparisonData(value, "\"DB200  HMI\".\"Manual-RemainCoalCollectionClose\"", "\"DB200  HMI\".\"Manual-RemainCoalCollectionOpen\"");
		break;
	case 31901:
		comparisonData(value, "\"DB200  HMI\".\"Sys-ManualMode\"");
		break;
	case 31902:
		comparisonData(value, "\"DB200  HMI\".\"Sys-SemiAutoMode\"");
		break;
	case 31903:
		comparisonData(value, "\"DB200  HMI\".\"Sys-AutoMode\"");
		break;
	case 31904:
		sendData((UA_Int16)value, UA_NODEID_STRING(3, const_cast<char*>("\"DB200  HMI\".\"Sys-GoalStove\"")));
		break;
	case 31905:
		comparisonData(value, "\"DB200  HMI\".\"SemiAuto-MoveToGoalStove\"");
		break;
	case 31906:
		comparisonData(value, "\"DB200  HMI\".\"SemiAuto-ChangeToTampStatus\"");
		break;
	case 31907:
		comparisonData(value, "\"DB200  HMI\".\"SemiAuto-CoalTamp\"");
		break;
	case 31908:
		comparisonData(value, "\"DB200  HMI\".\"SemiAuto-ChangeToCoalPushStatus\"");
		break;
	case 31909:
		comparisonData(value, "\"DB200  HMI\".\"SemiAuto-CoalPress\"");
		break;
	case 31910:
		comparisonData(value, "\"DB200  HMI\".\"SemiAuto-CoalPush\"");
		break;
	default:
		break;
	}
}

void JHOperateSync::cgtcOperateSync(int key, UINT8 value)
{
	switch (key)
	{
	case 41001:
		comparisonData(value, "\"DB500  ControlFromUE\".\"Manual-RUN_LEFT\"", "\"DB500  ControlFromUE\".\"Manual-RUN_RIGHT\"");
		break;
	case 41101:
		comparisonData(value, "\"DB500  ControlFromUE\".\"Manual-Head_Smoke_Collection_Trolley_Back\"", "\"DB500  ControlFromUE\".\"Manual-Head_Smoke_Collection_Trolley_Forward\"");
		break;
	case 41201:
		comparisonData(value, "\"DB500  ControlFromUE\".\"Manual-1#U_Guide_Sleeve_Lowering\"", "\"DB500  ControlFromUE\".\"Manual-1#U_Guide_Sleeve_Rising\"");
		break;
	case 41202:
		comparisonData(value, "\"DB500  ControlFromUE\".\"Manual-2#U_Guide_Sleeve_Lowering\"", "\"DB500  ControlFromUE\".\"Manual-2#U_Guide_Sleeve_Rising\"");
		break;
	case 41301:
		comparisonData(value, "\"DB500  ControlFromUE\".\"Manual-1#Uncovering_Lowering\"", "\"DB500  ControlFromUE\".\"Manual-1#Uncovering_Lifting\"");
		break;
	case 41302:
		comparisonData(value, "\"DB500  ControlFromUE\".\"Manual-2#Uncovering_Lowering\"", "\"DB500  ControlFromUE\".\"Manual-2#Uncovering_Lifting\"");
		break;
	case 41303:
		comparisonData(value, "\"DB500  ControlFromUE\".\"Manual-3#Uncovering_Lowering\"", "\"DB500  ControlFromUE\".\"Manual-3#Uncovering_Lifting\"");
		break;
	case 41304:
		comparisonData(value, "\"DB500  ControlFromUE\".\"Manual-4#Uncovering_Lowering\"", "\"DB500  ControlFromUE\".\"Manual-4#Uncovering_Lifting\"");
		break;
	case 41305:
		comparisonData(value, "\"DB500  ControlFromUE\".\"Manual-5#Uncovering_Lowering\"", "\"DB500  ControlFromUE\".\"Manual-5#Uncovering_Lifting\"");
		break;
	case 41306:
		comparisonData(value, "\"DB500  ControlFromUE\".\"Manual-6#Uncovering_Lowering\"", "\"DB500  ControlFromUE\".\"Manual-6#Uncovering_Lifting\"");
		break;
	case 41401:
		comparisonData(value, "\"DB500  ControlFromUE\".\"Manual-1#Release_Close\"", "\"DB500  ControlFromUE\".\"Manual-1#Release_Open\"");
		break;
	case 41402:
		comparisonData(value, "\"DB500  ControlFromUE\".\"Manual-2#Release_Close\"", "\"DB500  ControlFromUE\".\"Manual-2#Release_Open\"");
		break;
	case 41403:
		comparisonData(value, "\"DB500  ControlFromUE\".\"Manual-3#Release_Close\"", "\"DB500  ControlFromUE\".\"Manual-3#Release_Open\"");
		break;
	case 41404:
		comparisonData(value, "\"DB500  ControlFromUE\".\"Manual-4#Release_Close\"", "\"DB500  ControlFromUE\".\"Manual-4#Release_Open\"");
		break;
	case 41405:
		comparisonData(value, "\"DB500  ControlFromUE\".\"Manual-5#Release_Close\"", "\"DB500  ControlFromUE\".\"Manual-5#Release_Open\"");
		break;
	case 41406:
		comparisonData(value, "\"DB500  ControlFromUE\".\"Manual-6#Release_Close\"", "\"DB500  ControlFromUE\".\"Manual-6#Release_Open\"");
		break;
	case 41501:
		comparisonData(value, "\"DB500  ControlFromUE\".\"Manual-Back_Of_Riser_Cleaning\"", "\"DB500  ControlFromUE\".\"Manual-Advance_Of_Riser_Cleaning\"");
		break;
	case 41502:
		comparisonData(value, "\"DB500  ControlFromUE\".\"Manual-Down_Of_Riser_Cleaning\"", "\"DB500  ControlFromUE\".\"Manual-Up_Of_Riser_Cleaning\"");
		break;
	case 41901:
		comparisonData(value, "\"DB500  ControlFromUE\".\"Sys-ManualMode\"");
		break;
	case 41902:
		comparisonData(value, "\"DB500  ControlFromUE\".\"Sys-SemiAutoMode\"");
		break;
	case 41903:
		comparisonData(value, "\"DB500  ControlFromUE\".\"Sys-AutoMode\"");
		break;
	case 41904:
		sendData((UA_Int16)value, UA_NODEID_STRING(3, const_cast<char*>("\"DB500  ControlFromUE\".\"Sys-Targe_Oven\"")));
		break;
	case 41905:
		comparisonData(value, "\"DB500  ControlFromUE\".\"SemiAuto-MoveToGoalStove\"");
		break;
	case 41906:
		comparisonData(value, "\"DB500  ControlFromUE\".\"SemiAuto-SmokeGuideStart\"");
		break;
	case 41907:
		comparisonData(value, "\"DB500  ControlFromUE\".\"SemiAuto-SmokeGuideStop\"");
		break;
	default:
		break;
	}
}

void JHOperateSync::hcbcOperateSync(int key, UINT8 value)
{
	switch (key)
	{
	case 51001:
		comparisonData(value, "\"DB200  HMI\".\"Manual-MoveLeft\"", "\"DB200  HMI\".\"Manual-MoveRight\"");
		break;
	case 51101:
		comparisonData(value, "\"DB200  HMI\".\"Manual-OutsideFlipBoardClose\"", "\"DB200  HMI\".\"Manual-OutsideFlipBoardOpen\"");
		break;
	case 51102:
		comparisonData(value, "\"DB200  HMI\".\"Manual-InsideFlipBoardClose\"", "\"DB200  HMI\".\"Manual-InsideFlipBoardOpen\"");
		break;
	case 51201:
		comparisonData(value, "\"DB200  HMI\".\"Manual-1#HydrovalveClose\"", "\"DB200  HMI\".\"Manual-1#HydrovalveOpen\"");
		break;
	case 51202:
		comparisonData(value, "\"DB200  HMI\".\"Manual-2#HydrovalveClose\"", "\"DB200  HMI\".\"Manual-2#HydrovalveOpen\"");
		break;
	case 51203:
		comparisonData(value, "\"DB200  HMI\".\"Manual-3#HydrovalveClose\"", "\"DB200  HMI\".\"Manual-3#HydrovalveOpen\"");
		break;
	case 51204:
		comparisonData(value, "\"DB200  HMI\".\"Manual-4#HydrovalveClose\"", "\"DB200  HMI\".\"Manual-4#HydrovalveOpen\"");
		break;
	case 51901:
		comparisonData(value, "\"DB200  HMI\".\"Sys-ManualMode\"");
		break;
	case 51902:
		comparisonData(value, "\"DB200  HMI\".\"Sys-SemiAutoMode\"");
		break;
	case 51903:
		comparisonData(value, "\"DB200  HMI\".\"Sys-AutoMode\"");
		break;
	case 51904:
		sendData((UA_Int16)value, UA_NODEID_STRING(3, const_cast<char*>("\"DB200  HMI\".\"Sys-GoalStove\"")));
		break;
	case 51905:
		comparisonData(value, "\"DB200  HMI\".\"SemiAuto-MoveToGoalStove\"");
		break;
	case 51906:
		comparisonData(value, "\"DB200  HMI\".\"SemiAuto-LoadCokeStatus\"");
		break;
	case 51907:
		comparisonData(value, "\"DB200  HMI\".\"SemiAuto-CokeExtinguish\"");
		break;
	case 51908:
		comparisonData(value, "\"DB200  HMI\".\"SemiAuto-CokeUnload\"");
		break;
	default:
		break;
	}
}
