# Heartbeat_Monitoring_and_LED_Notification_System
Heartbeat Monitoring and LED Notification System using an ESP32

## **Project Overview**
This project utilizes an ESP32 microcontroller to monitor heartbeats using a Heartbeat Sensor Module. The data is transmitted to ThingSpeak for logging and visualization. An RGB LED is used to provide visual notifications based on the heartbeat readings.

## **Features**
- Real-Time Heartbeat Monitoring
- LED-Based Visual Feedback
  - **Green** for a normal heartbeat (60-100 BPM)
  - **Red** for an abnormal heartbeat (too high)
  - **Blue** for an abnormal heartbeat (too low)
- Logs data to ThingSpeak for cloud-based storage and visualization
  - Data will be sent to ThingSpeak every 2 seconds

## **Components**
- ESP32-D: Microcontroller
- Heartbeat Sensor Module
- RGB LED Module
- Jumper wires, breadboard, and micro USB cable.

## **Hardware Connections**
### **ESP32**
- Connect the ESP32 to a computer via the micro USB cable for power and programming.

### **Heartbeat Sensor Module**
- **VCC**: Connect to 3.3V on the ESP32.  
- **GND**: Connect to GND on the ESP32.  
- **SENSOR OUT**: Connect to an analog pin on the ESP32 (e.g., GPIO 34).  

### **RGB LED Module**
- **GND**: Connect to GND on the ESP32.
- **Red Pin**: Connect to a digital pin on the ESP32 (e.g., GPIO 25).
- **Green Pin**: Connect to a digital pin on the ESP32 (e.g., GPIO 26).
- **Blue Pin**: Connect to a digital pin on the ESP32 (e.g., GPIO 27).
  
## **Software Setup**
1. Install **Arduino IDE**.
2. Add **ESP32 Board** to the Arduino IDE.
3. Install Required Libraries:
   - **Heartbeat Sensor Library**.
   - **ThingSpeak Library**.
   - **ESP32 WiFi Library**.
4. Create a **ThingSpeak Channel**:
   - **Field 1**: Heartbeat Data.  
  
