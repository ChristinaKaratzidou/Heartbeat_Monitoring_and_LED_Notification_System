#include <WiFi.h>
#include <HTTPClient.h>

// Define your WiFi credentials and ThingSpeak channel details
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
const char* thingSpeakApiKey = "YOUR_API_KEY"; // Your Write API Key
const char* thingSpeakChannelID = "YOUR_CHANNEL_ID"; // Your Channel ID
const char* thingSpeakUrl = "http://api.thingspeak.com/update?api_key="; // ThingSpeak URL

// Pin definitions
const int heartbeatSensorPin = 34; // Analog pin for the heartbeat sensor
const int ledPinRed = 25; // Red LED pin
const int ledPinGreen = 26; // Green LED pin
const int ledPinBlue = 27; // Blue LED pin

// Heartbeat thresholds (adjust these based on your sensor and expected values)
const int normalLowThreshold = 60;  // Low threshold for normal heartbeat (in BPM)
const int normalHighThreshold = 100; // High threshold for normal heartbeat (in BPM)

// Moving average buffer (used to smooth sensor readings)
const int bufferSize = 20; // Number of samples in the moving average buffer
int buffer[bufferSize]; // Array to store recent sensor readings
int bufferIndex = 0; // Index for the current position in the buffer
int total = 0; // Total of all values in the buffer (used for calculating the average)

// Variables for heartbeat detection
int lastValue = 0; // Previous sensor value for edge detection
unsigned long lastBeatTime = 0; // Time of the last detected heartbeat
int heartRate = 0; // Calculated heart rate (in BPM)
int beatCount = 0; // Total number of beats detected

// Function prototypes
void connectToWiFi(); // Connects to WiFi
void sendToThingSpeak(int heartbeat); // Sends data to ThingSpeak
void updateLED(int heartbeat); // Updates the LED color based on heartbeat status
int readHeartbeat(); // Reads raw heartbeat data
int movingAverage(int newValue); // Smooths sensor readings using a moving average
void detectHeartbeat(int value); // Detects heartbeats and calculates heart rate

void setup() {
    Serial.begin(115200); // Initialize Serial Monitor for debugging

    // Configure pins for sensor and LEDs
    pinMode(heartbeatSensorPin, INPUT);
    pinMode(ledPinRed, OUTPUT);
    pinMode(ledPinGreen, OUTPUT);
    pinMode(ledPinBlue, OUTPUT);

    // Connect to WiFi
    connectToWiFi();

    // Initialize moving average buffer with zeros
    for (int i = 0; i < bufferSize; i++) {
        buffer[i] = 0;
    }
}

void loop() {
    // Read raw sensor value
    int rawValue = analogRead(heartbeatSensorPin);

    // Smooth the raw value using a moving average
    int smoothedValue = movingAverage(rawValue);

    // Detect heartbeats and calculate heart rate
    detectHeartbeat(smoothedValue);
    
    // Send the smoothed value to ThingSpeak
    sendToThingSpeak(smoothedValue);

    // Update the LED color based on the current heartbeat status
    updateLED(smoothedValue);

    // Add a small delay for noise reduction
    delay(20);
}

// Function to connect the ESP32 to WiFi
void connectToWiFi() {
    WiFi.begin(ssid, password); // Start WiFi connection
    while (WiFi.status() != WL_CONNECTED) { // Wait until connected
        delay(1000); // Retry every second
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi"); // Print confirmation
}

// Function to send data to ThingSpeak
void sendToThingSpeak(int heartbeat) {
    if (WiFi.status() == WL_CONNECTED) { // Ensure WiFi is connected
        HTTPClient http; // Create an HTTP client
        // Construct the URL with the heartbeat data
        String url = String(thingSpeakUrl) + thingSpeakApiKey + "&field1=" + String(heartbeat) + "&channel_id=" + String(thingSpeakChannelID);
        http.begin(url); // Start the HTTP request
        int httpResponseCode = http.GET(); // Send the request
        if (httpResponseCode > 0) {
            Serial.println("Data sent to ThingSpeak"); // Success message
        } else {
            Serial.println("Error sending data"); // Error message
        }
        http.end(); // Close the connection
    }
}

// Function to update the RGB LED based on the heartbeat status
void updateLED(int heartbeat) {
    if (heartbeat < normalLowThreshold) {
        // Low heartbeat: Turn on the blue LED
        digitalWrite(ledPinRed, HIGH);
        digitalWrite(ledPinGreen, HIGH);
        digitalWrite(ledPinBlue, LOW);
    } else if (heartbeat > normalHighThreshold) {
        // High heartbeat: Turn on the red LED
        digitalWrite(ledPinRed, LOW);
        digitalWrite(ledPinGreen, HIGH);
        digitalWrite(ledPinBlue, HIGH);
    } else {
        // Normal heartbeat: Turn on the green LED
        digitalWrite(ledPinRed, HIGH);
        digitalWrite(ledPinGreen, LOW);
        digitalWrite(ledPinBlue, HIGH);
    }
}

// Function to calculate a moving average of the sensor readings
int movingAverage(int newValue) {
    total -= buffer[bufferIndex]; // Remove the oldest value
    buffer[bufferIndex] = newValue; // Add the new value
    total += newValue; // Update the total
    bufferIndex = (bufferIndex + 1) % bufferSize; // Move to the next index
    return total / bufferSize; // Return the average
}

// Function to detect heartbeats and calculate heart rate
void detectHeartbeat(int value) {
    // Check for a rising edge (signal crossing the high threshold)
    if (value > normalHighThreshold && lastValue <= normalHighThreshold) {
        unsigned long currentTime = millis(); // Get the current time
        if (currentTime - lastBeatTime > 300) { // Check for debounce time (300 ms)
            // Calculate heart rate (BPM)
            heartRate = 60000 / (currentTime - lastBeatTime);
            lastBeatTime = currentTime; // Update last beat time
            beatCount++; // Increment beat count
            Serial.print("Heart Rate: "); // Print heart rate
            Serial.println(heartRate);
        }
    }
    lastValue = value; // Update the last sensor value
}
