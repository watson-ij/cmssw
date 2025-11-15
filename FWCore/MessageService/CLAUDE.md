# FWCore/MessageService

## Purpose
Service implementation of the message logging system. Backend for FWCore/MessageLogger.

## Key Components

- **MessageLogger service implementation**
  - Message routing to destinations
  - Formatting and output
  - Configuration handling
- **Threaded message dispatch**
- **Multiple output destinations**

## Configuration
```python
process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('cout', 'cerr'),
    # ... configuration ...
)
```

## Dependencies
- **FWCore**: MessageLogger, ParameterSet, Utilities
- **External**: Intel TBB

## Cross-References
- **FWCore/MessageLogger** - Logging interface
- **FWCore/ServiceRegistry** - Service framework
