# FWCore/Version

## Purpose
Framework version information and metadata.

## Key Components

- **GetReleaseVersion.h** (`interface/GetReleaseVersion.h:1`) - CMSSW release version
- **GetFileFormatVersion.h** - ROOT file format version

## Use Cases
- **Provenance**: Track which CMSSW version processed data
- **Compatibility**: Check file format compatibility
- **Versioning**: Software version tracking

## Dependencies
- Minimal (no external dependencies)

## Cross-References
- **FWCore/Framework** - Includes version in provenance
- **IOPool** - File format versioning
