#pragma once

#include <iostream>
#include <fstream>
#include "JHNetwork.h"

using namespace std;

struct CarInfo
{
	int furnaceId;			//炉号
	UINT8 pattern;			//模式
	float position;			//当前位置
};

struct CTCSystemStatus
{
	bool CTCMoving;			// 行驶中
	bool CTCRunOver;		// 行驶结束
	bool DoorGeting;		// 取门中
	bool DoorGeted;			// 取门结束
	bool DoorPuting;		// 放门中
	bool DoorPuted;			// 放门到位
	bool DoorCleaning;		// 清门中
	bool DoorCleaned;		// 清门停止
	bool DoorFrameCleaning;	// 清框中
	bool DoorFrameCleaned;	// 清框停止
	bool CokePushing;		// 推焦中
	bool CokePushed;		// 推焦完毕
};

struct CPMSystemStatus
{
	bool CPMMoving;			// 行驶中
	bool CPMRunOver;		// 行驶结束
	bool DoorGeting;		// 取门中
	bool DoorGeted;			// 取门结束
	bool DoorPuting;		// 放门中
	bool DoorPuted;			// 放门到位
	bool DoorCleaning;		// 清门中
	bool DoorCleaned;		// 清门停止
	bool DoorFrameCleaning;	// 清框中
	bool DoorFrameCleaned;	// 清框停止
	bool CokePushing;		// 推焦中
	bool CokePushed;		// 推焦完毕
	bool CoalCleaning;		// 清煤中
	bool CoalCleanOver;		// 清煤完毕
};

struct SCMSystemStatus
{
	bool SCMMoving;				// 行驶中
	bool SCMRunOver;			// 行驶结束
	bool CoalLoading;			// 装煤中
	bool CoalPushed;			// 推煤完毕
	bool CoalPressing;			// 煤饼压平中
	bool CoalPressed;			// 煤饼压平完毕
	bool SCMArrivedCoalTower;	// 装煤车在煤塔前
	bool CoalTampStatus;		// 捣固状态
	bool CoalPushStatus;		// 推煤状态
};

struct CGTCSystemStatus
{
	bool CGTCMoving;			// 行驶中
	bool SmokeGuiding; 			// 导烟中
	bool SmokeGuided; 			// 导烟结束
};

struct HCBCSystemStatus
{
	bool HCBCMoving;			// 行驶中
	bool HCBCRunOver;			// 行驶结束
	bool CokePushing; 			// 推焦中
	bool CokePushed; 			// 推焦完毕
	bool CokeLoading; 			// 熄焦车接焦中
	bool CokeLoadOver; 			// 熄焦车接焦完毕
	bool WetQuenchingRunning; 	// 熄焦车湿熄焦中
	bool WetQuenchingOver; 		// 熄焦车湿熄焦完毕
	bool CokeUnloading; 		// 熄焦车卸焦中
	bool CokeUnloadOver; 		// 熄焦车卸焦完毕
};

// 拦焦车及组件顶部UI
struct CTCTopUI
{
	int furnaceId;			//炉号
	UINT8 states;			//状态
	UINT8 pattern;			//模式
	UINT8 action;			//动作
	float position;			//当前位置
	int temperature;		//油温
	bool compressor;		//空压机：A线、B线
};

// 拦焦车左侧UI
struct CTCInverterParametersUI
{
	float runft1_frequency;		//运行频率
	float runft1_power;			//额定功率
	int runft1_current;		//输出电流
	float runft1_ratio;			//运行功率比
	int runft1_speed;			//变频速度

	float runft2_frequency;		//运行频率
	float runft2_power;			//额定功率
	int runft2_current;		//输出电流
	float runft2_ratio;			//运行功率比
	int runft2_speed;			//变频速度

	float pourft1_frequency;	//运行频率
	float pourft1_power;		//额定功率
	int pourft1_current;		//输出电流
	float pourft1_ratio;		//运行功率比
	int pourft1_speed;		//变频速度

