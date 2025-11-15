# Configuration/HLT

## Purpose
High-Level Trigger (HLT) configuration support - GlobalTags and menu selection for HLT.

## Key Files
- **autoCondHLT.py**: HLT-specific GlobalTags
- **autoHLT.py**: HLT menu selection for RelVals

## HLT Menus
HLT menus are large (10K+ lines) auto-generated configs containing:
- Trigger paths (hundreds of paths)
- HLT filters and producers
- L1 seed requirements
- Prescales

## Usage
```bash
cmsDriver.py --step HLT:@relval2024  # Use RelVal HLT menu
cmsDriver.py --hltProcess HLT2  # Custom HLT process name
```

## Cross-References
- **[HLTrigger](../../HLTrigger/CLAUDE.md)** packages - HLT algorithms
- **[L1Trigger](../../L1Trigger/CLAUDE.md)** - L1 seeds for HLT
- **[Configuration/AlCa](../AlCa/CLAUDE.md)** - HLT GlobalTags
