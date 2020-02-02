#include <FastLED.h>
#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>

// Variables for Led Strip ----
#define LED_PIN_ROW_1 7
#define LED_PIN_ROW_2 8
#define LED_PIN_ROW_3 9
#define COLOR_ORDER GRB
#define BRIGHTNESS 64
#define CHIPSET WS2812

#define NUM_STRIPS 3
#define NUM_LEDS_PER_STRIP 7
CRGB leds[NUM_STRIPS][NUM_LEDS_PER_STRIP];

// Variables for Real Time Clock -----
RTC_DS1307 rtc;
unsigned int mode = 0;

//Variable for Keypad
#define SELECT_KEY 2
#define MOVE_LEFT 5
#define MOVE_RIGHT 6
#define INCREMENT 4
#define DECREMENT 3

//Variables for LCD
#define BACKLIGHT_ON_TIME_MINUTES 0.5

LiquidCrystal_I2C lcd(0x27, 16, 2);
unsigned long backLightOnMoment;
byte menuarrow[8] = {  B10000, B10000, B10100, B10110, B11111, B00110, B00100};   //custom arrow symbol

//Variables for buzzer
const int buzzer = 10;

//State Variables
#define LCD_OFF 0
#define CLOCK_DISPLAY 1
#define INSIDE_MAIN_MENU 2
#define INSIDE_DISABLE_ALARM 3
#define DISABLE_ALARM_FINAL 4
#define INSIDE_SET_ALARM 5
#define INSIDE_SET_ALARM_MENU 6
#define SET_ALARM_FINAL 7
#define ADJUST_TIME_FINAL 8
#define INSIDE_ADJUST_TIME 9
#define INSIDE_ADJUST_DATE 10
#define ADJUST_DATE_FINAL 11


//MENU_IDs
#define MAIN_MENU 30
#define MAIN_MENU_PAGE_1 31
#define MAIN_MENU_PAGE_2 32
#define DISABLE_ALARM 33
#define DISABLE_ALARM_PAGE_1 34
#define DISABLE_ALARM_PAGE_2 35
#define DISABLE_ALARM_INTERFACE 36
#define SET_ALARM_MENU 37
#define SET_ALARM_MENU_PAGE_1 38
#define SET_ALARM_MENU_PAGE_2 39
#define ALARM_STATUS 40
#define SET_ALARM 41
#define SET_ALARM_CONFIRM 42
#define ALARM_RINGING 43
#define ADJUST_TIME 44
#define ADJUST_DATE 45
#define MAIN_MENU_PAGE_3 46
#define ADJUST_TIME_CONFIRM 47
#define ADJUST_DATE_CONFIRM 48

//Exit Codes
#define EXIT_CODE_MAIN_MENU 50
#define EXIT_CODE_DISABLE_ALARM 51
#define EXIT_CODE_SET_ALARM 52
#define EXIT_CODE_SET_ALARM_MENU 53

//Destination menu Codes
#define MOVE_TO_SET_ALARM 20
#define MOVE_TO_DISABLE_ALARM 21
#define MOVE_TO_SET_ALARM_MENU 22
#define MOVE_TO_SET_ALARM_FINAL 23
#define MOVE_TO_ADJUST_TIME_FINAL 24
#define MOVE_TO_ADJUST_TIME 25
#define MOVE_TO_ADJUST_DATE 26
#define MOVE_TO_ADJUST_DATE_FINAL 27

unsigned int state = CLOCK_DISPLAY;
boolean isOnAlarm = false;
unsigned int counter = 0;

String alarmOne = "";
String alarmTwo = "";
String alarmThree = "";

String times = "";
String dates = "";

int count = 0;

