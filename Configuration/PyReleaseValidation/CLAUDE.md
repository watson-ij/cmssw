# Configuration/PyReleaseValidation

## Purpose
Automated testing and validation matrix. Runs thousands of workflows to validate CMSSW releases.

## Main Tool: runTheMatrix.py

### Workflow Types
- **Standard workflows**: Physics processes (ttbar, ZMM, QCD, etc.)
- **Data workflows**: Re-reconstruction of real data
- **Upgrade workflows**: Phase-2 detector scenarios
- **Special workflows**: FastSim, premixing, GPU, etc.

### Workflow Numbering
- **1XXXX**: MC workflows
- **1XX.YY**: Variations (0.5=CPU-only, 0.7=MkFit, 0.91=DD4hep, etc.)
- **1XXXXX**: Upgrade workflows

### Usage Examples
```bash
# Run specific workflow
runTheMatrix.py -l 11634.0

# List upgrade workflows
runTheMatrix.py --what upgrade -n

# Run with 8 threads
runTheMatrix.py -l 11634.0 -t 8

# Run multiple workflows for testing
runTheMatrix.py -l 11634.0,11650.0,25202.0
```

### Common Workflow Variations
- **0.1**: Tracking-only validation
- **0.5-0.52**: Legacy CPU-only (pixel, ECAL, HCAL)
- **0.402-0.424**: Alpaka validation
- **0.7-0.704**: MkFit, LST tracking
- **0.911-0.914**: DD4hep geometry tests
- **0.97-0.99**: Premixing workflows

## Workflow Chains
Typical: GEN-SIM → DIGI-L1-HLT → RECO → VALIDATION → DQM

## Testing Scenarios
- **PR tests**: Limited matrix for pull requests
- **IB tests**: Nightly integration builds
- **Release validation**: Full matrix for releases

## Cross-References
- **[Configuration/Applications](../Applications/CLAUDE.md)** - Uses cmsDriver
- **[Validation](../../Validation/CLAUDE.md)** packages - Validation code
- **[DQM](../../DQM/CLAUDE.md)** packages - Quality monitoring
