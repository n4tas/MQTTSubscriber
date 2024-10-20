# MQTT Subscriber

## Overview
MQTT Subscriber is a lightweight implementation designed to subscribe to an MQTT broker and handle messages. It's written primarily in C, with additional support from shell scripts and a Makefile for ease of building and running the project.

## Features
- Connects to an MQTT broker
- Subscribes to topics
- Receives and processes messages in real-time

## Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/n4tas/MQTTSubscriber.git
   ```
2. Navigate into the directory:
   ```bash
   cd MQTTSubscriber
   ```
3. Compile the source code:
   ```bash
   make
   ```

## Usage
To run the MQTTSubscriber:
```bash
./MQTTSubscriber <broker_url> <topic>
```

Example:
```bash
./MQTTSubscriber mqtt://localhost:1883 test/topic
```
