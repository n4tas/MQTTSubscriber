# MQTTSubscriber

MQTTSubscriber is a lightweight MQTT client designed to subscribe to MQTT topics and handle incoming messages. This project is tailored for the **OpenWrt** system running on the **RUTX10 router** by **Teltonika Networks**. It allows the user to subscribe to specified MQTT topics and trigger actions based on the incoming MQTT payloads.

## Table of Contents
- [Requirements](#requirements)
- [Installation](#installation)
- [Configuration](#configuration)
- [Usage](#usage)

## Requirements

- **OpenWrt**: The MQTTSubscriber is developed specifically for OpenWrt, a Linux-based OS designed for embedded systems. This guide assumes you are running OpenWrt on the **Teltonika RUTX10 router**.

## Installation

1. Clone the repository:
    ```bash
    git clone https://github.com/n4tas/MQTTSubscriber.git
    ```
   
2. Router configuration:
    ```bash
    make menuconfig
    ```

3. Compilation:
    ```bash
    make package/MQTTSubsriber/{clean,compile}
    ```

4. Transfer the files to your Teltonika RUTX10 router using `scp`
   
## Configuration

Edit the `mqtt_topics` and `mqtt_login` config files to configure your MQTT connection and topics/event details. Here's a sample configuration:

```config
config topic
    option topic 'example1'

config topic
    option topic 'example2'

config event
    option topic 'example1'
    option comparison_type '='
    option data_type 'alphanumeric'
    option reference '"{"example_data": "string"}"'
```

```config

config login 
    option email example@example.com
    option password example-password
```

## Usage

After configuring your config files, you can run the MQTTSubscriber by executing:

```bash
./MQTTSubscriber -h `[HOST_IP]` -p `[PORT]`
```

This will start the program, which will subscribe to the specified MQTT topics and handle incoming messages. Actions can be customized in the `mqtt_topics` file.
