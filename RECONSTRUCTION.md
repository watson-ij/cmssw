# Reconstruction Packages (Reco*)

## Overview
The Reco* packages implement algorithms to reconstruct physics objects from detector signals. This is the core of event processing - transforming raw detector data into tracks, vertices, electrons, muons, jets, MET, etc.

## Reconstruction Chain

### Local Reconstruction (Detector-Specific)

**[RecoLocalTracker](RecoLocalTracker/)** - Tracker local reconstruction
- **SiPixelRecHits/**: Pixel cluster → position
- **SiStripRecHitConverter/**: Strip cluster → position
- Hit calibration and positioning
- **Output:** `SiPixelRecHit`, `SiStripRecHit2D`

**[RecoLocalCalo](RecoLocalCalo/)** - Calorimeter local reconstruction
- **EcalRecProducers/**: ECAL digi → rechit (energy, time)
- **HcalRecProducers/**: HCAL digi → rechit
- **CaloTowersCreator/**: Combined ECAL+HCAL towers
- **Output:** `EcalRecHit`, `HBHERecHit`, `CaloTower`

**[RecoLocalMuon](RecoLocalMuon/)** - Muon chamber local reconstruction
- **DTSegment/**: DT segment reconstruction
- **CSCSegment/**: CSC segment reconstruction
- **RPCRecHit/**, **GEMRecHit/**: RPC/GEM hits
- **Output:** `DTRecSegment4D`, `CSCSegment`, `RPCRecHit`

### Global Reconstruction

**[RecoTracker](RecoTracker/)** ⭐ - Track reconstruction
- **IterativeTracking/**: Multi-pass tracking (10+ iterations)
- **FinalTrackSelectors/**: Track quality and deduplication
- **MkFit/**: Parallel track building (optional)
- **ConversionSeedFinders/**: Photon conversion tracking
- **Output:** `reco::Track` collections (generalTracks, etc.)

**[RecoVertex](RecoVertex/)** ⭐ - Vertex reconstruction
- **PrimaryVertexProducer/**: Primary vertex finding
- **ConfigurableVertexReco/**: Adaptive vertex fitter
- **BeamSpotProducer/**: Beam spot determination
- **Output:** `reco::Vertex`, `reco::BeamSpot`

**[RecoEcal](RecoEcal/)** - ECAL clustering
- **EgammaClusterProducers/**: SuperCluster building for e/γ
- **EgammaCoreTools/**: Cluster shape, position refinement
- **Output:** `reco::SuperCluster`, `reco::BasicCluster`

**[RecoMuon](RecoMuon/)** ⭐ - Muon reconstruction
- **StandAloneMuonProducer/**: Muon-only tracks
- **GlobalMuonProducer/**: Combined tracker+muon tracks
- **MuonIdentification/**: Muon ID, isolation
- **Output:** `reco::Muon`

**[RecoEgamma](RecoEgamma/)** ⭐ - Electron and photon reconstruction
- **EgammaElectronProducers/**: GSF electron fitting
- **PhotonProducers/**: Photon reconstruction
- **EgammaIsolationAlgos/**: Isolation variables
- **Output:** `reco::GsfElectron`, `reco::Photon`

### Particle Flow

**[RecoParticleFlow](RecoParticleFlow/)** ⭐ - Particle Flow reconstruction
- **PFProducer/**: Main PF algorithm
  - Links tracks, ECAL, HCAL, muon chambers
  - Identifies e, μ, γ, h⁰, h± particles
- **PFTracking/**: Track-ECAL linking
- **PFClusterProducer/**: PF-specific clustering
- **Output:** `reco::PFCandidate` (basis for jets, MET, taus)

**Usage:** Most analyses use PF-based objects (PFJets, PFMET, PFTaus)

### High-Level Objects

**[RecoJets](RecoJets/)** ⭐ - Jet reconstruction
- **JetProducers/**: Anti-kT, Cambridge-Aachen, kT algorithms
- **JetAlgorithms/**: FastJet interface
- Jet types: CaloJet, PFJet, GenJet, TrackJet
- **Output:** `reco::PFJet`, `reco::CaloJet`

**[RecoMET](RecoMET/)** ⭐ - Missing ET reconstruction
- **METProducers/**: CaloMET, PFMET, GenMET
- **Type-1 corrections:** Propagate jet corrections to MET
- **Output:** `reco::PFMET`, `reco::CaloMET`

**[RecoTauTag](RecoTauTag/)** ⭐ - Hadronic tau reconstruction
- **RecoTau/**: HPS tau algorithm (Hadron-Plus-Strips)
- **ImpactParameter/**: Tau IP calculations
- Tau ID: decay mode, isolation, MVA discriminators
- **Output:** `reco::PFTau` + discriminators

**[RecoBTag](RecoBTag/)** ⭐ - B-tagging
- **ImpactParameter/**: Track IP-based tagging
- **SecondaryVertex/**: SV-based tagging
- **Combined/**: Combined tagger (DeepJet, DeepCSV)
- **SoftLepton/**: Soft lepton taggers
- **Output:** `reco::JetTag` (b-tag discriminators)

### Specialized Reconstruction

**[RecoHI](RecoHI/)** - Heavy-ion specific reconstruction
- Centrality, event plane, jet background subtraction

**[RecoPPS](RecoPPS/)** - Precision Proton Spectrometer
- Proton reconstruction in forward detectors

**[RecoLuminosity](RecoLuminosity/)** - Luminosity measurements

**[RecoHGCal](RecoHGCal/)** - HGCal reconstruction (Phase-2)
- High-granularity calorimeter clustering

**[RecoMTD](RecoMTD/)** - MTD/timing layer reconstruction (Phase-2)

**[RecoCaloTools](RecoCaloTools/)** - Calorimetry utilities

**[RecoBTau](RecoBTau/)** - Legacy b-tau reconstruction

**[RecoTBCalo](RecoTBCalo/)** - Test beam calorimetry

## Standard Reconstruction Sequence

From **Configuration/StandardSequences/Reconstruction_cff.py**:

```
reconstruction = cms.Sequence(
    localreco +       # Local: RecHits
    globalreco +      # Tracking, vertices
    highlevelreco     # PF, jets, MET, e/γ, μ, τ, b-tag
)
```

### localreco
```
Tracker RecHits (RecoLocalTracker)
+ ECAL RecHits (RecoLocalCalo/Ecal)
+ HCAL RecHits (RecoLocalCalo/Hcal)
+ CaloTowers (RecoLocalCalo)
+ Muon RecHits/Segments (RecoLocalMuon)
```

### globalreco
```
Iterative Tracking (RecoTracker)
+ Primary Vertices (RecoVertex)
+ SuperClusters (RecoEcal)
+ Standalone/Global Muons (RecoMuon)
```

### highlevelreco
```
Particle Flow (RecoParticleFlow)
+ Jets (RecoJets from PFCandidates)
+ MET (RecoMET from PFCandidates)
+ Electrons (RecoEgamma with PF)
+ Photons (RecoEgamma)
+ Muons (RecoMuon refinement)
+ Taus (RecoTauTag)
+ B-tagging (RecoBTag)
```

## Key Algorithms

### Iterative Tracking (RecoTracker)
Multi-pass strategy: easy tracks first, then difficult:
1. **InitialStep**: High-pT, prompt tracks
2. **LowPtTripletStep**: Low-pT triplets
3. **PixelPairStep**: Pixel pairs (conversions)
4. **DetachedTripletStep**: Displaced tracks (b/tau)
5. **MixedTripletStep**: Mixed pixel-strip
6. **PixelLessStep**: Strip-only
7-10. More specialized iterations

**Output:** `generalTracks` collection

### Particle Flow (RecoParticleFlow)
1. **Tracking** → charged particles
2. **ECAL clustering** → photons + electron ECAL energy
3. **HCAL clustering** → neutral hadrons
4. **Muon chambers** → muons
5. **Linking algorithm** → assigns energy to particles
6. **Calibration** → energy corrections

**Output:** `particleFlow` collection (e, μ, γ, h⁰, h±)

### HPS Tau Algorithm (RecoTauTag)
**Hadron-Plus-Strips (HPS):**
- Identifies π⁰ → γγ via ECAL strips
- Reconstructs decay modes: 1-prong, 1-prong+π⁰, 3-prong
- Isolation: tracks and photons in cone
- MVA discriminators vs. jets, electrons, muons

### B-Tagging (RecoBTag)
Multiple algorithms combined:
- **IP-based**: Track impact parameter significance
- **SV-based**: Secondary vertex reconstruction
- **DeepJet**: Deep neural network (default for Run 3)
- **DeepCSV**: Alternative DNN
- **Soft leptons**: Muons/electrons from b→c→l

## Data Flow

```
RAW (FEDRawData)
  ↓ EventFilter (Raw2Digi)
Digis (SiPixelDigi, EcalDigi, etc.)
  ↓ RecoLocal* (Local Reco)
RecHits (SiPixelRecHit, EcalRecHit, etc.)
  ↓ RecoTracker, RecoEcal, RecoMuon (Global Reco)
Tracks, SuperClusters, Muon Tracks
  ↓ RecoParticleFlow (PF)
PFCandidates
  ↓ RecoJets, RecoMET, RecoEgamma, RecoTauTag
Jets, MET, Electrons, Photons, Taus
  ↓ RecoBTag
B-tag discriminators
```

## Performance Considerations

- **CPU-intensive:** Tracking and PF are slowest
- **Iterative tracking:** ~50-70% of RECO time
- **Particle Flow:** ~20-30% of RECO time
- **GPU offloading:** Pixel tracking, ECAL, HCAL (via alpaka)
- **Threading:** All modules designed for stream parallelism

## Cross-References

- **[DataFormats](DataFormats/CLAUDE.md)** - Data products created
- **[Configuration/StandardSequences](Configuration/StandardSequences/CLAUDE.md)** - Reconstruction sequences
- **[FWCore/Framework](FWCore/CLAUDE.md)** - Framework for running modules
- **[Calibration](CalibTracker/)**, **[CalibCalorimetry](CalibCalorimetry/)** - Calibrations used
- **[JetMETCorrections](JetMETCorrections/)** - Jet energy corrections
- **[PhysicsTools](PhysicsTools/)** - Analysis-level tools

## Key Outputs for Analyses

Standard objects for physics analyses:
- **Tracks:** `generalTracks`
- **Vertices:** `offlinePrimaryVertices`
- **Muons:** `muons`
- **Electrons:** `gedGsfElectrons`
- **Photons:** `gedPhotons`
- **Jets:** `ak4PFJets`, `ak8PFJets`
- **MET:** `pfMet`
- **Taus:** `hpsPFTauProducer`
- **B-tags:** `pfDeepJetTagInfos` (jet discriminators)

These are the inputs to PAT (PhysicsTools/PatAlgos) for analysis-level objects.
