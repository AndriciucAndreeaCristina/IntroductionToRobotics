# Introduction To Robotics 

                           
                    ********                                
                  *** *  * ***                              
                    * **** *    ***                         
                    ********    ***                         
                       ***       *                          
                  ************   *                          
              ***** * *  *   *****                          
              *   * *** ***  *                              
              *   * * **   * *                              
             ***  ************                              
             ***    **    **                                
                    **    **                                
                    **    **                                
                    **    **                                
                   ***   ***                                
                   ***   ***                                
                                                            
_Introduction to Robotics laboratory homeworks, taken in the 3rd year at the Faculty of Mathematics and Computer Science, University of Bucharest (2023-2024).
Each homework assignment is a comprehensive package that includes detailed requirements, implementation instructions, code samples, and supplementary image files._

<details>
<summary>
  
## Homework 1
</summary> <br>
For this task, I had to create this readMe and familiarise myself with the structure.
</details>
  
<details>
<summary>
  
## [Homework 2](https://github.com/AndriciucAndreeaCristina/IntroductionToRobotics/tree/main/RGB_LED_h1)
  </summary> <br>
This assignment focuses on controlling each channel (Red, Green, and Blue) of  an  RGB  LED  using  individual  potentiometers.  
I have used a different potentiometer to control each color of the RGB Led, reading the potentiometer value with Arduino and then mapping it to specifin intensities to the LED pins.

<h3>Setup: </h3> 

![setup](https://github.com/AndriciucAndreeaCristina/IntroductionToRobotics/assets/68044359/47a75023-532e-4695-98f9-9b0872d84482)

<h3>Demo: </h3>
https://youtu.be/knbZWBB40HQ
</details>

<details>
<summary>
  
## [Homework 3](https://github.com/AndriciucAndreeaCristina/IntroductionToRobotics/tree/main/Elevator_Wannabe)
</summary>
  <br>
This assignment involves simulating a 3-floor elevator control system using LEDs and buttons.

<h3>Setup: </h3>

![setup](https://github.com/AndriciucAndreeaCristina/IntroductionToRobotics/assets/68044359/61f4eaf6-157f-4928-8e11-576548da6bd9)

<h3>Demo: </h3>
https://youtu.be/bWM0YoEcFig
</details>

<details>
<summary>
  
## [Homework 4](https://github.com/AndriciucAndreeaCristina/IntroductionToRobotics/tree/main/7segment_display_drawing)
</summary> <br>
Using the joystick, one can control the position of the segments of a seven segment display and "draw" on the display. The initial position is on the DP. The current
position always blinks (irrespective of the fact that the segment is on or off), using the joystick to move from one position to neighbors. Short pressing the button toggles the segment state from ON to OFF or from OFF to ON. Long pressing the button resets the entire display by turning all the segments OFF and moving the
current position to the decimal point.

<h3>Setup: </h3>
![setup](https://github.com/AndriciucAndreeaCristina/IntroductionToRobotics/assets/68044359/e5ffcb0f-cb3d-4a3b-b891-5a13b636caac)

<h3>Demo: </h3>
https://youtube.com/shorts/XIElTIi3FpQ?feature=share 
</details>

<details>
<summary>
  
## [Homework 5](https://github.com/AndriciucAndreeaCristina/IntroductionToRobotics/tree/main/StopwatchTimer)
</summary> <br>
  For this task, I have implemented a stopwatch timer, using a 4 digit 7 segment display, 3 buttons and 3 leds, that also has a save lap functionality.
  Each button has a differnt functionality:
      - Button 1 (coresponding to the green led): Start/Pause
      - Button 2 (coresponding to the yellow led): Reset
      - Button 3 (coresponding to the red led): Save lap/Cycle through laps when in reset.
  The leds light up in different states of the timer to let you know which buttons you can press and when.
  
<h3>Setup: </h3>

![setup](https://github.com/AndriciucAndreeaCristina/IntroductionToRobotics/assets/68044359/529359e6-ec3d-40b6-8a70-05a4dd2b2c6d)

<h3>Demo: </h3>
https://www.youtube.com/shorts/Zt4Y0uZAbXI
</details>

<details>
<summary>
  
## [Homework 6](https://github.com/AndriciucAndreeaCristina/IntroductionToRobotics/tree/main/EnvironmentMonitor)
</summary> <br>
  For this task, I have implemented ”Smart Environment Monitor and Logger” using Arduino. I have used an Ultrasonic Sensor (HC-SR04) and a LDR (Light-Dependent Resistor), as well as a RGB Led for status outputs. I have implemented a menu structrure that allows the user to: 
    1. Set data about the sensors like:
        1.1. Sampling Interval
        1.2. Ultrasonic Alert Threshold
        1.3. LDR Alert Threshold
    2. Reset Logger Data
    3. Get information about the status of the system
        3.1. Current Sensor Readings
        3.2. Current Sensor Settings
        3.3. Last 10 sensor readings for all sensors stored in EEPROM
    4. RGB LED Control
        4.1. Manual Color Control
        4.2. LED: Toggle Automatic ON/OFF
  
<h3>Setup: </h3>

![setup](https://github.com/AndriciucAndreeaCristina/IntroductionToRobotics/assets/68044359/2e5bf776-4a59-4d08-8cc3-9788e1281612)


<h3>Demo: </h3>

_To be added_

</details>                                            
                                                            
                                                            
                                                            
                                                            