	float pourft2_frequency;	//运行频率
	float pourft2_power;		//额定功率
	int pourft2_current;		//输出电流
	float pourft2_ratio;		//运行功率比
	int pourft2_speed;		//变频速度
};

// 拦焦车右侧，系统检查UI
struct CTCSystemCheckUI
{
	int oil_temperature;			//油温

	bool power_system;				//电源系统
	bool collision;					//防碰撞允许
	bool power_pause;				//液压走行电源快停
	bool pump_work;					//润滑泵A线工作 / B线工作（切换）
	bool pump_warn;					//润滑泵液位空报警
	bool braking;					//制动电阻故障
};

// 拦焦车右侧下部，参数设置
struct CTCParameterSetUI
{
	float oil_temperature_upper;	//油温上限
	float oil_temperature_lower;	//油温下限
	float left_limit;				//走行左限
	float right_limit;				//走行右限
	float max_speed;				//走行最大速度
	float point_speed;				//走行点动速度
	float braking_threshold;		//制动电阻阀值
};

// 取门台车左侧，装置参数UI状态值
struct CTCGDTParameterUI
{
	bool compressor;			//空压机
	UINT8 door_hook_status_ul;	//提门钩：上限 / 下限（中间态用箭头左 / 右）
	UINT8 door_hook_status_fb;	//提门钩：前限 / 后限（中间态用箭头左 / 右）
	UINT8 latch_statu;			//门闩开闭：前限 / 后限
	bool upper_latch_statu;		//上部压门闩：压紧 / 松开
	bool lower_latch_statu;		//下部压门闩：压紧 / 松开
	UINT8 trolly_statu;			//取门台车：前限 / 后限
	UINT8 tilt_statu;			//取门倾斜：前限 / 后限
};

// 倒焦栅左侧，装置参数UI状态值
struct CTCCGParameterUI
{
	UINT8 coke_grid_stutes_oc;		//导焦栅：锁开 / 锁闭
	UINT8 coke_grid_stutes_fb;		//导焦栅：前限 / 后限
	UINT8 flue_damper_status;		//主烟道风门：开限 / 闭限
	UINT8 fan_damper_status;		//风机风门：开限 / 闭限
	UINT8 emergency_release;		//紧急放散：开限 / 闭限
	bool inverter1_status;			//变频器1：运行 / 停止
	float inverter1_speed;			//变频器1：速度：28Hz
	float inverter1_current;		//变频器1：电流：15A
	bool inverter2_status;			//变频器2：运行 / 停止
	float inverter2_speed;			//变频器2：速度：28Hz
	float inverter2_current;		//变频器2：电流：15A
	bool braking1;					//制动电阻1：正常 / 异常
	bool braking2;					//制动电阻2：正常 / 异常    
};

// 清门台车右侧，装置参数UI
struct CTCFCTParameterUI
{
	UINT8 trolly;		//清框台车：前限 / 后限
	UINT8 machine;		//清框机：上限 / 下限
};

// 清门装置左侧，装置参数UI
struct CTCDCTParameterUI
{
	UINT8 trolly;			//清门台车：前限 / 后限
	UINT8 side_cutter;		//侧刮刀：上限 / 下限
	UINT8 upper_cutter;		//上侧铣刀：左限 / 右限
	UINT8 lower_cutter;		//下侧铣刀：左限 / 右限
	UINT8 clamp_device;		//夹紧装置：开限 / 闭限
};

// 设备状态UI
struct CTCDeviceStatusUI
{
	bool operation_type;	//·远程 ·就地
	bool auto_local;		//·自动（就地）
	bool auto_remote;		//·自动（远程）
	bool manual_local;		//·手动（就地）
	bool manual_remote;		//·手动（远程）
};

// 数值展示
struct NumericalShow
{
	UINT8	pattern;//		模式
	int		next_furnaceId;//		下一个推焦炉号(目标炉号)
	int		furnaceId;//		当前所在炉号
	int		oil_temperature;//		当前油温
	int		oil_pressure;//		当前油压
	float	speed;//			整车运行速度
	bool	alignment;//		是否对正	false:否；true:是
	bool	scm_states;//		装煤车当前状态	false : 装煤 ；true : 捣固
	bool	hcbc_states;//		熄焦模式	false : 干熄焦；true : 湿熄焦
};

