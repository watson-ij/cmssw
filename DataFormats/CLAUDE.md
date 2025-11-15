# DataFormats Package

## Overview
DataFormats contains C++ class definitions for all data products stored in CMSSW Events, Runs, and LuminosityBlocks. These are the "nouns" of CMSSW - what data looks like.

**Total Subpackages:** 123 (organized by detector subsystem and physics object type)

**Key Concept:** Data products are stored in ROOT files and accessed via the Framework using tokens.

## Core Infrastructure

### **Common** ⭐
Fundamental data structures used everywhere:
- `edm::Wrapper<T>` - Wraps products for ROOT storage
- `edm::Ref<C>`, `edm::RefVector<C>` - References between products
- `edm::AssociationMap` - Associations between collections
- `edm::View<T>` - Abstract collection interface
- `edm::Handle<T>` - Smart pointer for product access
- `edm::OrphanHandle<T>` - Handle without parentage

**Used by:** All packages

### **Provenance** ⭐
Data provenance and metadata:
- `EventID`, `RunID`, `LuminosityBlockID` - Event identification
- `ProductID` - Unique product identifier
- `ModuleDescription` - Module metadata
- `ProcessHistory` - Processing history
- `Parentage` - Product ancestry

**Used by:** Framework, all I/O

### **FWLite**
FWLite-specific data access classes

### **Streamer**
Data streaming for DAQ and storage

## Detector Raw Data

### **FEDRawData** ⭐
Raw detector data from Front-End Drivers (FEDs)
- `FEDRawData` - Single FED buffer
- `FEDRawDataCollection` - All FEDs in event

**Created by:** DAQ system
**Used by:** All Raw2Digi unpacking modules in EventFilter/

### **CTPPS**
- CTPPSDetId, CTPPSDigi, CTPPSReco - Precision Proton Spectrometer

**Used by:** RecoCTPPS/, Validation/CTPPS/

## Detector Identifiers

### **DetId** ⭐
Base detector identifier system
- `DetId` - Universal detector element ID (32-bit)
- Subdetector enums (Tracker, Muon, Ecal, Hcal, etc.)

**Used by:** All detector packages

### Tracker IDs
- **SiPixelDetId** - Pixel detector IDs
- **SiStripDetId** - Strip detector IDs
- **TrackerTopology** - Tracker geometry queries

**Used by:** RecoLocalTracker/, RecoTracker/

### Calorimeter IDs
- **EcalDetId** - ECAL crystal/tower IDs (EB, EE, ES)
- **HcalDetId** - HCAL tower IDs
- **CaloTowerDetId** - Combined calo tower IDs

**Used by:** RecoLocalCalo/, RecoEcal/, CaloOnlineTools/

### Muon IDs
- **MuonDetId** - Muon system base
- **DTChamberId**, **DTLayerId**, **DTWireId** - Drift Tubes
- **CSCDetId** - Cathode Strip Chambers
- **RPCDetId** - Resistive Plate Chambers
- **GEMDetId** - Gas Electron Multiplier

**Used by:** RecoLocalMuon/, RecoMuon/

### Forward IDs
- **ForwardDetId** - Forward detectors (HF, CASTOR, ZDC)
- **HGCalDetId** - High Granularity Calorimeter (Phase-2)
- **HGCScintillatorDetId**, **HGCSiliconDetId**

**Used by:** RecoLocalCalo/HGCal, SimCalorimetry/HGCal

## Detector Digis (Digitized Signals)

### Tracker Digis
- **SiPixelDigi** - Pixel hits
- **SiStripDigi** - Strip hits

**Created by:** SimTracker/ (MC), EventFilter/SiPixelRawToDigi/ (data)
**Used by:** RecoLocalTracker/

### Calorimeter Digis
- **EcalDigi** - ECAL ADC samples
- **HBHEDataFrame**, **HODataFrame**, **HFDataFrame** - HCAL digis
- **HGCDataFrame** - HGCal digis (Phase-2)
- **FTLDigi** - MTD/Timing Layer digis (Phase-2)

