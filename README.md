# CSMA/CD Implementation with Stop-and-Wait ARQ

This repository contains the implementation of CSMA/CD (Carrier Sense Multiple Access with Collision Detection) protocols with 1-persistent and non-persistent configurations, along with Stop-and-Wait ARQ. The implementation uses Arduino and communicates with a server for data exchange.

## Files in the Repository

- **`LAB5.ino`**: Arduino code implementing CSMA/CD and Stop-and-Wait ARQ.
- **`Lab 5.pdf`**: Detailed instructions for implementing CSMA/CD protocols, exponential backoff, and the Stop-and-Wait ARQ mechanism.
- **`LAB5.rar`**: A compressed archive containing additional resources and code related to this lab.

## Project Overview

### Protocol Features

#### CSMA/CD
1. **1-Persistent CSMA/CD**:
   - Continuously checks if the line is idle before transmitting.
   - Detects collisions during transmission and waits using an exponential backoff mechanism before retrying.

2. **Non-Persistent CSMA/CD**:
   - Checks if the line is idle. If busy, waits for a random time before checking again.
   - Detects collisions during transmission and employs exponential backoff for retransmission.

#### Exponential Backoff
A mechanism to handle collisions:
- For the first collision, waits for a random time slot \( U_1 \sim \text{Uniform}(0, 15) \).
- For subsequent collisions, waits for \( U_i \sim \text{Uniform}(0, 2^{i+3} - 1) \), where \( i \) is capped at 7 (maximum of 1024 time slots).
- Resets to \( U_1 \) after a successful transmission.

#### Stop-and-Wait ARQ
- Ensures reliable data transfer by requiring an acknowledgment (ACK) for each frame.
- Retransmits the frame if an ACK is not received within the timeout period.

### Frame Structure
The frame is structured as:
1. **Type**: Indicates whether the frame is data (0) or ACK (1).
2. **SN**: Sequence number for tracking frames.
3. **Data Length**: The size of the payload in bytes.
4. **Data**: Payload containing information (e.g., your names).

### Key Functions
1. **`setAddress(int number, int pair)`**: Initializes the Arduino's address.
2. **`setMode(int mod)`**: Sets the mode (1-persistent or non-persistent).
3. **`checkLine()`**: Checks if the communication medium is idle.
4. **`startPackage(void* payload, int payload_size)`**: Initiates frame transmission.
5. **`endPackage(int option)`**: Ends the frame transmission:
   - `0`: Stops transmission mid-way (collision detected).
   - `1`: Marks successful transmission.
6. **`readPackage(char* payload, int payload_size)`**: Reads incoming frames and checks for new data.

### Implementation Details
- **1-Persistent CSMA/CD**: Continuously monitors the line and retries transmission after collisions using exponential backoff.
- **Non-Persistent CSMA/CD**: Checks the line intermittently and uses random waiting periods to avoid collisions.
- **Server Communication**: Ensures compatibility with the university's server for data exchange.

## Getting Started

1. **Setup**: Connect the Arduino to the server's network while on campus.
2. **Upload Code**: Use `LAB5.ino` to upload the CSMA/CD implementation to the Arduino.
3. **Run Tests**:
   - Verify 1-persistent and non-persistent CSMA/CD operations.
   - Measure collision handling using exponential backoff.
   - Test the Stop-and-Wait ARQ mechanism for reliable data transfer.

## General Tips

- Use the provided `EthernetLab5.h` file for essential functions like `checkLine()` and `setMode()`.
- Implement the exponential backoff mechanism as a separate function for reuse.
- Debug and document your code thoroughly to ensure smooth functionality.

## Contribution

Contributions are welcome! Please ensure that your code is well-documented and adheres to the project structure.
