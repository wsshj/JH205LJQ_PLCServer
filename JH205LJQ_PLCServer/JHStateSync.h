#pragma once

#include <iostream>
#include <fstream>
#include "JHNetwork.h"

using namespace std;

//extern float m_fThreshold;

// 拦焦车状态数据
struct CTCStateData
{
	float CTC_Location; //行走位置值
	float CTC_LiftDoor_Location; //提门机位置值
	float CTC_GetDoorTrolly_Location; //取门台车位置值
	float CTC_LatchPressIn_Location; //门闩压紧状态值
	float CTC_LatchTurn_Location; //门闩旋转状态值
	float CTC_CokeTrayUpDown_Location; //焦盘升降
	float CTC_CokeTrayFrontRear_Location; //焦盘前后
	float CTC_DoorCleanerTrolly_Location; //清门台车位置值
	float CTC_SideCutter_Location; //侧铣刀位置值
	float CTC_UpperCutter_Location; //上铣刀位置值
	float CTC_LowerCutter_Location; //下铣刀位置值
	float CTC_FrameCleanerTrolly_Location; //清框台车位置值
	float CTC_FrameCleanMachine_Location; //清框机位置值
	float CTC_CokeGrid_Location; //导焦栅位置值
	float CTC_MoveSpeed; //拦焦车移动速度
};

// 推焦车状态数据
struct CPMStateData
{
	float CPM_Location;
	float CPM_LiftDoor_Location;
	float CPM_CokePushRod_Location;
	float CPM_GetDoorTrolly_Location;
	float CPM_LatchPressIn_Location;
	float CPM_LatchTurn_Location;
	float CPM_DoorCleanerTrolly_Location;
	float CPM_SideCutter_Location;
	float CPM_UpperCutter_Location;
	float CPM_LowerCutter_Location;
	float CPM_FrameCleanerTrolly_Location;
	float CPM_FrameCleanMachine_Location;
	float CPM_CoalClearMachine_Location;
	float CPM_MoveSpeed; //拦焦车移动速度
};

// 装煤车状态数据
struct SCMStateData
{
	float SCM_Location;
	float SCM_FrontArmor_Location;
	float SCM_CoalLoadBaseBoard_Location;
	float SCM_RearArmor_Location;
	float SCM_RearArmorLock_Location;
	float SCM_PackFrame_Location;
	float SCM_CoalPressMachine_Location;
	float SCM_CoalWall_Location;
	float SCM_MoveSpeed; //拦焦车移动速度
};

// 导烟车状态数据
struct CGTCStateData
{
	float CGTC_Location;
	float CGTC_NO1MGuideSleeve_Location;
	float CGTC_NO2MGuideSleeve_Location;
	float CGTC_NO1LidTakeOff_Location;
	float CGTC_NO2LidTakeOff_Location;
	float CGTC_NO3LidTakeOff_Location;
	float CGTC_NO4LidTakeOff_Location;
	float CGTC_NO5LidTakeOff_Location;
	float CGTC_NO6LidTakeOff_Location;
	float CGTC_OvenHeadSmoke_Location;
	float CGTC_MoveSpeed; //拦焦车移动速度
};

// 熄焦车状态数据
struct HCBCStateData
{
	float HCBC_Location;
	float HCBC_OutsideFlipBoard_Location;
	float HCBC_InsideFlipBoard_Location;
	float HCBC_MoveSpeed; //拦焦车移动速度
};

struct StateSyncProtocol
{
	INT32 key;
	bool state;
	UINT8 signal;
	float value;
};

class JHStateSync
{
private:
	CTCStateData* m_ctcLastData;
	CPMStateData* m_cpmLastData;
	SCMStateData* m_scmLastData;
	CGTCStateData* m_cgtcLastData;
	HCBCStateData* m_hcbcLastData;

	SocketUDP* m_sendToUE;
	UA_Client* m_recvFromPLC;

	int m_vehicleType;

public:
	JHStateSync();

	JHStateSync(UA_Client* recvFromPLC, SocketUDP* sendToUE, int vehicleType);

	~JHStateSync();

	void operator()();

	template <typename T>
	T* recvData(const char* strNodeId);

	bool recvBoolData(const char * strNodeId);

	//void startStateSync(UA_NodeId nodeId);
	void startStateSync();

	void sendData(float& lastValue, float newValue, INT32 key, float min, float max);

	void sendData(bool& lastValue, bool newValue, INT32 key);

	void comparisonData(CTCStateData* pd);

	void comparisonData(CPMStateData* pd);

	void comparisonData(SCMStateData* pd);

	void comparisonData(CGTCStateData* pd);

	void comparisonData(HCBCStateData* pd);
};

