#include "JHUIDataSync.h"

JHUIDataSync::JHUIDataSync()
{
	m_sendToUE = nullptr;
	m_recvFromPLC = nullptr;
}

JHUIDataSync::JHUIDataSync(UA_Client* recvFromPLC, SocketUDP* sendToUE, int vehicleType)
{
	m_sendToUE = sendToUE;
	m_recvFromPLC = recvFromPLC;
	m_vehicleType = vehicleType;
}

void JHUIDataSync::operator()()
{
	startSync();
}

NumericalShow JHUIDataSync::packNumericalShow()
{
	NumericalShow ns;

	int pattern;

	switch (m_vehicleType)
	{
	case VEHICLETYPE::CTC:
		recvData(pattern, "\"DB920  CTC2CO\".\"WorkMode\"");
		recvData(ns.alignment, "\"DB101  SystemStatus\".\"CTCAlignOven\"");
		recvData(ns.oil_temperature, "\"DB101  SystemStatus\".\"OilTemperature\"");
		recvData(ns.oil_pressure, "\"DB101  SystemStatus\".\"OilPressure\"");
		recvData(ns.next_furnaceId, "\"DB200  HMI\".\"Sys-GoalStove\"");
		recvData(ns.furnaceId, "\"DB200  HMI\".\"Sys-CurrentStove\"");
		recvData(&ns.speed, "\"VirtualCTC1ToUE\".\"CTC_MoveSpeed\"");
		break;
	case VEHICLETYPE::CPM:
		recvData(pattern, "\"DB920  CPM2CO\".\"WorkMode\"");
		recvData(ns.alignment, "\"DB101  SystemStatus\".\"CPMAlignOven\"");
		recvData(ns.oil_temperature, "\"DB101  SystemStatus\".\"OilTemperature\"");
		recvData(ns.oil_pressure, "\"DB101  SystemStatus\".\"OilPressure\"");
		recvData(ns.next_furnaceId, "\"DB200  HMI\".\"Sys-GoalStove\"");
		recvData(ns.furnaceId, "\"DB200  HMI\".\"Sys-CurrentStove\"");
		recvData(&ns.speed, "\"VirtualCPM1ToUE\".\"CPM_MoveSpeed\"");
		break;
	case VEHICLETYPE::SCM:
		recvData(pattern, "\"DB920  SCM2CO\".\"WorkMode\"");
		recvData(ns.alignment, "\"DB101  SystemStatus\".\"SCMAlignOven\""); 
		recvData(ns.oil_temperature, "\"DB101  SystemStatus\".\"OilTemperature\"");
		recvData(ns.oil_pressure, "\"DB101  SystemStatus\".\"OilPressure\"");
		recvData(ns.scm_states, "\"DB101  SystemStatus\".\"CoalTampStatus\"");
		recvData(ns.next_furnaceId, "\"DB200  HMI\".\"Sys-GoalStove\"");
		recvData(ns.furnaceId, "\"DB200  HMI\".\"Sys-CurrentStove\"");
		recvData(&ns.speed, "\"VirtualSCM1ToUE\".\"SCM_MoveSpeed\"");
		break;
	case VEHICLETYPE::CGTC:
		recvData(pattern, "\"DB920  CGTC2CO\".\"WorkMode\"");
		recvData(ns.alignment, "\"DB101  SystemStatus\".\"CGTCAlignOven\"");
		recvData(ns.oil_temperature, "\"DB012  SystemStatus\".\"OilTemperature\"");
		recvData(ns.oil_pressure, "\"DB012  SystemStatus\".\"OilPressure\"");
		recvData(ns.next_furnaceId, "\"DB500  ControlFromUE\".\"Sys-Targe_Oven\"");
		recvData(ns.furnaceId, "\"DB012  SystemStatus\".\"MiddleStoveNum\"");
		recvData(&ns.speed, "\"VirtualCGTC1ToUE\".\"CGTC_MoveSpeed\"");
		break;
	case VEHICLETYPE::HCBC:
		recvData(pattern, "\"DB920  HCBC2CO\".\"WorkMode\"");
		recvData(ns.alignment, "\"DB101  SystemStatus\".\"HCBCAlignOven\"");
		recvData(ns.oil_temperature, "\"DB101  SystemStatus\".\"OilTemperature\"");
		recvData(ns.oil_pressure, "\"DB101  SystemStatus\".\"OilPressure\"");
		recvData(ns.hcbc_states, "\"DB101  SystemStatus\".\"WetCokeQuenchingAllow\"");
		recvData(ns.next_furnaceId, "\"DB200  HMI\".\"Sys-GoalStove\"");
		recvData(ns.furnaceId, "\"DB200  HMI\".\"Sys-CurrentStove\"");
		recvData(&ns.speed, "\"VirtualHCBC1ToUE\".\"HCBC_MoveSpeed\"");
		break;
	default:
		break;
	}
	switch (pattern)
	{
	case 2048:
		ns.pattern = 1;
		break;
	case 1024:
		ns.pattern = 2;
		break;
	case 512:
		ns.pattern = 3;
		break;
	case 256:
		ns.pattern = 4;
		break;
	default:
		break;
	}

	return ns;
}