void setup() {
  //  Serial.begin(9600);

  //  getValuesFromEEPROMonStart();

  //Setting Keypad pins as inputs
  pinMode(SELECT_KEY, INPUT);
  pinMode(MOVE_LEFT, INPUT);
  pinMode(MOVE_RIGHT, INPUT);
  pinMode(INCREMENT, INPUT);
  pinMode(DECREMENT, INPUT);

  //buzzer
  pinMode(buzzer, OUTPUT);

  //Initializing LED Strip
  FastLED.addLeds<CHIPSET, LED_PIN_ROW_1, COLOR_ORDER>(leds[0], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<CHIPSET, LED_PIN_ROW_2, COLOR_ORDER>(leds[1], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<CHIPSET, LED_PIN_ROW_3, COLOR_ORDER>(leds[2], NUM_LEDS_PER_STRIP);

  FastLED.setBrightness( BRIGHTNESS );

  //Initialize Real Time Clock

  if (! rtc.begin()) {
    while (1);
  }

  if (! rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  delay(100);

  //Initialize LCD Display
  initDisplay();
  lcd.createChar(0, menuarrow);

}

void loop() {

  if ((millis() / 60000) - backLightOnMoment > BACKLIGHT_ON_TIME_MINUTES) {
    state = LCD_OFF;
    lcd.noBacklight();
  }

  times = getTimeInfo(rtc, mode);  //get time String in the format hh:mm
  dates = getDateInfo(rtc);         //get date String in the format dd.mm.yy

  //Printing time and date
  lcd.setCursor(5, 0);
  lcd.print(times);
  lcd.setCursor(3, 1);
  lcd.print(dates);

  //Show Main Menu when the SEL key is pressed
  selectKeyFunctionMapping();

  if (times.equalsIgnoreCase(alarmOne) || times.equalsIgnoreCase(alarmTwo) || times.equalsIgnoreCase(alarmThree)) {
    if (state == LCD_OFF) {
      lcd.backlight();
      backLightOnMoment = millis() / 60000;
      state = CLOCK_DISPLAY;
    }
    isOnAlarm = getAlarmStatus(getAlarmNumber(times));
    Serial.println(count);
    lightLED(count, rtc.now().dayOfTheWeek());
    while (isOnAlarm) {

      displayMenu(ALARM_RINGING);
      buzzAlarm(times);
      times = getTimeInfo(rtc, mode);
      isOnAlarm = getAlarmStatus(getAlarmNumber(times));
      lcd.clear();
    }
    turnOffAlarm (getAlarmNumber(times));
    ledOff(count, rtc.now().dayOfTheWeek());
  }
}

//Function to initialize lcd display and show welcome screen
void initDisplay() {
  lcd.begin();
  lcd.backlight();      //Turn on the backlight
  backLightOnMoment = millis() / 60000; //save the time when the backlight turned on
  lcd.setCursor(4, 0);  //Set the cursor at line 1 position 5
  lcd.print("Welcome");
  lcd.setCursor(1, 1);  //Set the cursor at line 2 position 2

  lcd.print("*MediReminder*");
  delay(2000);
  lcd.clear();          //Clearing the display for other things
}

void lightLED(unsigned int stripIndex, unsigned int ledNumber) {
  if (ledNumber == 0) ledNumber = 7;
  Serial.println(stripIndex + " " + ledNumber);
  if (stripIndex == 1) {
    Serial.println("sssss");
    leds[stripIndex][7 - ledNumber] = CHSV(255, 255, 250);
  } else {
    leds[stripIndex][ledNumber - 1] = CHSV(255, 255, 250);
  }
  FastLED.show();
}

void ledOff(unsigned int stripIndex, unsigned int ledNumber) {
  if (ledNumber == 0) ledNumber = 7;
  Serial.println(stripIndex + " " + ledNumber);
  if (stripIndex == 1) {
    leds[stripIndex][7 - ledNumber] = CRGB::Black;
  } else {
    leds[stripIndex][ledNumber - 1] = CRGB::Black;
  }
  FastLED.show();
}

int getRowToAlarm(String t) {
  if (t.equals(alarmOne)) return 0;
  if (t.equals(alarmTwo)) return 1;
  if (t.equals(alarmThree)) return 2;
}
