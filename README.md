# IoT-PowerPulse


1. Abstract 
"PowerPulse: Abnormal Load Monitoring and Detection System" is an IoT-based smart energy monitoring solution designed to track real-time power consumption and detect anomalies. The system employs smart sensors to measure voltage, current, and power usage, while an ESP32 microcontroller processes this data and communicates with a cloud-based platform. If an abnormal load condition such as a power surge or overload is detected, the system triggers instant alerts via a mobile app and can automatically cut off power to prevent damage. This scalable and adaptive system is applicable to residential, commercial, and industrial energy management. The project aligns with global smart grid initiatives, improving energy efficiency, operational safety, and cost reduction.  



2. Architecture Diagram
- You will need to draw the system architecture on paper.
- Ensure you include key components:
  - ESP32 Microcontroller (Central Processing Unit)
  - AC Voltage Sensor (Measures voltage)
  - Non-Invasive AC Current Sensor* (Measures current)
  - Relay Module (Disconnects power during anomalies)
  - LCD Display (Shows real-time power status)
  - Cloud Server (Stores data & sends alerts)
  - Mobile App (Receives notifications)
- Label all components clearly and show data flow between them.

---

3. Results 
- Circuit Setup: The system was successfully implemented with ESP32 interfacing with voltage and current sensors. The relay module responded correctly to abnormal loads, cutting off power when needed.  
  Normal Condition: When power usage was within the defined threshold, the system displayed real-time voltage and current readings on the LCD screen and the mobile app without any alerts.  
- Abnormal Condition: During a simulated power surge, the system detected a voltage spike above the threshold, activated the relay to disconnect the load, and sent an immediate alert to the mobile application.  
- Mobile App Notification: The app successfully displayed power consumption data and provided instant alerts when an anomaly was detected.  
- Performance Analysis: The system demonstrated high accuracy in detecting power anomalies, reducing response time compared to manual monitoring. The real-time cloud integration ensured remote access to power consumption data.  


