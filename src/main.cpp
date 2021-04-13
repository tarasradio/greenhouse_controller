#include <main.hpp>
#include <Arduino.h>
#include <stdio.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include <DS1302.h>

// Custom libraries
#include <enc.hpp>
#include <rtc_helper.hpp>

// Menu
#include <menu.hpp>

//========== All pages ============================
void printBackLine();
void clickBackLine();

//========== Home page ============================
void printHomePageTitle();
void printHomePageTime();

void clickHomePage();

//========== Menu page ============================
void printMenuPageLight();
void clickMenuPageLight();

void printMenuPagePomp();
void clickMenuPagePomp();

void printMenuPageClock();
void clickMenuPageClock();

//========== Light page ============================
void printLightPageTimeOn();
void clickLightPageTimeOn();

void printLightPageTimeOff();
void clickLightPageTimeOff();

//========== Pomp page ============================
void printPompPagePeriod();
void clickPompPagePeriod();

void printPompPageInterval();
void clickPompPageInterval();

//========== Clock page ============================
void printClockPageDate();
void clickClockPageDate();

void printClockPageTime();
void clickClockPageTime();

//========== Light set time on page ============================
void onSetTimeOnHoursUp();
void onSetTimeOnHoursDown();

void onSetTimeOnMinutesUp();
void onSetTimeOnMinutesDown();

void printSetTimeOn();

//========== Light set time off page ============================
void onSetTimeOffHoursUp();
void onSetTimeOffHoursDown();

void onSetTimeOffMinutesUp();
void onSetTimeOffMinutesDown();

void printSetTimeOff();

// All pages definition
Line backLine(printBackLine, clickBackLine);

// Home page definition
#define HOME_PAGE 0
Line homePageTitle(printHomePageTitle);
Line homePageTime(printHomePageTime);
Line homePageLines[2] = { homePageTitle, homePageTime };
Page homePage(homePageLines, 2, clickHomePage, -1);

// Menu page definition
#define MENU_PAGE 1
Line menuPageLight(printMenuPageLight, clickMenuPageLight);
Line menuPagePomp(printMenuPagePomp, clickMenuPagePomp);
Line menuPageClock(printMenuPageClock, clickMenuPageClock);

Line menuPageLines[4] = { menuPageLight, menuPagePomp, menuPageClock, backLine };
Page menuPage(menuPageLines, 4, HOME_PAGE);

// Light page definition
#define LIGHT_PAGE 2
Line lightPageTimeOn(printLightPageTimeOn, clickLightPageTimeOn);
Line lightPageTimeOff(printLightPageTimeOff, clickLightPageTimeOff);
Line lightPageLines[3] = { lightPageTimeOn, lightPageTimeOff, backLine };
Page lightPage(lightPageLines, 3, MENU_PAGE);

// Pomp page definition
#define POMP_PAGE 3
Line pompPagePeriod(printPompPagePeriod, clickPompPagePeriod);
Line pompPageInterval(printPompPageInterval, clickPompPageInterval);
Line pompPageLines[3] = { pompPagePeriod, pompPageInterval, backLine };
Page pompPage(pompPageLines, 3, MENU_PAGE);

// Clock page definition
#define CLOCK_PAGE 4
Line clockPageDate(printClockPageDate, clickClockPageDate);
Line clockPageTime(printClockPageTime, clickClockPageTime);

Line clockPageLines[3] = { clockPageDate, clockPageTime, backLine };
Page clockPage(clockPageLines, 3, MENU_PAGE);

// Light time on page definition
#define TIME_ON_PAGE 5
Item setTimeOnHours(onSetTimeOnHoursUp, onSetTimeOnHoursDown);
Item setTimeOnMinutes(onSetTimeOnMinutesUp, onSetTimeOnMinutesDown);

Item setTimeOnItems[2] = { setTimeOnHours, setTimeOnMinutes };
ItemsList setTimeOnItemsList(setTimeOnItems, 2);
Line setTimeOn(&setTimeOnItemsList, printSetTimeOn);

