# FWCore - Framework Core

## Overview
FWCore is the heart of CMSSW (CMS Software) - the complete event processing framework for the Compact Muon Solenoid experiment at CERN. It provides the infrastructure for running physics analysis code, including the event loop, module system, configuration management, plugin loading, and all core services.

**Key Capabilities:**
- Multi-threaded event processing with stream parallelism
- Plugin-based module architecture for extensibility
- Python-based configuration with C++ execution
- Type-safe data product management
- Conditions data (EventSetup) system
- Comprehensive logging and monitoring
- Service-oriented architecture

## Architecture Layers

### Layer 1: Foundation (No dependencies)
Core utilities and types used everywhere:

- **[Utilities](Utilities/CLAUDE.md)** - Essential types, exceptions, tokens, type system
  - Exception handling (`cms::Exception`)
  - Token system (`EDGetToken`, `EDPutToken`, `ESGetToken`)
  - Type identification (`TypeID`)
  - Stream/Run/Lumi identifiers

### Layer 2: Core Infrastructure
Fundamental services and plugin system:

- **[PluginManager](PluginManager/CLAUDE.md)** - Dynamic library loading and plugin factories
  - `DEFINE_FWK_MODULE` macro for registering modules
  - Factory pattern for creating instances
  - Cache-based plugin discovery

- **[MessageLogger](MessageLogger/CLAUDE.md)** - Thread-safe logging system
  - `LogError`, `LogWarning`, `LogInfo`, `LogDebug` macros
  - Severity levels and categories
  - Configurable routing and filtering

- **[Reflection](Reflection/CLAUDE.md)** - ROOT dictionary wrapper for runtime type info
  - Dynamic object manipulation
  - Used for loading products from files

- **[Concurrency](Concurrency/CLAUDE.md)** - Thread-safe primitives and task scheduling
  - `SerialTaskQueue` for protecting shared resources
  - `WaitingTaskHolder` for async operations (GPU, I/O)
  - Built on Intel TBB

### Layer 3: Configuration and Services
Parameter management and service framework:

- **[ParameterSet](ParameterSet/CLAUDE.md)** - Configuration system
  - Python → C++ parameter conversion
  - Validation and defaults
  - Type-safe access: `getParameter<T>()`

- **[ServiceRegistry](ServiceRegistry/CLAUDE.md)** - Service management and activity monitoring
  - Thread-local service access via `Service<T>`
  - `ActivityRegistry` for monitoring framework events
  - Context information (ModuleCallingContext, StreamContext, etc.)

### Layer 4: Event Processing Framework
The main event loop and module system:

- **[Framework](Framework/CLAUDE.md)** - Event processing orchestration ⭐ **CORE PACKAGE**
  - `EventProcessor` - Main application driver
  - `Event`, `Run`, `LuminosityBlock` - Data access interfaces
  - `EventSetup` - Conditions/calibrations data
  - Module base classes: `stream::EDProducer`, `global::EDProducer`, `one::EDProducer`
  - `InputSource`, `OutputModule` - I/O abstractions
  - Product resolution and lazy loading

- **[Common](Common/CLAUDE.md)** - Shared interfaces between Framework and FWLite
  - `EventBase`, `RunBase`, `LuminosityBlockBase`
  - `TriggerNames` for trigger access

### Layer 5: Specialized Components
I/O, testing, and alternative interfaces:

- **[Catalog](Catalog/CLAUDE.md)** - File location services
  - LFN → PFN mapping for grid access
  - Site configuration

- **[Sources](Sources/CLAUDE.md)** - Input source base classes
  - `ProducerSourceBase`, `RawInputSource`
  - Extended by IOPool for file reading

- **[FWLite](FWLite/CLAUDE.md)** - Lightweight ROOT-based interface
  - Interactive analysis without full framework
  - PyROOT scripting support

- **[Modules](Modules/CLAUDE.md)** - Standard framework modules
  - Output modules, analyzers, utilities

- **[Services](Services/CLAUDE.md)** - Standard framework services
  - RandomNumberGenerator, SiteLocalConfig

- **[MessageService](MessageService/CLAUDE.md)** - Logging service implementation

### Layer 6: Python Integration
Python-C++ bridge for configuration:

- **[PythonParameterSet](PythonParameterSet/CLAUDE.md)** - pybind11 bindings for ParameterSet
- **[PythonFramework](PythonFramework/CLAUDE.md)** - Python interface to EventProcessor
- **[ParameterSetReader](ParameterSetReader/CLAUDE.md)** - Parse Python configs
- **[PythonUtilities](PythonUtilities/CLAUDE.md)** - Python helper scripts

### Layer 7: Testing and Development Tools

- **[TestProcessor](TestProcessor/CLAUDE.md)** - Unit testing framework
  - Fast, isolated module testing
  - Mock EventSetup

- **[Integration](Integration/CLAUDE.md)** - Integration tests
  - Multi-component tests

