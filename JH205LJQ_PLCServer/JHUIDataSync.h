#pragma once

#include <iostream>
#include <fstream>
#include "JHNetwork.h"

using namespace std;

struct CTCStates
{
	bool CTCMoving;			// ��ʻ��
	bool CTCRunOver;		// ��ʻ����
	bool DoorGeting;		// ȡ����
	bool DoorGeted;			// ȡ�Ž���
	bool DoorPuting;		// ������
	bool DoorPuted;			// ���ŵ�λ
	bool DoorCleaning;		// ������
	bool DoorCleaned;		// ����ֹͣ
	bool DoorFrameCleaning;	// �����
	bool DoorFrameCleaned;	// ���ֹͣ
	bool CokePushing;		// �ƽ���
	bool CokePushed;		// �ƽ����
};

struct CPMStates
{
	bool CPMMoving;			// ��ʻ��
	bool CPMRunOver;		// ��ʻ����
	bool DoorGeting;		// ȡ����
	bool DoorGeted;			// ȡ�Ž���
	bool DoorPuting;		// ������
	bool DoorPuted;			// ���ŵ�λ
	bool DoorCleaning;		// ������
	bool DoorCleaned;		// ����ֹͣ
	bool DoorFrameCleaning;	// �����
	bool DoorFrameCleaned;	// ���ֹͣ
	bool CokePushing;		// �ƽ���
	bool CokePushed;		// �ƽ����
	bool CoalCleaning;		// ��ú��
	bool CoalCleanOver;		// ��ú���
};
struct SCMStates
{
	bool SCMMoving;				// ��ʻ��
	bool SCMRunOver;			// ��ʻ����
	bool CoalLoading;			// װú��
	bool CoalPushed;			// ��ú���
	bool CoalPressing;			// ú��ѹƽ��
	bool CoalPressed;			// ú��ѹƽ���
	bool SCMArrivedCoalTower;	// װú����ú��ǰ
	bool CoalTampStatus;		// ����״̬
	bool CoalPushStatus;		// ��ú״̬
};
struct CGTCStates
{
	bool CGTCMoving;			// ��ʻ��
	bool SmokeGuiding; 			// ������
	bool SmokeGuided; 			// ���̽���
};
struct HCBCStates
{
	bool HCBCMoving;			// ��ʻ��
	bool HCBCRunOver;			// ��ʻ����
	bool CokePushing; 			// �ƽ���
	bool CokePushed; 			// �ƽ����
	bool CokeLoading; 			// Ϩ�����ӽ���
	bool CokeLoadOver; 			// Ϩ�����ӽ����
	bool WetQuenchingRunning; 	// Ϩ����ʪϨ����
	bool WetQuenchingOver; 		// Ϩ����ʪϨ�����
	bool CokeUnloading; 		// Ϩ����ж����
	bool CokeUnloadOver; 		// Ϩ����ж�����
};

// ���������������UI
struct CTCTopUI
{
	int furnaceId;			//¯��
	UINT8 states;			//״̬
	UINT8 pattern;			//ģʽ
	UINT8 action;			//����
	float position;			//��ǰλ��
	int temperature;		//����
	bool compressor;		//��ѹ����A�ߡ�B��
};

// ���������UI
struct CTCInverterParametersUI
{
	float runft1_frequency;		//����Ƶ��
	float runft1_power;			//�����
	int runft1_current;		//�������
	float runft1_ratio;			//���й��ʱ�
	int runft1_speed;			//��Ƶ�ٶ�

	float runft2_frequency;		//����Ƶ��
	float runft2_power;			//�����
	int runft2_current;		//�������
	float runft2_ratio;			//���й��ʱ�
	int runft2_speed;			//��Ƶ�ٶ�

	float pourft1_frequency;	//����Ƶ��
	float pourft1_power;		//�����
	int pourft1_current;		//�������
	float pourft1_ratio;		//���й��ʱ�
	int pourft1_speed;		//��Ƶ�ٶ�

	float pourft2_frequency;	//����Ƶ��
	float pourft2_power;		//�����
	int pourft2_current;		//�������
	float pourft2_ratio;		//���й��ʱ�
	int pourft2_speed;		//��Ƶ�ٶ�
};

// �������Ҳ࣬ϵͳ���UI
struct CTCSystemCheckUI
{
	int oil_temperature;			//����

	bool power_system;				//��Դϵͳ
	bool collision;					//����ײ����
	bool power_pause;				//Һѹ���е�Դ��ͣ
	bool pump_work;					//�󻬱�A�߹��� / B�߹������л���
	bool pump_warn;					//�󻬱�Һλ�ձ���
	bool braking;					//�ƶ��������
};

// �������Ҳ��²�����������
struct CTCParameterSetUI
{
	float oil_temperature_upper;	//��������
	float oil_temperature_lower;	//��������
	float left_limit;				//��������
	float right_limit;				//��������
	float max_speed;				//��������ٶ�
	float point_speed;				//���е㶯�ٶ�
	float braking_threshold;		//�ƶ����跧ֵ
};

