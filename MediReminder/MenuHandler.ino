//Hack Variables
int index = 0;
unsigned int action = 0;

//Variables for select key debounce handling
int select_buttonState;
int select_lastButtonState = LOW;
unsigned long select_debounceDelay = 100;

//Variables for menus
char mainMenuOptions[5][14] = {"SET ALARM", "ADJUST TIME", "ADJUST DATE", "DISABLE ALARM", "EXIT"};
char diableClockOptions[4][8] = {"ALARM 1", "ALARM 2", "ALARM 3", "BACK"};
char alarmSetOptions[4][8] = {"ALARM 1", "ALARM 2", "ALARM 3", "BACK"};
char alarmSetCommands[2][7] = {"SET", "CANCEL"};
char disableAlarmCommands[2][7] = {"UNSET", "CANCEL"};
unsigned int optionArrowRow = 0, optionArrowColumn = 0;



void showMainMenu() {
  lcd.clear();
  while (1) {
    handleVerticalArrowMovement(MAIN_MENU);

    if (optionArrowRow < 2) {
      displayMenu(MAIN_MENU_PAGE_1);
    } else if (optionArrowRow < 4) {
      displayMenu(MAIN_MENU_PAGE_2);
    } else {
      displayMenu(MAIN_MENU_PAGE_3);
    }

    action = selectKeyFunctionMapping();

    if (action == EXIT_CODE_MAIN_MENU) break;
    if (action == MOVE_TO_SET_ALARM_MENU) showSetAlarm();
    if (action == MOVE_TO_DISABLE_ALARM) showDisableAlarm();
    if (action == MOVE_TO_ADJUST_TIME) showAdjustTime();
    if (action == MOVE_TO_ADJUST_DATE) showAdjustDate();

  }
  lcd.clear();
  delay(200); //switch debounce handling
}

void showDisableAlarm() {
  lcd.clear();
  while (1) {

    while (state == DISABLE_ALARM_FINAL) {
      handleHorizontalArrowMovement();
      displayMenuWithTime(DISABLE_ALARM_INTERFACE, temp);
      selectKeyFunctionMapping();
    }

    handleVerticalArrowMovement(DISABLE_ALARM);

    if (optionArrowRow < 2) {
      displayMenu(DISABLE_ALARM_PAGE_1);
    } else {
      displayMenu(DISABLE_ALARM_PAGE_2);
    }



    action = selectKeyFunctionMapping();

    if (action == EXIT_CODE_DISABLE_ALARM) break;
  }
  lcd.clear();
  delay(200); //switch debounce handling
}

void showSetAlarm() {
  lcd.clear();
  while (1) {
    handleVerticalArrowMovement(SET_ALARM_MENU);

    if (optionArrowRow < 2) {
      displayMenu(SET_ALARM_MENU_PAGE_1);
    } else {
      displayMenu(SET_ALARM_MENU_PAGE_2);
    }

    action = selectKeyFunctionMapping();

    if (action == MOVE_TO_SET_ALARM) setAlarm();
    if (action == EXIT_CODE_SET_ALARM_MENU) break;
  }
  lcd.clear();
  delay(200); //switch debounce handling
}


void setAlarm() {
  lcd.clear();

  displayMenuWithTime(SET_ALARM, temp);

  while (state == INSIDE_SET_ALARM) {
    decreaseValue();
    increaseValue();
    decrementCursor();
    incrementCursor();

    action = selectKeyFunctionMapping();

    if (action == MOVE_TO_SET_ALARM_FINAL) lcd.clear();

    while (state == SET_ALARM_FINAL) {
      handleHorizontalArrowMovement();
      displayMenuWithTime(SET_ALARM_CONFIRM, 0);
      selectKeyFunctionMapping();
    }
  }
  lcd.clear();
  delay(200); //switch debounce handling
}

