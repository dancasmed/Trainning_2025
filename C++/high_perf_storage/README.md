# High-Performance Financial Data Storage System

A C++ implementation of a low-latency data storage system designed for high-frequency financial data processing.

## Key Features

### Core Storage Capabilities
- **Asynchronous Buffered Writing** 
  - Thread-safe memory buffering with configurable capacity
  - Automatic flushing mechanisms with background thread management
  - Lock-free synchronization using `std::mutex` and `std::lock_guard`

### Distributed Storage Architecture
- Redundant writes across multiple storage nodes
- Configurable redundancy levels (N-way replication)
- Randomized node selection for load balancing

### Performance Optimization
- Zero-copy memory mapping with `mmap`/`munmap`
- Zstandard compression integration (ZSTD)
- Memory-mapped file I/O for low-latency persistence
- Move semantics for efficient buffer transfers

### Real-Time Data Handling
- High-resolution timestamp tracking (nanosecond precision)
- Thread-safe queue implementation for producer-consumer patterns
- Concurrent data collection from multiple sources

### File System Management
- Automated directory creation and validation
- Atomic file size management with `ftruncate`
- Data integrity assurance through `msync` operations

### Data Processing
- Batch processing with configurable buffer sizes
- Compression ratio detection and adaptive buffering
- Endianness-agnostic binary serialization

## Technical Specifications

### System Characteristics
- **Compression Support**: Configurable Zstandard compression
- **Redundancy**: Tunable replication factor
- **Throughput**: Designed for 10K+ events/second
- **Latency**: Sub-millisecond write thresholds

### Key Technologies
- **Concurrency**: `std::thread`, `std::atomic`, RAII guards
- **Memory Management**: Smart pointers, custom buffer pools
- **File Systems**: POSIX file descriptors, memory mapping
- **Compression**: ZSTD streaming API integration
- **Modern C++**: C++17 features including move semantics

### Configuration Options
```ini
Buffer Capacity: 10,000 records
Compression Enabled: true
Redundancy Level: 2
Output Directories: 4 nodes
Collection Threads: 10