int move_right_buttonState;
int move_right_lastButtonState = LOW;

int move_left_buttonState;
int move_left_lastButtonState = LOW;

int increment_buttonState;
int increment_lastButtonState = LOW;

int decrement_buttonState;
int decrement_lastButtonState = LOW;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

int cursorPosition = 5;
int currentValue = 0;
unsigned int temp = 0;

int alarmClock[5] = {0, 0, 0, 0, 0};

boolean alarm_one = false;
boolean alarm_two = false;
boolean alarm_three = false;

void printTime(int timeArray[] ) {
  lcd.print(timeArray[0]);
  lcd.print(timeArray[1]);
  lcd.print(":");
  lcd.print(timeArray[3]);
  lcd.print(timeArray[4]);
}

String createAlarmString() {
  return String(alarmClock[0]) + String(alarmClock[1]) + ":" + String(alarmClock[3]) + String(alarmClock[4]);
}

void disableAlarm() {
  String diabledAlarm = "";
  if (temp == 1) {
    alarm_one = false;
    diabledAlarm = alarmOne;
    alarmOne = "";
  }
  if (temp == 2) {
    alarm_two = false;
    diabledAlarm = alarmTwo;
    alarmTwo = "";
  }
  if (temp == 3) {
    alarm_three = false;
    diabledAlarm = alarmThree;
    alarmThree = "";
  }
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print(diabledAlarm);
  lcd.setCursor(2, 1);
  lcd.print("Alarm Removed");
  delay(1500);
}

void enableAlarm() {
  String createdAlarm = "";
  if (temp == 1) {
    alarmOne = createAlarmString();
    createdAlarm = alarmOne;
    alarm_one = true;
  }
  if (temp == 2) {
    alarmTwo = createAlarmString();
    createdAlarm = alarmTwo;
    alarm_two = true;
  }
  if (temp == 3) {
    alarmThree = createAlarmString();
    createdAlarm = alarmThree;
    alarm_three = true;
  }
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print(createdAlarm);
  lcd.setCursor(2, 1);
  lcd.print("Alarm Created");
  delay(1500);

}

void incrementCursor() {
  int reading = digitalRead(MOVE_RIGHT);

  if (reading != move_right_lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != move_right_buttonState) {
      move_right_buttonState = reading;

      if (move_right_buttonState == HIGH) {
        ++cursorPosition;
        if (cursorPosition == 7) ++cursorPosition;
        if (cursorPosition < 5) cursorPosition = 5;
        if (cursorPosition > 9) cursorPosition = 9;
        lcd.setCursor(cursorPosition, 0);
      }

    }
  }
  move_right_lastButtonState = reading;
}

void decrementCursor() {
  int reading = digitalRead(MOVE_LEFT);

  if (reading != move_left_lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != move_left_buttonState) {
      move_left_buttonState = reading;

      if (move_left_buttonState == HIGH) {
        cursorPosition -- ;
        if (cursorPosition == 7) --cursorPosition;
        if (cursorPosition < 5) cursorPosition = 5;
        if (cursorPosition > 9) cursorPosition = 9;
        lcd.setCursor(cursorPosition, 0);
      }

    }
  }
  move_left_lastButtonState = reading;
}

void increaseValue() {
  int reading = digitalRead(INCREMENT);

  if (reading != increment_lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != increment_buttonState) {
      increment_buttonState = reading;

      if (increment_buttonState == HIGH) {
        currentValue = alarmClock[cursorPosition - 5];
        currentValue ++;
        alarmClock[cursorPosition - 5] = handleInvalidTimeSettings(cursorPosition, currentValue);
        lcd.clear();
        lcd.setCursor(5, 0);
        printTime(alarmClock);
        lcd.setCursor(cursorPosition, 0);
      }

    }
  }
  increment_lastButtonState = reading;
}

void decreaseValue() {
  int reading = digitalRead(DECREMENT);

  if (reading != decrement_lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != decrement_buttonState) {
      decrement_buttonState = reading;

      if (decrement_buttonState == HIGH) {
        currentValue = alarmClock[cursorPosition - 5];
        currentValue --;
        alarmClock[cursorPosition - 5] = handleInvalidTimeSettings(cursorPosition, currentValue);
        lcd.clear();
        lcd.setCursor(5, 0);
        printTime(alarmClock); 
        lcd.setCursor(cursorPosition, 0);
      }

    }
  }
  decrement_lastButtonState = reading;
}


unsigned int handleInvalidTimeSettings(int cursor_position, int current_value) {
  if (cursor_position == 5) {
    if (current_value > 2) current_value = 0;
    if (current_value < 0) current_value = 2;
  }
  if (cursor_position == 6) {
    if (current_value > 9) current_value = 0;
    if (current_value < 0) current_value = 9;
  }
  if (cursor_position == 8) {
    if (current_value > 5) current_value = 0;
    if (current_value < 0) current_value = 5;
  }
  if (cursor_position == 9) {
    if (current_value > 9) current_value = 0;
    if (current_value < 0) current_value = 9;
  }
  return current_value;
}
