# CMSSW - CMS Software

## What is CMSSW?

**CMSSW (CMS Software)** is the complete offline software framework for the **Compact Muon Solenoid (CMS)** experiment at CERN's Large Hadron Collider (LHC). It handles everything from simulating particle collisions to reconstructing detector signals to analyzing physics data.

**Scale:** Processes **petabytes** of data annually from ~40 million proton collisions per second.

**Language:** Primarily C++ with Python configuration, built on ROOT and other scientific computing libraries.

**Architecture:** Event-driven framework with plugin-based modules, multi-threaded processing, and comprehensive calibration/conditions system.

## Quick Navigation

- **[FWCore/](FWCore/CLAUDE.md)** ⭐ - Framework Core (event loop, module system, services)
- **[Configuration/](Configuration/CLAUDE.md)** ⭐ - Workflow coordination and configuration
- **[DataFormats/](DataFormats/CLAUDE.md)** ⭐ - Data product definitions (123 packages)
- **[RECONSTRUCTION.md](RECONSTRUCTION.md)** ⭐ - Reconstruction packages overview (Reco*)
- **[SIMULATION.md](SIMULATION.md)** ⭐ - Simulation packages overview (Sim*, generators)

## CMSSW Architecture at a Glance

```
┌─────────────────────────────────────────────────────────────────┐
│                    CMSSW Event Processing                       │
│                                                                 │
│  User writes:     Python Configuration                         │
│                         ↓                                       │
│  cmsDriver builds:  Complete workflow config                   │
│                         ↓                                       │
│  FWCore runs:      Event loop with modules                     │
│                         ↓                                       │
│  Modules process:  Events through algorithms                   │
│                         ↓                                       │
│  DataFormats store: Physics objects (tracks, jets, etc.)       │
│                         ↓                                       │
│  Output:           ROOT files (RAW, RECO, AOD, MINIAOD)        │
└─────────────────────────────────────────────────────────────────┘
```

## Major Package Categories

### 1. Framework and Infrastructure ⭐

**[FWCore/](FWCore/CLAUDE.md)** (30 subpackages)
- Event processing framework
- Module system (EDProducer, EDAnalyzer, EDFilter)
- Configuration (ParameterSet)
- Plugin management
- Services and logging
- Threading and concurrency
- Python-C++ integration

**Key:** This is the **engine** - everything runs in FWCore.

**[Configuration/](Configuration/CLAUDE.md)** (17 subpackages)
- Workflow coordination (cmsDriver.py)
- Standard sequences (reconstruction, simulation)
- Detector geometries and eras
- Event content (data tiers)
- GlobalTags (calibrations)
- Validation matrix

**Key:** This is the **control center** - orchestrates workflows.

### 2. Data Definitions ⭐

**[DataFormats/](DataFormats/CLAUDE.md)** (123 subpackages!)
Defines all data products:
- **Detector data:** Digis, RecHits, Clusters
- **Tracks and Vertices:** `reco::Track`, `reco::Vertex`
- **Physics objects:** Muons, Electrons, Photons, Jets, MET, Taus
- **Particle Flow:** `reco::PFCandidate`
- **Generator:** `reco::GenParticle`
- **Trigger:** L1 and HLT objects

**Organized by:** Detector (Tracker, ECAL, HCAL, Muon) and physics (Jets, MET, etc.)

