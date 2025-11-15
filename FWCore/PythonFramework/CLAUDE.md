# FWCore/PythonFramework

## Purpose
Python interface to EventProcessor. Enables `cmsRun` (Python-based executable) to drive the C++ framework.

## Key Components

- **PythonEventProcessor.h** (`interface/PythonEventProcessor.h:1`) - Python-accessible event processor
  - Exposed to Python via pybind11
  - Wraps C++ EventProcessor

## Usage
```python
# Inside cmsRun
from FWCore.PythonFramework.CmsRun import CmsRun
cms_run = CmsRun()
cms_run.run(config)  # Calls C++ EventProcessor
```

## Dependencies
- **FWCore**: Framework, ParameterSet, PythonParameterSet, ServiceRegistry
- **External**: pybind11, Python3, boost

## Cross-References
- **FWCore/Framework** - EventProcessor
- **FWCore/ParameterSetReader** - Config parsing
- `cmsRun` command - Uses this for Python→C++ bridge