// ȡ��̨����࣬װ�ò���UI״ֵ̬
struct CTCGDTParameterUI
{
	bool compressor;			//��ѹ��
	UINT8 door_hook_status_ul;	//���Ź������� / ���ޣ��м�̬�ü�ͷ�� / �ң�
	UINT8 door_hook_status_fb;	//���Ź���ǰ�� / ���ޣ��м�̬�ü�ͷ�� / �ң�
	UINT8 latch_statu;			//���ſ��գ�ǰ�� / ����
	bool upper_latch_statu;		//�ϲ�ѹ���ţ�ѹ�� / �ɿ�
	bool lower_latch_statu;		//�²�ѹ���ţ�ѹ�� / �ɿ�
	UINT8 trolly_statu;			//ȡ��̨����ǰ�� / ����
	UINT8 tilt_statu;			//ȡ����б��ǰ�� / ����
};

// ����դ��࣬װ�ò���UI״ֵ̬
struct CTCCGParameterUI
{
	UINT8 coke_grid_stutes_oc;		//����դ������ / ����
	UINT8 coke_grid_stutes_fb;		//����դ��ǰ�� / ����
	UINT8 flue_damper_status;		//���̵����ţ����� / ����
	UINT8 fan_damper_status;			//������ţ����� / ����
	UINT8 emergency_release;			//������ɢ������ / ����
	bool inverter1_status;			//��Ƶ��1������ / ֹͣ
	float inverter1_speed;			//��Ƶ��1���ٶȣ�28Hz
	float inverter1_current;		//��Ƶ��1��������15A
	bool inverter2_status;			//��Ƶ��2������ / ֹͣ
	float inverter2_speed;			//��Ƶ��2���ٶȣ�28Hz
	float inverter2_current;		//��Ƶ��2��������15A
	bool braking1;					//�ƶ�����1������ / �쳣
	bool braking2;					//�ƶ�����2������ / �쳣    
};

// ����̨���Ҳ࣬װ�ò���UI
struct CTCFCTParameterUI
{
	UINT8 trolly;		//���̨����ǰ�� / ����
	UINT8 machine;		//���������� / ����
};

// ����װ����࣬װ�ò���UI
struct CTCDCTParameterUI
{
	UINT8 trolly;			//����̨����ǰ�� / ����
	UINT8 side_cutter;		//��ε������� / ����
	UINT8 upper_cutter;		//�ϲ�ϳ�������� / ����
	UINT8 lower_cutter;		//�²�ϳ�������� / ����
	UINT8 clamp_device;		//�н�װ�ã����� / ����
};

// �豸״̬UI
struct CTCDeviceStatusUI
{
	bool operation_type;	//��Զ�� ���͵�
	bool auto_local;		//���Զ����͵أ�
	bool auto_remote;		//���Զ���Զ�̣�
	bool manual_local;		//���ֶ����͵أ�
	bool manual_remote;		//���ֶ���Զ�̣�
};

// ��ֵչʾ
struct NumericalShow
{
	UINT8	pattern;//		ģʽ
	int		next_furnaceId;//		��һ���ƽ�¯��(Ŀ��¯��)
	int		furnaceId;//		��ǰ����¯��
	int		oil_temperature;//		��ǰ����
	int		oil_pressure;//		��ǰ��ѹ
	float	speed;//			���������ٶ�
	bool	alignment;//		�Ƿ����	false:��true:��
	bool	scm_states;//		װú����ǰ״̬	false : װú ��true : ����
	bool	hcbc_states;//		Ϩ��ģʽ	false : ��Ϩ����true : ʪϨ��
};

// �豸״̬
struct DeviceStatus
{
	bool	oil_pump_status1;//	1#�ͱ�״̬
	bool	oil_pump_status2;//	2#�ͱ�״̬������ / ֹͣ��
	bool	heater_status;//	������״̬������ / ֹͣ��
	bool	compressor_status;//	��ѹ��״̬������ / ֹͣ��
	bool	lubrication_pump_status;//	�󻬱�״̬������ / ֹͣ��
	bool	directional_valve_status;//	����״̬������ / ֹͣ��
	bool	base_plate_lubrication_status;//	�װ���״̬������ / ֹͣ��
	bool	oil_cooled_motor_status;//	������״̬������ / ֹͣ��
	bool	resistance_room_fan;//�����ҷ��ȣ����� / ֹͣ��
	bool	hydraulic_chamber_fan;//	Һѹ�ҷ��ȣ����� / ֹͣ��
	bool	cab_fan;//	˾���ҷ��ȣ����� / ֹͣ��
	bool	oil_cooling_fan;//	������ȣ����� / ֹͣ��
	bool	electric_room_fan;//	�����ҷ��ȣ����� / ֹͣ��
	bool	transformer_room_fan;//	��ѹ���ҷ��ȣ����� / ֹͣ��
};

// ��ɢ״̬
struct DiffuseStatus
{
	bool	dust_collecting_fan1;//	1#������������� / ֹͣ��	false:ֹͣ��true:����
	bool	dust_collecting_fan2;//	2#������������� / ֹͣ��	false:ֹͣ��true:����
	bool	main_flue_damper;//	���̵����ţ����� / ֹͣ��	false : ֹͣ��true : ����
	bool	fan_damper;//	������ţ����� / ֹͣ��	false : ֹͣ��true : ����
	bool	emergency_release;//	������ɢ������ / ֹͣ��	false : ֹͣ��true : ����
};