**Created by:** SimCalorimetry/ (MC), EventFilter/EcalRawToDigi/ (data)
**Used by:** RecoLocalCalo/

### Muon Digis
- **CSCDigi** - CSC strip/wire digis
- **DTDigi** - DT digis
- **RPCDigi** - RPC digis
- **GEMDigi** - GEM digis

**Created by:** SimMuon/ (MC), EventFilter/DTRawToDigi/ (data)
**Used by:** RecoLocalMuon/

## Detector RecHits (Reconstructed Hits)

### Tracker RecHits
- **SiPixelRecHit** - Pixel clusters with positions
- **SiStripRecHit2D** - Strip hits
- **SiPixelCluster**, **SiStripCluster** - Clusters
- **Phase2TrackerRecHit1D** - Phase-2 tracker hits

**Created by:** RecoLocalTracker/
**Used by:** RecoTracker/ (tracking)

### Calorimeter RecHits
- **EcalRecHit** - ECAL energy deposits (EB, EE, ES)
- **HBHERecHit**, **HORecHit**, **HFRecHit** - HCAL rechits
- **CaloRecHit** - Base class
- **HGCRecHit** - HGCal rechits (Phase-2)
- **FTLRecHit** - MTD rechits (Phase-2)

**Created by:** RecoLocalCalo/
**Used by:** RecoEcal/, RecoEgamma/ (clustering, calibration)

### Muon RecHits
- **DTRecHit1D**, **DTRecSegment2D**, **DTRecSegment4D** - DT segments
- **CSCRecHit2D**, **CSCSegment** - CSC segments
- **RPCRecHit** - RPC hits
- **GEMRecHit** - GEM rechits

**Created by:** RecoLocalMuon/
**Used by:** RecoMuon/ (muon reconstruction)

### Combined
- **CaloTowers** - Combined ECAL+HCAL towers

**Created by:** RecoLocalCalo/CaloTowersCreator/
**Used by:** RecoJets/, RecoMET/

## Tracking Objects

### **TrackReco** ⭐
Track and vertex data structures:
- `reco::Track` - Reconstructed track (χ², hits, parameters)
- `reco::TrackExtra` - Extended track info (hit patterns)
- `reco::Vertex` - Vertex (position, tracks, χ²)
- `reco::TrackBase` - Common track interface

**Created by:** RecoTracker/, RecoVertex/
**Used by:** All reconstruction (jets, MET, taus, electrons, muons)

### **TrackingRecHit**
Generic tracking hit interface
- `TrackingRecHit` - Base class for all tracker hits

**Used by:** RecoTracker/

### **VertexReco**
Vertex-specific objects:
- `reco::Vertex` - Primary/secondary vertices

**Created by:** RecoVertex/
**Used by:** RecoJets/ (JVF), RecoBTag/ (SV tagging)

### **BeamSpot**
Beam spot position and width:
- `reco::BeamSpot`

**Created by:** RecoVertex/BeamSpotProducer/
**Used by:** Tracking, trigger

## Particle Flow Objects

### **ParticleFlowReco**
Particle Flow elements:
- `reco::PFCluster` - Calorimeter clusters for PF
- `reco::PFRecHit` - Rechits for PF
- `reco::PFBlock` - Connected PF elements
- `reco::PFBlockElement` - Tracks, clusters, etc.

**Created by:** RecoParticleFlow/
**Used by:** RecoParticleFlow/PFProducer/

### **ParticleFlowCandidate** ⭐
Reconstructed PF particles:
- `reco::PFCandidate` - PF particle (e, μ, γ, h⁰, h±)
- `reco::PFJet`, `reco::PFMETCollection` - PF jets/MET

**Created by:** RecoParticleFlow/PFProducer/
**Used by:** Most analyses (standard analysis objects)

## Jet Objects

### **JetReco** ⭐
Jet data structures:
- `reco::CaloJet` - Calorimeter-only jets
- `reco::PFJet` - Particle Flow jets (default)
- `reco::GenJet` - Generator-level jets
- `reco::TrackJet` - Track-only jets
- `reco::BasicJet` - Base jet class
- `reco::Jet` - Generic jet interface

