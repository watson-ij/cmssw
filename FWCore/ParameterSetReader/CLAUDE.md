# FWCore/ParameterSetReader

## Purpose
Read and parse Python configuration files into C++ ParameterSet objects.

## Key Components

- **ParameterSetReader.h** (`interface/ParameterSetReader.h:1`) - Parse Python config files
  - Executes Python configuration
  - Extracts process configuration
- **ProcessDescImpl.h** - Process description implementation

## Usage
Used by `cmsRun` to load Python configuration:
```
cmsRun config_cfg.py
```

## Flow
1. `cmsRun` calls ParameterSetReader
2. Executes Python config file
3. Extracts `process` object
4. Converts to C++ ParameterSet
5. Passes to EventProcessor

## Dependencies
- **FWCore**: ParameterSet, PythonParameterSet

## Cross-References
- **FWCore/PythonParameterSet** - Python bindings
- **FWCore/PythonFramework** - Python EventProcessor interface
- **Configuration/** - All config files read by this
