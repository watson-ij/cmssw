# Configuration/AlCa

## Purpose
Alignment and Calibration infrastructure: GlobalTags, AlCa sequences, and Prompt Calibration Loop (PCL).

## Key Components

### GlobalTags (`autoCond.py`)
Maps symbolic names to GlobalTag strings:
- **run3_data**: Latest Run 3 data GlobalTag
- **run3_data_prompt**: Prompt reconstruction GT
- **run3_hlt**: HLT GlobalTag
- **phase1_2024_realistic**: 2024 MC GT
- **phase2_realistic**: Phase-2 upgrade MC GT

Usage: `--conditions auto:phase1_2024_realistic`

### AlCa Sequences (`autoAlca.py`)
Defines calibration workflows:
- **Express**: Fast calibrations (run-by-run)
- **Prompt**: Standard calibrations
- **Cosmics**: Cosmic ray calibrations

### AlCa Streams
- **TkAl***: Track-based alignment
- **SiPixelCal*, SiStripCal***: Tracker calibration
- **MuAlGlobal***: Muon alignment
- **EcalUncal*, EcalCal***: ECAL calibration
- **HcalCal***: HCAL calibration

### Prompt Calibration Loop (`autoPCL.py`)
Automated calibration during data-taking:
1. AlCa stream runs in prompt reco
2. Harvesting produces calibrations
3. Upload to database
4. Applied in next prompt reco

## GlobalTag Structure
GlobalTag = database snapshot with:
- Detector alignment
- Energy calibrations
- Dead channels
- Trigger menus
- Pileup scenarios

## Cross-References
- **[CondCore](../../CondCore/CLAUDE.md)** - Conditions database
- **[CondFormats](../../CondFormats/CLAUDE.md)** - Calibration data formats
- **[CalibTracker](../../CalibTracker/CLAUDE.md)**, **[CalibCalorimetry](../../CalibCalorimetry/CLAUDE.md)** - Calibration algorithms
