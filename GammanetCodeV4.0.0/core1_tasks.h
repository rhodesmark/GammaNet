#ifndef CORE1_TASKS_H
#define CORE1_TASKS_H

#include <Arduino.h>

void core1Task(void *parameter);
bool shouldStayAwake(int minute, String mode);
int timeToNextWake(int minute, String mode);
void goToSleep(int sleepTime);
void executeTask(String mode);
void displayGPSInfoSerial(); //DisplayGPS info With Serial Print Statments
void mainProgramme(); //Call the Main programmes based on thier Node Type
#endif