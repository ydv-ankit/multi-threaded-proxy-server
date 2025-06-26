# Multi-Threaded TCP Proxy Server

A learning project for understanding socket programming in C and implementing a TCP proxy server.

## Overview

This project demonstrates fundamental concepts of network programming in C, including socket creation, binding, and listening. It's designed as an educational tool for learning TCP socket programming and multi-threading.

## Project Structure

```
multi-threaded-proxy-server/
├── main.c          # Main program entry point
├── socket.c        # Socket management functions
└── README.md       # This file
```

## Currently Implemented

- **Socket Creation**: Basic TCP socket creation with error handling
- **Socket Binding**: Binding socket to specified port using `sockaddr_in` structure
- **Network Byte Order**: Proper use of `htons()` _(host to network short)_ for port conversion
- **Error Handling**: Basic error checking
- **Modular Design**: Separated operations logic

## Building the Project

```bash
# Compile the project
gcc -o main main.c socket.c

# Run the proxy server
./main <port>
```

## Usage

```bash
# Start the proxy server on port 8080
./main 8080
```

## Key Functions

### `createSocket(int port)`

Creates and binds a TCP socket to the specified port.

### `listenConn(int *sfd)`

Starts listening for incoming connections (currently implemented but not called).

## Development Status

- [x] Basic socket creation and binding
- [x] Socket listening functionality (implemented, needs integration)
- [ ] Connection acceptance
- [ ] Multi-threading implementation
- [ ] Proxy forwarding logic

## Learning Focus

This project helps understand:

- Socket programming fundamentals
- TCP connection setup
- Network address structures
- Error handling in C
- Modular code organization
