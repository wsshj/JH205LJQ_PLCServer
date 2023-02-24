#pragma once

#include <iostream>
#include <fstream>
#include "JHNetwork.h"

using namespace std;

//extern float m_fThreshold;

// ������״̬����
struct CTCStateData
{
	float CTC_Location; //����λ��ֵ
	float CTC_LiftDoor_Location; //���Ż�λ��ֵ
	float CTC_GetDoorTrolly_Location; //ȡ��̨��λ��ֵ
	float CTC_LatchPressIn_Location; //����ѹ��״ֵ̬
	float CTC_LatchTurn_Location; //������ת״ֵ̬
	float CTC_CokeTrayUpDown_Location; //��������
	float CTC_CokeTrayFrontRear_Location; //����ǰ��
	float CTC_DoorCleanerTrolly_Location; //����̨��λ��ֵ
	float CTC_SideCutter_Location; //��ϳ��λ��ֵ
	float CTC_UpperCutter_Location; //��ϳ��λ��ֵ
	float CTC_LowerCutter_Location; //��ϳ��λ��ֵ
	float CTC_FrameCleanerTrolly_Location; //���̨��λ��ֵ
	float CTC_FrameCleanMachine_Location; //����λ��ֵ
	float CTC_CokeGrid_Location; //����դλ��ֵ
	float CTC_MoveSpeed; //�������ƶ��ٶ�
};

// �ƽ���״̬����
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
	float CPM_MoveSpeed; //�������ƶ��ٶ�
};

// װú��״̬����
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
	float SCM_MoveSpeed; //�������ƶ��ٶ�
};

// ���̳�״̬����
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
	float CGTC_MoveSpeed; //�������ƶ��ٶ�
};

// Ϩ����״̬����
struct HCBCStateData
{
	float HCBC_Location;
	float HCBC_OutsideFlipBoard_Location;
	float HCBC_InsideFlipBoard_Location;
	float HCBC_MoveSpeed; //�������ƶ��ٶ�
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

	vector<SocketUDP*> m_sendToUE;
	UA_Client* m_recvFromPLC;

	int m_vehicleType;

public:
	JHStateSync();

	JHStateSync(UA_Client* recvFromPLC, vector<SocketUDP*> sendToUE, int vehicleType);

	~JHStateSync();

	void operator()();

	template <typename T>
	T recvData(const char* strNodeId);

	bool recvBoolData(const char * strNodeId);

	//void startStateSync(UA_NodeId nodeId);
	void startStateSync();

	void sendData(float& lastValue, float newValue, INT32 key, float min, float max);

	//void sendData(bool& lastValue, bool newValue, INT32 key);

	void comparisonData(CTCStateData pd);

	void comparisonData(CPMStateData pd);

	void comparisonData(SCMStateData pd);

	void comparisonData(CGTCStateData pd);

	void comparisonData(HCBCStateData pd);
};

