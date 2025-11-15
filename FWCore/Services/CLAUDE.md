# FWCore/Services

## Purpose
Standard framework services implementations. Provides core services used across CMSSW.

## Key Services (Plugins)

### Site Configuration
- **SiteLocalConfigService** - Site-specific configuration
  - Storage site setup
  - Catalog endpoints

### Random Numbers
- **RandomNumberGeneratorService** - Per-module RNG streams
  - Reproducible random numbers
  - Independent streams per module

### Monitoring Services
See standard services in plugins directory

## Dependencies
- **FWCore**: ServiceRegistry, ParameterSet, MessageLogger, Catalog
- **External**: CLHEP, ROOT

## Cross-References
- **FWCore/ServiceRegistry** - Service infrastructure
- All packages - Use these standard services
