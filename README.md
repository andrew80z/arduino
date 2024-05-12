# arduino
Arduino projects
1. Watering system
   // Button and button LED pins
   -- measureLedPin = 3;
   -- measureButtonPin = 4;
   -- RTC and LCD are connected via i2C
   -- Boundary value of dryness is 30%
   -- Humividty sensor is connected to A0 pin
   -- Maxmimum value of sensor(0%) = 682;
   -- Minimum value of sensor(100%) = 456;
   -- Relay pin = 8;
   -- Humidity check will be done each 31st minute
   -- Pump working time is 5 mins(300000 ms)
   -- Pressing button forces humidity check
