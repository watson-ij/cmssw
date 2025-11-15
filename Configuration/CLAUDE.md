# Configuration Package

## Overview
The Configuration package is CMSSW's central coordination system - the "mission control" for all workflows. It provides Python-based configurations that orchestrate detector geometry, reconstruction sequences, data processing, event content, validation, and more.

**Key Role:** Translates high-level workflow intent ("reconstruct Run 3 data") into complete CMSSW configurations.

## Architecture

The Configuration package acts as a coordinator between:
- **User intent** (via cmsDriver.py commands)
- **Detector state** (geometry, calibrations, eras)
- **Processing steps** (GEN, SIM, RECO, etc.)
- **Output requirements** (RAW, RECO, AOD, MINIAOD)
- **Computing resources** (CPUs, GPUs, threading)

## Package Structure

### Core Infrastructure

**[Applications](Applications/CLAUDE.md)** - Configuration Building Tools ⭐
- **cmsDriver.py**: Command-line configuration builder
- **ConfigBuilder**: Python class for programmatic config building
- **Central to all workflows** - this is what users interact with

**[StandardSequences](StandardSequences/CLAUDE.md)** - Processing Sequences ⭐
- reconstruction, simulation, digitization sequences
- RawToDigi, DigiToRaw data format conversion
- Magnetic field, services, validation sequences
- **Building blocks** for all processing

**[EventContent](EventContent/CLAUDE.md)** - Data Tier Definitions ⭐
- RAW, RECO, AOD, MINIAOD, NANOAOD tiers
- Controls what products are saved (file size management)
- GEN-SIM-DIGI for simulation

### Detector Configuration

**[Geometry](Geometry/CLAUDE.md)** - Detector Geometries ⭐
- Run 2/3 detector configurations
- Phase-2 (HL-LHC) geometries
- DD4hep and legacy DDD systems
- Material budget variations

**[Eras](Eras/CLAUDE.md)** - Time-Dependent Configurations ⭐
- Run2_2016, Run2_2017, Run2_2018
- Run3, Run3_2023, Run3_2024
- Phase2 upgrade scenarios
- Applies detector evolution over time

**[AlCa](AlCa/CLAUDE.md)** - Calibration and Conditions ⭐
- GlobalTags (detector conditions snapshots)
- AlCa sequences (alignment, calibration streams)
- Prompt Calibration Loop (PCL)

### Workflow Customization

**[ProcessModifiers](ProcessModifiers/CLAUDE.md)** - Conditional Modifications
- **gpu**, **alpaka**: Hardware acceleration
- **trackingMkFit**, **mlpf**: Alternative algorithms
- **pp_on_AA**, **premix**: Special running modes
- **dd4hep**: Geometry system selection

**[DataProcessing](DataProcessing/CLAUDE.md)** - Production Scenarios
- Tier-0 prompt reconstruction
- Express stream processing
- Scenario: pp, HeavyIons, cosmics, AlCa
- Used in automated data processing

### Physics and Validation

**[Generator](Generator/CLAUDE.md)** - MC Event Generation
- Hundreds of pre-configured processes
- QCD, electroweak, top, Higgs, BSM
- Heavy flavor, heavy ions
- Pythia, Herwig, MadGraph configs

**[PyReleaseValidation](PyReleaseValidation/CLAUDE.md)** - Automated Testing ⭐
- **runTheMatrix.py**: Validation matrix
- Thousands of test workflows
- PR tests, IB tests, release validation
- Physics + upgrade workflows

**[Skimming](Skimming/CLAUDE.md)** - Event Selection
- Physics-group skims (BPH, EXO, EWK)
- Reduces data volume for analyses

### Specialized

**[HLT](HLT/CLAUDE.md)** - Trigger Configurations
- HLT GlobalTags and menu selection

**[JetMET](JetMET/CLAUDE.md)** - Jet/MET Helpers
- Jet and MET reconstruction configs

**[EcalTB](EcalTB/CLAUDE.md)**, **[GlobalRuns](GlobalRuns/CLAUDE.md)** - Legacy
- Historical test beam and Run 1 configs

## cmsDriver.py - The Central Tool

### Basic Usage
```bash
cmsDriver.py <fragment> \
  --conditions <GlobalTag> \
  --era <Era> \
  --geometry <Geometry> \
  --step <Steps> \
  --eventcontent <Tier> \
  --datatier <Tier> \
  [options]
```

