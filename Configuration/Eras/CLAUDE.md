# Configuration/Eras

## Purpose
Time-dependent detector configurations for different data-taking periods. Applies era-specific customizations to account for detector evolution.

## Main Eras

### Run 2 (2015-2018)
- **Run2_2016**, **Run2_2016_HIPM**: 2016 data (pre/post-VFP)
- **Run2_2017**: 2017 data
- **Run2_2018**, **Run2_2018_pp_on_AA**: 2018 data
- **Run2_HI**: Heavy ion runs

### Run 3 (2022-2025)
- **Run3**: Generic Run 3
- **Run3_2023**, **Run3_2024**, **Run3_2025**: Year-specific
- **Run3_pp_on_PbPb**: Heavy ions
- **Run3_UPC**: Ultra-peripheral collisions

### Phase 2 (HL-LHC, 2029+)
- **Phase2C17**: Phase-2 with specific detector configuration
- Many geometry variations (C9-C25, T15-T39, etc.)

## Era Structure
```python
Run3 = cms.ModifierChain(
    Run2_2018.copyAndExclude([...]),  # Start from Run 2
    run3_common,        # Common Run 3 changes
    run3_egamma,        # ECAL/tracker changes
    run3_muon,          # Muon system updates
    run3_GEM,           # GE1/1 installed
    run3_HB,            # HCAL backend changes
    # ... more modifiers
)
```

## Era Customizations
- Detector geometry
- Reconstruction algorithms
- Calibration constants
- Dead/masked channels
- Pileup scenarios
- Energy thresholds

## Usage
```bash
cmsDriver.py --era Run3  # Applies all Run 3 customizations
```

## Cross-References
- **[Configuration/Geometry](../Geometry/CLAUDE.md)** - Detector geometries
- **[Configuration/StandardSequences](../StandardSequences/CLAUDE.md)** - Modified by eras
- **[Configuration/ProcessModifiers](../ProcessModifiers/CLAUDE.md)** - Additional modifiers
