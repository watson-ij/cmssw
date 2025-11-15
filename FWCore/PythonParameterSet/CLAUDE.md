# FWCore/PythonParameterSet

## Purpose
Python bindings for ParameterSet using pybind11. Enables Python configuration to create C++ ParameterSet objects.

## Key Components

- **Python bindings** for ParameterSet classes
- Conversion between Python cms.PSet and C++ edm::ParameterSet

## Use Cases
- **Configuration Files**: Python configs → C++ ParameterSets
- **cmsRun**: Python-based job configuration

## Example Flow
```python
# In config_cfg.py
process.myModule = cms.EDProducer("MyProducer",
    param1 = cms.int32(10),
    param2 = cms.double(3.14)
)
```
↓ (PythonParameterSet bindings)
```cpp
// In MyProducer constructor
edm::ParameterSet pset = /* from Python */;
int p1 = pset.getParameter<int>("param1");  // 10
double p2 = pset.getParameter<double>("param2");  // 3.14
```

## Dependencies
- **FWCore**: ParameterSet, Utilities
- **DataFormats**: Provenance
- **External**: pybind11, Python3

## Cross-References
- **FWCore/ParameterSet** - C++ side
- **FWCore/ParameterSetReader** - Reads Python configs
- **Configuration/** - All Python configs use this
