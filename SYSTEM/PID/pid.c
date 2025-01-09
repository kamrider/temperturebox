#include "pid.h"
#include "bsp_ds18b20.h"
#include "bsp_GeneralTim.h" 

extern float temperature;

PID_struct PID;

void PID_Init()
{
	PID.setTemp = 17.0;
	PID.Kp = 30.0;
	PID.Ti = 0.001;
	PID.Td = 0.1;
	
	PID.eLast = 0.0;
	PID.OUT = 0.0;
	PID.t1 = 0.0;
	PID.t2 = 0.0;
	PID.t3 = 0.0;
}

void PID_Calculate()
{
	float error;
	
	PID.tempFromSensor = DS18B20_GetTemp_SkipRom();
	
	error = PID.tempFromSensor - PID.setTemp;
	
	PID.t1 = PID.Kp * error;
	PID.t2 += PID.Kp * (PID.Ti) * error;
	PID.t3 = PID.Kp * PID.Td * (error - PID.eLast);
	
	PID.OUT = PID.t1 + PID.t2 + PID.t3;
	
	if(PID.t2 > 400) PID.t2 = 400;
	if(PID.t2 < -400) PID.t2 = -400;
	
	if(PID.OUT > 500) PID.OUT = 500;
	if(PID.OUT < 0) PID.OUT = 0;
	
	PID.eLast = error;
}
		
		