**Created by:** RecoJets/
**Used by:** Analyses, b-tagging, JetMETCorrections/

### **BTauReco**
B-tagging discriminators:
- `reco::JetTag` - B-tag discriminator values
- `reco::TrackIPTagInfo`, `reco::SecondaryVertexTagInfo` - Tag info

**Created by:** RecoBTag/
**Used by:** Analyses

## MET Objects

### **METReco** ⭐
Missing transverse energy:
- `reco::MET` - Base MET
- `reco::CaloMET` - Calorimeter MET
- `reco::PFMET` - Particle Flow MET (default)
- `reco::GenMET` - Generator-level MET

**Created by:** RecoMET/
**Used by:** Analyses, trigger

## Muon Objects

### **MuonReco** ⭐
Reconstructed muons:
- `reco::Muon` - Combined muon (tracker + muon system)
- `reco::MuonTrackLinks` - Links between tracks
- `reco::MuonSegmentMatch` - Muon-segment matching

**Created by:** RecoMuon/
**Used by:** Analyses, trigger

## Electron and Photon Objects

### **EgammaCandidates** ⭐
Electron and photon candidates:
- `reco::GsfElectron` - GSF-fitted electron
- `reco::Photon` - Photon
- `reco::ConvertedPhoton` - Converted photons

**Created by:** RecoEgamma/
**Used by:** Analyses, trigger

### **EgammaReco**
Intermediate egamma objects:
- `reco::SuperCluster` - ECAL superclusters
- `reco::BasicCluster`, `reco::Cluster` - ECAL clusters
- `reco::ElectronSeed` - Electron seeding
- `reco::PhotonCore` - Photon core

**Created by:** RecoEcal/, RecoEgamma/
**Used by:** RecoEgamma/

### **GsfTrackReco**
Gaussian Sum Filter tracks for electrons:
- `reco::GsfTrack` - GSF track (handles bremsstrahlung)

**Created by:** RecoEgamma/
**Used by:** RecoEgamma/EgammaElectronProducers/

## Tau Objects

### **TauReco** ⭐
Hadronic tau reconstruction:
- `reco::PFTau` - Particle Flow tau
- `reco::PFTauDiscriminator` - Tau ID discriminators

**Created by:** RecoTauTag/
**Used by:** Analyses

## Candidate System

### **Candidate** ⭐
Generic particle interface:
- `reco::Candidate` - Abstract particle (p4, charge, vertex)
- `reco::CompositeCandidate` - Composite particles
- `reco::LeafCandidate` - Leaf (no daughters)

**Used by:** All physics objects inherit from Candidate

## Generator-Level Objects

### **HepMCCandidate**
HepMC event record interface:
- `reco::GenParticle` - Generator particle (from Pythia, etc.)
- `reco::GenParticleCollection`

**Created by:** GeneratorInterface/
**Used by:** Analyses (MC truth)

### **JetMatching**
MC-reco jet matching

## Simulation Objects

### **SimDataFormats**
Simulation hits and truth:
- **SimVertex**, **SimTrack** - Generator vertices/tracks
- **TrackingHit** - Sim tracking hits
- **CaloHit** - Sim calorimeter hits
- **PSimHit** - Parametrized sim hits

**Created by:** SimG4Core/, SimTracker/, SimCalorimetry/
**Used by:** Digitization, validation

## Trigger Objects

### **L1Trigger**
L1 trigger data formats (see L1Trigger/ packages)

### **HLTReco**
HLT trigger objects:
- `trigger::TriggerObject` - HLT reconstructed object
- `trigger::TriggerEvent` - HLT event summary

**Created by:** HLTrigger/
**Used by:** Trigger analyses

### **L1GlobalTrigger**
L1 Global Trigger decisions

## Heavy Flavor

### **HeavyIonEvent**
Heavy-ion event characterization:
- Centrality, event plane, spectators

**Used by:** RecoHI/