// 设备状态
struct DeviceStatus
{
	bool	oil_pump_status1;//	1#油泵状态
	bool	oil_pump_status2;//	2#油泵状态（启动 / 停止）
	bool	heater_status;//	加热器状态（启动 / 停止）
	bool	compressor_status;//	空压机状态（启动 / 停止）
	bool	lubrication_pump_status;//	润滑泵状态（启动 / 停止）
	bool	directional_valve_status;//	方向阀状态（启动 / 停止）
	bool	base_plate_lubrication_status;//	底板润滑状态（启动 / 停止）
	bool	oil_cooled_motor_status;//	油冷电机状态（启动 / 停止）
	bool	resistance_room_fan;//电阻室风扇（启动 / 停止）
	bool	hydraulic_chamber_fan;//	液压室风扇（启动 / 停止）
	bool	cab_fan;//	司机室风扇（启动 / 停止）
	bool	oil_cooling_fan;//	油冷风扇（启动 / 停止）
	bool	electric_room_fan;//	电气室风扇（启动 / 停止）
	bool	transformer_room_fan;//	变压器室风扇（启动 / 停止）
};

// 放散状态
struct DiffuseStatus
{
	bool	dust_collecting_fan1;//	1#集尘风机（启动 / 停止）	false:停止；true:启动
	bool	dust_collecting_fan2;//	2#集尘风机（启动 / 停止）	false:停止；true:启动
	bool	main_flue_damper;//	主烟道风门（启动 / 停止）	false : 停止；true : 启动
	bool	fan_damper;//	风机风门（启动 / 停止）	false : 停止；true : 启动
	bool	emergency_release;//	紧急放散（启动 / 停止）	false : 停止；true : 启动
};

// 头尾焦状态
struct HeadTailStatus
{
	bool	scraper1;//	1#刮板机（启动 / 停止）	false:停止；true:启动
	bool	scraper2;//	2#刮板机（启动 / 停止）	false:停止；true:启动
	bool	coke_storage_hopper;//	储焦斗（打开 / 关闭）	false : 关闭；true : 打开
	bool	dust_collecting_hopper;//	集尘斗（打开 / 关闭）	false : 关闭；true : 打开
	bool	dust_collecting_motor;//	集尘电机（启动 / 停止）	false : 停止；true : 启动
};

// 导烟管状态
struct SmokeDuctStatus
{
	int	smoke_duct_temperature1;//	1#导烟管温度  Int(℃）。
	int	smoke_duct_pressure1;//	1#导烟管压力  Int(MPa)。
	int	smoke_duct_temperature2;//	2#导烟管温度  Int(℃）。
	int	smoke_duct_pressure2;//	2#导烟管压力  Int(MPa) 。
};

// 余煤回收
struct CoalRecovery
{
	bool	scraper1;//	1#刮板机（启动 / 停止）	false:停止；true:启动
	bool	residual_coal_hopper;//	余煤斗（打开 / 关闭）	false : 关闭；true : 打开
	bool	dust_collecting_hopper;//	集尘斗（打开 / 关闭）	false : 关闭；true : 打开
};

// 熄焦塔状态
struct CokeQuenchingTowerStatus
{
	bool	coke_quenching_car;//	熄焦车熄焦塔前	false:不在；true:在
	bool	adequate_water;//	熄焦塔水源	false : 不充足；true : 充足
	bool	coke_quenching;//	熄焦塔熄焦	false : 停止；true : 开始
	bool	valgus_plate;//	熄焦塔请求外翻板	false : 关闭；true : 打开
	bool	driving;//	熄焦车驶出	false : 不允许；true : 允许
	bool	fault;//	熄焦塔故障	false : 故障；true : 正常
	bool	emergency_driving;//	熄焦车紧急驶出	false : 停止；true : 驶出
};

