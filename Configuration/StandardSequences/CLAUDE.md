# Configuration/StandardSequences

## Purpose
Standard processing sequences for reconstruction, digitization, simulation, and data processing. Building blocks for all CMSSW workflows.

## Key Sequences

### Reconstruction (`Reconstruction_cff.py`)
- **localreco**: Local reconstruction (tracker hits, muon segments, calorimeter clusters)
- **globalreco**: Global reconstruction (tracking, jets, MET, vertices)
- **highlevelreco**: High-level objects (particle flow, b-tagging, taus)
- **reconstruction**: Complete chain (local + global + highlevel)

### Raw Data Processing
- **RawToDigi_cff.py**: Unpack raw detector data to digis (all subdetectors)
- **DigiToRaw_cff.py**: Pack digis back to raw format
- **L1Reco_cff.py**: L1 trigger object reconstruction

### Simulation/Digitization
- **Simulation_cff.py**: Geant4 simulation
- **Digi_cff.py**: Digitization (convert sim hits to detector signals)
- **SimL1Emulator_cff.py**: L1 trigger emulation

### Infrastructure
- **MagneticField_cff.py**: Magnetic field configurations (uniform, mapped, 0T)
- **FrontierConditions_GlobalTag_cff.py**: GlobalTag setup
- **Services_cff.py**: Standard services (RandomNumberGenerator, etc.)

## Usage in cmsDriver
```bash
cmsDriver.py --step GEN,SIM,DIGI,L1,DIGI2RAW,RAW2DIGI,RECO
```
Each step loads corresponding `_cff.py` file from this package.

## Cross-References
- **[Configuration/Eras](../Eras/CLAUDE.md)** - Modifies sequences per era
- **[Configuration/EventContent](../EventContent/CLAUDE.md)** - Defines what to save
- **Reco*** packages - Implement reconstruction algorithms
- **[SimGeneral](../../SimGeneral/CLAUDE.md)** - Simulation infrastructure
