1.it is required to create a new KEIL project and name the project "project_1". The project folder is named "Name +
student ID".
2. Complete the specific requirements according to some of the program documents provided in the question. The requirements are as follows:
There are 8 LED lights on the experimental equipment, which are connected to pins 0-7 of GPIOE. LED hardware interface is as follows
As shown in the figure:
There are four buttons S1-S4, which are connected to pins 0-3 of GPIOC respectively. As shown below:
The alarm circuit is connected to the PC4 port. As shown in the figure below: 3. Control function
1) When S1 is pressed, the Gray code is used to display my student number on the LED, D1-D4 display the tens digit, and D5-D8
Displays the ones digit, LED off means "0", LED on means "1";
2) When S2 is pressed, the LEDs light up in pairs with an interval of 0.5S: (D4, D5) - (D3, D6) -
(D2, D7) - (D1, D8), after staying for 2S, the LEDs will turn off in pairs at 1S intervals:
(D1, D8) - (D2, D7) - (D3, D6) - (D4, D5);
3) After the serial port assistant sends "monitor" and then presses S2, the
The serial port prints the status of the LED lights, such as D4 and D5 ON or D1 and D8 OFF; send
Send any other command, the serial port prints the prompt message "command error: please input
again";
4) After the serial port assistant sends "alarm", the buzzer alarm sounds. The first alarm frequency is 0.5 Hz.
Lasts for 2S. The second alarm frequency is 1 Hz and lasts for 2S. This cycle lasts three times to end the alarm. hair
Send any other command, the serial port prints the prompt message "command error: please input
again".
4. Control requirements
(1) It is required to use key interrupt mode to control LED lights;
(2) The general timer TIM3 interrupt method is used to implement the required interval time;
(3) The alarm signal is required to be remapped to PC6 using TIM3_CH1

1. It is required to create a new KEIL project and name the project "project_2". The project folder is named "Name +
student ID".
2. Complete the specific requirements according to some of the program documents provided in the question. The requirements are as follows:
There are 8 LED lights on the experimental equipment, which are connected to pins 0-7 of GPIOE. LED hardware interface is as follows
As shown in the figure:
There are four buttons S1-S4, which are connected to pins 0-3 of GPIOC respectively. As shown below:
The alarm circuit is connected to the PC4 port. As shown in the figure below: 3. Control function
1) When S1 is pressed, the serial port assistant displays "When adjusting, please enter the adjustment value (00-23): ", through the serial port
The serial port assistant sends the corresponding adjustment value to modify the hour time. If the input data exceeds the range, the serial port assistant displays
Display "set error: please input again";
2) When S2 is pressed, the serial port assistant displays "Minute adjustment, please enter the adjustment value (00-59): ", through the serial port
The serial port assistant sends the corresponding adjustment value to modify the minute time. If the input data exceeds the range, the serial port assistant displays
Display "set error: please input again";
3) When S3 is pressed, the serial port assistant displays "Alarm clock setting, please enter the alarm time (format hh-mm): ",
Send the corresponding alarm value through the serial port assistant. When the input data format is incorrect, the serial port assistant will display
"format error: please input again", when the set alarm is reached, the buzzer alarm
Sounds, alarm frequency 1 Hz, does not stop;
4) When S4 is pressed, the serial port assistant displays "alarm off" and the buzzer stops the alarm.
4. Control requirements
(1) The buttons are required to be controlled by interrupt;
(2) The second clock is required to be implemented in the universal timer TIM3 interrupt mode;
(3) The alarm signal is required to be implemented by remapping TIM3_CH1 to PC6;
(4) Use serial port 1 for sending and receiving communication control;
(5) When no button is pressed, the serial port assistant displays the current time every second;
(6) Expansion options: When no button is pressed, the current time is displayed on the TFT LCD screen
