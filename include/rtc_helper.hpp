#include <Arduino.h>
#include <DS1302.h>


void rtcInit(DS1302 &rtc);
void rtcSetTime(DS1302 &rtc, Time t);

String dayAsString(const Time::Day day);
void printTime(DS1302 &rtc);