# Multi-Threaded HTTP Proxy Server

A project for understanding socket programming in C and implementing a fully functional multi-threaded HTTP proxy server with request forwarding capabilities.

## Overview

This project demonstrates advanced concepts of network programming in C, including multi-threaded socket handling, HTTP request parsing, proxy forwarding, and response handling. It's designed as an educational tool for learning TCP socket programming, multi-threading, HTTP protocol implementation, and proxy server architecture.

## Project Structure

```
multi-threaded-proxy-server/
├── main.c              # Main program entry point
├── socket/
│   ├── socket.c        # Socket creation and multi-threading
│   ├── socket.h        # Socket function declarations
│   └── conn.c          # Client connection handling
├── proxy/
│   ├── proxy.c         # Proxy forwarding logic
│   └── proxy.h         # Proxy function declarations
├── utils/
│   ├── logger.c        # Logging functionality
│   ├── logger.h        # Logger declarations
│   ├── parser.c        # HTTP request parser
│   ├── parser.h        # Parser declarations
│   ├── helper.c        # Utility functions
│   └── helper.h        # Helper declarations
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

### Proxy Forwarding

- **URL Path Extraction**: Converts full URLs to proper HTTP request paths
- **Request Reconstruction**: Rebuilds HTTP requests for target servers
- **Target Server Connection**: Establishes connections to target servers
- **Response Forwarding**: Forwards complete responses back to clients

### Response Handling

- **Chunked Transfer Encoding**: Properly handles chunked responses
- **Content-Length Support**: Handles responses with Content-Length headers
- **Complete Response Detection**: Ensures full responses are received before forwarding

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
# Use localhost:8888 as the proxy server
```

## Key Functions

### `initializeSocket(int port)`

Creates, binds, and starts listening for connections with multi-threading support.

### `handleClient(int c_fd)`

Handles individual client connections with dynamic buffer management and HTTP parsing.

### `parseRequest(char *buff, size_t tsize)`

Manually parses HTTP requests, extracting method, path, headers, and body.

### `forwardRequest(struct parsed_request *req, char *req_buff, int req_buff_len, char *resp_buff, size_t *resp_len)`

Forwards HTTP requests to target servers and receives responses.

### `receiveResponse(int tar_con_fd, char *resp_buff, size_t *resp_buff_len)`

Receives complete HTTP responses from target servers, handling both chunked and Content-Length encoding.

## Development Status

- [x] Multi-threaded server implementation
- [x] Dynamic buffer expansion
- [x] HTTP request parsing (method, path, headers, body)
- [x] Signal handling for graceful shutdown
- [x] Memory management and cleanup
- [x] Structured logging system
- [x] Proxy forwarding logic
- [x] Target server connection
- [x] Response forwarding
- [x] Chunked transfer encoding support
- [x] URL path extraction and request reconstruction
- [x] Complete response handling

## Features

### HTTP Protocol Support

- **Methods**: GET, POST, PUT, DELETE, etc.
- **Headers**: All standard HTTP headers
- **Body**: Request and response body handling
- **Encoding**: Chunked transfer encoding and Content-Length

### Proxy Capabilities

- **URL Parsing**: Extracts paths from full URLs
- **Request Forwarding**: Reconstructs requests for target servers
- **Response Handling**: Forwards complete responses to clients
- **Multi-threading**: Handles multiple concurrent requests

### Error Handling

- **Connection Errors**: Proper handling of connection failures
- **Memory Errors**: Graceful handling of allocation failures
- **Protocol Errors**: Robust HTTP parsing with error recovery

## Learning Focus

This project helps understand:

### Network Programming

- Socket programming fundamentals
- Multi-threading with pthread
- TCP connection lifecycle
- Signal handling
- Proxy server architecture

### HTTP Protocol

- HTTP request structure
- Header parsing techniques
- Body handling
- Protocol completion detection
- Chunked transfer encoding
- URL parsing and reconstruction

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

The proxy server can be tested with:

- **Postman**: Send HTTP requests to any target server
- **curl**: Command-line HTTP requests
- **Web browsers**: Configure proxy settings
- **Custom HTTP clients**

### Example Test Scenarios

```bash
# Test with local server
curl -x localhost:8888 http://localhost:3000/

# Test with external server
curl -x localhost:8888 http://httpbin.org/get

# Test with Postman
# Set proxy to localhost:8888 and send requests to any URL
```

## Enhancements can be made to the project

- [ ] Request/response caching
- [ ] Load balancing
- [ ] Request filtering
- [ ] Performance monitoring
- [ ] Logging
- [ ] Error handling
- [ ] Memory management
- [ ] Thread synchronization
- [ ] File descriptor management
- [ ] Process signals
