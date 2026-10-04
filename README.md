# Linux System Monitor

A C++ tool built with the purpose to:
- Monitor linux system resources
- Inspect running processes
- Monitor network traffic

## System monitoring

The `system` command displays basic system and resource information such as:

- Hostname
- Kernel version
- Architecture
- System uptime
- Load average
- CPU usage
- Memory usage
- Swap usage
- Disk usage

It also supports watch mode for continuous monitoring.

## Process monitoring

The `process` command reads information about running processes and provides a few ways to view them:

- PID
- Process name
- Process state
- Parent PID
- CPU usage
- Memory usage
- Thread count

Processes can be sorted by CPU, memory, or PID. You can also show only the top N processes or filter processes by name.

## Network monitoring

The `network` command monitors network traffic for available interfaces.

It currently shows:

- RX/TX throughput
- RX/TX packets per second
- RX/TX errors
- RX/TX dropped packets

A specific interface can also be selected, and watch mode is supported.

## Alerts and logging

The system monitor supports configurable alerts for:

- CPU usage
- Memory usage
- Disk usage

Resource information and triggered alerts can also be written to a log file with timestamps.

## Example commands

```bash
./sysmonitor system
./sysmonitor system --watch
./sysmonitor system --watch --refresh 2
./sysmonitor system --alert-cpu 80 --alert-memory 80 --alert-disk 90
./sysmonitor system --log system.log

./sysmonitor process
./sysmonitor process --sort cpu --top 10
./sysmonitor process --sort memory --top 5
./sysmonitor process --name chrome
./sysmonitor process --watch

./sysmonitor network
./sysmonitor network --interface eth0
./sysmonitor network --watch --refresh 5
```

Run:

```bash
./sysmonitor help
```

to see all available options.

## Project structure

```text
Linux_System_Monitor/
│
├── src/
│   ├── main.cpp
│   ├── util.cpp
│   ├── util.h
│   │
│   ├── system/
│   │   ├── system.cpp
│   │   ├── system.h
│   │   ├── cpu.cpp
│   │   ├── cpu.h
│   │   ├── alerts.cpp
│   │   └── alerts.h
│   │
│   ├── process/
│   │   ├── process.cpp
│   │   └── process.h
│   │
│   └── network/
│       ├── network.cpp
│       └── network.h
│
├── Makefile
└── README.md
```

## Building

Make sure `g++` and `make` are installed.

```bash
make
```

This creates the executable:

```text
sysmonitor
```

To remove the executable:

```bash
make clean
```