// ͷβ��״̬
struct HeadTailStatus
{
	bool	scraper1;//	1#�ΰ�������� / ֹͣ��	false:ֹͣ��true:����
	bool	scraper2;//	2#�ΰ�������� / ֹͣ��	false:ֹͣ��true:����
	bool	coke_storage_hopper;//	���������� / �رգ�	false : �رգ�true : ��
	bool	dust_collecting_hopper;//	���������� / �رգ�	false : �رգ�true : ��
	bool	dust_collecting_motor;//	������������� / ֹͣ��	false : ֹͣ��true : ����
};

// ���̹�״̬
struct SmokeDuctStatus
{
	int	smoke_duct_temperature1;//	1#���̹��¶�  Int(�棩��
	int	smoke_duct_pressure1;//	1#���̹�ѹ��  Int(MPa)��
	int	smoke_duct_temperature2;//	2#���̹��¶�  Int(�棩��
	int	smoke_duct_pressure2;//	2#���̹�ѹ��  Int(MPa) ��
};

// ��ú����
struct CoalRecovery
{
	bool	scraper1;//	1#�ΰ�������� / ֹͣ��	false:ֹͣ��true:����
	bool	residual_coal_hopper;//	��ú������ / �رգ�	false : �رգ�true : ��
	bool	dust_collecting_hopper;//	���������� / �رգ�	false : �رգ�true : ��
};

// Ϩ����״̬
struct CokeQuenchingTowerStatus
{
	bool	coke_quenching_car;//	Ϩ����Ϩ����ǰ	false:���ڣ�true:��
	bool	adequate_water;//	Ϩ����ˮԴ	false : �����㣻true : ����
	bool	coke_quenching;//	Ϩ����Ϩ��	false : ֹͣ��true : ��ʼ
	bool	valgus_plate;//	Ϩ���������ⷭ��	false : �رգ�true : ��
	bool	driving;//	Ϩ����ʻ��	false : ������true : ����
	bool	fault;//	Ϩ��������	false : ���ϣ�true : ����
	bool	emergency_driving;//	Ϩ��������ʻ��	false : ֹͣ��true : ʻ��
};

// UI����ͬ��Э��
struct UIDataSyncProtocol
{
	INT32 key;
	UINT16 size;
	char value[256];
};

class JHUIDataSync
{
public:
	SocketUDP* m_sendToUE;
	UA_Client* m_recvFromPLC;
	int m_vehicleType;
	UINT8 m_nCTCStates;

public:
	JHUIDataSync();

	JHUIDataSync(UA_Client* recvFromPLC, SocketUDP* sendToUE, int vehicleType);

	void operator()();

	// ����PLC��ȡ�������ݷ�װ�����ݽṹ
	//��������С�����豸״̬
	DeviceStatus packDeviceStatus();

	//��������С��������չʾ
	NumericalShow packNumericalShow();

	//��������С������ɢ״̬
	DiffuseStatus packDiffuseStatus();

	//������С����ͷβ��״̬
	HeadTailStatus packHeadTailStatus();
	
	SmokeDuctStatus	packSmokeDuctStatus();

	CoalRecovery packCoalRecovery();

	CokeQuenchingTowerStatus packCokeQuenchingTowerStatus();

	CTCStates packCTCStates();
	CPMStates packCPMStates();
	SCMStates packSCMStates();
	CGTCStates packCGTCStates();
	HCBCStates packHCBCStates();

	//���������������ܳ����г�����ǩ
	CTCTopUI packCTCTotalTopUI();

	CTCTopUI packCPMTotalTopUI();

	CTCTopUI packSCMTotalTopUI();

	CTCTopUI packCGTCTotalTopUI();

	CTCTopUI packHCBCTotalTopUI();

	CTCTopUI packCTCTopUI();

	CTCTopUI packCTCGDTTopUI();

	CTCTopUI packCTCDCTTopUI();

	CTCTopUI packCTCFCTTopUI();

	CTCTopUI packCTCCGTopUI();

	CTCParameterSetUI packCTCParameterSetUI();

	CTCGDTParameterUI packCTCGDTParameterUI();

	CTCCGParameterUI packCTCCGParameterUI();

	//CTCFCTParameterUI packCTCFCTParameterUI();

	//CTCDCTParameterUI packCTCDCTParameterUI();

	CTCInverterParametersUI packCTCInverterParametersUI();

	CTCSystemCheckUI packCTCSystemCheckUI();

	CTCDeviceStatusUI packCTCDeviceStatusUI();

	void recvData(bool& state, const char* strNodeId);

	void recvData(UINT8& state, const char* strNodeId);

	void recvData(int& state, const char* strNodeId);

	void recvData(float* state, const char* strNodeId);

	void startSync();

	template <typename T>
	void sendData(T t, int key);

	//void recvData(INT16& state, const char* strNodeId);
	//void startStateSync(UA_NodeId nodeId);
};