// UI数据同步协议
struct UIDataSyncProtocol
{
	INT32 key;
	UINT16 size;
	char value[256];
};

class JHUIDataSync
{
private:
	const int m_nSyncFrequency = 1000;

public:
	SocketUDP* m_sendToUE;
	UA_Client* m_recvFromPLC;
	int m_vehicleType;
	UINT8 m_nCTCStates;

public:
	JHUIDataSync();

	JHUIDataSync(UA_Client* recvFromPLC, SocketUDP* sendToUE, int vehicleType);

	void operator()();

	// 将从PLC获取到的数据封装至数据结构
	//拦焦车，小屏，设备状态
	DeviceStatus packDeviceStatus();

	//拦焦车，小屏，数据展示
	NumericalShow packNumericalShow();

	//拦焦车\推焦车，小屏，放散状态
	DiffuseStatus packDiffuseStatus();

	//推焦车，小屏，头尾焦状态
	HeadTailStatus packHeadTailStatus();
	
	//导烟车，小屏，导烟管状态
	SmokeDuctStatus	packSmokeDuctStatus();

	//推焦车，小屏，余煤回收
	CoalRecovery packCoalRecovery();

	//熄焦车，小屏，熄焦塔状态
	CokeQuenchingTowerStatus packCokeQuenchingTowerStatus();

	//打包拦焦车系统状态
	CTCSystemStatus packCTCSystemStatus();

	//打包推焦车系统状态
	CPMSystemStatus packCPMSystemStatus();

	//打包装煤车系统状态
	SCMSystemStatus packSCMSystemStatus();

	//打包导烟车系统状态
	CGTCSystemStatus packCGTCSystemStatus();

	//打包熄焦车系统状态
	HCBCSystemStatus packHCBCSystemStatus();

	//打包拦焦车，大屏，总场景中车顶标签
	CTCTopUI packCTCTotalTopUI();

	//打包推焦车，大屏，总场景中车顶标签
	CTCTopUI packCPMTotalTopUI();

	//打包装煤车，大屏，总场景中车顶标签
	CTCTopUI packSCMTotalTopUI();

	//打包导烟车，大屏，总场景中车顶标签
	CTCTopUI packCGTCTotalTopUI();

	//打包熄焦车，大屏，总场景中车顶标签
	CTCTopUI packHCBCTotalTopUI();

	// 打包拦焦车参数设置
	CTCTopUI packCTCTopUI();

	// 打包取门装置参数
	CTCTopUI packCTCGDTTopUI();

	// 打包清门装置参数
	CTCTopUI packCTCDCTTopUI();

	// 打包清框装置参数
	CTCTopUI packCTCFCTTopUI();

	// 打包导焦栅/除尘装置参数
	CTCTopUI packCTCCGTopUI();

	// 打包拦焦车参数设置
	CTCParameterSetUI packCTCParameterSetUI();

	// 打包取门装置参数
	CTCGDTParameterUI packCTCGDTParameterUI();

	// 打包导焦栅/除尘装置参数
	CTCCGParameterUI packCTCCGParameterUI();

	//// 打包清框装置参数
	//CTCFCTParameterUI packCTCFCTParameterUI();

	//// 打包清门装置参数
	//CTCDCTParameterUI packCTCDCTParameterUI();

	// 打包变频器参数
	CTCInverterParametersUI packCTCInverterParametersUI();

	// 打包系统检查
	CTCSystemCheckUI packCTCSystemCheckUI();

	// 打包设备状态
	CTCDeviceStatusUI packCTCDeviceStatusUI();

	//// 接收bool类型数据
	//void recvData(bool& state, const char* strNodeId);

	//// 接收UINT8类型数据
	//void recvData(UINT8& state, const char* strNodeId);

	//// 接收int类型数据
	//void recvData(int& state, const char* strNodeId);

	//// 接收float类型数据
	//void recvData(float& state, const char* strNodeId);

	template <typename T>
	void recvData(T& state, const char* strNodeId);

	void startSync();

	template <typename T>
	void sendData(T t, int key);
};