DeviceStatus JHUIDataSync::packDeviceStatus()
{
	DeviceStatus ds;

	recvData(ds.oil_pump_status1, "\"1#OilPumpRunning\"");
	recvData(ds.oil_pump_status2, "\"2#OilPumpRunning\"");
	recvData(ds.heater_status, "\"HeaterRunning\"");
	recvData(ds.compressor_status, "\"AirCompressorRunning\"");
	recvData(ds.lubrication_pump_status, "\"LubricantPumpRunning\"");
	recvData(ds.directional_valve_status, "\"DirectionValveRunning\"");
	//recvData(ds.base_plate_lubrication_status, "HeaterRunning");
	recvData(ds.oil_cooled_motor_status, "\"OilCoolingMotorRunning\"");
	recvData(ds.resistance_room_fan, "\"ResistanceRoomFanRunning\"");
	recvData(ds.hydraulic_chamber_fan, "\"HydraulicRoomFanRunning\"");
	recvData(ds.cab_fan, "\"DriverRoomFanRunning\"");
	recvData(ds.oil_cooling_fan, "\"OilCoolingFanRunning\"");
	recvData(ds.electric_room_fan, "\"ElectricRoomFanRunning\"");
	recvData(ds.transformer_room_fan, "\"TransformerRoomFanRunning\"");

	return ds;
}

DiffuseStatus JHUIDataSync::packDiffuseStatus()
{
	DiffuseStatus ds;

	recvData(ds.dust_collecting_fan1, "1#DustCollectingRunning");
	recvData(ds.dust_collecting_fan2, "2#DustCollectingRunning");
	recvData(ds.main_flue_damper, "\"DB101  SystemStatus\".\"MainFlueDamperInTheAction\"");
	recvData(ds.fan_damper, "\"DB101  SystemStatus\".\"FanDamperInTheAction\"");
	recvData(ds.emergency_release, "\"DB101  SystemStatus\".\"EmergencyDifuseInTheAction\"");

	return ds;
}

HeadTailStatus JHUIDataSync::packHeadTailStatus()
{
	HeadTailStatus hts;

	recvData(hts.scraper1, "\"NO1ScraperMachineRunning\"");
	recvData(hts.scraper2, "\"NO2ScraperMachineRunning\"");
	recvData(hts.dust_collecting_hopper, "\"DustHopperVibrateMotorRunning\"");
	recvData(hts.dust_collecting_motor, "\"DustHopperVibrateMotorRunning\"");
	recvData(hts.coke_storage_hopper, "");

	return hts;
}

SmokeDuctStatus JHUIDataSync::packSmokeDuctStatus()
{
	SmokeDuctStatus sds;

	recvData(sds.smoke_duct_pressure1, "");
	recvData(sds.smoke_duct_pressure2, "");
	recvData(sds.smoke_duct_temperature1, "");
	recvData(sds.smoke_duct_temperature2, "");

	return sds;
}

CoalRecovery JHUIDataSync::packCoalRecovery()
{
	CoalRecovery cr;

	recvData(cr.scraper1, "\"ScraperRunnning\"");
	recvData(cr.dust_collecting_hopper, "");
	recvData(cr.residual_coal_hopper, "\"DB129  SystemStatus\".\"RemainCoalCollectionMoving\"");

	return cr;
}

CokeQuenchingTowerStatus JHUIDataSync::packCokeQuenchingTowerStatus()
{
	CokeQuenchingTowerStatus cqts;

	recvData(cqts.adequate_water, "\"DB101  SystemStatus\".\"QuenchingTowerPrepared\"");
	recvData(cqts.coke_quenching, "\"DB101  SystemStatus\".\"QuenchingTowerSlaking\"");
	recvData(cqts.coke_quenching_car, "\"DB101  SystemStatus\".\"HCBC_AlignQuenchingTower\"");
	recvData(cqts.driving, "");
	recvData(cqts.emergency_driving, "");
	recvData(cqts.fault, "");
	recvData(cqts.valgus_plate, "\"DB101  SystemStatus\".\"OutsideFlipBoardOpening\"");

	return cqts;
}

