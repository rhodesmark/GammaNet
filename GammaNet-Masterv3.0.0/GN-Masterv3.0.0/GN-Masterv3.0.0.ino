#include <Arduino.h>
#include "settings.h"
#include "masterPinsSetup.h"
// Function prototypes for the different submenus
void showNodeInformation();
void showNodeSetup();
void showNodeDiagnostics();
void showDeviceInformation();
void showDeviceSetup();
void showDeviceDiagnostics();

// Submenus for Node Diagnostics
void showPowerDiagnostics();
void showLocationDiagnostics();
void showSensorDiagnostics();

void setup() {
  Serial.begin(115200);
  Serial.println("Type 'Menu' to start the menu system.");

    pinMode(RELAY1_1, OUTPUT); //Pattery Pack C
    //4 Channel Relay Module
    pinMode(RELAY4_1, OUTPUT); //Battery Pack A
    pinMode(RELAY4_2, OUTPUT); //Battery Pack B
    pinMode(RELAY4_3, OUTPUT); //Geiger Tube Relay
    pinMode(RELAY4_4, OUTPUT); //Red Warning Strobe
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove any trailing newlines or spaces

    // Check if the input is the word "Menu"
    if (input.equalsIgnoreCase("Menu")) {
      showMainMenu();
    } else {
      // Otherwise, try to parse the input as an integer
      int option = input.toInt();
      if (option > 0) {
        // If the input is a valid number (greater than 0), show the corresponding menu
        handleMenuOption(option);
      } else {
        Serial.println("Invalid input. Please enter 'Menu' to start or a valid option number.");
      }
    }
  }
  Serial.println(F("Normal Operating mode"));
  delay(500);
}

void showMainMenu() {
  while (true) {
    Serial.println("\nMain Menu:");
    Serial.println("1 = Node Information");
    Serial.println("2 = Node Set Up");
    Serial.println("3 = Node Diagnostics");
    Serial.println("4 = Device Information");
    Serial.println("5 = Device Setup");
    Serial.println("6 = Device Diagnostics");
    Serial.print("Enter an option (1-6) or type 'Exit' to quit: ");
  
    while (Serial.available() == 0) {
      // Wait for user input
    }

    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove any trailing newlines or spaces

    if (input.equalsIgnoreCase("Exit")) {
      Serial.println(F("Exiting the menu system."));
      delay(500);
      break; // Exit the menu system
    }

    int option = input.toInt();
    if (option > 0) {
      handleMenuOption(option);
    } else {
      Serial.println("Invalid input. Try again.");
    }
  }
}

void handleMenuOption(int option) {
  if (option == 1) {
    showNodeInformation();
  } else if (option == 2) {
    showNodeSetup();
  } else if (option == 3) {
    showNodeDiagnostics();
  } else if (option == 4) {
    showDeviceInformation();
  } else if (option == 5) {
    showDeviceSetup();
  } else if (option == 6) {
    showDeviceDiagnostics();
  } else {
    Serial.println("Invalid option. Try again.");
    showMainMenu(); // Return to the main menu after invalid option
  }
}

void showNodeInformation() {
  Serial.println("Node Information");
  // Insert code here to show node info
  while (true) {
    Serial.print("Type 'Back' to return to the main menu: ");
    while (Serial.available() == 0) {
      // Wait for user input
    }

    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove any trailing newlines or spaces

    if (input.equalsIgnoreCase("Back")) {
      break; // Go back to the previous menu (Main Menu)
    } else {
      Serial.println("Invalid input. Type 'Back' to return.");
    }
  }
  showMainMenu();  // Return to the main menu
}

void showNodeSetup() {
  Serial.println("Node Setup");
  // Insert code here to setup node
  while (true) {
    Serial.print("Type 'Back' to return to the main menu: ");
    while (Serial.available() == 0) {
      // Wait for user input
    }

    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove any trailing newlines or spaces

    if (input.equalsIgnoreCase("Back")) {
      break; // Go back to the previous menu (Main Menu)
    } else {
      Serial.println("Invalid input. Type 'Back' to return.");
    }
  }
  showMainMenu();  // Return to the main menu
}

void showNodeDiagnostics() {
  Serial.println("Node Diagnostics:");
  Serial.println("1 = Power Diagnostics");
  Serial.println("2 = Location Diagnostics");
  Serial.println("3 = Sensor Diagnostics");
  Serial.print("Enter an option (1-3) or type 'Back' to return to the main menu: ");
  
  while (true) {
    while (Serial.available() == 0) {
      // Wait for user input
    }

    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove any trailing newlines or spaces

    if (input.equalsIgnoreCase("Back")) {
      break; // Go back to the previous menu (Main Menu)
    }

    int option = input.toInt();
    if (option == 1) {
      showPowerDiagnostics();
    } else if (option == 2) {
      showLocationDiagnostics();
    } else if (option == 3) {
      showSensorDiagnostics();
    } else {
      Serial.println("Invalid option. Try again.");
    }
  }
  showMainMenu();  // Return to the main menu
}

void showDeviceInformation() {
  Serial.println("Device Information");
  // Insert code here to show device info
  while (true) {
    Serial.print("Type 'Back' to return to the main menu: ");
    while (Serial.available() == 0) {
      // Wait for user input
    }

    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove any trailing newlines or spaces

    if (input.equalsIgnoreCase("Back")) {
      break; // Go back to the previous menu (Main Menu)
    } else {
      Serial.println("Invalid input. Type 'Back' to return.");
    }
  }
  showMainMenu();  // Return to the main menu
}

