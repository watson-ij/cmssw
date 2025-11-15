# Configuration/Applications

## Purpose
Core configuration building tools, centered on `cmsDriver.py`.

## Main Components

### ConfigBuilder (`ConfigBuilder.py`)
Python class that builds complete CMSSW configurations:
- Chains processing steps (GEN→SIM→DIGI→RECO)
- Applies era customizations
- Configures I/O
- Manages event content
- Handles process modifiers

### cmsDriver.py
Command-line interface to ConfigBuilder:
```bash
cmsDriver.py step1 \
  --conditions auto:phase1_2024_realistic \
  --era Run3 \
  --geometry DB:Extended \
  --step GEN,SIM,DIGI,L1,DIGI2RAW,HLT:@relval2024 \
  --eventcontent FEVTDEBUGHLT \
  --datatier GEN-SIM-DIGI-RAW-HLTDEBUG \
  -n 100 \
  --python_filename config.py
```

### Key Options
- `--step`: Processing steps to run
- `--conditions`: GlobalTag
- `--era`: Detector era (Run2_2018, Run3, Phase2)
- `--geometry`: Detector geometry
- `--procModifiers`: Process modifiers (gpu, alpaka)
- `--eventcontent`/`--datatier`: Output format
- `--nThreads`/`--nStreams`: Parallelism

### Processing Steps
- **GEN**: Event generation
- **SIM**: Geant4 simulation
- **DIGI**: Digitization
- **L1**: L1 trigger emulation
- **DIGI2RAW**: Pack to raw format
- **HLT**: High Level Trigger
- **RAW2DIGI**: Unpack raw data
- **RECO**: Reconstruction
- **VALIDATION**: Validation histograms
- **DQM**: Data Quality Monitoring

## Cross-References
- **[Configuration/StandardSequences](../StandardSequences/CLAUDE.md)** - Loads sequences
- **[Configuration/Eras](../Eras/CLAUDE.md)** - Era definitions
- **[Configuration/EventContent](../EventContent/CLAUDE.md)** - Output content
- All Configuration packages - Used by cmsDriver
