#include "pitches.h" //add Equivalent frequency for musical note
#include "themes.h" //add Note vale and duration 

#define ALARM_ONE 100
#define ALARM_TWO 200
#define ALARM_THREE 300


boolean getAlarmStatus(unsigned int alarm) {
  if (alarm == ALARM_ONE) {
    return alarm_one;
  } else if (alarm == ALARM_TWO) {
    return alarm_two;
  } else if (alarm == ALARM_THREE) {
    return alarm_three;
  } else {
    return false;
  }
}

unsigned int getAlarmNumber(String t) {
  if (t.equals(alarmOne)) {
    count = 0;
    return ALARM_ONE;

  }
  if (t.equals(alarmTwo)) {
    count = 1;
    return ALARM_TWO;

  }
  if (t.equals(alarmThree)) {
    count = 2;
    return ALARM_THREE;
  }
}

void turnOffAlarm (unsigned int alarm) {
  switch (alarm) {
    case ALARM_ONE:
      alarm_one = false;
      alarmOne = "";
      break;
    case ALARM_TWO:
      alarm_two = false;
      alarmTwo = "";
      break;
    case ALARM_THREE:
      alarm_three = false;
      alarmThree = "";
      break;
  }
}
void buzzAlarm(String t) {
  for (int thisNote = 0; thisNote < (sizeof(Pirates_note) / sizeof(int)); thisNote++) {
    if (digitalRead(SELECT_KEY) == HIGH) {
      turnOffAlarm (getAlarmNumber(t));
      break;
    }
    int noteDuration = 1000 / Pirates_duration[thisNote];//convert duration to time delay
    tone(buzzer, Pirates_note[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.05; //Here 1.05 is tempo, increase to play it slower
    delay(pauseBetweenNotes);
    noTone(buzzer); //stop music on pin 8
  }

}
