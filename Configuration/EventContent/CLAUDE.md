# Configuration/EventContent

## Purpose
Defines data tiers - what products are saved in output ROOT files. Controls file size and content granularity.

## Main Data Tiers

### Production Data
- **RAW**: FED raw data, minimal processing (~1-2 MB/event)
- **RECO**: Full reconstruction objects (~500 KB/event)
- **AOD**: Analysis Object Data - reduced RECO (~150 KB/event)
- **MINIAOD**: Compact analysis format (~50 KB/event)
- **NANOAOD**: Flat ntuple format (~1-5 KB/event)

### Simulation Data
- **GEN**: Generator-level particles
- **SIM**: Geant4 simulation hits
- **RAWSIM**, **RECOSIM**, **AODSIM**: MC versions with GEN+SIM
- **MINIAODSIM**, **NANOAODSIM**: Analysis tiers with MC truth

### Combined Formats
- **FEVT**: RAW + RECO (Full EVenT)
- **FEVTDEBUG**: FEVT + additional debug info
- **FEVTHLTALL**: FEVT + all HLT debug objects

### Special Formats
- **DQM**: Data Quality Monitoring histograms
- **ALCARECO**: Alignment and calibration data
- **PREMIX**: Pre-mixed pileup samples

## Configuration Structure
```python
RECOEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring(
        'drop *',
        'keep recoTracks_generalTracks_*_*',
        'keep recoVertexs_offlinePrimaryVertices_*_*',
        # ... hundreds of keep statements
    )
)
```

## Usage
```python
process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = process.RECOEventContent.outputCommands,
    fileName = cms.untracked.string('output.root')
)
```

## Cross-References
- **[IOPool/Output](../../IOPool/CLAUDE.md)** - Output module implementation
- **[DataFormats](../../DataFormats/CLAUDE.md)** - Data products saved
- **[Configuration/StandardSequences](../StandardSequences/CLAUDE.md)** - Creates products