- **[Skeletons](Skeletons/CLAUDE.md)** - Code generation templates
  - `mkedanlzr`, `mkedproducer`, `mkedfltr` commands

### Specialized Packages

- **[SOA](SOA/CLAUDE.md)** - Structure-of-Arrays support
  - Cache-friendly data layout
  - GPU-ready data structures

- **[SharedMemory](SharedMemory/CLAUDE.md)** - Inter-process communication
  - Multi-process parallelism

- **[PrescaleService](PrescaleService/CLAUDE.md)** - Trigger prescale management
  - Used by HLT

- **[Version](Version/CLAUDE.md)** - Version information
  - Release and file format versions

## Module Concurrency Models

CMSSW supports three module types for different thread-safety scenarios:

| Type | Location | Instances | Thread Safety | Best For |
|------|----------|-----------|---------------|----------|
| **stream** | `interface/stream/` | One per stream | Automatic | Most modules (default) |
| **global** | `interface/global/` | One shared | Must ensure | Expensive setup, thread-safe code |
| **one** | `interface/one/` | One | Serial | Legacy, simple algorithms |

Each type has variants: `EDProducer`, `EDAnalyzer`, `EDFilter`

### Module Abilities (Mixins)
Modules can declare capabilities:
- `WatchRuns`, `WatchLuminosityBlocks` - Monitor transitions
- `RunCache<T>`, `LuminosityBlockCache<T>` - Per-run/lumi data
- `ExternalWork` - Async operations (GPU, network)

## Key Design Patterns

### 1. Plugin Architecture
All modules discovered and loaded dynamically:
```cpp
// In module file
class MyProducer : public edm::stream::EDProducer<> { /*...*/ };
DEFINE_FWK_MODULE(MyProducer);  // Register plugin
```

### 2. Token Pattern
Type-safe, declarative data access:
```cpp
// In constructor - declare what you need
MyProducer::MyProducer(edm::ParameterSet const& pset) :
  inputToken_(consumes<InputType>(pset.getParameter<edm::InputTag>("input"))) {}

// In produce - use token
void MyProducer::produce(edm::Event& event, edm::EventSetup const&) {
  auto input = event.get(inputToken_);  // Type-safe access
  event.put(outputToken_, result);
}
```

### 3. Configuration via Python
```python
# config.py
process.myProducer = cms.EDProducer("MyProducer",
    input = cms.InputTag("source"),
    threshold = cms.double(5.0)
)
```
```cpp
// MyProducer.cc constructor
double threshold = pset.getParameter<double>("threshold");
```

### 4. Service Access
```cpp
edm::Service<RandomNumberGenerator> rng;
auto& engine = rng->getEngine(streamID);
```

### 5. Activity Monitoring
```cpp
// In service constructor
MyService::MyService(edm::ParameterSet const&, edm::ActivityRegistry& ar) {
  ar.watchPreEvent([this](StreamContext const& sc) {
    this->startTimer(sc.eventID());
  });
}
```

## Event Processing Flow

```
┌─────────────────────────────────────────────────────┐
│ 1. cmsRun config.py                                 │
│    ↓                                                │
│ 2. PythonFramework reads config                     │
│    ↓                                                │
│ 3. PluginManager loads module libraries             │
│    ↓                                                │
│ 4. EventProcessor initializes                       │
│    - Create modules (constructors)                  │
│    - Call beginJob()                                │
│    ↓                                                │
│ 5. Process Runs                                     │
│    ├─ beginRun()                                    │
│    ├─ Process Lumis                                 │
│    │  ├─ beginLuminosityBlock()                     │
│    │  ├─ Process Events (parallel streams)          │
│    │  │  ├─ InputSource::readEvent()                │
│    │  │  ├─ Execute Paths (sequence of modules)     │
│    │  │  │  └─ Module::produce/analyze/filter()     │
│    │  │  └─ OutputModule::write()                   │
│    │  └─ endLuminosityBlock()                       │
│    └─ endRun()                                      │
│    ↓                                                │
│ 6. EventProcessor shutdown                          │
│    - Call endJob()                                  │
│    - Cleanup                                        │
└─────────────────────────────────────────────────────┘
```

## Data Model

### Event Data (Event, Run, LuminosityBlock)
- **Physics data**: Tracks, clusters, particles, etc.
- **Per-event/run/lumi**: Changes every event/run/lumi
- **Access**: `event.get(token)`
- **Storage**: ROOT TTree branches

### Conditions Data (EventSetup)
- **Calibrations**: Detector alignment, energy calibrations
- **Geometry**: Detector geometry
- **Conditions**: Trigger menus, dead channels
- **Interval of Validity (IOV)**: Valid for run range
- **Access**: `eventSetup.get<Record>().get(token)`
- **Storage**: Database (SQLite, Oracle) or ROOT files

## Thread Safety in CMSSW

### Stream Parallelism
- **N concurrent streams** (typically 4-8)
- Each stream processes independent events
- **stream modules**: One instance per stream (automatic thread safety)
- **global modules**: Single instance, must be thread-safe

