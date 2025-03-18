#ifndef CORE1_TASKS_H
#define CORE1_TASKS_H

bool isWakePeriod(int minute);
int calculateSleepDuration(int minute, int second);
void goToSleep(int seconds);
void stayAwake(int centisecondsUntilNextSleep);
void core1Task(void *pvParameters);
bool getGPSTime(int &hour, int &minute, int &second, int &centisecond);
void startGetGPSTime();
void getGPSInfo();
void displayGPSInfo();
void displayNodeInfo();
void getNodeInfo();
void CentralHubProgramme(); //Central Hub Programme

void save2SD(); //Save to SD Card
void CH2Web(); //Central Hub to Web Communicatons Programme.


#endif  // CORE1_TASKS_H
