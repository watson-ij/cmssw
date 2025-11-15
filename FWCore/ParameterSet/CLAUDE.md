# FWCore/ParameterSet

## Purpose
Python configuration parsing and parameter management system. Provides the bridge between Python configuration files and C++ module parameters, with validation and type safety.

## Key Components

### Core Parameter Storage
- **ParameterSet.h** (`interface/ParameterSet.h:1`) - Main parameter container
  - Key-value storage for configuration
  - Supports nested ParameterSets
  - Type-safe getParameter<T>() methods
  - Methods: `getParameter<T>()`, `getUntrackedParameter<T>()`, `exists()`, `addParameter()`
- **Entry.h** - Base parameter entry
- **ParameterSetEntry.h** - Nested ParameterSet entry
- **VParameterSetEntry.h** - Vector of ParameterSets

### Configuration Validation
- **ParameterSetDescription.h** (`interface/ParameterSetDescription.h:1`) - Describes valid configuration
  - Declares allowed/required parameters
  - Provides defaults
  - Validates at job start
- **ParameterDescription.h** - Single parameter descriptor
- **ConfigurationDescriptions.h** (`interface/ConfigurationDescriptions.h:1`) - Module configuration metadata
  - `fillDescriptions()` static method in modules
  - Auto-generated documentation
- **AllowedLabelsDescription.h** - Describes allowed labels
- **PluginDescription.h** - Plugin parameter validation

### Advanced Configuration Patterns
- **ParameterSwitch.h** - Conditional parameters based on switch value
- **ParameterWildcard.h** - Accept arbitrary parameter names matching pattern
- **IfExistsDescription.h** - Optional parameter groups
- **ParameterDescriptionNode.h** - Tree structure for complex validation

### Process Configuration
- **ProcessDesc.h** (`interface/ProcessDesc.h:1`) - Process-level configuration
- **ProcessDescImpl.h** - Implementation details
- **MainParameterSet.h** - Top-level process parameters

### Parameter Registry
- **Registry.h** (`interface/Registry.h:1`) - Global ParameterSet registry
  - All ParameterSets registered with unique ID
  - Used for provenance tracking
  - Thread-safe access
- **ParameterSetID.h** - Hash-based ID for ParameterSets

### Utilities
- **FileInPath.h** (`interface/FileInPath.h:1`) - Resolves file paths relative to CMSSW_SEARCH_PATH
- **DocFormatHelper.h** - Documentation formatting
- **ParameterSetConverter.h** - Convert between ParameterSet versions

### Type Support
Supports parameters of types:
- Basic: `int32_t`, `uint32_t`, `int64_t`, `uint64_t`, `double`, `bool`, `std::string`
- Vectors: `std::vector<T>` for all basic types
- Special: `InputTag`, `FileInPath`, `EventID`, `LuminosityBlockID`, `EventRange`, `LuminosityBlockRange`
- Nested: `ParameterSet`, `std::vector<ParameterSet>`
- ESInputTag: EventSetup data tags

## Python to C++ Flow

1. **Python Configuration** (user writes):
   ```python
   process.myProducer = cms.EDProducer("MyProducer",
       inputTag = cms.InputTag("source"),
       threshold = cms.double(5.0),
       nested = cms.PSet(
           value = cms.int32(10)
       )
   )
   ```

2. **Python ParameterSet** (FWCore/PythonParameterSet):
   - Converted to string representation

3. **C++ ParameterSet** (this package):
   - Parsed and stored
   - Validated against ParameterSetDescription
   - Registered in Registry

4. **Module Constructor** (FWCore/Framework):
   ```cpp
   MyProducer::MyProducer(edm::ParameterSet const& pset) :
     inputToken_(consumes<Input>(pset.getParameter<edm::InputTag>("inputTag"))),
     threshold_(pset.getParameter<double>("threshold")) {
     auto nested = pset.getParameter<edm::ParameterSet>("nested");
     value_ = nested.getParameter<int>("value");
   }
   ```

## Dependencies
- **FWCore**: Utilities, MessageLogger, PluginManager
- **External**: boost, TBB

## Integration Points

### Used By
- **FWCore/Framework** - Module configuration
- **FWCore/ServiceRegistry** - Service configuration
- **FWCore/PluginManager** - Plugin configuration
- **All modules** - Read parameters in constructor

### Uses
- **FWCore/Utilities** - InputTag, exceptions
- **FWCore/MessageLogger** - Error reporting

## Key Design Patterns

1. **Fluent Interface** (Python side):
   ```python
   cms.PSet(x=cms.int32(1), y=cms.int32(2))
   ```

2. **Type-Safe Access** (C++ side):
   ```cpp
   int x = pset.getParameter<int>("x");  // Type enforced
   ```

3. **Validation Pattern**:
   ```cpp
   static void fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
     edm::ParameterSetDescription desc;
     desc.add<edm::InputTag>("inputTag");
     desc.add<double>("threshold", 5.0);  // with default
     descriptions.add("myProducer", desc);
   }
   ```

4. **Provenance**: Every ParameterSet gets unique ID based on content hash

## Architecture Notes
- ParameterSets are immutable after construction
- Stored in global registry for provenance
- Validation happens at job initialization, not at module construction
- Supports both tracked (saved to output) and untracked (not saved) parameters

## Common Usage Patterns

### Required Parameter
```cpp
double threshold = pset.getParameter<double>("threshold");
```

### Optional Parameter with Default
```cpp
double threshold = pset.getUntrackedParameter<double>("threshold", 5.0);
```

### Nested ParameterSet
```cpp
edm::ParameterSet nested = pset.getParameter<edm::ParameterSet>("nested");
```

### Vector of Parameters
```cpp
std::vector<std::string> labels = pset.getParameter<std::vector<std::string>>("labels");
```

### Checking Existence
```cpp
if (pset.exists("optionalParam")) {
  value = pset.getParameter<int>("optionalParam");
}
```

## Cross-References
- **FWCore/Framework** - Uses ParameterSet for all module configuration
- **FWCore/PythonParameterSet** - Python bindings
- **FWCore/ParameterSetReader** - Reads Python config files
- **Configuration/** - All configuration packages use this
