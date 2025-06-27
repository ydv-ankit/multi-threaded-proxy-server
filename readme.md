# Multi-Threaded TCP Proxy Server

A learning project for understanding socket programming in C and implementing a TCP proxy server with HTTP request parsing capabilities.

## Overview

This project demonstrates advanced concepts of network programming in C, including multi-threaded socket handling, HTTP request parsing, and dynamic buffer management. It's designed as an educational tool for learning TCP socket programming, multi-threading, and HTTP protocol implementation.

## Project Structure

```
multi-threaded-proxy-server/
├── main.c              # Main program entry point
├── socket/
│   ├── socket.c        # Socket creation and multi-threading
│   ├── socket.h        # Socket function declarations
│   └── conn.c          # Client connection handling
├── utils/
│   ├── logger.c        # Logging functionality
│   ├── logger.h        # Logger declarations
│   └── parser.c        # HTTP request parser
├── Makefile            # Build configuration
├── main                # Compiled executable
└── README.md           # This file
```

## Currently Implemented

### Core Socket Functionality

- **Multi-threaded Server**: Handles multiple client connections simultaneously
- **Socket Creation & Binding**: TCP socket creation with proper error handling
- **Connection Acceptance**: Accepts client connections with thread creation

### HTTP Request Processing

- **Dynamic Buffer Management**: Handles requests larger than buffer size
- **HTTP Request Parsing**: Manual character-by-character parsing
- **Header Extraction**: Parses all HTTP headers into linked list structure
- **Body Handling**: Extracts request body when present
- **Completion Detection**: Detects complete HTTP requests with proper boundary checking

### Memory Management

- **Dynamic Memory Allocation**: Proper buffer expansion for large requests
- **Linked List Headers**: Efficient storage of HTTP headers
- **Memory Cleanup**: Proper resource management

## Building the Project

```bash
# Clean and build
make clean && make

# Run the proxy server
./main <port>
```

## Usage

```bash
# Start the proxy server on port 8888
./main 8888

# Test with HTTP client (Postman, curl, etc.)
# Send HTTP requests to localhost:8888
```

## Key Functions

### `initializeSocket(int port)`

Creates, binds, and starts listening for connections with multi-threading support.

### `handleClient(int c_fd)`

Handles individual client connections with dynamic buffer management and HTTP parsing.

### `parseRequest(char *buff, size_t tsize)`

Manually parses HTTP requests, extracting method, path, headers, and body.

## Development Status

- [x] Multi-threaded server implementation
- [x] Dynamic buffer expansion
- [x] HTTP request parsing (method, path, headers, body)
- [x] Signal handling for graceful shutdown
- [x] Memory management and cleanup
- [x] Structured logging system
- [ ] Proxy forwarding logic
- [ ] Target server connection
- [ ] Response forwarding

## Learning Focus

This project helps understand:

### Network Programming

- Socket programming fundamentals
- Multi-threading with pthread
- TCP connection lifecycle
- Signal handling

### HTTP Protocol

- HTTP request structure
- Header parsing techniques
- Body handling
- Protocol completion detection

### Memory Management

- Dynamic memory allocation
- Buffer management
- Linked list implementation
- Resource cleanup

### System Programming

- File descriptor management
- Thread synchronization
- Error handling
- Process signals

## Testing

The server can be tested with:

- **Postman**: Send HTTP requests (GET, POST, PUT, DELETE)
- **curl**: Command-line HTTP requests
- **telnet**: Raw TCP connections
- **Custom HTTP clients**
