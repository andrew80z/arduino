# arduino
 ## Arduino projects ##

####  Watering system ####
   // Button and button LED pins <br />
 -- measureLedPin = 3<br /> 
 -- measureButtonPin = 4<br />
 -- RTC and LCD are connected via i2C<br />
 -- Boundary value of dryness is 30%<br />
 -- Humividty sensor is connected to A0 pin<br />
 -- Maxmimum value of sensor(0%) = 682;<br />
 -- Minimum value of sensor(100%) = 456;<br />
 -- Relay pin = 8;<br />
 -- Humidity check will be done each 31st minute<br />
 -- Pump working time is 5 mins(300000 ms)<br />
 -- Pressing button forces humidity check<br />