**SimDataFormats/** (26 packages) - Simulation truth (SimHits, SimTracks)

**CUDADataFormats/** (17 packages) - GPU-compatible data formats

**AnalysisDataFormats/** (5 packages) - Analysis-specific formats

### 3. Reconstruction ⭐

**[RECONSTRUCTION.md](RECONSTRUCTION.md)** - See detailed overview

**Local Reconstruction:** Detector signals → RecHits
- **RecoLocalTracker/** - Pixel/strip clustering
- **RecoLocalCalo/** - ECAL/HCAL rechits, CaloTowers
- **RecoLocalMuon/** - Muon segments

**Global Reconstruction:** RecHits → Tracks, Vertices, Clusters
- **RecoTracker/** - Iterative tracking, MkFit
- **RecoVertex/** - Primary vertices, beam spot
- **RecoEcal/** - SuperClusters
- **RecoMuon/** - Muon reconstruction

**High-Level Objects:** Combined information → Physics objects
- **RecoParticleFlow/** - Particle Flow (PF) algorithm
- **RecoJets/** - Jet clustering (anti-kT, etc.)
- **RecoMET/** - Missing ET
- **RecoEgamma/** - Electrons, photons (GSF)
- **RecoTauTag/** - Hadronic tau reconstruction
- **RecoBTag/** - B-tagging (DeepJet, etc.)

**Specialized:**
- **RecoHI/** - Heavy ions
- **RecoPPS/** - Precision proton spectrometer
- **RecoLuminosity/** - Luminosity measurements

### 4. Simulation and Event Generation ⭐

**[SIMULATION.md](SIMULATION.md)** - See detailed overview

**Event Generation:**
- **GeneratorInterface/** - Pythia, Herwig, MadGraph interfaces
- **IOMC/** - Monte Carlo I/O, particle guns
- **Configuration/Generator/** - Pre-configured processes

**Detector Simulation:**
- **SimG4Core/** - Geant4 engine integration
- **SimG4CMS/** - CMS-specific Geant4 code
- **SimTracker/** - Tracker simulation/digitization
- **SimCalorimetry/** - Calorimeter simulation/digitization
- **SimMuon/** - Muon chamber simulation/digitization
- **SimGeneral/** - Utilities, mixing (pileup)

**Fast Simulation:**
- **FastSimulation/** - Parametrized detector response (~10x faster)
- **FastSimDataFormats/** - FastSim data products

### 5. Trigger Systems ⭐

**L1Trigger/** (57 subpackages)
- **L1TGlobal/** - Global Trigger decision
- **L1TCalorimeter/** - Calorimeter triggers
- **L1TMuon/** - Muon triggers
- **L1TTrackTrigger/** - Track triggers (Phase-2)
- **Phase2L1Trigger/** - Phase-2 upgrade trigger

**L1TriggerConfig/** (16 packages) - L1 trigger configuration

**HLTrigger/** (14 packages)
- **HLTcore/** - HLT framework
- **Muon/**, **Egamma/**, **JetMET/** - HLT algorithms by object
- **Configuration/HLT/** - HLT menus

**HLTriggerOffline/** - HLT offline validation

### 6. Calibration and Conditions

**CondCore/** (42 packages)
- Conditions database framework
- Frontier/SQLite access
- Payload handling

**CondFormats/** (43 packages)
- Data formats for calibrations
- Alignment, energy scales, dead channels

**Calib* packages:**
- **CalibTracker/** (24 packages) - Tracker calibration
- **CalibCalorimetry/** (23 packages) - Calorimeter calibration
- **CalibMuon/** (7 packages) - Muon calibration
- **CalibFormats/** (8 packages) - Calibration data formats
- **Calibration/** (15 packages) - Calibration tools

**Alignment/** (28 packages) - Detector alignment

### 7. Detector Geometry

**Geometry/** (58 packages!)
- **CaloGeometry/** - Calorimeter geometry
- **TrackerGeometry/** - Tracker geometry
- **DTGeometry/**, **CSCGeometry/**, **RPCGeometry/** - Muon geometry
- **HGCalGeometry/** - HGCal (Phase-2)
- **MTDGeometry/** - MTD/timing (Phase-2)
- **Records/** - Geometry records for EventSetup

**DetectorDescription/** (7 packages)
- **DDCMS/** - DD4hep geometry system (modern)
- **Core/** - Legacy DDD system

**Configuration/Geometry/** - Geometry configurations for all eras

### 8. Data Quality and Validation

**DQM/** (66 packages!)
- **Integration/** - DQM framework
- **DTMonitorModule/**, **EcalMonitorTasks/**, etc. - Per-detector DQM
- **Physics/** - Physics object monitoring
- **TrackingMonitor/** - Tracking quality

**DQMOffline/** (19 packages) - Offline DQM

**DQMServices/** (9 packages) - DQM infrastructure

**Validation/** (63 packages)
- Validation plots and metrics
- Efficiency calculations
- Truth matching

### 9. Event Filtering and I/O

**EventFilter/** (34 packages)
- **RawToDigi/** modules for all subdetectors
- **HLTfilter/** - HLT filtering
- **Utilities/** - DAQ interfaces

**IOPool/** (9 packages)
- **Input/** - ROOT file reading (PoolSource)
- **Output/** - ROOT file writing (PoolOutputModule)
- **Streamer/** - Event streaming

**IORawData/** (7 packages) - Raw data I/O

### 10. Analysis Tools

**PhysicsTools/** (36 packages)
- **PatAlgos/** - Physics Analysis Toolkit (PAT)
  - `pat::Electron`, `pat::Muon`, etc. (analysis objects)
- **NanoAOD/** - Flat ntuple format
- **UtilAlgos/** - Analysis utilities
- **SelectorUtils/** - Object selectors
- **RecoAlgos/** - Reconstruction helpers

**CommonTools/** (18 packages)
- **Utils/** - Common utilities
- **ParticleFlow/** - PF tools
- **CandAlgos/** - Candidate algorithms

### 11. Corrections and Systematics

**JetMETCorrections/** (16 packages)
- **Modules/** - JEC/JER corrections
- **Type1MET/** - Type-1 MET corrections

**EgammaAnalysis/** - Electron/photon analysis tools

**RecoEgamma/EgammaTools/** - E/gamma corrections

**TrackingTools/** (25 packages) - Tracking utilities

### 12. Heterogeneous Computing ⭐

**HeterogeneousCore/** (19 packages)
- **CUDACore/** - CUDA infrastructure
- **AlpakaCore/** - Alpaka (portable acceleration)
- **SonicCore/** - Services for inference

**HeterogeneousTest/** - GPU tests

**Configuration/ProcessModifiers/** - `gpu`, `alpaka` modifiers

### 13. Other Key Packages

**OnlineDB/** (8 packages) - Online database access

**CondTools/** (23 packages) - Conditions manipulation tools

**Utilities/** (16 packages) - General utilities, timing, etc.

**PerfTools/** (8 packages) - Performance profiling

**Documentation/** (6 packages) - Documentation tools

**MagneticField/** (11 packages) - Magnetic field map

**TrackPropagation/** (5 packages) - Track propagation

## Data Flow Through CMSSW

### Monte Carlo Production
```
1. Event Generation (GeneratorInterface)
   GenParticles
   ↓
2. Simulation (SimG4Core, SimTracker, SimCalorimetry, SimMuon)
   SimHits (energy deposits)
   ↓
3. Digitization (Sim*Digitizer)
   Digis (detector signals)
   ↓
4. Pileup Mixing (Mixing)
   Digis with pileup
   ↓
5. L1 Trigger Emulation (L1Trigger)
   L1 decision + objects
   ↓
6. HLT Emulation (HLTrigger)
   HLT decision + paths
   ↓
7. Raw Packing (EventFilter/DigiToRaw)
   FEDRawData
   ↓
8. Raw Unpacking (EventFilter/RawToDigi)
   Digis
   ↓
9. Reconstruction (RecoLocal*, RecoTracker, RecoParticleFlow, etc.)
   Tracks, Jets, MET, Muons, Electrons, Photons, Taus
   ↓
10. PAT (PhysicsTools/PatAlgos)
    pat::Jet, pat::Muon, etc.
   ↓
11. NanoAOD (PhysicsTools/NanoAOD)
    Flat ntuples for analysis
```

### Real Data Processing
```
1. DAQ → RAW data (FEDRawData)
   ↓
2. HLT (online) → Filter events
   ↓
3. Transfer to Tier-0
   ↓
4. Express Stream (Configuration/DataProcessing)
   Fast reco for monitoring + calibration
   ↓
5. Prompt Reconstruction (~48h)
   RAW → RECO, AOD
   ↓
6. AlCa Streams → Calibrations
   ↓
7. Skimming (Configuration/Skimming)
   Physics-specific selections
   ↓
8. MINIAOD/NANOAOD production
   Analysis formats
   ↓
9. User Analysis
```

## Data Tiers (File Formats)

| Tier | Size/Event | Content | Use |
|------|-----------|---------|-----|
| **RAW** | 1-2 MB | FED raw data | Archive, re-reco |
| **RECO** | ~500 KB | Full reconstruction | Physics studies |
| **AOD** | ~150 KB | Analysis objects | Standard analysis |
| **MINIAOD** | ~50 KB | Compressed AOD | Most analyses |
| **NANOAOD** | 1-5 KB | Flat ntuple | Simple analyses |
| **GEN-SIM** | - | MC truth + sim hits | MC only |

## Execution Model

### cmsRun (The Main Executable)

```bash
cmsRun config.py
```

**What happens:**
1. **FWCore/PythonFramework** - Python reads config
2. **FWCore/ParameterSet** - Converts to C++ ParameterSets
3. **FWCore/PluginManager** - Loads module libraries
4. **FWCore/Framework/EventProcessor** - Runs event loop
   - `beginJob()` for all modules
   - For each Run: `beginRun()`
   - For each LuminosityBlock: `beginLuminosityBlock()`
   - For each Event (parallel streams):
     - Execute modules in Paths
     - Modules produce/analyze/filter
   - `endLuminosityBlock()`, `endRun()`, `endJob()`
5. **IOPool/Output** - Writes results to ROOT files

### Parallelism

- **N streams:** Process N events concurrently (typically 8)
- **M threads:** Thread pool for task execution
- **Module types:**
  - `stream::EDProducer`: One per stream, automatic parallelism
  - `global::EDProducer`: Single instance, must be thread-safe
  - `one::EDProducer`: Serial execution

## Configuration System

### Python Configuration (config.py)
```python
import FWCore.ParameterSet.Config as cms

process = cms.Process("DEMO")

# Load services
process.load("FWCore.MessageService.MessageLogger_cfi")

# Input
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:input.root')
)

# Module configuration
process.myProducer = cms.EDProducer("MyProducer",
    input = cms.InputTag("source"),
    threshold = cms.double(5.0)
)

# Path
process.p = cms.Path(process.myProducer)

# Output
process.output = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('output.root'),
    outputCommands = process.RECOEventContent.outputCommands
)
process.outpath = cms.EndPath(process.output)
```

### cmsDriver.py (Configuration Builder)
Instead of writing configs manually:
```bash
cmsDriver.py step1 \
  --conditions auto:phase1_2024_realistic \
  --era Run3 \
  --step GEN,SIM,DIGI,L1,DIGI2RAW,HLT:@relval2024 \
  --eventcontent FEVTDEBUGHLT \
  -n 100
```

Generates complete Python config automatically!

## Key Technologies

### Languages and Frameworks
- **C++17:** Core implementation
- **Python 3:** Configuration and scripting
- **ROOT:** I/O, histograms, analysis framework
- **Intel TBB:** Threading and parallelism
- **pybind11:** Python-C++ bindings

### Libraries
- **Geant4:** Detector simulation
- **FastJet:** Jet clustering
- **Eigen:** Linear algebra
- **Boost:** C++ utilities
- **fmt:** String formatting
- **CLHEP:** HEP-specific utilities

### Hardware Acceleration
- **CUDA:** NVIDIA GPU support
- **Alpaka:** Portable acceleration (CPU/GPU)
- **Offloading:** Pixel tracking, ECAL, HCAL reconstruction

### Databases
- **Frontier:** Conditions delivery
- **SQLite:** Local conditions
- **Oracle:** Central conditions database

## Development Workflow

### Creating a New Module
```bash
# Generate skeleton
mkedanlzr MyAnalyzer  # or mkedproducer, mkedfltr

# Edit MyAnalyzer.cc
# Add to BuildFile.xml

# Build
scram b

# Refresh plugin cache
edmPluginRefresh

# Test
cmsRun test_cfg.py
```

### Testing
- **Unit tests:** FWCore/TestProcessor
- **Integration tests:** Configuration/PyReleaseValidation
- **runTheMatrix.py:** Full validation matrix (thousands of workflows)

## Important Concepts

### Products and Tokens
```cpp
// Declare in constructor (type-safe)
inputToken_ = consumes<std::vector<reco::Track>>(iConfig.getParameter<edm::InputTag>("tracks"));

// Use in produce()
auto tracks = iEvent.get(inputToken_);
```

### EventSetup (Conditions Data)
```cpp
// Get geometry
auto const& geom = iSetup.getData(geomToken_);

// Get magnetic field
auto const& bField = iSetup.getData(bFieldToken_);
```

### Eras and GlobalTags
- **Era:** Detector configuration era (Run2_2018, Run3, Phase2)
- **GlobalTag:** Database snapshot (alignment, calibrations)

### Process Modifiers
Conditional configuration changes:
- `gpu`: Enable GPU paths
- `alpaka`: Portable acceleration
- `trackingMkFit`: MkFit tracking

## Directory Structure Summary

```
cmssw/
├── FWCore/           # Framework (30 packages)
├── Configuration/    # Workflow coordination (17 packages)
├── DataFormats/      # Data definitions (123 packages)
├── Reco*/            # Reconstruction (34 packages)
├── Sim*/             # Simulation (19 packages)
├── L1Trigger/        # L1 trigger (57 packages)
├── HLTrigger/        # HLT trigger (14 packages)
├── Geometry/         # Detector geometry (58 packages)
├── Cond*/            # Conditions/calibrations (108 packages total)
├── Calib*/           # Calibration algorithms (77 packages total)
├── EventFilter/      # Raw data handling (34 packages)
├── IOPool/           # I/O (9 packages)
├── DQM/              # Data quality (94 packages total)
├── Validation/       # Validation (63 packages)
├── PhysicsTools/     # Analysis tools (36 packages)
├── HeterogeneousCore/ # GPU support (19 packages)
└── ...               # Many more!
```

**Total:** ~1000 packages!

## Getting Help

### Documentation
- TWiki: https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBook
- Framework Guide: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrameWork

### Tools
```bash
edmPluginHelp <PluginName>    # Plugin documentation
edmProvDump file.root          # Provenance
edmFileUtil file.root          # File contents
edmConfigDump file.root        # Configuration used
```

### Common Commands
```bash
cmsRun config.py               # Run CMSSW
cmsDriver.py ...               # Build configuration
runTheMatrix.py -l 11634.0     # Run validation workflow
scram b                        # Build CMSSW
edmPluginRefresh               # Refresh plugin cache
```

## Summary

CMSSW is a **comprehensive physics software framework**:

✅ **Complete:** Generation → Simulation → Reconstruction → Analysis
✅ **Scalable:** Petabytes of data, millions of CPU hours
✅ **Flexible:** Plugin architecture, Python configuration
✅ **Robust:** Extensive validation, quality monitoring
✅ **Modern:** Multi-threading, GPU support, portable acceleration
✅ **Evolving:** Active development for LHC Run 3 and HL-LHC

**Bottom line:** CMSSW enables world-class particle physics research at the LHC!

## Documentation Organization

This repository contains detailed documentation:

- **This file (CLAUDE.md):** High-level overview
- **[FWCore/CLAUDE.md](FWCore/CLAUDE.md):** Complete framework documentation
- **[Configuration/CLAUDE.md](Configuration/CLAUDE.md):** Workflow coordination details
- **[DataFormats/CLAUDE.md](DataFormats/CLAUDE.md):** All data products reference
- **[RECONSTRUCTION.md](RECONSTRUCTION.md):** Reconstruction packages overview
- **[SIMULATION.md](SIMULATION.md):** Simulation packages overview
- **Subpackage CLAUDE.md files:** Detailed documentation for individual packages

Navigate by following links or exploring subdirectories!