CTCStates JHUIDataSync::packCTCStates()
{
	CTCStates states;

	recvData(states.CTCMoving, "\"DB101  SystemStatus\".\"CTCMoving\"");
	recvData(states.CTCRunOver, "\"DB101  SystemStatus\".\"CTCRunOver\"");
	recvData(states.DoorGeting, "\"DB101  SystemStatus\".\"DoorGeting\"");
	recvData(states.DoorGeted, "\"DB101  SystemStatus\".\"DoorGeted\"");
	recvData(states.DoorPuting, "\"DB101  SystemStatus\".\"DoorPuting\"");
	recvData(states.DoorPuted, "\"DB101  SystemStatus\".\"DoorPuted\"");
	recvData(states.DoorCleaning, "\"DB101  SystemStatus\".\"DoorCleaning\"");
	recvData(states.DoorCleaned, "\"DB101  SystemStatus\".\"DoorCleaned\"");
	recvData(states.DoorFrameCleaning, "\"DB101  SystemStatus\".\"DoorFrameCleaning\"");
	recvData(states.DoorFrameCleaned, "\"DB101  SystemStatus\".\"DoorFrameCleaned\"");
	recvData(states.CokePushing, "\"DB101  SystemStatus\".\"CokePushing\"");
	recvData(states.CokePushed, "\"DB101  SystemStatus\".\"CokePushed\"");

	return states;
}

CPMStates JHUIDataSync::packCPMStates()
{
	CPMStates states;

	recvData(states.CPMMoving, "\"DB101  SystemStatus\".\"CPMMoving\"");
	recvData(states.CPMRunOver, "\"DB101  SystemStatus\".\"CPMRunOver\"");
	recvData(states.DoorGeting, "\"DB101  SystemStatus\".\"DoorGeting\"");
	recvData(states.DoorGeted, "\"DB101  SystemStatus\".\"DoorGeted\"");
	recvData(states.DoorPuting, "\"DB101  SystemStatus\".\"DoorPuting\"");
	recvData(states.DoorPuted, "\"DB101  SystemStatus\".\"DoorPuted\"");
	recvData(states.DoorCleaning, "\"DB101  SystemStatus\".\"DoorCleaning\"");
	recvData(states.DoorCleaned, "\"DB101  SystemStatus\".\"DoorCleaned\"");
	recvData(states.DoorFrameCleaning, "\"DB101  SystemStatus\".\"DoorFrameCleaning\"");
	recvData(states.DoorFrameCleaned, "\"DB101  SystemStatus\".\"DoorFrameCleaned\"");
	recvData(states.CokePushing, "\"DB101  SystemStatus\".\"CokePushing\"");
	recvData(states.CokePushed, "\"DB101  SystemStatus\".\"CokePushed\"");
	recvData(states.CoalCleaning, "\"DB101  SystemStatus\".\"CoalCleaning\"");
	recvData(states.CoalCleanOver, "\"DB101  SystemStatus\".\"CoalCleanOver\"");

	return states;
}

SCMStates JHUIDataSync::packSCMStates()
{
	SCMStates states;

	recvData(states.SCMMoving, "\"DB101  SystemStatus\".\"SCMMoving\"");
	recvData(states.SCMRunOver, "\"DB101  SystemStatus\".\"SCMRunOver\"");
	recvData(states.CoalLoading, "\"DB101  SystemStatus\".\"CoalLoading\"");
	recvData(states.CoalPushed, "\"DB101  SystemStatus\".\"CoalPushed\"");
	recvData(states.CoalPressing, "\"DB101  SystemStatus\".\"CoalPressing\"");
	recvData(states.CoalPressed, "\"DB101  SystemStatus\".\"CoalPressed\"");
	recvData(states.CoalTampStatus, "\"DB101  SystemStatus\".\"CoalTampStatus\"");
	recvData(states.CoalPushStatus, "\"DB101  SystemStatus\".\"CoalPushStatus\"");
	recvData(states.SCMArrivedCoalTower, "\"DB101  SystemStatus\".\"SCMArrivedCoalTower\"");

	return states;
}

CGTCStates JHUIDataSync::packCGTCStates()
{
	CGTCStates states;

	recvData(states.CGTCMoving, "\"DB012  SystemStatus\".\"CGTCMoving\"");
	recvData(states.SmokeGuiding, "\"DB012  SystemStatus\".\"SmokeGuiding\"");
	recvData(states.SmokeGuided, "\"DB012  SystemStatus\".\"SmokeGuided\"");

	return states;
}

