# Configuration/ProcessModifiers

## Purpose
Conditional modifications to configurations for special running modes, hardware acceleration, and algorithm variants.

## Key Process Modifiers

### Hardware Acceleration
- **gpu**: GPU offloading (pixel, ECAL, HCAL reconstruction)
- **alpaka**: Portable acceleration (CPU/GPU via Alpaka)
- **cuda**: CUDA-specific paths

### Tracking Algorithms
- **trackingMkFit**: MkFit tracking (parallelized track building)
- **trackingLST**: Line Segment Tracking
- **approxSiStripClusters**: Approximate strip clustering

### Particle Flow
- **mlpf**: Machine Learning Particle Flow

### Running Modes
- **pp_on_AA**: Proton-proton algorithms on heavy-ion events
- **peripheralPbPb**: Peripheral heavy-ion collisions

### Geometry
- **dd4hep**: Use DD4hep geometry system

### Pileup
- **premix_stage1**, **premix_stage2**: Pre-mixed pileup

### Special Studies
- **bParking**: B-parking data stream
- **run2_miniAOD_devel**: MiniAOD development

## Usage
```bash
# Single modifier
cmsDriver.py --procModifiers gpu

# Multiple modifiers
cmsDriver.py --procModifiers gpu,alpaka,trackingMkFit
```

## Implementation
Uses `cms.Modifier()` for conditional parameter changes:
```python
gpu = cms.Modifier()

# Only applied if 'gpu' modifier active
gpu.toModify(process.hltSiPixelRecHits,
    onGPU = cms.bool(True)
)
```

## Cross-References
- **[HeterogeneousCore](../../HeterogeneousCore/CLAUDE.md)** - GPU infrastructure
- **[Configuration/Eras](../Eras/CLAUDE.md)** - Similar modifier system
