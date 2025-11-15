# FWCore/FWLite

## Purpose
Lightweight framework interface for reading EDM ROOT files without running the full framework. Enables interactive analysis in ROOT/PyROOT scripts.

## Key Components

- **FWLiteEnabler.h** (`interface/FWLiteEnabler.h:1`) - Initialize FWLite environment
  - Sets up ROOT environment
  - Loads necessary libraries
- **AutoLibraryLoader.h** - Automatic ROOT dictionary loading
  - Loads libraries on-demand when accessing products
- **BranchMapReader.h** - Read provenance from ROOT files
- **BareRootProductGetterBase.h** - Base for product access

## Use Cases
- **Interactive Analysis**: ROOT macros, PyROOT scripts
- **Quick Checks**: Inspect file contents without full framework
- **Lightweight Jobs**: Simple analysis without framework overhead

## Example Usage (PyROOT)
```python
from DataFormats.FWLite import Events, Handle

events = Events('file.root')
handle = Handle('std::vector<reco::Track>')

for event in events:
    event.getByLabel('generalTracks', handle)
    tracks = handle.product()
    print(f"Event has {len(tracks)} tracks")
```

## Dependencies
- **DataFormats**: Common, Provenance
- **FWCore**: Reflection, PluginManager, Utilities, Common
- **External**: ROOT

## Cross-References
- **FWCore/Common** - Shared interfaces
- **FWCore/Framework** - Full framework alternative
- **IOPool** - File reading