HCBCStates JHUIDataSync::packHCBCStates()
{
	HCBCStates states;

	recvData(states.HCBCMoving, "\"DB101  SystemStatus\".\"HCBC_IsMoving\"");
	recvData(states.HCBCRunOver, "\"DB101  SystemStatus\".\"HCBC_RunOver\"");
	recvData(states.CokePushing, "\"DB101  SystemStatus\".\"CokePushing\"");
	recvData(states.CokePushed, "\"DB101  SystemStatus\".\"CokePushed\"");
	recvData(states.CokeLoading, "\"DB101  SystemStatus\".\"CokeLoading\"");
	recvData(states.CokeLoadOver, "\"DB101  SystemStatus\".\"CokeLoadOver\"");
	recvData(states.WetQuenchingRunning, "\"DB101  SystemStatus\".\"WetQuenchingRunning\"");
	recvData(states.WetQuenchingOver, "\"DB101  SystemStatus\".\"WetQuenchingOver\"");
	recvData(states.CokeUnloading, "\"DB101  SystemStatus\".\"CokeUnloading\"");
	recvData(states.CokeUnloadOver, "\"DB101  SystemStatus\".\"CokeUnloadOver\"");

	return HCBCStates();
}

CTCTopUI JHUIDataSync::packCTCTotalTopUI()
{
	CTCTopUI ui;

	recvData(ui.furnaceId, "\"DB200  HMI\".\"Sys-CurrentStove\"");

	bool b = false;
	ui.states = 0;

	recvData(b, "\"DB101  SystemStatus\".\"CTCMoving\"");

	if (b)
	{
		ui.states = 1;
		m_nCTCStates = 1;
		return ui;
	}

	recvData(b, "\"DB101  SystemStatus\".\"DoorGeting\"");

	if (b)
	{
		ui.states = 2;
		m_nCTCStates = 2;
		return ui;
	}

	recvData(b, "\"DB101  SystemStatus\".\"CokeGridMoving\""); 

	if (b)
	{
		ui.states = 4;
		m_nCTCStates = 4;
		return ui;
	}

	recvData(b, "\"DB101  SystemStatus\".\"DoorFrameCleaning\"");

	if (b)
	{
		ui.states = 5;
		m_nCTCStates = 5;
		return ui;
	}

	recvData(b, "\"DB101  SystemStatus\".\"DoorPuting\"");

	if (b)
	{
		ui.states = 6;
		m_nCTCStates = 6;
		return ui;
	}

	recvData(b, "\"DB101  SystemStatus\".\"DoorCleaning\"");

	if (b)
	{
		ui.states = 3;
		m_nCTCStates = 3;
		return ui;
	}

	ui.states = 7;
	m_nCTCStates = 7;

	return ui;
}

CTCTopUI JHUIDataSync::packCPMTotalTopUI()
{
	CTCTopUI ui;

	recvData(ui.furnaceId, "\"DB200  HMI\".\"Sys-CurrentStove\"");

	bool b = false;
	ui.states = 0;

	recvData(b, "\"DB101  SystemStatus\".\"CTCMoving\"");

	if (b)
	{
		ui.states = 1;
		return ui;
	}

	recvData(b, "\"DB101  SystemStatus\".\"DoorGeting\"");

	if (b)
	{
		ui.states = 2;
		return ui;
	}

	recvData(b, "\"DB101  SystemStatus\".\"DoorCleaning\"");

	if (b)
	{
		ui.states = 3;
		return ui;
	}

	recvData(b, "\"DB101  SystemStatus\".\"CokePushing\"");

	if (b)
	{
		ui.states = 4;
		return ui;
	}

	recvData(b, "\"DB101  SystemStatus\".\"DoorFrameCleaning\"");

	if (b)
	{
		ui.states = 5;
		return ui;
	}

	recvData(b, "\"DB101  SystemStatus\".\"DoorPuting\"");

	if (b)
	{
		ui.states = 6;
		return ui;
	}

	ui.states = 8;

	return ui;
}