void showDeviceSetup() {
  Serial.println("Device Setup");
  // Insert code here to setup device
  while (true) {
    Serial.print("Type 'Back' to return to the main menu: ");
    while (Serial.available() == 0) {
      // Wait for user input
    }

    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove any trailing newlines or spaces

    if (input.equalsIgnoreCase("Back")) {
      break; // Go back to the previous menu (Main Menu)
    } else {
      Serial.println("Invalid input. Type 'Back' to return.");
    }
  }
  showMainMenu();  // Return to the main menu
}

void showDeviceDiagnostics() {
  Serial.println("Device Diagnostics");
  // Insert code here for device diagnostics
  while (true) {
    Serial.print("Type 'Back' to return to the main menu: ");
    while (Serial.available() == 0) {
      // Wait for user input
    }

    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove any trailing newlines or spaces

    if (input.equalsIgnoreCase("Back")) {
      break; // Go back to the previous menu (Main Menu)
    } else {
      Serial.println("Invalid input. Type 'Back' to return.");
    }
  }
  showMainMenu();  // Return to the main menu
}

void showPowerDiagnostics() {
  Serial.println(F("***** Power Diagnostics *****"));
  Serial.println(F("'Solar' - Solar Panel Diagnostics"));
  Serial.println(F("'PackA On' - Power Pack A On"));
  Serial.println(F("'PackA Off' - Power Pack A Off"));
  Serial.println(F("'PackB On' - Power Pack B On"));
  Serial.println(F("'PackB Off' - Power Pack B Off"));
  Serial.println(F("'PackC On' - Power Pack C On"));
  Serial.println(F("'PackC Off' - Power Pack C Off"));

  // Insert code here to perform power diagnostics
  
  while (true) {
    
    while (Serial.available() == 0) {
      // Wait for user input
    }

    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove any trailing newlines or spaces

    // Handle 'Back' command to return to the Node Diagnostics menu
    if (input.equalsIgnoreCase("Back")) {
      break; // Go back to the Node Diagnostics menu
    }

    // Handle Relay commands
    if (input.equalsIgnoreCase("Get Solar")) {
      // Code to turn Relay1 ON
      Serial.println("Get Values from Solar In");
      // Add code to physically turn on the relay here
    }else if (input.equalsIgnoreCase("PackA On")) {
      // Code to turn Relay1 ON
      Serial.println("Power Pack A is turned ON");
      digitalWrite(RELAY4_1, LOW); //7.4v Battery Pack A On - More Power To ESP32.
    } else if (input.equalsIgnoreCase("PackA Off")) {
      // Code to turn Relay1 OFF
      Serial.println("Power Pack A turned OFF");
      digitalWrite(RELAY4_1, HIGH); //7.4v Battery Pack A On - More Power To ESP32.
    } ///
    else if (input.equalsIgnoreCase("PackB On")) {
      // Code to turn Relay1 OFF
      digitalWrite(RELAY4_2, LOW); //7.4v Battery Pack B On - Sensor Power.
      Serial.println("Power Pack B turned On");
      // Add code to physically turn off the relay here
    } else if (input.equalsIgnoreCase("PackB Off")) {
      // Code to turn Relay1 OFF
      digitalWrite(RELAY4_2, HIGH); //7.4v Battery Pack B On - Sensor Power.
      Serial.println("Power Pack B turned Off");
      // Add code to physically turn off the relay here
    }///
    else if (input.equalsIgnoreCase("PackC On")) {
      // Code to turn Relay1 OFF
      digitalWrite(RELAY1_1, HIGH); //3.3v Battery Pack B On - Sensor Power.
      Serial.println("Power Pack C turned On");
      // Add code to physically turn off the relay here
    } else if (input.equalsIgnoreCase("PackC Off")) {
      // Code to turn Relay1 OFF
      digitalWrite(RELAY1_1, LOW); //3.3v Battery Pack B On - Sensor Power.
      Serial.println("Power Pack C turned Off");
      // Add code to physically turn off the relay here
    } else if (input.equalsIgnoreCase("Strobe On")) {
      // Code to turn Relay1 OFF
      digitalWrite(RELAY4_4, LOW); //3.3v Battery Pack B On - Sensor Power.
      Serial.println("Strobe On");
      // Add code to physically turn off the relay here
    } else if (input.equalsIgnoreCase("Strobe Off")) {
      // Code to turn Relay1 OFF
      digitalWrite(RELAY4_4, HIGH); //3.3v Battery Pack B On - Sensor Power.
      Serial.println("Strobe Off");
      // Add code to physically turn off the relay here
    }
    
    else {
      Serial.println("Invalid input. Type 'Back' to return.");
    }
  }
  
  showNodeDiagnostics();  // Return to the Node Diagnostics menu
}

void showLocationDiagnostics() {
  Serial.println("Location Diagnostics");
  // Insert code here to perform location diagnostics
  while (true) {
    Serial.print("Type 'Back' to return to the Node Diagnostics menu: ");
    while (Serial.available() == 0) {
      // Wait for user input
    }

    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove any trailing newlines or spaces

    if (input.equalsIgnoreCase("Back")) {
      break; // Go back to the Node Diagnostics menu
    } else {
      Serial.println("Invalid input. Type 'Back' to return.");
    }
  }
  showNodeDiagnostics();  // Return to the Node Diagnostics menu
}

void showSensorDiagnostics() {
  Serial.println("Sensor Diagnostics");
  // Insert code here to perform sensor diagnostics
  while (true) {
    Serial.print("Type 'Back' to return to the Node Diagnostics menu: ");
    while (Serial.available() == 0) {
      // Wait for user input
    }

    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove any trailing newlines or spaces

    if (input.equalsIgnoreCase("Back")) {
      break; // Go back to the Node Diagnostics menu
    } else {
      Serial.println("Invalid input. Type 'Back' to return.");
    }
  }
  showNodeDiagnostics();  // Return to the Node Diagnostics menu
}
