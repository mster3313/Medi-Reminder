unsigned int selectKeyFunctionMapping() {
  int reading = digitalRead(SELECT_KEY);

  if (reading != select_lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > select_debounceDelay) {
    if (reading != select_buttonState) {
      select_buttonState = reading;

      if (select_buttonState == HIGH) {
        backLightOnMoment = millis() / 60000;
        if (state == LCD_OFF) {
          backLightOnMoment = millis() / 60000;   //reset backlight on moment
          state = CLOCK_DISPLAY;
          lcd.backlight();
          return;
        } else if (state == CLOCK_DISPLAY) {
          state = INSIDE_MAIN_MENU;
          showMainMenu();
        } else if ( state == INSIDE_MAIN_MENU) {

          String returned = mainMenuOptions[optionArrowRow];

          if (returned.equals("SET ALARM")) {
            optionArrowRow = 0;
            state = INSIDE_SET_ALARM_MENU;
            return MOVE_TO_SET_ALARM_MENU;
          } else if (returned.equals("ADJUST TIME")) {
            state = INSIDE_ADJUST_TIME;
            return MOVE_TO_ADJUST_TIME;
          } else if (returned.equals("DISABLE ALARM")) {
            optionArrowRow = 0;
            state = INSIDE_DISABLE_ALARM;
            return MOVE_TO_DISABLE_ALARM;
          } else if (returned.equals("ADJUST DATE")) {
            state = INSIDE_ADJUST_DATE;
            return MOVE_TO_ADJUST_DATE;
          } else {
            optionArrowRow = 0;
            state = CLOCK_DISPLAY;
            return EXIT_CODE_MAIN_MENU;
          }
        } else if (state == INSIDE_ADJUST_TIME) {
          optionArrowRow = 1;
          optionArrowColumn = 0;
          state = ADJUST_TIME_FINAL;
          return MOVE_TO_ADJUST_TIME_FINAL;

        } else if (state == ADJUST_TIME_FINAL) {
          if (optionArrowColumn == 0) index = 0;
          if (optionArrowColumn == 7) index = 1;

          String returned = alarmSetCommands[index];

          if (returned.equalsIgnoreCase("SET")) {
            optionArrowRow = 0;
            optionArrowColumn = 0;
            setTime();
            state = INSIDE_MAIN_MENU;
          } else if (returned.equalsIgnoreCase("CANCEL")) {
            optionArrowRow = 0;
            optionArrowColumn = 0;
            state = INSIDE_MAIN_MENU;
            lcd.clear();
          }
        } else if (state == INSIDE_ADJUST_DATE) {
          optionArrowRow = 1;
          optionArrowColumn = 0;
          state = ADJUST_DATE_FINAL;
          return MOVE_TO_ADJUST_DATE_FINAL;

        } else if (state == ADJUST_DATE_FINAL) {
          if (optionArrowColumn == 0) index = 0;
          if (optionArrowColumn == 7) index = 1;

          String returned = alarmSetCommands[index];

          if (returned.equalsIgnoreCase("SET")) {
            optionArrowRow = 0;
            optionArrowColumn = 0;
            setDate();
            state = INSIDE_MAIN_MENU;
          } else if (returned.equalsIgnoreCase("CANCEL")) {
            optionArrowRow = 0;
            optionArrowColumn = 0;
            state = INSIDE_MAIN_MENU;
            lcd.clear();
          }
        } else if (state == INSIDE_DISABLE_ALARM) {
          String returned = diableClockOptions[optionArrowRow];
          if (returned.equals("ALARM 1")) {
            if (alarmOne != NULL) {
              moveToDisableMenu(alarmOne);      //Shows the current set alarm, UNSET and CANCEL options
              temp = 1;                         //Setting the alarm_one
            } else {
              moveBack("ALARM 1");
            }
          } else if (returned.equals("ALARM 2")) {
            if (alarmOne != NULL) {
              moveToDisableMenu(alarmTwo);
              temp = 2;                         //Setting the alarm_two
            } else {
              moveBack("ALARM 2");
            }
          } else if (returned.equals("ALARM 3")) {
            if (alarmOne != NULL) {
              moveToDisableMenu(alarmThree);
              temp = 3;                         //Setting the alarm_three
            } else {
              moveBack("ALARM 3");
            }
          } else {
            optionArrowRow = 0;
            optionArrowColumn = 0;
            state = INSIDE_MAIN_MENU;
            return EXIT_CODE_DISABLE_ALARM;
          }

        } else if (state == DISABLE_ALARM_FINAL) {
          if (optionArrowColumn == 0) index = 0;
          if (optionArrowColumn == 7) index = 1;

          String returned = disableAlarmCommands[index];

          if (returned.equals("UNSET")) {
            optionArrowRow = 0;
            optionArrowColumn = 0;
            disableAlarm();
            state = INSIDE_DISABLE_ALARM;
            lcd.clear();
          } else if (returned.equals("CANCEL")) {
            optionArrowRow = 0;
            optionArrowColumn = 0;
            state = INSIDE_DISABLE_ALARM;
            lcd.clear();
          }

        } else if (state == INSIDE_SET_ALARM_MENU) {
          String returned = alarmSetOptions[optionArrowRow];
          if (returned.equals("ALARM 1")) {
            temp = 1;
            state = INSIDE_SET_ALARM;
            return MOVE_TO_SET_ALARM;
          } else if (returned.equals("ALARM 2")) {
            temp = 2;
            state = INSIDE_SET_ALARM;
            return MOVE_TO_SET_ALARM;
          } else if (returned.equals("ALARM 3")) {
            temp = 3;
            state = INSIDE_SET_ALARM;
            return MOVE_TO_SET_ALARM;
          } else {
            optionArrowRow = 0;
            state = INSIDE_MAIN_MENU;
            return EXIT_CODE_SET_ALARM_MENU;
          }
        } else if (state == INSIDE_SET_ALARM) {
          optionArrowRow = 1;
          optionArrowColumn = 0;
          state = SET_ALARM_FINAL;
          return MOVE_TO_SET_ALARM_FINAL;

        } else if (state == SET_ALARM_FINAL) {
          if (optionArrowColumn == 0) index = 0;
          if (optionArrowColumn == 7) index = 1;

          String returned = alarmSetCommands[index];

          if (returned.equalsIgnoreCase("SET")) {
            optionArrowRow = 0;
            optionArrowColumn = 0;
            enableAlarm();
            state = INSIDE_SET_ALARM_MENU;
          } else if (returned.equalsIgnoreCase("CANCEL")) {
            optionArrowRow = 0;
            optionArrowColumn = 0;
            state = INSIDE_SET_ALARM_MENU;
            lcd.clear();
          }
        }
      }

    }
  }
  select_lastButtonState = reading;
}

void moveToDisableMenu(String currentAlarm) {
  optionArrowColumn = 0;
  optionArrowRow = 1;
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print(currentAlarm);
  displayMenu(DISABLE_ALARM_INTERFACE);
  state = DISABLE_ALARM_FINAL;
}

void moveBack(String alarm) {
  optionArrowRow = 0;
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print(alarm);
  lcd.setCursor(3, 1);
  lcd.print("Not Created");
  delay(1500);
  lcd.clear();
  state = INSIDE_DISABLE_ALARM;
}
