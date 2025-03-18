#include "MenuSystem.h"
#include "core0_tasks.h"  // Updated include

void displayMenu() {
    Serial.println("\n==== Main Menu ====");
    Serial.println("1. Run Core 0 Task");
    Serial.println("2. System Diagnostics");
    Serial.println("3. Exit");
    Serial.print("Enter choice: ");
}

void displayDiagnosticsMenu() {
    Serial.println("\n==== System Diagnostics ====");
    Serial.println("1. Check Memory");
    Serial.println("2. Check CPU Speed");
    Serial.println("3. Back to Main Menu");
    Serial.print("Enter choice: ");
}

void handleMenu() {
    static int state = 0;
    static String input = "";
    
    if (Serial.available()) {
        char ch = Serial.read();
        
        if (ch == '\n' || ch == '\r') { 
            if (state == 0) {
                if (input == "1") {
                    Serial.println("\nStarting Core 0 Task...");
                    startCore0Task();
                } else if (input == "2") {
                    state = 1;
                    displayDiagnosticsMenu();
                } else if (input == "3") {
                    Serial.println("\nExiting...");
                } else {
                    Serial.println("\nInvalid choice! Try again.");
                    displayMenu();
                }
            } else if (state == 1) {
                if (input == "1") {
                    Serial.printf("\nFree Heap: %d bytes\n", ESP.getFreeHeap());
                } else if (input == "2") {
                    Serial.printf("\nCPU Frequency: %d MHz\n", ESP.getCpuFreqMHz());
                } else if (input == "3") {
                    state = 0;
                    displayMenu();
                } else {
                    Serial.println("\nInvalid choice! Try again.");
                    displayDiagnosticsMenu();
                }
            }
            input = ""; // Reset input
        } else {
            input += ch;
        }
    }
}