void showAdjustTime() {
  lcd.clear();

  displayMenu(ADJUST_TIME);

  while (state == INSIDE_ADJUST_TIME) {
    decreaseValue();
    increaseValue();
    decrementCursor();
    incrementCursor();

    action = selectKeyFunctionMapping();

    if (action == MOVE_TO_ADJUST_TIME_FINAL) lcd.clear();

    while (state == ADJUST_TIME_FINAL) {
      handleHorizontalArrowMovement();
      displayMenu(ADJUST_TIME_CONFIRM);
      selectKeyFunctionMapping();
    }
  }
  lcd.clear();
  delay(200); //switch debounce handling
}

void showAdjustDate() {
  lcd.clear();

  displayMenu(ADJUST_DATE);

  while (state == INSIDE_ADJUST_DATE) {
    decValue();
    incValue();
    cursorRight();
    cursorLeft();

    action = selectKeyFunctionMapping();

    if (action == MOVE_TO_ADJUST_DATE_FINAL) lcd.clear();

    while (state == ADJUST_DATE_FINAL) {
      handleHorizontalArrowMovement();
      displayMenu(ADJUST_DATE_CONFIRM);
      selectKeyFunctionMapping();
    }
  }
  lcd.clear();
  delay(200); //switch debounce handling
}


void handleVerticalArrowMovement(int menuId) {
  menuDownFunction();
  menuUpFunction();
  switch (menuId) {
    case MAIN_MENU:
      if (optionArrowRow < 0) optionArrowRow = 4;
      if (optionArrowRow > 4) optionArrowRow = 0;
      break;
    case DISABLE_ALARM:
      if (optionArrowRow < 0) optionArrowRow = 3;
      if (optionArrowRow > 3) optionArrowRow = 0;
      break;
    case SET_ALARM_MENU:
      if (optionArrowRow < 0) optionArrowRow = 3;
      if (optionArrowRow > 3) optionArrowRow = 0;
      break;
  }
}

void handleHorizontalArrowMovement() {
  if (digitalRead(MOVE_RIGHT) == HIGH) {
    optionArrowColumn = 7;
    lcd.clear();
  }

  if (digitalRead(MOVE_LEFT) == HIGH) {
    optionArrowColumn = 0;
    lcd.clear();
  }
}

void menuDownFunction() {
  int reading = digitalRead(DECREMENT);

  if (reading != decrement_lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != decrement_buttonState) {
      decrement_buttonState = reading;

      if (decrement_buttonState == HIGH) {
        lcd.clear();
        optionArrowRow++;
      }

    }
  }
  decrement_lastButtonState = reading;
}

void menuUpFunction() {
  int reading = digitalRead(INCREMENT);

  if (reading != increment_lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != increment_buttonState) {
      increment_buttonState = reading;

      if (increment_buttonState == HIGH) {
        lcd.clear();
        optionArrowRow--;
      }

    }
  }
  increment_lastButtonState = reading;
}


void displayMenuWithTime(unsigned int menuId, int whichAlarm) {
  String prevAlarm = "NOT SET";
  if (whichAlarm == 1) prevAlarm = alarmOne;
  if (whichAlarm == 2) prevAlarm = alarmTwo;
  if (whichAlarm == 3) prevAlarm = alarmThree;

  switch (menuId) {
    case DISABLE_ALARM_INTERFACE:
      lcd.setCursor(5, 0);
      lcd.print(prevAlarm);
      lcd.setCursor(optionArrowColumn, 1);
      lcd.write(0);
      lcd.setCursor(1, 1);
      lcd.print(disableAlarmCommands[0]);
      lcd.setCursor(8, 1);
      lcd.print(disableAlarmCommands[1]);
      break;
    case SET_ALARM:
      lcd.setCursor(0, 1);
      lcd.print("Prev Alrm");
      lcd.setCursor(9, 1);
      lcd.print(whichAlarm);
      lcd.setCursor(10, 1);
      lcd.print(":");
      lcd.setCursor(11, 1);
      lcd.print(prevAlarm);
      lcd.setCursor(5, 0);
      printTime(alarmClock);
      lcd.setCursor(5, 0);
      lcd.blink();
      break;
    case SET_ALARM_CONFIRM:
      lcd.noBlink();
      lcd.noCursor();
      lcd.setCursor(5, 0);
      printTime(alarmClock);
      lcd.setCursor(optionArrowColumn, 1);
      lcd.write(0);
      lcd.setCursor(2, 1);
      lcd.print(alarmSetCommands[0]);
      lcd.setCursor(9, 1);
      lcd.print(alarmSetCommands[1]);
      break;
  }
}