Line setTimeOnLines[2] = { setTimeOn, backLine };
Page setTimeOnPage(setTimeOnLines, 2, LIGHT_PAGE);

// Light time off page definition
#define TIME_OFF_PAGE 6
Item setTimeOffHours(onSetTimeOffHoursUp, onSetTimeOffHoursDown);
Item setTimeOffMinutes(onSetTimeOffMinutesUp, onSetTimeOffMinutesDown);

Item setTimeOffItems[2] = { setTimeOffHours, setTimeOffMinutes };
ItemsList setTimeOffItemsList(setTimeOffItems, 2);
Line setTimeOff(&setTimeOffItemsList, printSetTimeOff);

Line setTimeOffLines[2] = { setTimeOff, backLine };
Page setTimeOffPage(setTimeOffLines, 2, LIGHT_PAGE);

Page pages[] = { homePage, menuPage, lightPage, pompPage, clockPage, setTimeOnPage, setTimeOffPage };
Menu menu(pages);

LiquidCrystal_I2C lcd(0x27, 16, 2);

Encoder encoder(ENCODER_A_PIN, ENCODER_B_PIN);
DS1302 rtc(RTC_CE_PIN, RTC_IO_PIN, RTC_CLK_PIN);
uint8_t relayPins[] = {RELAY_A_PIN, RELAY_B_PIN, RELAY_C_PIN};

void encoderHandle();

void rtcInit();
void rtcSetTime();

void relaysInit();
void relaysBlink();

void updateLightState();

//========== Settings =============================
struct TimeData
{
  uint8_t hours = 0;
  uint8_t minutes = 0;
};

TimeData timeOnData;
TimeData timeOffData;

TimeData EEMEM timeOnDataAddr;
TimeData EEMEM timeOffDataAddr;

void readSettings() {
  eeprom_read_block( (void*)&timeOnData, (const void*)&timeOnDataAddr, sizeof(TimeData) );
  eeprom_read_block( (void*)&timeOffData, (const void*)&timeOffDataAddr, sizeof(TimeData) );
}

void writeTimeOnSettings() {
  eeprom_write_block( (void*)&timeOnData, (const void*)&timeOnDataAddr, sizeof(TimeData));
}

void writeTimeOffSettings() {
  eeprom_write_block( (void*)&timeOffData, (const void*)&timeOffDataAddr, sizeof(TimeData));
}

//========== All pages ============================

void printBackLine() {
  lcd.setCursor(0, 1);
  if(menu.getSelectedLine() == menu.getSelectedPageLinesCount() - 1) {
    lcd.print(">");
  }
  lcd.print("Back ");
}

void clickBackLine() {
  menu.selectParentPage();
  lcd.clear();
  menu.update();
}

//========== Home page ============================

void printHomePageTitle() {
  lcd.setCursor(0, 0);
  lcd.print(" Greenhouse 1.0 ");
}

void printHomePageTime() {
  Time t = rtc.time();
  char buf[16];
  snprintf(buf, sizeof(buf), "%02d:%02d:%02d", t.hr, t.min, t.sec);

  lcd.setCursor(4, 1);
  lcd.print(buf);
}

void clickHomePage() {
  menu.selectPage(MENU_PAGE);
  lcd.clear();
  menu.update();
}

//========== Menu page ============================

void printMenuPageLight() {
  if(menu.getSelectedLine() == 0) {
    lcd.setCursor(0, 0);
    lcd.print(">");
    lcd.print("Light settings ");
  }
}
void clickMenuPageLight() {
  menu.selectPage(LIGHT_PAGE);
  lcd.clear();
  menu.update();
}

void printMenuPagePomp() {
  if(menu.getSelectedLine() == 1) {
    lcd.setCursor(0, 0);
    lcd.print(">");
    lcd.print("Pomp settings ");
  }
}
void clickMenuPagePomp() {
  menu.selectPage(POMP_PAGE);
  lcd.clear();
  menu.update();
}