### Complete Example
```bash
cmsDriver.py TTbar_14TeV_TuneCP5_cfi \
  --conditions auto:phase1_2024_realistic \
  --era Run3 \
  --geometry DB:Extended \
  --procModifiers gpu,alpaka \
  --step GEN,SIM,DIGI,L1,DIGI2RAW,HLT:@relval2024 \
  --eventcontent FEVTDEBUGHLT \
  --datatier GEN-SIM-DIGI-RAW-HLTDEBUG \
  --fileout file:step1.root \
  --nThreads 8 --nStreams 4 \
  -n 100 \
  --python_filename step1_cfg.py \
  --no_exec
```

### What Happens
1. **Applications/ConfigBuilder** parses command line
2. **Generator/TTbar_***: Loads event generation config
3. **AlCa/autoCond**: Resolves GlobalTag
4. **Eras/Era_Run3**: Applies Run 3 customizations
5. **Geometry**: Loads detector geometry
6. **ProcessModifiers**: Enables GPU paths
7. **StandardSequences**: Loads GEN, SIM, DIGI, L1, HLT
8. **EventContent**: Sets FEVTDEBUGHLT output
9. **Output**: Writes complete Python config

## Processing Steps

| Step | Package | Purpose |
|------|---------|---------|
| **GEN** | Generator | Event generation (Pythia, etc.) |
| **SIM** | SimG4Core | Geant4 simulation |
| **DIGI** | Sim*/Digi | Digitization (hits→signals) |
| **L1** | L1Trigger | L1 trigger emulation |
| **DIGI2RAW** | EventFilter | Pack to raw FED data |
| **HLT** | HLTrigger | High Level Trigger |
| **RAW2DIGI** | EventFilter | Unpack raw data |
| **RECO** | Reco* | Reconstruction |
| **VALIDATION** | Validation | Validation histograms |
| **DQM** | DQM | Data Quality Monitoring |

## Data Tiers

| Tier | Size/Event | Content | Use Case |
|------|------------|---------|----------|
| **RAW** | 1-2 MB | FED raw data | Archive, re-reco |
| **RECO** | ~500 KB | Full objects | Re-analysis |
| **AOD** | ~150 KB | Analysis objects | Standard analysis |
| **MINIAOD** | ~50 KB | Compressed AOD | Most analyses |
| **NANOAOD** | 1-5 KB | Flat ntuple | Simple analyses |

## Era System

Eras encode detector evolution:

```python
# Run 3 era structure
Run3 = Run2_2018.copyAndExclude([old_features]) + [
    run3_common,      # Common changes
    run3_GEM,         # GE1/1 installed
    run3_HB,          # HCAL backend
    run3_ecal_devel,  # ECAL changes
    # ... more modifiers
]
```

**Effect**: Automatically adjusts algorithms, thresholds, geometry for each era.

## GlobalTags

GlobalTags are database snapshots containing:
- Detector alignment
- Energy calibrations
- Dead channels
- Trigger menus
- Pileup scenarios

**Key GlobalTags:**
- `auto:run3_data_prompt`: Latest Run 3 data
- `auto:phase1_2024_realistic`: 2024 MC
- `auto:phase2_realistic`: Phase-2 MC

## Workflow Integration

### Typical MC Workflow
```
1. GEN: Generate events
   ↓ (GenParticles)
2. SIM: Geant4 simulation
   ↓ (SimHits)
3. DIGI: Digitization + pileup
   ↓ (Digis)
4. L1: L1 trigger emulation
   ↓ (L1 objects + trigger decision)
5. DIGI2RAW: Pack to raw
   ↓ (FEDRawData)
6. HLT: High Level Trigger
   ↓ (Trigger decision)
7. RAW2DIGI: Unpack
   ↓ (Digis)
8. RECO: Reconstruction
   ↓ (Tracks, Jets, MET, etc.)
9. VALIDATION: Make plots
10. DQM: Quality histograms
```

### Data Workflow
```
1. RAW data from DAQ
   ↓
2. HLT (online)
   ↓
3. Express: Fast reco + calibration
   ↓
4. Prompt: Full reco (~48h)
   ↓
5. AlCa: Calibration streams
   ↓
6. Skims: Physics selections
   ↓
7. AOD/MINIAOD: Analysis formats
```

## Process Modifiers Usage

