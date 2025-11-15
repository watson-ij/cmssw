# FWCore/PrescaleService

## Purpose
Service for applying prescales to trigger paths dynamically.

## Key Components

- **PrescaleService.h** (`interface/PrescaleService.h:1`) - Prescale management
  - Per-path prescale factors
  - Lumi-section based prescale changes

## Use Cases
- **HLT**: High Level Trigger prescale management
- **Rate control**: Reduce output rate for high-rate triggers

## Dependencies
- **FWCore**: ServiceRegistry, ParameterSet, Utilities
- **DataFormats**: Provenance

## Cross-References
- **HLTrigger** packages - Primary user
- **FWCore/ServiceRegistry** - Service infrastructure
