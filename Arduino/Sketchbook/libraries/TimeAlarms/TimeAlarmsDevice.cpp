// TimeAlarmDevice.cpp

#include "TimeAlarmsDevice.h"

DeviceAlarmsClass::DeviceAlarmsClass()  : alarmId(ALARM_DISABLED)  {}
  
void DeviceAlarmsClass::begin( int deviceID, OnAlarmTrigger onAlarmActivate, OnAlarmTrigger onAlarmDeactivate)
{
  deviceId = deviceID;
  alarmActivate = onAlarmActivate;
  onAlarmDeactivate = onAlarmDeactivate;
  
}  

void DeviceAlarmsClass::setActivateTime( uint8_t h, uint8_t m, uint8_t s)
{
  startHour   = h;
  startMinute = m;
  startSecond = s;
}

void DeviceAlarmsClass::setDeactivateTime( uint8_t h, uint8_t m, uint8_t s)
{
  endHour   = h;
  endMinute = m;
  endSecond = s;
}

void DeviceAlarmsClass::setAlarmTimes( uint8_t h, uint8_t m, uint8_t s, uint8_t h1, uint8_t m1, uint8_t s1)
{
   setActivateTime(h,m,s);
   setDeactivateTime(h1,m1,s1); 
   if( alarmId== ALARM_DISABLED) {
   // no alarm set 
      alarmId = alarmOnce(h,h,s, onAlarmActivate);
   }   
   else {
   // alarm set, check if active
   }
}

bool DeviceAlarmsClass::enable()
{
  
}

void DeviceAlarmsClass::disable()
{

}
  
time_t DeviceAlarmsClass::getEndTime()
{
  return AlarmHMS(endHour, endMinute, endSecond);
}

time_t DeviceAlarmsClass::getStartTime()
{
  return AlarmHMS(startHour,startMinute, startSecond);
}

// returns true if the device is active now 
bool DeviceAlarmsClass::isDeviceActiveNow()
{
  time_t t = now();
  if( isDeviceActive != isDeviceActiveAt(t) ){
    //Serial.println("err: activate was not synced");
	isDeviceActive = isDeviceActiveAt(t);
  }
  return isDeviceActive;  
}

// returns true of the device should be active at the given time 
bool DeviceAlarmsClass::isDeviceActiveAt(time_t t)
{ 
  // example - start 9, end 17: inactive if now = 8 or 18
  // example - start 23, end 2 : inactive if now = 22 or 3 
  bool isActive;
 
  time_t startTime = getStartTime();
  time_t endTime   = getEndTime();
  if(endTime > startTime){
     isActive = t >= startTime && t < endTime;
  }
  else{
     isActive = t >= startTime || t < endTime;
  }
  return isActive;
}

// this is called when alarm time is updated
void DeviceAlarmsClass::updateAlarmState( time_t newactiveTime, time_t newDeactiveTime)
{
/*
   bool isOldAlarmActiveNow = isDeviceActiveNow()
   bool isNewAlarmActiveNow = ;
   if( isOldAlarmActiveNow = true && isNewAlarmActiveNow == true ) {
      setAlarmOn; // set the off time 
   }
   else if( isOldAlarmActiveNow == false && isNewAlarmActiveNow == false) {
      setAlarmOn(device);// set the on time 
   }
   else if( isOldAlarmActiveNow == true && isNewAlarmActiveNow == false) {
      // turn the alarm off and set the on time 
   }
   else if( isOldAlarmActiveNow == false && isNewAlarmActiveNow == true) {
      setAlarmOn(device);// turn the alarm on and set the off time 
   }
  // alarmId[alarm] = Alarm.alarmOnce(startHour[alarm],startMinute[alarm],startSecond[alarm], alarmOnHandler0);
*/  
}
  