## Special Formats

### **PatCandidates** (in PhysicsTools/)
Physics Analysis Toolkit objects - not in DataFormats/ but important:
- `pat::Electron`, `pat::Muon`, `pat::Jet`, etc.
- Extended versions with embedded info

### **NanoAOD**
Flat table format:
- `nanoaod::FlatTable` - Column-based storage

**Created by:** PhysicsTools/NanoAOD/
**Used by:** Analysis frameworks

## Organization by Physics Domain

### Tracking Chain
```
SiPixelDigi, SiStripDigi (DataFormats/*Digi)
  ↓ RecoLocalTracker
SiPixelCluster, SiStripCluster (*Cluster)
  ↓ RecoLocalTracker
SiPixelRecHit, SiStripRecHit2D (*RecHit)
  ↓ RecoTracker
TrackReco::Track
  ↓ RecoVertex
VertexReco::Vertex
```

### ECAL Chain
```
EcalDigi (EcalDigi/)
  ↓ RecoLocalCalo/EcalRecProducers
EcalRecHit (EcalRecHit/)
  ↓ RecoEcal/EgammaClusterProducers
EgammaReco::SuperCluster
  ↓ RecoEgamma
EgammaCandidates::GsfElectron, Photon
```

### Jet Chain
```
CaloTowers (CaloTowers/)
  ↓ OR
ParticleFlowCandidate (ParticleFlowCandidate/)
  ↓ RecoJets
JetReco::PFJet
  ↓ RecoBTag
BTauReco::JetTag
```

### Muon Chain
```
CSCDigi, DTDigi, RPCDigi (*Digi)
  ↓ RecoLocalMuon
CSCRecHit2D, DTRecSegment4D (*RecHit)
  ↓ RecoMuon
MuonReco::Muon
```

## Cross-Package Usage Map

| DataFormat | Created By | Primary Users |
|------------|-----------|---------------|
| **Track** | RecoTracker | Jets, MET, Taus, E/γ, μ |
| **Vertex** | RecoVertex | B-tagging, Jets, Taus |
| **PFCandidate** | RecoParticleFlow | Jets, MET, Taus |
| **GsfElectron** | RecoEgamma | Analyses, trigger |
| **Muon** | RecoMuon | Analyses, trigger |
| **PFJet** | RecoJets | Analyses, trigger |
| **PFMET** | RecoMET | Analyses, trigger |
| **SuperCluster** | RecoEcal | RecoEgamma |
| **BeamSpot** | RecoVertex | Tracking, trigger |

## ROOT Persistency

All DataFormats classes are:
- ROOT-serializable (have dictionaries)
- Wrapped in `edm::Wrapper<T>` for storage
- Accessed via `edm::Handle<T>` in Framework

## Key Design Patterns

1. **Collections**: Most objects stored as `std::vector<T>`
2. **References**: `edm::Ref<C>` for inter-product references
3. **Associations**: `edm::AssociationMap` for 1-to-N mappings
4. **Inheritance**: Many inherit from `reco::Candidate`
5. **Const-correctness**: Products are const after creation

## Summary

DataFormats defines the **language** of CMSSW data:

- ✅ **123 subpackages** organized by detector/physics
- ✅ **Comprehensive coverage** from raw data → physics objects
- ✅ **ROOT integration** for persistence
- ✅ **Reference system** for inter-product links
- ✅ **Type-safe** access via Framework

**Key insight:** Understanding DataFormats is essential for writing CMSSW modules - you need to know what data exists and how to access it!

## Cross-References

- **[FWCore/Framework](../FWCore/CLAUDE.md)** - Accesses data products
- **RecoLocalTracker**, **RecoTracker/** - Create tracking objects
- **RecoEcal/**, **RecoEgamma/** - Create e/γ objects
- **RecoMuon/** - Create muon objects
- **RecoJets/**, **RecoMET/** - Create jet/MET objects
- **RecoParticleFlow/** - Create PF objects
- **Sim*** - Create simulation objects
- **EventFilter/** - Raw2Digi unpacking