CTCTopUI JHUIDataSync::packSCMTotalTopUI()
{
	CTCTopUI ui;

	recvData(ui.furnaceId, "\"DB200  HMI\".\"Sys-CurrentStove\"");

	bool b = false;
	ui.states = 0;

	recvData(b, "\"DB117  SystemStatus\".\"SCMMoving\"");

	if (b)
	{
		ui.states = 2;
		return ui;
	}

	recvData(b, "\"DB119  SystemStatus\".\"CoalPressing\"");

	if (b)
	{
		ui.states = 3;
		return ui;
	}

	recvData(b, "\"DB118  SystemStatus\".\"CoalLoading\"");

	if (b)
	{
		ui.states = 4;
		return ui;
	}

	ui.states = 5;

	return ui;
}

CTCTopUI JHUIDataSync::packCGTCTotalTopUI()
{
	CTCTopUI ui;

	recvData(ui.furnaceId, "\"DB012  SystemStatus\".\"MiddleStoveNum\"");

	bool b = false;
	ui.states = 0;

	recvData(b, "\"DB012  SystemStatus\".\"CGTCMoving\"");

	if (b)
	{
		ui.states = 1;
		return ui;
	}

	recvData(b, "\"DB012  SystemStatus\".\"SmokeGuiding\"");

	if (b)
	{
		ui.states = 3;
		return ui;
	}

	ui.states = 2;

	return ui;
}

CTCTopUI JHUIDataSync::packHCBCTotalTopUI()
{
	CTCTopUI ui;

	recvData(ui.furnaceId, "\"DB200  HMI\".\"Sys-CurrentStove\"");

	bool b = false;
	ui.states = 0;

	recvData(b, "\"DB101  SystemStatus\".\"HCBC_IsMoving\"");

	if (b)
	{
		ui.states = 1;
		return ui;
	}

	recvData(b, "\"DB101  SystemStatus\".\"WetQuenchingRunning\"");

	if (b)
	{
		ui.states = 3;
		return ui;
	}

	recvData(b, "\"DB101  SystemStatus\".\"CokeUnloading\"");

	if (b)
	{
		ui.states = 4;
		return ui;
	}

	ui.states = 2;

	return ui;
}

CTCTopUI JHUIDataSync::packCTCTopUI()
{
	CTCTopUI ui;
	
	recvData(ui.furnaceId, "\"DB200  HMI\".\"Sys-CurrentStove\"");
	recvData(&ui.position, "\"VirtualHCBC1ToUE\".\"CTC_Location\"");
	recvData(ui.temperature, "\"DB101  SystemStatus\".\"OilTemperature\"");
	recvData(ui.compressor, "");

	int pattern;
	recvData(pattern, "\"DB920  CTC2CO\".\"WorkMode\"");
	switch (pattern)
	{
	case 2048:
		ui.pattern = 1;
		break;
	case 1024:
		ui.pattern = 2;
		break;
	case 512:
		ui.pattern = 3;
		break;
	case 256:
		ui.pattern = 4;
		break;
	default:
		break;
	}

	bool b= false;
	ui.action = 0;

	recvData(b, "\"DB101  SystemStatus\".\"CTCMovingLeft\"");

	if (b)
	{
		ui.action = 1;
		return ui;
	}

	recvData(b, "\"DB101  SystemStatus\".\"CTCMovingRight\"");

	if (b)
	{
		ui.action = 2;
		return ui;
	}

	recvData(b, "\"DB101  SystemStatus\".\"CokeGridMoving\"");

	if (b)
	{
		ui.action = 3;
		return ui;
	}

	recvData(b, "\"DB101  SystemStatus\".\"DoorGeting\"");

	if (b)
	{
		ui.action = 5;
		return ui;
	}

	recvData(b, "\"DB101  SystemStatus\".\"DoorPuting\"");

	if (b)
	{
		ui.action = 6;
		return ui;
	}

	recvData(b, "\"DB101  SystemStatus\".\"DoorFrameCleaning\"");

	if (b)
	{
		ui.action = 7;
		return ui;
	}

	return ui;
}

CTCTopUI JHUIDataSync::packCTCGDTTopUI()
{
	CTCTopUI ui;

	recvData(ui.furnaceId, "\"DB200  HMI\".\"Sys-CurrentStove\"");
	
	int pattern;
	recvData(pattern, "\"DB920  CTC2CO\".\"WorkMode\"");
	switch (pattern)
	{
	case 2048:
		ui.pattern = 1;
		break;
	case 1024:
		ui.pattern = 2;
		break;
	case 512:
		ui.pattern = 3;
		break;
	case 256:
		ui.pattern = 4;
		break;
	default:
		break;
	}

	bool b = false;
	ui.action = 0;

	recvData(b, "\"DB101  SystemStatus\".\"DoorGeting\"");

	if (b)
	{
		ui.action = 1;
		return ui;
	}

	recvData(b, "\"DB101  SystemStatus\".\"DoorPuting\"");

	if (b)
	{
		ui.action = 2;
		return ui;
	}

	return ui;
}

