# Simulated GPIO Kernel Module

## Overview

This Linux kernel module simulates **GPIO LED** and **Button** functionality using the `/proc` filesystem on Ubuntu x86_64.  
It allows you to **read and write virtual GPIO states** without any physical hardware. This is useful for learning **Linux kernel module programming** and **GPIO interfaces**.

---

## Features

- **LED Simulation**
  - `/proc/led`
  - Write `1` → LED ON
  - Write `0` → LED OFF
  - Read → Returns current LED state (`0` or `1`)

- **Button Simulation**
  - `/proc/button`
  - Write `1` → Press button
  - Write `0` → Release button
  - Read → Returns current button state (`0` or `1`)

- Handles **EOF properly** on reads to prevent infinite loops.
- Kernel logs each write operation to help with debugging.

---

## Basic Theory

- **GPIO (General Purpose Input/Output)**: Pins that can be used as input or output in embedded systems.  
- **/proc Filesystem**: Virtual interface to communicate between **user space** and **kernel space**.  
- **Kernel Variables**: Stores the state of LED and Button internally.  
- **User-Kernel Interaction**: `copy_to_user()` and `copy_from_user()` ensure safe data transfer.  

---


---

## Build Instructions

1. Install kernel headers and build tools:

```bash
sudo apt update
sudo apt install build-essential linux-headers-$(uname -r) -y
```

2.Compile the module:

```make```

Load and Test Module

1. Insert the module:
```
sudo insmod gpio_module.ko
sudo dmesg | tail
````

2. Interact with LED:

```
cat /proc/led               # Read LED state
echo 1 | sudo tee /proc/led # Turn LED ON
echo 0 | sudo tee /proc/led # Turn LED OFF
cat /proc/led               # Verify state

```
3. Interact with Button:
```
cat /proc/button             # Read button state
echo 1 | sudo tee /proc/button # Press button
echo 0 | sudo tee /proc/button # Release button
cat /proc/button             # Verify state
```

4. Interact with Button:
```
sudo rmmod gpio_module
sudo dmesg | tail
```

Expected Output

| Command            | Output                 |                                  |
| ------------------ | ---------------------- | -------------------------------- |
| `cat /proc/led`    | `0` (initial OFF)      |                                  |
| `echo 1 `           | `sudo tee /proc/led`    | `1` and `dmesg`: LED ON          |
| `cat /proc/led`    | `1`                    |                                  |
| `echo 0 `           | `sudo tee /proc/led`    | `0` and `dmesg`: LED OFF         |
| `cat /proc/button` | `0` (initial Released) |                                  |
| `echo 1`            |` sudo tee /proc/button` | `1` and `dmesg`: Button Pressed  |
| `cat /proc/button` | `1`                    |                                  |
| `echo 0   `         | `sudo tee /proc/button` | `0` and `dmesg`: Button Released |



