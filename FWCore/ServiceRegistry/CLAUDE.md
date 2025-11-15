# FWCore/ServiceRegistry

## Purpose
Thread-local service management and activity monitoring system. Provides global access to services (singletons or thread-local) and a comprehensive signal/slot system for monitoring framework lifecycle events.

## Key Components

### Service Access
- **ServiceRegistry.h** (`interface/ServiceRegistry.h:1`) - Thread-local service container
  - One registry per thread/stream
  - Provides access to all configured services
  - Methods: `get<T>()`, `isAvailable<T>()`
- **Service.h** (`interface/Service.h:1`) - Template for service access in modules
  ```cpp
  edm::Service<MyService> service;
  service->doSomething();
  ```
- **ServiceToken.h** (`interface/ServiceToken.h:1`) - Service context token
  - Captures current service configuration
  - Allows service context switching

### Service Lifecycle
- **ServicesManager.h** (`interface/ServicesManager.h:1`) - Manages service lifecycle
  - Construction, initialization, destruction
  - Coordinates service dependencies
- **ServiceWrapper.h** - Wraps service instance
- **ServiceMaker.h** - Service factory base class
- **ServiceMakerBase.h** - Abstract factory

### Activity Monitoring (Signals/Slots)
- **ActivityRegistry.h** (`interface/ActivityRegistry.h:1`) - Central event notification system
  - 100+ signals for framework transitions
  - Services connect to signals to monitor events
  - Example signals:
    - `preModuleEvent`, `postModuleEvent` - Around module execution
    - `preEvent`, `postEvent` - Around event processing
    - `preModule`, `postModule` - Module construction/destruction
    - `preOpenFile`, `postOpenFile` - File operations
    - `preGlobalBeginRun`, `postGlobalBeginRun` - Run transitions

### Context Information
Context classes passed to signal callbacks:
- **ModuleCallingContext.h** - Module execution context
  - Module label, type
  - Call stack information
- **StreamContext.h** - Stream processing context
  - Stream ID
  - Event ID being processed
- **GlobalContext.h** - Global transition context
- **PathContext.h** - Path execution context
- **ProcessContext.h** - Process-level context
- **EventID.h**, **Timestamp.h** - Event identification

### Service Types

#### Global Services
Single instance shared across all threads:
```cpp
class MyGlobalService {
public:
  MyGlobalService(ParameterSet const&, ActivityRegistry&);
  // Must be thread-safe
};
```

#### Per-Stream Services
One instance per processing stream:
```cpp
class MyPerStreamService {
public:
  MyPerStreamService(ParameterSet const&, ActivityRegistry&);
  // No thread-safety needed - one per stream
};
```

#### Legacy Services
Single instance, not thread-aware (deprecated).

### Service Registration
- **ServicePluginFactory.h** - Plugin factory for services
- Macros (in PluginManager): `DEFINE_FWK_SERVICE(Type)`

## Service Categories

### Monitoring Services
- **Timing**: Module/event timing
- **Memory**: Memory usage tracking
- **ResourceInformation**: System resource monitoring
- **MessageLogger**: Logging configuration

### I/O Services
- **SiteLocalConfig**: Site-specific configuration
- **Tracer**: Framework activity tracing

### Random Number Services
- **RandomNumberGenerator**: Per-module RNG streams

### Condition Services
- **CondDB**: Conditions database access

### Custom Services
User-defined services for cross-module communication or monitoring.

## Dependencies
- **FWCore**: ParameterSet, PluginManager, Utilities, MessageLogger
- **External**: TBB

## Integration Points

### Used By
- **FWCore/Framework** - Service initialization and access
- **Modules** - Access services via `Service<T>`
- **FWCore/Services** - Standard service implementations
- **All packages** - Can provide or use services

### Uses
- **FWCore/ParameterSet** - Service configuration
- **FWCore/PluginManager** - Dynamic service loading
- **FWCore/Utilities** - Exceptions, types

## Key Design Patterns

1. **Service Locator Pattern**:
   ```cpp
   edm::Service<MyService> service;
   if (service.isAvailable()) {
     service->method();
   }
   ```

2. **Observer Pattern** (ActivityRegistry):
   ```cpp
   MyService::MyService(ParameterSet const&, ActivityRegistry& ar) {
     ar.watchPreEvent([this](StreamContext const& sc) {
       this->preEvent(sc);
     });
   }
   ```

3. **Thread-Local Storage**: Different service instance per stream

4. **Token Pattern**: ServiceToken captures service configuration

## Signal Execution Order

Event processing signals:
```
preGlobalBeginRun
  preStreamBeginRun
    preEvent
      preModuleEvent (for each module)
        Module::produce()
      postModuleEvent
    postEvent
  postStreamEndRun
postGlobalEndRun
```

## Common Usage Patterns

### Accessing a Service
```cpp
#include "FWCore/ServiceRegistry/interface/Service.h"

void MyProducer::produce(Event& e, EventSetup const&) {
  edm::Service<MyService> service;
  service->doWork();
}
```

### Monitoring Module Execution
```cpp
class TimingService {
public:
  TimingService(ParameterSet const&, ActivityRegistry& ar) {
    ar.watchPreModuleEvent([this](StreamContext const&, ModuleCallingContext const& mcc) {
      startTimes_[mcc.moduleDescription()->moduleLabel()] = std::chrono::steady_clock::now();
    });
    ar.watchPostModuleEvent([this](StreamContext const&, ModuleCallingContext const& mcc) {
      auto elapsed = std::chrono::steady_clock::now() - startTimes_[mcc.moduleDescription()->moduleLabel()];
      // Log timing...
    });
  }
};
```

### Defining a Service
```cpp
// In MyService.h
class MyService {
public:
  MyService(edm::ParameterSet const&, edm::ActivityRegistry&);
  void doWork();
};

// In MyService.cc
#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"
DEFINE_FWK_SERVICE(MyService);
```

## Thread Safety Considerations
- Global services must be thread-safe (use mutexes)
- Per-stream services don't need synchronization
- ActivityRegistry callbacks can be from any thread
- ServiceRegistry is thread-local (cheap access)

## Architecture Notes
- Services initialized once at job start
- ServiceRegistry is thread-local for performance
- ActivityRegistry uses boost::signals for callbacks
- Service dependencies resolved automatically
- Services can call other services

## Available Standard Services
See **FWCore/Services** for standard implementations:
- `MessageLogger` - Log message routing
- `Timing` - Performance monitoring
- `SimpleMemoryCheck` - Memory monitoring
- `EnableFloatingPointExceptions` - FPE handling
- `RandomNumberGenerator` - RNG management
- `InitRootHandlers` - ROOT setup
- `SiteLocalConfigService` - Site configuration

## Cross-References
- **FWCore/Framework** - Creates and uses services
- **FWCore/Services** - Standard service implementations
- **FWCore/ParameterSet** - Service configuration
- **FWCore/PluginManager** - Service plugin loading
- **DQM** packages - Heavy users of monitoring services
