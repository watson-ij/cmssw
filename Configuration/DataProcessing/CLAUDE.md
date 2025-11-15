# Configuration/DataProcessing

## Purpose
Data processing scenarios for Tier-0 prompt reconstruction and express streams. Handles different collision types and running conditions.

## Main Scenarios

### Collision Types
- **pp**: Proton-proton collisions (standard physics)
- **ppEra_Run3**, **ppEra_Run2_2018**: Era-specific pp
- **HeavyIons**: Heavy-ion collisions (PbPb, pPb)
- **cosmics**: Cosmic ray data

### Special Data Streams
- **hcalnzs**: HCAL non-zero suppression
- **AlCa**: Alignment and calibration
- **DataScouting**: Scouting streams
- **trackingOnly**: Tracking-only reconstruction

## Scenario Methods

### promptReco()
Prompt reconstruction at Tier-0 within ~48h of data-taking

### expressProcessing()
Express stream - fast reconstruction within ~1h for monitoring and calibration

### alcaSkim()
Alignment and calibration skimming

### dqmHarvesting()
DQM histogram aggregation

## Scenario Class
```python
class Scenario:
    def promptReco(self, globalTag, **args):
        # Build prompt reco config
        process = cms.Process("RECO")
        process.load('Configuration.StandardSequences.Reconstruction_cff')
        # ... customize for scenario
        return process
```

## Usage
Invoked by Tier-0 automation:
```python
from Configuration.DataProcessing.GetScenario import getScenario
scenario = getScenario("ppEra_Run3")
process = scenario.promptReco(globalTag="auto:run3_data_prompt")
```

## Cross-References
- **[Configuration/StandardSequences](../StandardSequences/CLAUDE.md)** - Uses reconstruction sequences
- **[Configuration/AlCa](../AlCa/CLAUDE.md)** - GlobalTags and AlCa streams
- Tier-0 system - Automated data processing
