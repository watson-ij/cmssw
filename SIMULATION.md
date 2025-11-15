# Simulation Packages (Sim*, IOMC, GeneratorInterface)

## Overview
Simulation packages generate Monte Carlo events and simulate detector response. This creates synthetic data for algorithm development, efficiency studies, and physics predictions.

## Simulation Chain

```
Event Generation (GeneratorInterface, IOMC)
  ↓
Particle Transport (SimG4Core, SimG4CMS)
  ↓
Detector Simulation (SimTracker, SimCalorimetry, SimMuon)
  ↓
Digitization (Sim*Digitizer)
  ↓
Pileup Mixing (Mixing)
  ↓
Reconstruction (Reco*)
```

## Event Generation

### **[GeneratorInterface](GeneratorInterface/)** ⭐
Interfaces to external event generators:
- **Pythia8Interface/**: Pythia 8 (default)
- **HerwigInterface/**: Herwig++ / Herwig 7
- **AlpgenInterface/**, **MadGraphInterface/**: Matrix element generators
- **GenFilters/**: Event filters (pT cuts, particle selections)
- **Output:** `reco::GenParticle` (HepMC format)

Generators:
- **Pythia 8**: QCD, EW, hadronization, underlying event
- **MadGraph/aMC@NLO**: Matrix element (NLO)
- **POWHEG**: NLO matching
- **Sherpa**: Multi-purpose generator

### **[IOMC](IOMC/)** - I/O for Monte Carlo
- **EventVertexGenerators/**: Primary vertex generation
- **ParticleGuns/**: Particle gun sources
- **Input/**: External generator file reading

### **[Configuration/Generator](Configuration/Generator/)**
Pre-configured generator fragments for standard processes

**Output:** GenParticles saved to SIM tier

## Geant4 Simulation

### **[SimG4Core](SimG4Core/)** ⭐ - Geant4 core
- **Application/**: Main Geant4 application
- **Geometry/**: Convert CMSSW → Geant4 geometry
- **Physics/**: Physics lists (FTFP_BERT, etc.)
- **Watcher/**: Simulation watchers/monitors
- **PrintGeomInfo/**: Geometry debugging

**Process:** Transports particles through detector, creates SimHits

### **[SimG4CMS](SimG4CMS/)** - CMS-specific Geant4 code
- **Calo/**: Calorimeter-specific simulation
- **Tracker/**: Tracker-specific simulation
- **Muon/**: Muon chamber simulation
- **Forward/**: Forward detector simulation (CASTOR, ZDC, etc.)
- Sensitive detector definitions

## Detector Simulation

### **[SimTracker](SimTracker/)** - Tracker simulation
- **SiPixelDigitizer/**: Pixel digitization
- **SiStripDigitizer/**: Strip digitization
- **TrackerHitAssociation/**: Truth matching
- **Phase2TrackerDigitizer/**: Phase-2 tracker (IT/OT)

**Flow:** SimHits (energy deposits) → Digis (ADC counts)

### **[SimCalorimetry](SimCalorimetry/)** - Calorimeter simulation
- **EcalSimProducers/**: ECAL simulation
- **HcalSimProducers/**: HCAL simulation
- **CaloSimAlgos/**: Common algorithms (noise, pileup)
- **HGCalSimProducers/**: HGCal simulation (Phase-2)
- **EcalEBTrigPrimProducers/**, **HcalTrigPrimProducers/**: Trigger primitives

**Effects modeled:** Energy deposition, APD/photodetector response, electronics, noise

### **[SimMuon](SimMuon/)** - Muon system simulation
- **DTDigitizer/**: Drift Tube digitization
- **CSCDigitizer/**: CSC digitization
- **RPCDigitizer/**: RPC digitization
- **GEMDigitizer/**: GEM digitization

**Effects:** Drift time, efficiency, noise, cross-talk

### **[SimGeneral](SimGeneral/)** - General simulation utilities
- **MixingModule/**: Pileup mixing (overlaying events)
- **TrackingAnalysis/**: Tracking truth
- **HepPDTRecord/**: Particle data table
- **DataMixingModule/**: Data-driven pileup

### **[SimFastTiming](SimFastTiming/)**, **[SimPPS](SimPPS/)** - Specialized detectors
- MTD timing layer (Phase-2)
- Precision Proton Spectrometer

## Pileup Simulation

### **[Mixing](Mixing/)** ⭐
Overlays multiple events to simulate pileup:
- **In-time pileup:** Same bunch crossing
- **Out-of-time pileup:** Adjacent bunch crossings (before/after)
- **Pre-mixing:** Pre-generated pileup (faster)

**Configuration:** `--pileup` option in cmsDriver
- Poisson mean (e.g., `--pileup_input file:pileup.root`)

## Data Formats

### **[SimDataFormats](SimDataFormats/)** - Simulation data products
- **SimHits:** Energy deposits in sensitive volumes
  - `PSimHit`: Parametrized sim hit (tracker, muon)
  - `PCaloHit`: Calorimeter hit
- **SimTracks/SimVertices:** Generator-level particles
- **SimTrackContainer/SimVertexContainer**
- **TrackingHit:** Tracker hit truth
- **CaloHit:** Calorimeter hit truth
- **Associations:** Digi ↔ SimHit, Track ↔ SimTrack

**Saved in:** GEN-SIM tier

### **[FastSimDataFormats](FastSimDataFormats/)** - Fast simulation
- Parametrized detector response (no Geant4)

## Fast Simulation

### **[FastSimulation](FastSimulation/)**
Parametrized simulation (much faster than Geant4):
- **Tracking/**: Fast tracker simulation
- **Calorimetry/**: Fast calorimeter simulation
- **ParticlePropagator/**: Simplified propagation
- **MaterialEffects/**: Energy loss, multiple scattering

**Use case:** Large MC samples where speed > accuracy

**Configuration:** `--fast` option or special FastSim workflows

## Mixing Scenarios

### Standard Mixing
Overlays minimum-bias events on signal:
```python
process.mix = cms.EDProducer("MixingModule",
    input = cms.SecSource("EmbeddedRootSource",
        fileNames = cms.untracked.vstring('file:minbias.root')
    ),
    bunchspace = cms.int32(25),  # 25 ns
    minBunch = cms.int32(-12),    # -12 to +3 BX
    maxBunch = cms.int32(3),
)
```

### Pre-Mixing
Two-stage approach (faster):
1. **Stage 1:** Simulate pileup once
2. **Stage 2:** Mix with signal

**Configuration:** `--procModifiers premix_stage1` or `premix_stage2`

## Detector Description for Simulation

### **[DetectorDescription](DetectorDescription/)** ⭐
Geometry description system:
- **DDCMS/**: DD4hep-based geometry (modern)
- **Core/**, **OnlineDB/**: Legacy DDD system
- XML files describing detector volumes, materials

**Used by:** SimG4Core to build Geant4 geometry

### **[Geometry](Geometry/)** packages
Detector geometry definitions:
- Tracker, ECAL, HCAL, Muon subsystem geometries
- Phase-2 upgrade geometries (HGCal, MTD, etc.)
- Used by both simulation and reconstruction

## Generator-Level Objects

### GenParticles (`reco::GenParticle`)
HepMC particles from generator:
- Status codes (1=final state, 2=decayed, etc.)
- Mother-daughter relationships
- 4-momentum, PDG ID

### GenJets
Jets from generator particles (before detector):
- Used for matching reco jets to truth

## Truth Matching

### TrackingParticle
Generator particle that leaves tracker hits:
```
reco::GenParticle → SimTrack → SimHits → TrackingParticle
```
Used for tracking efficiency studies

### Associations
- **SimHitAssociation:** Digi → SimHit
- **TrackAssociation:** reco::Track → TrackingParticle
- **JetPartonMatch:** Reco jet → Gen jet/parton

## Validation

Track/jet/object efficiency = (Reco matched to Gen) / (Gen in acceptance)

## Standard Simulation Workflows

### Full Simulation (GEN-SIM-DIGI-HLT-RECO)
```bash
# Step 1: GEN-SIM
cmsDriver.py TTbar_14TeV_cfi --step GEN,SIM \
  --conditions auto:phase1_2024_realistic \
  --era Run3

# Step 2: DIGI-L1-DIGI2RAW-HLT
cmsDriver.py step2 --step DIGI,L1,DIGI2RAW,HLT \
  --pileup_input file:minbias.root \
  --pileup Run3_Flat55To75_PoissonOOTPU

# Step 3: RAW2DIGI-RECO
cmsDriver.py step3 --step RAW2DIGI,L1Reco,RECO
```

### Fast Simulation
```bash
cmsDriver.py TTbar_14TeV_cfi --step GEN,SIM,DIGI,L1,RECO \
  --conditions auto:phase1_2024_realistic --fast
```
~10x faster, less accurate

## Pileup Scenarios

Run 3 pileup (collisions per bunch crossing):
- **Run3_Flat55To75:** Flat distribution, 55-75 pileup
- **Run3_Realistic:** Data-driven distribution

Phase-2 HL-LHC:
- **Phase2_Realistic:** 140-200 pileup

## Key Packages Summary

| Package | Purpose |
|---------|---------|
| **GeneratorInterface** | Event generation (Pythia, etc.) |
| **SimG4Core** | Geant4 engine |
| **SimG4CMS** | CMS-specific Geant4 code |
| **SimTracker** | Tracker simulation/digitization |
| **SimCalorimetry** | Calorimeter simulation/digitization |
| **SimMuon** | Muon simulation/digitization |
| **SimGeneral** | Utilities, mixing, truth |
| **Mixing** | Pileup overlay |
| **SimDataFormats** | SimHits, SimTracks, truth data |
| **FastSimulation** | Parametrized simulation |

## Performance

- **Full sim:** ~5-30 seconds/event (depends on pileup, particles)
- **Fast sim:** ~0.5-2 seconds/event
- **Bottleneck:** Geant4 tracking (especially with pileup)
- **CPU usage:** Simulation is most CPU-intensive step

## Cross-References

- **[GeneratorInterface](GeneratorInterface/)** - Event generators
- **[Configuration/Generator](Configuration/Generator/)** - Generator configs
- **[DataFormats](DataFormats/CLAUDE.md)** - SimHits, GenParticles
- **[Geometry](Geometry/)** - Detector description
- **[Reco*](RECONSTRUCTION.md)** - Uses simulation output
- **[Validation](Validation/)** - Validates simulation
- **[Configuration/StandardSequences](Configuration/StandardSequences/CLAUDE.md)** - SIM, DIGI sequences

## Use Cases

1. **Physics studies:** Predict signals, backgrounds
2. **Detector design:** Optimize detector for Phase-2
3. **Algorithm development:** Develop reconstruction algorithms
4. **Efficiency studies:** Measure trigger, reconstruction efficiency
5. **Systematic uncertainties:** Generator, pileup, detector modeling