CTCTopUI JHUIDataSync::packCTCDCTTopUI()
{
	CTCTopUI ui;

	recvData(ui.furnaceId, "\"DB200  HMI\".\"Sys-CurrentStove\"");
	
	int pattern;
	recvData(pattern, "\"DB920  CTC2CO\".\"WorkMode\"");
	switch (pattern)
	{
	case 2048:
		ui.pattern = 1;
		break;
	case 1024:
		ui.pattern = 2;
		break;
	case 512:
		ui.pattern = 3;
		break;
	case 256:
		ui.pattern = 4;
		break;
	default:
		break;
	}

	bool b = false;
	ui.action = 0;

	recvData(b, "\"DB101  SystemStatus\".\"DoorCleaning\"");

	if (b)
	{
		ui.action = 1;
		return ui;
	}

	return ui;
}

CTCTopUI JHUIDataSync::packCTCFCTTopUI()
{
	CTCTopUI ui;

	recvData(ui.furnaceId, "\"DB200  HMI\".\"Sys-CurrentStove\"");
	
	int pattern;
	recvData(pattern, "\"DB920  CTC2CO\".\"WorkMode\"");
	switch (pattern)
	{
	case 2048:
		ui.pattern = 1;
		break;
	case 1024:
		ui.pattern = 2;
		break;
	case 512:
		ui.pattern = 3;
		break;
	case 256:
		ui.pattern = 4;
		break;
	default:
		break;
	}

	bool b = false;
	ui.action = 0;

	recvData(b, "\"DB101  SystemStatus\".\"DoorCleaning\"");

	if (b)
	{
		ui.action = 1;
		return ui;
	}

	return ui;
}

CTCTopUI JHUIDataSync::packCTCCGTopUI()
{
	CTCTopUI ui;

	recvData(ui.furnaceId, "\"DB200  HMI\".\"Sys-CurrentStove\"");
	
	int pattern;
	recvData(pattern, "\"DB920  CTC2CO\".\"WorkMode\"");
	switch (pattern)
	{
	case 2048:
		ui.pattern = 1;
		break;
	case 1024:
		ui.pattern = 2;
		break;
	case 512:
		ui.pattern = 3;
		break;
	case 256:
		ui.pattern = 4;
		break;
	default:
		break;
	}

	bool b = false;
	ui.action = 0;

	recvData(b, "\"DB101  SystemStatus\".\"CokeGridMoving\"");

	if (b)
	{
		ui.action = 2;
		return ui;
	}

	return ui;
}

CTCInverterParametersUI JHUIDataSync::packCTCInverterParametersUI()
{
	CTCInverterParametersUI ui;

	//recvData(ui.runft1_current, "\"DB101  SystemStatus\".\"MoveTransducer1\".\"Electricity\"");
	//recvData(ui.runft1_speed, "\"DB101  SystemStatus\".\"MoveTransducer1\".\"Speed\"");
	ui.runft1_current = 13.8;
	ui.runft1_speed = 38;
	ui.runft1_frequency = 35.1;
	ui.runft1_ratio = 40.6;
	ui.runft1_power = 15;
	//recvData(ui.runft2_current, "\"DB101  SystemStatus\".\"MoveTransducer2\".\"Electricity\"");
	//recvData(ui.runft2_speed, "\"DB101  SystemStatus\".\"MoveTransducer2\".\"Speed\"");
	ui.runft2_current = 13.8;
	ui.runft2_speed = 38;
	ui.runft2_frequency = 35.1;
	ui.runft2_ratio = 40.6;
	ui.runft2_power = 15;
	//recvData(ui.pourft1_current, "\"DB101  SystemStatus\".\"CokeGridTransducer1\".\"Electricity\"");
	//recvData(ui.pourft1_speed, "\"DB101  SystemStatus\".\"CokeGridTransducer1\".\"Speed\"");
	ui.pourft1_current = 13.8;
	ui.pourft1_speed = 38;
	ui.pourft1_frequency = 35.1;
	ui.pourft1_ratio = 40.6;
	ui.pourft1_power = 15;
	//recvData(ui.pourft2_current, "\"DB101  SystemStatus\".\"CokeGridTransducer2\".\"Electricity\"");
	//recvData(ui.pourft2_speed, "\"DB101  SystemStatus\".\"CokeGridTransducer2\".\"Speed\"");
	ui.pourft2_current = 13.8;
	ui.pourft2_speed = 38;
	ui.pourft2_frequency = 35.1;
	ui.pourft2_ratio = 40.6;
	ui.pourft2_power = 15;

	return ui;
}

