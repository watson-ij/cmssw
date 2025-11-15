# FWCore/Framework

## Purpose
The heart of CMSSW - orchestrates event processing, module execution, data flow, and manages the complete event processing lifecycle. This is the main event loop framework that drives all particle physics data processing.

## Key Components

### Event Processor
- **EventProcessor.h** (`interface/EventProcessor.h:1`) - Main application object that drives the framework
  - Manages run/lumi/event transitions
  - Coordinates parallel stream processing
  - Handles framework initialization and finalization
- **Schedule.h** - Manages Paths and EndPaths
- **Path.h**, **EndPath.h** - Sequences of modules to execute

### Data Access Interfaces
- **Event.h** (`interface/Event.h:1`) - Primary interface for event data access
  - `get(token)` - Retrieve products by token
  - `put(token, product)` - Insert products
  - `getByToken()`, `getByLabel()` - Product access methods
- **EventSetup.h** (`interface/EventSetup.h:1`) - Container for calibration/conditions data
  - Non-event data (alignment, calibrations, geometry)
  - Validity intervals (IOV - Interval of Validity)
- **Run.h**, **LuminosityBlock.h**, **ProcessBlock.h** - Higher-level granularity interfaces

### Module Base Classes

#### Stream Modules (Parallel Processing)
- **stream/EDProducer.h** (`interface/stream/EDProducer.h:1`) - Parallel event producers
- **stream/EDAnalyzer.h** - Parallel event analyzers
- **stream/EDFilter.h** - Parallel event filters
- Thread-safe, one instance per stream

#### Global Modules (Shared State)
- **global/EDProducer.h** (`interface/global/EDProducer.h:1`) - Thread-safe producers
- **global/EDAnalyzer.h** - Thread-safe analyzers
- **global/EDFilter.h** - Thread-safe filters
- Single instance, must be thread-safe, can have Run/Lumi summaries

#### One Modules (Serial Processing)
- **one/EDProducer.h** (`interface/one/EDProducer.h:1`) - Serial producers
- **one/EDAnalyzer.h** - Serial analyzers
- **one/EDFilter.h** - Serial filters
- Single instance, serial execution, simple to write

#### Legacy Modules (Deprecated)
- **EDProducer.h**, **EDAnalyzer.h**, **EDFilter.h** - Old-style modules

### EventSetup System
- **ESProducer.h** - EventSetup data producers
- **ESHandle.h** - Smart pointer for EventSetup data
- **ESTransientHandle.h** - Transient data handle
- **EventSetupRecord.h** - Base for condition records
- **EventSetupRecordIntervalFinder.h** - Determines record validity
- **DataProxyProvider.h** - Provides data proxies

### Input/Output
- **InputSource.h** (`interface/InputSource.h:1`) - Abstract base for all input sources
  - `readEvent()`, `readRun()`, `readLuminosityBlock()` - Read data
- **OutputModule.h** - Base for output modules
- **OutputModuleCommunicator.h** - Output coordination

### Data Management
- **EventPrincipal.h** (`interface/EventPrincipal.h:1`) - Event data container
- **RunPrincipal.h**, **LuminosityBlockPrincipal.h**, **ProcessBlockPrincipal.h** - Higher-level containers
- **ProductResolver.h** - Lazy product loading mechanism
- **ProductResolverBase.h** - Base for product resolution
- **Principal.h** - Base data container

### Module Infrastructure
- **EDConsumerBase.h** - Base class for all data-consuming modules
  - Declares what products the module will access
  - Token registration
- **ProducerBase.h** - Base for producers
- **WorkerT.h** - Module execution wrapper
- **Worker.h** - Abstract worker base
- **Maker.h** - Module factory

### Concurrency Support
- **StreamSchedule.h** - Per-stream schedule
- **GlobalSchedule.h** - Global schedule coordination
- **WaitingTaskHolder.h** - Async task management
- **TriggerResultInserter.h** - Insert trigger results

### Module Abilities (Mixins)
- **stream/moduleAbilities.h** - Capabilities modules can declare:
  - `WatchRuns`, `WatchLuminosityBlocks` - Monitor run/lumi transitions
  - `RunCache<T>`, `LuminosityBlockCache<T>` - Per-run/lumi data
  - `RunSummaryCache<T>`, `LuminosityBlockSummaryCache<T>` - Aggregated data
  - `BeginRunProducer`, `EndRunProducer` - Produce run products
  - `ExternalWork` - Asynchronous work (GPU, network)

### Special Modules
- **TriggerNamesService.h** - Access to trigger path names
- **ConstProductRegistry.h** - Product registry access
- **MakerMacros.h** - `DEFINE_FWK_MODULE` macro for plugin registration

## Module Types Summary

| Module Type | Instances | Thread Safety | Use Case |
|-------------|-----------|---------------|----------|
| stream::EDProducer | One per stream | Automatic | Most producers |
| global::EDProducer | One shared | Must ensure | Expensive initialization |
| one::EDProducer | One | Serial | Simple algorithms |
| EDAnalyzer (legacy) | One per stream | Automatic | Old code |

## Dependencies
- **FWCore**: Utilities, ParameterSet, ServiceRegistry, PluginManager, Concurrency, MessageLogger
- **DataFormats**: Common, Provenance, all specific data formats
- **External**: ROOT, TBB, boost

## Integration Points

### Used By
- All reconstruction packages (Reco*)
- All simulation packages (Sim*)
- All analysis packages
- Configuration system creates modules
- User plugins (producers, analyzers, filters)

### Uses
- **FWCore/Utilities** - Tokens, types, exceptions
- **FWCore/ParameterSet** - Module configuration
- **FWCore/ServiceRegistry** - Service access, activity registry
- **FWCore/PluginManager** - Dynamic module loading
- **DataFormats/** - All data products

## Key Design Patterns

1. **Event Processing Model**
   - Path-based execution (chains of modules)
   - Lazy product loading (on-demand)
   - Provenance tracking (where data came from)

2. **Concurrency Model**
   - Stream parallelism (multiple events in flight)
   - Task-based scheduling (TBB)
   - Thread-safe data access

3. **Module Lifecycle**
   ```
   Constructor → beginJob → beginRun → beginLumi →
   produce/analyze/filter (per event) →
   endLumi → endRun → endJob → Destructor
   ```

4. **Token Pattern**
   - Declare tokens in constructor: `consumes<T>(inputTag)`
   - Use in produce: `event.get(token)`
   - Framework validates at job start

5. **EventSetup Pattern**
   - Conditions data separate from event data
   - Validity intervals
   - Lazy loading and caching

## Architecture Notes
- Core event loop is single-threaded
- Parallelism via multiple concurrent streams
- Module execution parallelized via TBB tasks
- Products identified by (type, module, instance, process)
- Extensive use of smart pointers for memory management

## Example Module Structure
```cpp
class MyProducer : public edm::stream::EDProducer<> {
public:
  explicit MyProducer(edm::ParameterSet const&);
  void produce(edm::Event&, edm::EventSetup const&) override;

private:
  edm::EDGetTokenT<InputType> inputToken_;
  edm::EDPutTokenT<OutputType> outputToken_;
};
```

## Cross-References
- **FWCore/ParameterSet** - Module configuration
- **FWCore/ServiceRegistry** - Services used by modules
- **DataFormats/Common** - EDProduct, Handle, Wrapper
- **IOPool/Input** - Concrete input sources
- **Configuration/StandardSequences** - Standard processing paths
