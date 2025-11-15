# Configuration/Generator

## Purpose
Event generator configurations for Monte Carlo production. Hundreds of pre-configured physics processes.

## Generator Categories

### Standard Model
- **QCD**: Multi-jet, di-jet (`QCD_Pt_*_TuneCP5_*.py`)
- **Electroweak**: W, Z, photon production
- **Top**: ttbar, single top (`TTbar_*.py`)
- **Higgs**: Various production modes (ggH, VBF, VH, ttH)

### Beyond Standard Model
- **SUSY**: Supersymmetric particles
- **Exotica**: Long-lived particles, extra dimensions, etc.

### Heavy Flavor
- **B-physics**: Specific B-meson decays (`BsToJpsiPhi_*.py`, etc.)
- **Charmonium**: J/ψ, ψ(2S)
- **Bottomonium**: Υ states

### Heavy Ions
- **HYDJET**: Heavy-ion event generator
- **AMPT**, **Pyquen**: Alternative HI generators
- **Starlight**: Ultra-peripheral collisions

## Generator Tools
- **Pythia 8**: Default hadronization and UE
- **Herwig 7**: Alternative generator
- **MadGraph/aMC@NLO**: Matrix element generator
- **Sherpa**: Multi-purpose generator
- **POWHEG**: NLO generator

## Configuration Structure
```python
generator = cms.EDFilter("Pythia8GeneratorFilter",
    PythiaParameters = cms.PSet(
        processParameters = cms.vstring(
            'Top:gg2ttbar = on',
            'Top:qqbar2ttbar = on',
        ),
        parameterSets = cms.vstring('processParameters')
    )
)
```

## Usage
```bash
cmsDriver.py TTbar_14TeV_TuneCP5_cfi --step GEN,SIM
```

## Cross-References
- **[GeneratorInterface](../../GeneratorInterface/CLAUDE.md)** - Generator wrappers
- **[IOMC](../../IOMC/CLAUDE.md)** - Generator input/output
- **[Configuration/StandardSequences](../StandardSequences/CLAUDE.md)** - GEN step