CTCSystemCheckUI JHUIDataSync::packCTCSystemCheckUI()
{
	CTCSystemCheckUI ui;

	recvData(ui.oil_temperature, "\"DB101  SystemStatus\".\"OilTemperature\"");

	ui.power_system = false;
	ui.collision = false;
	ui.power_pause = false;
	ui.pump_work = false;
	ui.pump_warn = false;
	ui.braking = false;	

	return CTCSystemCheckUI();
}

CTCParameterSetUI JHUIDataSync::packCTCParameterSetUI()
{
	CTCParameterSetUI ui;

	ui.oil_temperature_upper = 500;
	ui.oil_temperature_lower = -10;
	ui.left_limit = 150000;
	ui.right_limit = 0;
	ui.max_speed = 5000;
	ui.point_speed = 2;
	ui.braking_threshold = 30;

	return ui;
}

CTCGDTParameterUI JHUIDataSync::packCTCGDTParameterUI()
{
	CTCGDTParameterUI ui;

	ui.compressor = false;

	return ui;
}

CTCCGParameterUI JHUIDataSync::packCTCCGParameterUI()
{
	CTCCGParameterUI ui;

	ui.coke_grid_stutes_oc = true;		//导焦栅：锁开 / 锁闭
	ui.inverter1_status = true;			//变频器1：运行 / 停止
	ui.inverter1_current = 15;
	ui.inverter1_speed = 28;
	//recvData(ui.inverter1_current, "\"DB101  SystemStatus\".\"CokeGridTransducer1\".\"Electricity\"");
	//recvData(ui.inverter1_speed, "\"DB101  SystemStatus\".\"CokeGridTransducer1\".\"Speed\"");
	ui.inverter2_status = true;			//变频器2：运行 / 停止
	ui.inverter2_current = 15;
	ui.inverter2_speed = 28;
	//recvData(ui.inverter2_current, "\"DB101  SystemStatus\".\"CokeGridTransducer2\".\"Electricity\"");
	//recvData(ui.inverter2_speed, "\"DB101  SystemStatus\".\"CokeGridTransducer2\".\"Speed\"");
	ui.braking1 = true;					//制动电阻1：正常 / 异常
	ui.braking2 = true;					//制动电阻2：正常 / 异常

	return ui;
}

CTCDeviceStatusUI JHUIDataSync::packCTCDeviceStatusUI()
{
	CTCDeviceStatusUI ui;

	int pattern;
	recvData(pattern, "\"DB920  CTC2CO\".\"WorkMode\"");
	switch (pattern)
	{
	case 1024:
		ui.manual_local = true;
		ui.manual_remote = true;
		ui.auto_local = false;
		ui.auto_remote = false;
		break;
	case 256:
		ui.auto_local = true;
		ui.auto_remote = true;
		ui.manual_local = false;
		ui.manual_remote = false;
		break;
	default:
		break;
	}

	return CTCDeviceStatusUI();
}

void JHUIDataSync::recvData(bool& state, const char* strNodeId)
{
	UA_NodeId nodeId = UA_NODEID_STRING(3, const_cast<char*>(strNodeId));

	UA_Variant value; /* Variants can hold scalar values and arrays of any type */
	UA_Variant_init(&value);

	// 客户端接收"Testt"传来的数据
	UA_StatusCode retval = UA_Client_readValueAttribute(m_recvFromPLC, nodeId, &value);

	// 判断接收状态和数据类型是否符合
	if (retval == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_BOOLEAN]))
	{
		state = *(bool*)value.data;
	}
	UA_Variant_clear(&value);
}

void JHUIDataSync::recvData(UINT8& state, const char* strNodeId)
{
	UA_NodeId nodeId = UA_NODEID_STRING(3, const_cast<char*>(strNodeId));

	UA_Variant value; /* Variants can hold scalar values and arrays of any type */
	UA_Variant_init(&value);

	// 客户端接收"Testt"传来的数据
	UA_StatusCode retval = UA_Client_readValueAttribute(m_recvFromPLC, nodeId, &value);

	// 判断接收状态和数据类型是否符合
	if (retval == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_BYTE]))
	{
		state = *(UINT8*)value.data;
	}
}