void displayMenu(unsigned int menu_id) {
  switch (menu_id) {
    case MAIN_MENU_PAGE_1:
      lcd.setCursor(0, optionArrowRow);
      lcd.write(0);
      lcd.setCursor(2, 0);
      lcd.print(mainMenuOptions[0]);
      lcd.setCursor(2, 1);
      lcd.print(mainMenuOptions[1]);
      break;

    case MAIN_MENU_PAGE_2:
      lcd.setCursor(0, optionArrowRow - 2);
      lcd.write(0);
      lcd.setCursor(2, 0);
      lcd.print(mainMenuOptions[2]);
      lcd.setCursor(2, 1);
      lcd.print(mainMenuOptions[3]);
      break;

    case MAIN_MENU_PAGE_3:
      lcd.setCursor(0, optionArrowRow - 4);
      lcd.write(0);
      lcd.setCursor(2, 0);
      lcd.print(mainMenuOptions[4]);
      break;

    case DISABLE_ALARM_PAGE_1:
      lcd.setCursor(0, optionArrowRow);
      lcd.write(0);
      lcd.setCursor(2, 0);
      lcd.print(diableClockOptions[0]);
      lcd.setCursor(2, 1);
      lcd.print(diableClockOptions[1]);
      break;

    case DISABLE_ALARM_PAGE_2:
      lcd.setCursor(0, optionArrowRow - 2);
      lcd.write(0);
      lcd.setCursor(2, 0);
      lcd.print(diableClockOptions[2]);
      lcd.setCursor(2, 1);
      lcd.print(diableClockOptions[3]);
      break;

    case SET_ALARM_MENU_PAGE_1:
      lcd.setCursor(0, optionArrowRow);
      lcd.write(0);
      lcd.setCursor(2, 0);
      lcd.print(alarmSetOptions[0]);
      lcd.setCursor(2, 1);
      lcd.print(alarmSetOptions[1]);
      break;

    case SET_ALARM_MENU_PAGE_2:
      lcd.setCursor(0, optionArrowRow - 2);
      lcd.write(0);
      lcd.setCursor(2, 0);
      lcd.print(alarmSetOptions[2]);
      lcd.setCursor(2, 1);
      lcd.print(alarmSetOptions[3]);
      break;

    case ALARM_RINGING:
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("PRESS SEL TO");
      lcd.setCursor(4, 1);
      lcd.print("TURN OFF");
      break;

    case ADJUST_TIME:
      lcd.setCursor(5, 0);
      printTime(alarmClock);
      lcd.setCursor(5, 0);
      lcd.blink();
      break;

    case ADJUST_TIME_CONFIRM:
      lcd.noBlink();
      lcd.noCursor();
      lcd.setCursor(5, 0);
      printTime(alarmClock);
      lcd.setCursor(optionArrowColumn, 1);
      lcd.write(0);
      lcd.setCursor(2, 1);
      lcd.print(alarmSetCommands[0]);
      lcd.setCursor(9, 1);
      lcd.print(alarmSetCommands[1]);
      break;

    case ADJUST_DATE:
      lcd.setCursor(3, 0);
      printDate();
      lcd.setCursor(3, 0);
      lcd.blink();
      break;

    case ADJUST_DATE_CONFIRM:
      lcd.noBlink();
      lcd.noCursor();
      lcd.setCursor(3, 0);
      printDate();
      lcd.setCursor(optionArrowColumn, 1);
      lcd.write(0);
      lcd.setCursor(2, 1);
      lcd.print(alarmSetCommands[0]);
      lcd.setCursor(9, 1);
      lcd.print(alarmSetCommands[1]);
      break;
  }

}
