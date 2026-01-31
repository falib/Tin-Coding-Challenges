
🛰️ Project Name: [TitanTech Smart streetlights]
Participant: [Jiles Ramjattan/ JRamjattan]
Skill Level: [Beginner]
Language/Tech Stack: [c++, esp32 microcontroller]

📖 The "Sentinel" Concept
TitanTech Smart streetlights seeks to reduce light pollution on and around 
highways and major roads by having the lights run ay minimal operating voltages
when they are not required amd switches to full power when there is a vehicle driving 
along the road. The device also seeks to provide data on traffic density as well as 
average vehicle speed which can provide insights on population driving habits.

⚙️ How it Works
Briefly explain the flow of data:
 * Sensor: *Doppler sensor* detects on comming vehicle.
           *PIR sensor* detects hot body presence reducing false positives from
           doppler sensor.
 * Logic: Doppler Sensor detects vehicle. Device calculates speed distance and direction of 
          oncoming vehicle. Device checks PIR input to verify vehicle is approaching.
          device switches output on Trigger pin for mosfet to high voltage for 60 second.If
          another vehicle does not pass in the 60 second window the devices sets Trigger pin 
          to low voltage.
 * Output: device logs speed distance and angle of approaching vehicles along with a time stamp 
           to an SD Card
🚀 How to Run
Provide the exact steps for the judges to see your project in action:
 * Open this https://wokwi.com/projects/454663731763661825
 * Click the "Play" button.
 * Observe Logs and LED to see lights turn off when a vehicle is not present. 

🌟 The "Flex" 
Honestly I already have a fully fleshed out plan for a fully scaleable one of these 
with data collection dedicated mesh network and full hierarchical design. But I 
don't have the time or resources right now to do anything besides plan and code it.
🎥 Demo Link
[Link to your 60-second video or screen recording]
💡 Pro-Tip for Participants:
When you submit your Pull Request, use the title format: SUBMISSION: [Your Name] - [Project Name]. This makes it much easier for everyone to browse the entries during the voting phase!
