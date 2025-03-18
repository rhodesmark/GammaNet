[v3.0.4] - 18/03/2025
1 - Still Working on The Wake Sleep Function.
2 - Added core0_tasks.h and .cpp to allow diagnoistics via Core0.
3 - Added a Menu Systems to run via the Serial Monitor - Working Progress.
4 - Added variables.cpp and variables.h to store Sensor Variables.
5 - Added Sensors.cpp and Sensors.h header files - All Sensors are handeled in these folders.
6 - Added MenuSystem.cpp and MenuSystem.h header files. This will handle the Menu System in these files.
7 - Added MicroSD Card Module to store Captured Data for later interrogation.
    a. Pins are defined in thenPinSetup.h header file.
    b. SD Card Detection successfull and shown in the Serial Monitor - Confirmed.
    c. SD Card Detected and Type shown in Serial Monitor - Confirmed.

[v3.0.4] - 17/03/2025
1 - Removed the Wifi Connection for Core0.
    Reason: Running the wifi connecton process on Core0 kept the ESP32 awake constantly and it needs to go to sleep at determined intervals.
2 - Still having issues with the Sleep/Wake function.


[v3.0.3] - 17/03/2025 - 1
1 - Adding Wifi Connecton and Communication on Core 0.
2 - Wifi Connection successfully added to system - 10:49am
3 - Can connect a iPhone Device to the System - 10:49am
4 - 3 Web Pages Added to web_pages.h file. Home, About and Sensors.
------Working----- Committing to Git Hub - Successful
-------------------------
[v3.0.2] - 17/03/2025 - 1
1 - Deepsleep and Wake Cycle seems to be working at 07:20-07:25.
2 - Saving this version to GitHub
-------------------------
[v3.0.2] - 16/03/2025 - 1
- Setting the GPS Module to Run for 60 seconds on Start up to get Time and ascertain if DeepSleep or Wake programme is needed.
- Sleep Calulation to next Wake seems to be working. in this version.