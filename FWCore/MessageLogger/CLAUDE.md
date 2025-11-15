# FWCore/MessageLogger

## Purpose
Thread-safe, configurable message logging system with severity levels, categories, and multiple output destinations. Provides diagnostic and error reporting throughout CMSSW.

## Key Components

### Logging Interface
- **MessageLogger.h** (`interface/MessageLogger.h:1`) - Main logging macros
  - `LogError(category)` - Error messages
  - `LogWarning(category)` - Warning messages
  - `LogInfo(category)` - Informational messages
  - `LogDebug(category)` - Debug messages (compiled out in release builds)
  - `LogSystem(category)` - System messages (always logged)
  - `LogAbsolute(category)` - High-priority messages
  - `LogProblem(category)` - Problem reports
  - `LogPrint(category)` - Print-style messages
  - `LogVerbatim(category)` - Verbatim messages (no formatting)

### Message Objects
- **MessageSender.h** (`interface/MessageSender.h:1`) - Dispatches log messages
  - Collects message content
  - Routes to destinations
- **MessageDrop.h** (`interface/MessageDrop.h:1`) - Thread-local context
  - Current module, event, run information
  - Automatic context capture
- **ErrorObj.h** - Message object with metadata
  - Severity, category, timestamp
  - Context (module, event ID)

### Severity Levels
- **ELseverityLevel.h** (`interface/ELseverityLevel.h:1`) - Severity hierarchy
  - `ELzeroSeverity` (0)
  - `ELdebug` (10)
  - `ELinfo` (20)
  - `ELwarning` (30)
  - `ELerror` (40)
  - `ELfatal` (50)

### Job Reporting
- **JobReport.h** (`interface/JobReport.h:1`) - Structured job reporting
  - Framework job summary (FrameworkJobReport XML)
  - Input/output file tracking
  - Event counts, error statistics
  - Performance metrics

### Configuration
- Configured via `MessageLogger` service in Python config:
  ```python
  process.MessageLogger = cms.Service("MessageLogger",
      destinations = cms.untracked.vstring('cout', 'cerr', 'warnings'),
      categories = cms.untracked.vstring('MyModule', 'MyCategory'),
      cout = cms.untracked.PSet(
          threshold = cms.untracked.string('INFO'),
          MyCategory = cms.untracked.PSet(
              limit = cms.untracked.int32(10)
          )
      )
  )
  ```

## Logging Macros

### Standard Logging
```cpp
LogError("MyCategory") << "Error occurred: " << value;
LogWarning("MyCategory") << "Warning: " << message;
LogInfo("MyCategory") << "Info: " << info;
LogDebug("MyCategory") << "Debug: " << details;  // Release builds: no-op
```

### Verbatim Logging (no line breaks added)
```cpp
LogVerbatim("MyCategory") << "Line 1" << "\n" << "Line 2";
```

### Conditional Logging
```cpp
LogTrace("MyCategory") << "Trace message";  // Only in TRACE builds
```

### Stream-Style
```cpp
edm::LogError("MyCategory") << "Error code: " << code
                             << ", value: " << value
                             << ", expected: " << expected;
```

## Message Routing

### Flow
1. **Log statement** in module code
2. **MessageSender** captures message and context
3. **MessageDrop** adds current module/event info
4. **MessageLogger service** routes to destinations
5. **Destinations** format and output (cout, files, etc.)

### Destinations
- **cout**: Standard output
- **cerr**: Standard error
- **File**: Log files
- **JobReport**: XML job summary
- **Custom**: User-defined destinations

### Categories
- **Purpose**: Filter and limit messages by type
- **Usage**: Organize related messages
- **Configuration**: Per-category limits, thresholds

## Dependencies
- **FWCore/Utilities** - Exception handling
- **External**: fmt library (modern formatting)

## Integration Points

### Used By
- **All packages** - Ubiquitous logging
- **FWCore/Framework** - Event loop messages
- **Modules** - Algorithm diagnostics
- **Services** - Service status messages

### Uses
- **FWCore/Utilities** - Exception types
- **fmt library** - Message formatting

## Key Design Patterns

1. **Stream Operator Pattern**:
   ```cpp
   LogError("MyModule") << "Value " << x << " out of range [" << min << ", " << max << "]";
   ```

2. **Category-Based Filtering**:
   ```cpp
   // In module
   LogDebug("DetailedDebug") << "Detailed info...";

   // In config
   process.MessageLogger.debugModules = ['myModule']
   process.MessageLogger.DetailedDebug = {...}
   ```

3. **Thread-Local Context**: MessageDrop automatically captures module/event

4. **Lazy Evaluation**: Debug messages not constructed in release builds

## Common Usage Patterns

### Basic Error Logging
```cpp
if (error) {
  edm::LogError("MyProducer") << "Failed to process event " << event.id();
}
```

### Warnings with Context
```cpp
if (value < threshold) {
  edm::LogWarning("ThresholdCheck")
    << "Value " << value << " below threshold " << threshold
    << " in module " << moduleLabel_;
}
```

### Debug Information
```cpp
#ifdef EDM_ML_DEBUG
LogDebug("AlgorithmSteps") << "Step 1: processed " << count << " items";
#endif
```

### Info Messages
```cpp
LogInfo("ConfigInfo") << "Using algorithm version " << version;
```

### Verbatim Output
```cpp
LogVerbatim("DataDump") << "Data:\n" << formatData();
```

## Configuration Examples

### Basic Configuration
```python
process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('cout'),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    )
)
```

### Multi-Destination with Limits
```python
process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('cout', 'errors'),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO'),
        noLineBreaks = cms.untracked.bool(True)
    ),
    errors = cms.untracked.PSet(
        threshold = cms.untracked.string('ERROR'),
        filename = cms.untracked.string('errors.log')
    ),
    categories = cms.untracked.vstring('MyModule'),
    MyModule = cms.untracked.PSet(
        limit = cms.untracked.int32(10)  # Max 10 messages
    )
)
```

### Debug Modules
```python
process.MessageLogger.debugModules = cms.untracked.vstring('myProducer', 'myAnalyzer')
process.MessageLogger.cout.threshold = 'DEBUG'
```

## Performance Considerations
- **Debug builds**: LogDebug compiled in, impacts performance
- **Release builds**: LogDebug compiled out (zero overhead)
- **Message limits**: Prevent log flooding
- **Thread-safe**: Small synchronization cost
- **Lazy evaluation**: Messages not constructed if suppressed

## Thread Safety
- MessageLogger service is thread-safe
- MessageDrop is thread-local (captures stream context)
- Multiple threads can log concurrently
- Message order may interleave across threads

## Architecture Notes
- Singleton MessageLogger service
- Thread-local MessageDrop for context
- Configurable routing and filtering
- Exception handling integrated
- JobReport for structured output

## Best Practices
1. **Use categories**: Organize messages logically
2. **Appropriate severity**: Error=failure, Warning=issue, Info=status
3. **Limit debug**: Use sparingly, expensive in debug builds
4. **Context**: Include relevant values (event ID, module, etc.)
5. **Rate limiting**: Configure limits for repeated messages

## Cross-References
- **FWCore/MessageService** - Service implementation
- **FWCore/Framework** - Framework messages
- **FWCore/ServiceRegistry** - Service infrastructure
- **All packages** - Use for diagnostics
