// TimeAlarmDevice.h

#ifndef DeviceAlarms_h
#define DeviceAlarms_h

#include <inttypes.h>

#include <TimeAlarms.h>

// definition for alarm callback functions 
typedef void (*OnAlarmTrigger)(int device);  
const int ALARM_DISABLED = -1; // alarmID value to indicate that the alarm is disabled

class DeviceAlarmsClass
{
  
private:
  uint8_t startHour;
  uint8_t startMinute;
  uint8_t startSecond;
  uint8_t endHour;
  uint8_t endMinute;
  uint8_t endSecond;
  int     deviceId;     // the Id provided in begin function to identify the device
  AlarmID_t alarmId;    // the Id provided by TimeAlarms
  bool isDeviceActive;  // true when within the active on and off times

  time_t getStartTime();
  time_t getEndTime();
  bool isDeviceActiveNow();   // returns true if the device is active now 
  bool isDeviceActiveAt(time_t t); // returns true of the device should be active at the given time 
  void updateAlarmState( time_t newactiveTime, time_t newDeactiveTime);
  OnAlarmTrigger alarmActivate;     // callback when alarm activates
  OnAlarmTrigger onAlarmDeactivate; // callback when alarm deactivates


public:
  DeviceAlarmsClass();
  void begin( int deviceID, OnAlarmTrigger onAlarmActivate, OnAlarmTrigger onAlarmDeactivate); 
  void setAlarmTimes( uint8_t h, uint8_t m, uint8_t s, uint8_t h1, uint8_t m1, uint8_t s1);
  void setActivateTime( uint8_t h, uint8_t m, uint8_t s);
  void setDeactivateTime( uint8_t h, uint8_t m, uint8_t s);
  bool enable();
  void disable();
  
};
#endif


