# FWCore/Sources

## Purpose
Base classes for specialized input sources that produce or read events from various sources.

## Key Components

- **ProducerSourceBase.h** (`interface/ProducerSourceBase.h:1`) - Base for sources that generate data
  - Not from files (e.g., random particle gun)
- **RawInputSource.h** - Base for raw data input
  - DAQ data, raw detector readout
- **VectorInputSource.h** - Vector-based input source
- **EventSkipperByID.h** - Filter events by ID
- **DaqProvenanceHelper.h** - Provenance for DAQ data

## Use Cases
- **Event Generators**: Particle gun, random events
- **DAQ Input**: Online detector data
- **Custom Sources**: Non-EDM file formats

## Dependencies
- **FWCore**: Framework, ParameterSet, Reflection, Utilities
- **DataFormats**: Provenance

## Cross-References
- **IOPool/Input** - Extends these for file-based sources
- **IOMC** - Monte Carlo event generators
- **IORawData** - Raw data reading
