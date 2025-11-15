# Configuration/Skimming

## Purpose
Physics-group event skimming configurations. Reduces data volume by selecting events of interest.

## Skim Categories

### B-Physics (BPH)
- J/ψ → μμ
- Υ → μμ
- B → J/ψ K
- B_s → J/ψ φ

### Exotics (EXO)
- Displaced jets
- Long-lived particles
- High MET
- Displaced vertices

### Electroweak
- **ZEE**: Z → e⁺e⁻
- **ZMM**: Z → μ⁺μ⁻
- **DiPhoton**: γγ events
- **DiElectron**: e⁺e⁻ events

### QCD
- **DiJet**: Jet pairs
- **MultiJet**: Multi-jet events

### Combined
- **EGM**: E-gamma + JetMET selections

## Skim Structure
```python
skimPath = cms.Path(
    selectionSequence +  # Event selection
    cms.EDFilter("EventCountProducer")
)

SKIMStreamMyPhysics = cms.FilteredStream(
    responsible = 'Physics Group',
    name = 'MyPhysics',
    paths = (skimPath,),
    content = MYPHYSICSEventContent.outputCommands,
    dataTier = cms.untracked.string('USER')
)
```

## Usage
Applied in central skimming or user analysis:
```python
process.load('Configuration.Skimming.PDWG_EXODisplacedJet_cff')
```

## Cross-References
- **[Configuration/EventContent](../EventContent/CLAUDE.md)** - Output content
- **Physics groups** - Define skim criteria