Enable special features:
```bash
# GPU acceleration
--procModifiers gpu

# Multiple modifiers
--procModifiers gpu,alpaka,trackingMkFit,mlpf

# PP algorithms on heavy-ion events
--procModifiers pp_on_AA
```

## Validation Matrix

**runTheMatrix.py** runs thousands of workflows:

```bash
# Standard ttbar workflow
runTheMatrix.py -l 11634.0

# With GPU
runTheMatrix.py -l 11634.502  # Alpaka+GPU

# Phase-2 workflow
runTheMatrix.py -l 23434.0

# List all upgrade workflows
runTheMatrix.py --what upgrade -n
```

## Common Patterns

### Local Reconstruction Test
```bash
cmsDriver.py step3 \
  --conditions auto:run3_data \
  --era Run3 \
  --step RAW2DIGI,L1Reco,RECO \
  --filein file:raw.root \
  --fileout file:reco.root
```

### MC Production
```bash
# Step 1: GEN-SIM
cmsDriver.py --step GEN,SIM --conditions auto:phase1_2024_realistic

# Step 2: DIGI-HLT
cmsDriver.py --step DIGI,L1,DIGI2RAW,HLT --pileup_input file:pileup.root

# Step 3: RECO
cmsDriver.py --step RAW2DIGI,L1Reco,RECO --eventcontent AODSIM
```

### Fast Validation
```bash
# Track-only reconstruction
cmsDriver.py --step RAW2DIGI,L1Reco,RECO:reconstruction_trackingOnly
```

## Package Dependencies

### Used By
- **All CMSSW users** - via cmsDriver.py
- **Tier-0** - Data processing (DataProcessing scenarios)
- **MC production** - Generator configs, workflows
- **Validation** - runTheMatrix.py

### Uses
- **[FWCore](../FWCore/CLAUDE.md)** - Framework infrastructure
- **[Geometry](../Geometry/CLAUDE.md)** - Detector descriptions
- **All Reco* packages** - Reconstruction algorithms
- **All Sim* packages** - Simulation code
- **[DataFormats](../DataFormats/CLAUDE.md)** - Data products

## Key Design Principles

1. **Declarative**: Specify *what* you want, not *how*
2. **Modular**: Mix and match components (sequences, modifiers, eras)
3. **Versioned**: Eras track detector evolution
4. **Validated**: Extensive testing via runTheMatrix
5. **Flexible**: Process modifiers for special cases
6. **Coordinated**: Single source of truth for workflows

## Best Practices

1. **Use cmsDriver**: Don't write configs by hand
2. **Appropriate era**: Match data/MC to detector era
3. **Correct GlobalTag**: Critical for data quality
4. **Right data tier**: Balance size vs. information
5. **Process modifiers**: Enable features as needed
6. **Test with runTheMatrix**: Validate workflow changes

## Common Issues

- **Wrong era**: Leads to incorrect geometry/algorithms
- **Missing GlobalTag**: No calibrations loaded
- **Incompatible steps**: Not all step combinations valid
- **Event content mismatch**: Missing products in output
- **Geometry mismatch**: MC vs. data geometry confusion

## Cross-References

### CMSSW Packages
- **[FWCore](../FWCore/CLAUDE.md)** - Framework (executes configs)
- **[DataFormats](../DataFormats/CLAUDE.md)** - Data products (what's saved)
- **Reco*** - Reconstruction (RECO step)
- **Sim*** - Simulation (GEN, SIM, DIGI steps)
- **[HLTrigger](../HLTrigger/CLAUDE.md)** - Trigger (HLT step)
- **[L1Trigger](../L1Trigger/CLAUDE.md)** - L1 trigger (L1 step)
- **[Validation](../Validation/CLAUDE.md)**, **[DQM](../DQM/CLAUDE.md)** - Quality monitoring

### External Resources
- TWiki: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuide
- cmsDriver documentation
- Workflow management system (WMAgent)

## Summary

Configuration is the **control center** for CMSSW:

- ✅ **Unified interface**: cmsDriver.py for all workflows
- ✅ **Complete coverage**: GEN→SIM→RECO→VALIDATION
- ✅ **Time-aware**: Eras track detector evolution
- ✅ **Flexible**: Modifiers for special cases
- ✅ **Validated**: Extensive test matrix
- ✅ **Production-ready**: Used for PBs of data annually

**Bottom line:** If you're running CMSSW, you're using Configuration!
