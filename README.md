# Tin-Coding-Challenges
Master repo for coding challenge


## 🏙️ Challenge: The Smart City Sentinel


Welcome to our 2026 Language-Agnostic IoT Challenge! Whether you are a total beginner, a university graduate, or a seasoned professional, this project is designed to test your ability to bridge the gap between hardware (real or virtual) and software logic.


## 📝 The Concept
Participants must build a "Sentinel" system that monitors an urban environment—think smart parking, air quality, street lighting, or waste management. The goal is to ingest data from a sensor and perform a meaningful action or visualization.


## 🛠️ Choose Your Skill Path
### 🟢 Level 1: The Newbie (The "Environment Node")
 * Focus: Basic I/O and Logic.
 * The Task: Build a virtual sensor (e.g., Temperature/Humidity) that triggers a local event.
 * The Flex: Handle "noise" in your data or add a physical/virtual notification (like a blinking LED).

### 🔵 Level 2: The Graduate (The "Analytics Hub")
 * Focus: Data Processing & Integration.
 * The Task: Create a gateway that processes data over time (e.g., calculating moving averages rather than reacting to single data points).
 * The Flex: Connect your system to an external API or store historical data in a local database (SQLite/InfluxDB).

### 🔴 Level 3: The Professional (The "Fleet Commander")
 * Focus: Reliability & Scalability.
 * The Task: Build a robust backend using a "Device Shadow" pattern or a message broker.
 * The Flex: Containerize your stack with Docker, implement JWT-based security, or handle high-concurrency data streams.

## 🚀 Steps to Participate
### 1. Set Up Your Environment
 * Physical: Use an Arduino, ESP32, or Raspberry Pi if you have one.
 * Virtual (Recommended): Use Wokwi for a free browser-based ESP32/Arduino simulator.


### 2. Fork This Repository
 * Fork this repo to your own GitHub account.
 * Create a folder named [YourName]-[ProjectName].
 * Maintain your code within that folder.
 * See our submission template that you can drop in the root of your directory


### 3. Build & Document
 * Week 1: Establish connectivity (Sensor → Console).
 * Week 2: Implement the "Smart" logic.
 * Week 3: Build the UI/Dashboard and polish.
 * Note: Your folder must include a README.md explaining how to run your project and what language/tools you used.


### 4. Submit Your Entry
 * Submit a Pull Request (PR) to this master repository before the deadline.
 * Include a link to a 60-second screen recording or demo in your PR description.


## 📊 Evaluation Criteria
Voting will be done by the community based on:
 * Creativity: How unique is your "City Sentinel" use case?
 * Robustness: Does the code handle errors or lost connections gracefully?
 * Utility: Does this solve a real-world urban problem?


## 💡 Quick Start Resources
 * Simulator: Wokwi.com
 * MQTT Broker: broker.hivemq.com (Public)
 * Protocols: You are encouraged to use MQTT, HTTP, or WebSockets.


Good luck, Alchemists! Let’s build the city of the future.