void printMenuPageClock() {
  if(menu.getSelectedLine() > 1) {
    lcd.setCursor(0, 0);
    if(menu.getSelectedLine() == 2) {
      lcd.print(">");
    }
    lcd.print("Clock settings ");
  }
}
void clickMenuPageClock() {
  menu.selectPage(CLOCK_PAGE);
  lcd.clear();
  menu.update();
}

//========== Light page ============================
void printLightPageTimeOn() {
  if(menu.getSelectedLine() == 0) {
    lcd.setCursor(0, 0);
    lcd.print(">");
    lcd.print("Set Time On ");
  }
}
void clickLightPageTimeOn() {
  menu.selectPage(TIME_ON_PAGE);
  lcd.clear();
  menu.update();
}

void printLightPageTimeOff() {
  if(menu.getSelectedLine() > 0) {
    lcd.setCursor(0, 0);
    if(menu.getSelectedLine() == 1) {
      lcd.print(">");
    }
    lcd.print("Set Time Off ");
  }
}
void clickLightPageTimeOff() {
  menu.selectPage(TIME_OFF_PAGE);
  lcd.clear();
  menu.update();
}

//========== Pomp page ============================
void printPompPagePeriod() {
  if(menu.getSelectedLine() == 0) {
    lcd.setCursor(0, 0);
    lcd.print(">");
    lcd.print("Set Period ");
  }
}
void clickPompPagePeriod() {

}

void printPompPageInterval() {
  if(menu.getSelectedLine() > 0) {
    lcd.setCursor(0, 0);
    if(menu.getSelectedLine() == 1) {
      lcd.print(">");
    }
    lcd.print("Set Interval ");
  }
}
void clickPompPageInterval() {

}

//========== Clock page ============================
void printClockPageDate() {
  if(menu.getSelectedLine() == 0) {
    lcd.setCursor(0, 0);
    lcd.print(">");
    lcd.print("Set Date ");
  }
}
void clickClockPageDate() {

}

void printClockPageTime() {
  if(menu.getSelectedLine() > 0) {
    lcd.setCursor(0, 0);
    if(menu.getSelectedLine() == 1) {
      lcd.print(">");
    }
    lcd.print("Set Time ");
  }
}
void clickClockPageTime() {

}

//========== Light set time on page ============================
void onSetTimeOnHoursUp() { 
  timeOnData.hours = timeOnData.hours == 23 ? 0 : timeOnData.hours + 1; 
  writeTimeOnSettings();
}
void onSetTimeOnHoursDown() { 
  timeOnData.hours = timeOnData.hours == 0 ? 23 : timeOnData.hours - 1;
  writeTimeOnSettings();
}
void onSetTimeOnMinutesUp() { 
  timeOnData.minutes = timeOnData.minutes == 59 ? 0 : timeOnData.minutes + 1;
  writeTimeOnSettings();
}
void onSetTimeOnMinutesDown() { 
  timeOnData.minutes = timeOnData.minutes == 0 ? 59 : timeOnData.minutes - 1;
  writeTimeOnSettings();
}

void printSetTimeOn() {
  lcd.setCursor(0, 0);
  lcd.print("Time On ");

  char buf[16];
  
  if(menu.getSelectedLine() == 0) {
    if(setTimeOnItemsList.getSelectedItem() == 0) {
      snprintf(buf, sizeof(buf), "<%02d>:%02d", timeOnData.hours, timeOnData.minutes);
    } else if(setTimeOnItemsList.getSelectedItem() == 1) {
      snprintf(buf, sizeof(buf), "%02d:<%02d>", timeOnData.hours, timeOnData.minutes);
    }
  } else {
    snprintf(buf, sizeof(buf), "%02d:%02d  ", timeOnData.hours, timeOnData.minutes);
  }

  lcd.print(buf);
}