void JHUIDataSync::recvData(int& state, const char* strNodeId)
{
	UA_NodeId nodeId = UA_NODEID_STRING(3, const_cast<char*>(strNodeId));

	UA_Variant value; /* Variants can hold scalar values and arrays of any type */
	UA_Variant_init(&value);

	// 客户端接收"Testt"传来的数据
	UA_StatusCode retval = UA_Client_readValueAttribute(m_recvFromPLC, nodeId, &value);

	// 判断接收状态和数据类型是否符合
	if (retval == UA_STATUSCODE_GOOD)
	{
		if (UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_INT32]))
		{
			state = *(int*)value.data;
		}
		else if (UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_INT16]))
		{
			state = *(short*)value.data;
		}
	}
	UA_Variant_clear(&value);
}

void JHUIDataSync::recvData(float* state, const char* strNodeId)
{
	UA_NodeId nodeId = UA_NODEID_STRING(3, const_cast<char*>(strNodeId));

	UA_Variant value; /* Variants can hold scalar values and arrays of any type */
	UA_Variant_init(&value);

	// 客户端接收"Testt"传来的数据
	UA_StatusCode retval = UA_Client_readValueAttribute(m_recvFromPLC, nodeId, &value);

	// 判断接收状态和数据类型是否符合
	if (retval == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_FLOAT]))
	{
		*state = *(float*)value.data;
	}
	UA_Variant_clear(&value);
}

template <typename T>
void JHUIDataSync::sendData(T t, int key)
{
	UIDataSyncProtocol data;
	data.key = key;
	data.size = sizeof(t);
	memcpy(data.value, &t, sizeof(t));

	int send_len = sendto(m_sendToUE->svr, (char*)&data, sizeof(data), 0, (const SOCKADDR*)&m_sendToUE->sin, sizeof(m_sendToUE->sin));

	if (SOCKET_ERROR == send_len)
	{
		cout << "DeviceStatus sendto error !" << endl;
	}
	//Sleep(100);
}

void JHUIDataSync::startSync()
{
	while (true)
	{
		switch (m_vehicleType)
		{
		case VEHICLETYPE::CTC:
			sendData(packCTCTotalTopUI(), 12001);
			sendData(packCTCTopUI(), 12002);

			switch (m_nCTCStates)
			{
			case 1:
				break;
			case 2:
				sendData(packCTCGDTTopUI(), 12101);
				break;
			case 3:
				sendData(packCTCDCTTopUI(), 12301);
				break;
			case 4:
				sendData(packCTCCGTopUI(), 12501);
				break;
			case 5:
				sendData(packCTCFCTTopUI(), 12401);
				break;
			case 6:
				sendData(packCTCGDTTopUI(), 12101);
				break;
			default:
				break;
			}
			
			//sendData(packCTCGDTTopUI(), 12102);
			//
			//sendData(packCTCGDTTopUI(), 12302);
			//
			//sendData(packCTCGDTTopUI(), 12402);
			//
			//sendData(packCTCGDTTopUI(), 12502);

			// 大屏都要的数据
			//sendData(packCTCSystemCheckUI(), 12003);
			//sendData(packCTCDeviceStatusUI(), 12004);
			//sendData(packCTCParameterSetUI(), 12005);
			//sendData(packCTCInverterParametersUI(), 12006);

			// 小屏数据
			sendData(packDeviceStatus(), 13002);
			sendData(packNumericalShow(), 13001);
			sendData(packDiffuseStatus(), 13003);
			break;
		case VEHICLETYPE::CPM:
			sendData(packCPMTotalTopUI(), 22001);
			sendData(packDeviceStatus(), 23002);
			sendData(packNumericalShow(), 23001);
			sendData(packDiffuseStatus(), 23003);
			sendData(packHeadTailStatus(), 23004);
			break;
		case VEHICLETYPE::SCM:
			sendData(packSCMTotalTopUI(), 32001);
			sendData(packDeviceStatus(), 33002);
			sendData(packNumericalShow(), 33001); 
			sendData(packCoalRecovery(), 33003);
			break;
		case VEHICLETYPE::CGTC:
			sendData(packCGTCTotalTopUI(), 42001);
			sendData(packDeviceStatus(), 43002);
			sendData(packNumericalShow(), 43001);
			sendData(packSmokeDuctStatus(), 43003);
			break;
		case VEHICLETYPE::HCBC:
			sendData(packHCBCTotalTopUI(), 52001);
			sendData(packDeviceStatus(), 53002);
			sendData(packNumericalShow(), 53001);
			sendData(packCokeQuenchingTowerStatus(), 53003);
			break;
		default:
			break;
		}
		Sleep(1000);
	}
}



