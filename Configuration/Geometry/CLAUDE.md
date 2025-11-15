# Configuration/Geometry

## Purpose
Detector geometry configurations for different run periods, detector versions, and upgrade scenarios.

## Run 3 Geometries (2022-2025)
Format: `Extended2023D<X>` where components are:
- **T**: Tracker (T3 = Run 3 tracker)
- **C**: Calorimetry (C1, C2 variants)
- **M**: Muon system (M1-M9: GE1/1, iRPC additions)
- **P**: PPS (P7 = baseline)
- **F**: Forward detectors

Examples:
- `Extended2023D88`: Run 3 baseline (T3,C1,M9,P7,F1)
- `Extended2024`: 2024 detector configuration

## Phase 2 Geometries (HL-LHC)
- **Tracker**: T15-T39 (tilted tracker, IT/OT variants)
- **HGCal**: C9-C25 (post-TDR evolution)
- **Muon**: M4-M12 (GE0, ME0, RE3/1, RE4/1)
- **MTD**: I10-I17 (timing layer)
- **Combined**: D95-D116

Example: `Extended2026D110` = Phase-2 detector

## Geometry Systems
- **DD4hep**: Modern geometry system (`GeometryDD4hepExtended*`)
- **DDD**: Legacy CMSSW geometry (`GeometryExtended*`)
- **SimDB**: Database-driven geometry

## Material Budget Variations
- **ZeroMaterial**: No material (tracking studies)
- **FlatMinus/Plus05/10Percent**: ±5%, ±10% material

## Usage
```bash
cmsDriver.py --geometry DB:Extended  # Use default from DB
cmsDriver.py --geometry Extended2023D88  # Specific Run 3 geometry
```

## Cross-References
- **[Geometry](../../Geometry/CLAUDE.md)** packages - Implement geometry
- **[DetectorDescription](../../DetectorDescription/CLAUDE.md)** - Geometry framework
- **[Configuration/Eras](../Eras/CLAUDE.md)** - Geometry selection per era
