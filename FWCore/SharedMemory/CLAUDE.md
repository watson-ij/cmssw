# FWCore/SharedMemory

## Purpose
Shared memory primitives for inter-process communication. Supports multi-process parallelization.

## Key Components

- **Shared memory management**
- **Inter-process data sharing**

## Use Cases
- **Multi-process jobs**: Parallel processing across processes (not just threads)
- **Memory efficiency**: Share large data structures

## Dependencies
- **FWCore**: Utilities
- **External**: boost (shared memory), ROOT

## Cross-References
- **FWCore/Concurrency** - Thread-based parallelism
- Advanced parallelization schemes
