# FWCore/Common

## Purpose
Simplified base classes and interfaces shared between the full Framework and FWLite. Enables code reuse between heavy framework and lightweight ROOT-based analysis.

## Key Components

- **EventBase.h** (`interface/EventBase.h:1`) - Minimal event interface
  - Common methods: `get()`, `getByLabel()`, `id()`, `time()`
  - Implemented by both `edm::Event` (Framework) and `fwlite::Event` (FWLite)
- **RunBase.h** - Minimal run interface
- **LuminosityBlockBase.h** - Minimal luminosity block interface
- **TriggerNames.h** (`interface/TriggerNames.h:1`) - Access trigger path names
  - Maps trigger indices to names
- **TriggerResultsByName.h** - Access trigger decisions by name
- **ProcessBlockHelper.h** - Process block management utilities

## Use Cases
- **Framework**: Full event processing
- **FWLite**: Interactive ROOT analysis
- **Common code**: Can use EventBase instead of Event, works in both contexts

## Dependencies
- **DataFormats**: Common, Provenance
- **FWCore**: ParameterSet, Utilities
- **External**: Intel TBB

## Cross-References
- **FWCore/Framework** - Implements these interfaces
- **FWCore/FWLite** - Also implements these interfaces
- **DataFormats/Common** - Common data structures
