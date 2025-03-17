#include "core0_tasks.h"
#include "web_pages.h"

const char* ap_ssid = "GammaNetCH001";       // Set your AP SSID
const char* ap_password = "123456789";      // Set your AP password

// Task handle
TaskHandle_t WiFiTaskHandle;

// Create a Wi-Fi server on port 80 (HTTP)
WiFiServer server(80);

// Wi-Fi Access Point Task running on Core 0
void WiFiAPTask(void *parameter) {
    Serial.print("Running Wi-Fi AP on core ");
    Serial.println(xPortGetCoreID());

    // Start the ESP32 as an Access Point
    WiFi.softAP(ap_ssid, ap_password); 
    Serial.println("Access Point Started");
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());   // Print the IP address of the AP

    // Start the server
    server.begin();
    Serial.println("Server Started on Port 80");

    while (true) {
        WiFiClient client = server.available();  // Check for incoming client connections
        if (client) {
            Serial.println("New Client Connected");

            String request = "";
            while (client.available()) {
                request += (char)client.read();  // Read the HTTP request from the client
            }

            // Log the HTTP request (for debugging)
            Serial.println("Request received: ");
            Serial.println(request);

            // Prepare an HTTP response (HTML page with Bootstrap)
            String response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";

           if (request.indexOf("/about") != -1) {
                response += ABOUT_PAGE;
            } else if  {
                response += SENSOR_PAGE;
            }
            else {
                response += HOME_PAGE;
            }

            // Send the response to the client (iPhone)
            client.print(response);
            Serial.println("Response sent to client");

            client.stop();  // Close the connection
            Serial.println("Client Disconnected");
        }
        delay(10);  // Small delay to avoid maxing out the CPU
    }
}

// Start Wi-Fi AP Task
void startWiFiAPTask() {
    xTaskCreatePinnedToCore(
        WiFiAPTask,         // Function
        "WiFiAPTask",       // Task name
        4096,               // Stack size
        NULL,               // Task parameters
        1,                  // Priority
        &WiFiTaskHandle,    // Task handle
        0                   // Run on Core 0
    );
}