//========== Light set time off page ============================
void onSetTimeOffHoursUp() { 
  timeOffData.hours = timeOffData.hours == 23 ? 0 : timeOffData.hours + 1;
  writeTimeOffSettings();
}
void onSetTimeOffHoursDown() { 
  timeOffData.hours = timeOffData.hours == 0 ? 23 : timeOffData.hours - 1;
  writeTimeOffSettings();
}
void onSetTimeOffMinutesUp() { 
  timeOffData.minutes = timeOffData.minutes == 59 ? 0 : timeOffData.minutes + 1;
  writeTimeOffSettings();
}
void onSetTimeOffMinutesDown() { 
  timeOffData.minutes = timeOffData.minutes == 0 ? 59 : timeOffData.minutes - 1;
  writeTimeOffSettings();
}

void printSetTimeOff() {
  lcd.setCursor(0, 0);
  lcd.print("Time Off ");

  char buf[16];
  
  if(menu.getSelectedLine() == 0) {
    if(setTimeOffItemsList.getSelectedItem() == 0) {
      snprintf(buf, sizeof(buf), "<%02d>:%02d", timeOffData.hours, timeOffData.minutes);
    } else if(setTimeOffItemsList.getSelectedItem() == 1) {
      snprintf(buf, sizeof(buf), "%02d:<%02d>", timeOffData.hours, timeOffData.minutes);
    }
  } else {
    snprintf(buf, sizeof(buf), "%02d:%02d  ", timeOffData.hours, timeOffData.minutes);
  }

  lcd.print(buf);
}

volatile uint8_t currentRelay = 1;

volatile long lastTime = millis();

volatile int value = 0;

bool buttonWasPressed = false;
uint8_t clicks = 0;

void updateButton() {
  if(digitalRead(ENCODER_SW_PIN) == 0) {
    delay(20);
    if(digitalRead(ENCODER_SW_PIN) == 0) {
      buttonWasPressed = true;
    }
  } else {
    if(buttonWasPressed) {
      buttonWasPressed = !buttonWasPressed;
      clicks++;
    }
  }
}

uint8_t getClicks() {
  uint8_t tmp = clicks;
  clicks = 0;
  return tmp;
}

void updateLightState() {
  Time t = rtc.time();

  if((t.hr == timeOnData.hours) && (t.min == timeOnData.minutes)) {
    digitalWrite(RELAY_A_PIN, 1);
    Serial.println("led is on");
  }

  if((t.hr == timeOffData.hours) && (t.min == timeOffData.minutes)) {
    digitalWrite(RELAY_A_PIN, 0);
    Serial.println("led is off");
  }
}

//#define SET_TIME

void setup() 
{
  Serial.begin(9600);
  Serial.println("I am ready!");

  readSettings();
  Serial.println("Settings has been readed");

	lcd.begin();

	lcd.backlight();

  rtcInit(rtc);

#ifdef SET_TIME
  Time t(2021, 04, 01, 12, 00, 00, Time::kWednesday);
  rtcSetTime(rtc, t);
#endif
  
  relaysInit();
}

void loop() 
{
  encoderUpdate(encoder);
  encoderHandle();

  updateButton();

  if(getClicks() > 0) {
    menu.click();
    menu.update();
  }

  if(millis() - lastTime >= 1000) 
  {
    updateLightState();
    //relaysBlink();
    menu.update();
    
    lastTime = millis();
  }

  delay(20);
}

// void printValue() {
//   char buf[16];
//   snprintf(buf, sizeof(buf), "value = %4d", value);
//   lcd.print(buf);
// }

void encoderHandle() {
  if(millis() - lastEncoderTime >= 100) 
  {
    lastEncoderTime = millis();
    
    EncoderStates state = encoderReadState();

    switch (state)
    {
    case TurnLeft:
      menu.up();
      menu.update();
      break;
    case TurnRight:
      menu.down();
      menu.update();
      break;
    
    default:
      break;
    }

    if(state != Nothing) {
      
    }
  }
}

void relaysBlink() 
{
  digitalWrite(relayPins[currentRelay], 1);
  currentRelay++;
  if(currentRelay > 2) currentRelay = 1;
  digitalWrite(relayPins[currentRelay], 0);
}

void relaysInit()
{
  for(uint8_t i = 0; i < 3; ++i) {
    pinMode(relayPins[i], 1);
  }
}



