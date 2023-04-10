# DeviceMetricsCollector

![CI](https://github.com/Zarrett/DeviceMetricsCollector/actions/workflows/c-cpp.yml/badge.svg)

## Introduction

This project is a simple implementation of conversation between abstract devices and data collector.

The conversation is implemented using UDP protocol, by implementing simple server and client side applications.

Project consists of 2 separate targets:
- MetricsCollector - server side data collector, responsible for accepting messages and displaying their content in logs.
- DeviceMockClient - client side application which mimics a device, sending random measurments with a small delay.

## How to use

- Before building, make sure Boost is installed on your system.
- Build, using cmake<br>
Example:<br>
```
mkdir build
cd build
cmake ..
make -j8
```

### MetricsCollector

```./MetricsCollector <optional: port>```

By default port is 8888, remember that clients and server should be on the same port.

### DeviceMockClient

```./DeviceMockClient <optional: port> <optional: deviceID> <optional: numberOfMessages>```

deviceID - should be unique<br>
numberOfMessages - how many messages to generate and send<br>

You can create multiple instances of DeviceMockClient, as long as each has it's own unique deviceID, you should see messages received from each of them in the collector.

## Additional notes

### Log format

Example:<br>
2023-04-09 19:23:22.7984870; DeviceID: 123; TotalRcv: 31; Message: Measurements: 2197899581;

Format:<br>
Timestamp; DeviceID; Totat received; Message;

- Timestamp - current timestamp
- DeviceID - unique device identifier
- Totat received - total messages ```received from deviceID```
- Message - payload

### Packet format
+-----------------------------------------------------+<br>
| Command Code (4 bytes) | Device ID (4 bytes)        |<br>
+-----------------------------------------------------+<br>
| Validation ID (4 bytes)| Length (4 bytes)           |<br>
+-----------------------------------------------------+<br>
| Binary Data (0 - 65535 bytes)                       |<br>
+-----------------------------------------------------+<br>