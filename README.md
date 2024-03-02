# arduino-alarm-clock
---
This project implements alarm clock functionalities on an Arduino board in C.

- Hardware: Adafruit RGB 16x2 Character LCD Shield
- Programming Language: C

### Functional Requirements
---
- As default behaviour, the alarm clock displays the current time
- Set the current time in hour/minute/second
- Display the current alarm time on the LCD
- Set current alarm time in hour/minute/second
- Turn on/off alarm
- Leave the menu (return to default behaviour)
- Alarm goes off (indicated on the LCD)
- Using buttons on the LCD keypad shield, The user can then disable the alarm permanently, or for 30 seconds (“snooze function”)

### State Diagram and Descriptions
---
![[Pasted image 20240302191421.png]]
**DISPLAY_TIME_ALARM_OFF** 
	The default state when the program is initiated. 
	It displays the curent time with the alarm off; on the lcd display, the time is on the top left corner, with the rest of the screen accommodating what the button presses will do.
	Buttons:
	- Up: edit the time
	- Down: edit the alarm time
	- Left: turn the alarm on
	- Right: show the current alarm time
	![[Screenshot 2024-03-02 at 19.39.54.png]]

**DISPLAY_TIME_ALARM_ON** 
	Essentially the same as DISPLAY_ALARM_TIME_OFF with the alarm on. 
	When the current time reaches the alarm time, the alarm will go off. The functionality of the buttons are also the same as DISPLAY_ALARM_TIME_OFF but with the left button turning off the alarm instead of turning it on.
	![[Screenshot 2024-03-02 at 19.41.41.png]]

**DISPLAY_ALARM_TIME_OFF** 
	Displays alarm time when in the DISPLAY_TIME_ALARM_OFF state and the right button is pressed. State will automatically revert to DISPLAY_TIME_ALARM_OFF after 3 seconds.
**DISPLAY_ALARM_TIME_ON** 
	Identical functionality as DISPLAY_ALARM_TIME_OFF, but reverts to DISPLAY_TIME_ALARM_ON instead of DISPLAY_TIME_ALARM_OFF.
	![[Pasted image 20240302194321.png]]

**SET_CURRENT_TIME_OFF** and **SET_CURRENT_TIME_ON**
	Enables user to edit the current time using the arrow buttons.
	Toggled from main menu (up button on DISPLAY_TIME_ALARM_OFF/DISPLAY_TIME_ALARM_ON).
	Current time is set when the 'select' button is pressed, which brings up the main menu with the updated time.
	![[Pasted image 20240302194425.png]]

**SET_ALARM_TIME**
	Enables user to set the alarm time using the arrow buttons.
	Toggled from main menu (down button on DISPLAY_TIME_ALARM_OFF/DISPLAY_TIME_ALARM_ON).
	Alarm time is set when the 'select' button is pressed, which brings up the main menu with the updated time.
	![[Pasted image 20240302194530.png]]

**BUZZER_ON**
	Toggled when current time is the alarm time, and the alarm is on. 
	The LCD display will flash multiple colours, indicating that the alarm is going off.
	![[buzzer_on.gif]]

**SNOOZE**
	Toggled when the user presses any of the arrow keys when the alarm is going off. 
	Disables the alarm for 30 seconds, and will revert to the BUZZER_ON state after.
	![[snooze.gif]]