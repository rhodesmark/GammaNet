#include "core0_tasks.h"
#include "variables.h"  // Node Settings

TaskHandle_t Core0TaskHandle = NULL;

void core0Task(void *parameter) {
    while (1) {
        Serial.println("Core 0 Task Running...");
        delay(2000);
    }

}

void startCore0Task() {
    xTaskCreatePinnedToCore(
        core0Task,   // Function
        "Core0Task", // Task name
        10000,       // Stack size
        NULL,        // Task input parameter
        1,           // Priority
        &Core0TaskHandle, // Task handle
        0            // Core 0
    );
}
