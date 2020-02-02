#define MORNING 1
#define AFTERNOON 2
#define NIGHT 3

int date[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 2020};

int hours;
int minutes;
int days;
int months;

int cursorPositionDate = 3;
int currentValueDate = 0;

String getDateInfo(RTC_DS1307 rtc) {
  int dd=0,mm=0;
  String d="",m="";
  DateTime now = rtc.now();

  dd = now.day();
  mm = now.month();

  d = String(now.day());
  m = String(now.month());

  if (dd < 10) d = "0" + d;
  if (mm < 10) m = "0" + m;

  String dateString =  d + "." + m + "." + String(now.year());
  return dateString;
}

String getTimeInfo(RTC_DS1307 rtc, int MODE) {
  String h = "", m = "", returnTime = "", postfix = " AM";
  int hours = 0, minutes = 0;
  DateTime now = rtc.now();

  hours = now.hour();
  minutes = now.minute();

  h = String(now.hour());
  m = String(now.minute());

  if (MODE == 0) {
    if (hours < 10)  h = "0" + String(hours);
    if (minutes < 10) m = "0" + String(minutes);

    returnTime = h + ":" + m;
    return returnTime;
  } 
}

void setTime() {
  String h = String(alarmClock[0]) + String(alarmClock[1]) ;
  String m = String(alarmClock[3]) + String(alarmClock[4]) ;
  hours = h.toInt();
  minutes = m.toInt();

  rtc.adjust(DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), hours, minutes, 0));
}

void setDate() {
  String d = String(date[0]) + String(date[1]) ;
  String m = String(date[3]) + String(date[4]) ;
  days = d.toInt();
  months = m.toInt();
  rtc.adjust(DateTime(date[9], months, days, rtc.now().hour(), rtc.now().minute(), 0));
}

void printDate() {
  lcd.print(date[0]);
  lcd.print(date[1]);
  lcd.print(".");
  lcd.print(date[3]);
  lcd.print(date[4]);
  lcd.print(".");
  lcd.print(date[9]);
}

void cursorRight() {
  int reading = digitalRead(MOVE_RIGHT);

  if (reading != move_right_lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != move_right_buttonState) {
      move_right_buttonState = reading;

      if (move_right_buttonState == HIGH) {
        ++cursorPositionDate;
        if (cursorPositionDate == 5 || cursorPositionDate == 8) ++cursorPositionDate;
        if (cursorPositionDate < 3) cursorPositionDate = 12;
        if (cursorPositionDate > 8 && cursorPositionDate < 12) cursorPositionDate = 12;
        if (cursorPositionDate > 12) cursorPositionDate = 3;
        lcd.setCursor(cursorPositionDate, 0);
      }

    }
  }
  move_right_lastButtonState = reading;
}

void cursorLeft() {
  int reading = digitalRead(MOVE_LEFT);

  if (reading != move_left_lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != move_left_buttonState) {
      move_left_buttonState = reading;

      if (move_left_buttonState == HIGH) {
        cursorPositionDate -- ;
        if (cursorPositionDate == 5 || cursorPositionDate == 8) --cursorPositionDate;
        if (cursorPositionDate < 3) cursorPositionDate = 12;
        if (cursorPositionDate < 12 && cursorPositionDate > 8) cursorPositionDate = 7;
        if (cursorPositionDate > 12) cursorPositionDate = 3;
        lcd.setCursor(cursorPositionDate, 0);
      }

    }
  }
  move_left_lastButtonState = reading;
}

void incValue() {
  int reading = digitalRead(INCREMENT);

  if (reading != increment_lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != increment_buttonState) {
      increment_buttonState = reading;

      if (increment_buttonState == HIGH) {
        currentValueDate = date[cursorPositionDate - 3];
        currentValueDate ++;
        date[cursorPositionDate - 3] = handleInvalidDateSettings(cursorPositionDate, currentValueDate);
        lcd.clear();
        lcd.setCursor(3, 0);
        printDate();
        lcd.setCursor(cursorPositionDate, 0);
      }

    }
  }
  increment_lastButtonState = reading;
}

void decValue() {
  int reading = digitalRead(DECREMENT);

  if (reading != decrement_lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != decrement_buttonState) {
      decrement_buttonState = reading;

      if (decrement_buttonState == HIGH) {
        currentValueDate = date[cursorPositionDate - 3];
        currentValueDate --;
        date[cursorPositionDate - 3] = handleInvalidDateSettings(cursorPositionDate, currentValueDate);
        lcd.clear();
        lcd.setCursor(3, 0);
        printDate();
        lcd.setCursor(cursorPositionDate, 0);
      }

    }
  }
  decrement_lastButtonState = reading;
}


unsigned int handleInvalidDateSettings(int cursor_position, int current_value) {
  if (cursor_position == 3) {
    if (current_value > 3) current_value = 0;
    if (current_value < 0) current_value = 3;
  }
  if (cursor_position == 4) {
    if (current_value > 9) current_value = 0;
    if (current_value < 0) current_value = 9;
  }
  if (cursor_position == 6) {
    if (current_value > 1) current_value = 0;
    if (current_value < 0) current_value = 1;
  }
  if (cursor_position == 7) {
    if (current_value > 9) current_value = 0;
    if (current_value < 0) current_value = 9;
  }
  if (cursor_position == 12) {
    if (current_value < 2019) current_value = 2019;
  }
  return current_value;
}