### Protecting Shared Resources
```cpp
class MyProducer : public edm::global::EDProducer<> {
  edm::SerialTaskQueue queue_;  // Serialize access
  NonThreadSafeResource resource_;

public:
  void produce(edm::StreamID, edm::Event& e, edm::EventSetup const&) const override {
    queue_.pushAndWait([&]() {
      resource_.doWork();  // Protected access
    });
  }
};
```

## Running CMSSW

### Basic Command
```bash
cmsRun config.py
```

### Configuration Example
```python
import FWCore.ParameterSet.Config as cms

process = cms.Process("DEMO")

# Configure services
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'

# Input
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:input.root')
)

# Configure module
process.myAnalyzer = cms.EDAnalyzer("MyAnalyzer",
    tracks = cms.InputTag("generalTracks"),
    ptMin = cms.double(1.0)
)

# Path
process.p = cms.Path(process.myAnalyzer)
```

## Package Dependencies

FWCore packages used by:
- **All reconstruction packages** (Reco*) - Use Framework for modules
- **All simulation packages** (Sim*) - Use Framework for event generation
- **All analysis packages** - Use Framework or FWLite
- **Configuration** - Uses ParameterSet system
- **DataFormats** - Products accessed via Framework
- **CondCore** - EventSetup system
- **DQM** - Uses Framework and services

External dependencies:
- **ROOT** - I/O, histograms, reflection
- **Intel TBB** - Task parallelism
- **boost** - Various utilities
- **Python 3** - Configuration
- **pybind11** - Python-C++ bindings
- **fmt** - String formatting

## Key Concepts Summary

- **Module**: Algorithm that produces, analyzes, or filters events
- **Path**: Sequence of modules to execute
- **Event**: Single collision event
- **Run**: Data-taking period with fixed detector configuration
- **LuminosityBlock**: Subdivision of run (~23 seconds)
- **Product**: Data object stored in Event/Run/Lumi
- **Token**: Type-safe handle for accessing products
- **EventSetup**: Conditions/calibrations data system
- **Service**: Global or thread-local singleton
- **Plugin**: Dynamically loaded module/service
- **Stream**: Concurrent event processing lane
- **ParameterSet**: Configuration object

## Cross-References

### Within CMSSW
- **[Configuration](../Configuration/CLAUDE.md)** - Python configs use FWCore/ParameterSet
- **[DataFormats](../DataFormats/CLAUDE.md)** - Data products accessed via FWCore/Framework
- **IOPool** - Input/output using FWCore/Sources and Framework
- **Reco*** - Reconstruction modules use FWCore/Framework base classes
- **Sim*** - Simulation modules use FWCore/Framework
- **CondCore** - Conditions infrastructure on top of EventSetup
- **HeterogeneousCore** - GPU support via ExternalWork pattern

### External Resources
- CMSSW Documentation: https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBook
- Framework Guide: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrameWork
- EDM Plugin System: https://twiki.cern.ch/twiki/bin/view/CMSPublic/PluginManager

## Development Workflow

### Creating a New Module
```bash
# Create analyzer
mkedanlzr MyAnalyzer
# Creates MyAnalyzer.cc with skeleton code

# Add to BuildFile.xml
# Implement analyze() method
# Build
scram b

# Update plugin cache
edmPluginRefresh

# Run
cmsRun config.py
```

### Module Lifecycle
```
Constructor(ParameterSet)
  ↓
beginJob()
  ↓
beginRun(Run const&)
  ↓
produce/analyze/filter(Event&, EventSetup const&)  // Per event
  ↓
endRun(Run const&)
  ↓
endJob()
  ↓
Destructor
```

## Performance Considerations

- **Stream modules**: Preferred for parallelism
- **Token declaration**: Allows framework to optimize data loading
- **Lazy loading**: Products loaded only when accessed
- **SerialTaskQueue**: Use sparingly, limits parallelism
- **Memory**: Framework manages product lifetimes
- **Cache**: Use run/lumi caches for expensive computations

## Common Pitfalls

1. **Static variables in modules**: Not thread-safe in global modules
2. **Missing token declaration**: Runtime error, not compile error
3. **Wrong module type**: stream vs global vs one
4. **ROOT not thread-safe**: Use SerialTaskQueue
5. **Missing DEFINE_FWK_MODULE**: Plugin not found
6. **Forgot edmPluginRefresh**: Plugin cache out of date

## Summary

FWCore provides a complete, production-ready event processing framework:
- ✅ **Scalable**: Multi-threaded, task-based parallelism
- ✅ **Flexible**: Plugin architecture for extensibility
- ✅ **Type-safe**: Compile-time and runtime type checking
- ✅ **Configurable**: Python-based configuration with validation
- ✅ **Robust**: Exception handling, provenance tracking
- ✅ **Observable**: Comprehensive logging and monitoring
- ✅ **Tested**: Extensive integration and unit tests

Used to process **petabytes** of LHC collision data annually!
