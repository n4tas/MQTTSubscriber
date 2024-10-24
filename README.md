# MQTTSubscriber

MQTTSubscriber is a lightweight MQTT client designed to subscribe to MQTT topics and handle incoming messages. This project is tailored for the **OpenWrt** system running on the **RUTX10 router** by **Teltonika Networks**. It allows the user to subscribe to specified MQTT topics and trigger actions based on the incoming MQTT payloads.


## Table of Contents
- [Teltonika](#teltonika)
- [Requirements](#requirements)
- [Installation](#installation)
- [Configuration](#configuration)
- [Usage](#usage)

## Teltonika RUTX10 Router and OpenWrt

This project has been tested and optimized for use with the **Teltonika RUTX10** router running the **OpenWrt** operating system. The RUTX10 is a high-performance router designed for enterprise-grade IoT and network management applications. With MQTTSubscriber, it can easily integrate into larger IoT setups, providing a lightweight way to handle MQTT messaging.

## Requirements

- **OpenWrt**: The MQTTSubscriber is developed specifically for OpenWrt, a Linux-based OS designed for embedded systems. This guide assumes you are running OpenWrt on the **Teltonika RUTX10 router**.

## Installation

1. Clone the repository:
    ```bash
    git clone https://github.com/n4tas/MQTTSubscriber.git
    ```
   
2. Transfer the files to your Teltonika RUTX10 router using `scp` or any other secure file transfer method.

3. ```make menuconfig```
   
## Configuration

Edit the `config.json` file to configure your MQTT connection details. Here's a sample configuration:

```json
{
    "broker": "your_mqtt_broker_ip",
    "port": 1883,
    "topic": "your_mqtt_topic",
    "username": "mqtt_username",
    "password": "mqtt_password",
    "client_id": "client_id_for_mqtt",
    "qos": 1
}
```

- **broker**: The IP address or hostname of your MQTT broker.
- **port**: The port used by your MQTT broker (typically 1883 for non-TLS connections).
- **topic**: The MQTT topic you wish to subscribe to.
- **username**: The MQTT broker username.
- **password**: The MQTT broker password.
- **client_id**: The unique client ID for this MQTT client.
- **qos**: The Quality of Service level for the subscription.

## Usage

After configuring your `config.json` file, you can run the MQTTSubscriber by executing:

```bash
python3 mqtt_subscriber.py
```

This will start the client, which will subscribe to the specified MQTT topics and handle incoming messages. Actions based on the payload can be customized in the `mqtt_subscriber.py` file.

### Example: Subscribing to a topic and printing messages

By default, the script subscribes to the topic defined in `config.json` and prints the received messages to the console.

