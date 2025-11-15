# FWCore/Reflection

## Purpose
Wrapper around ROOT's reflection system (Cling/TClass) for runtime type information and dynamic object manipulation.

## Key Components

- **TypeWithDict.h** (`interface/TypeWithDict.h:1`) - Runtime type information
  - Query type properties (size, name, members)
  - Create instances dynamically
- **ObjectWithDict.h** - Object manipulation via reflection
  - Get/set members dynamically
  - Call methods
- **MemberWithDict.h** - Member (field) access
- **FunctionWithDict.h** - Function/method invocation
- **DictionaryTools.h** - Utilities for working with ROOT dictionaries

## Use Cases
- **Product Loading**: Load data products from ROOT files without compile-time knowledge
- **FWLite**: Dynamic type handling in interactive analysis
- **Provenance**: Inspect object types at runtime

## Dependencies
- **FWCore**: Utilities
- **External**: ROOT, Intel TBB

## Cross-References
- **FWCore/Framework** - Uses for dynamic product loading
- **FWCore/FWLite** - Uses for interactive access
- **IOPool** - Uses for reading ROOT files
