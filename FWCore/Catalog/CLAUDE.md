# FWCore/Catalog

## Purpose
File location and catalog management for distributed data access. Maps logical file names (LFNs) to physical file names (PFNs).

## Key Components

- **SiteLocalConfig.h** (`interface/SiteLocalConfig.h:1`) - Site-specific configuration
  - Storage site configuration
  - Catalog endpoint configuration
- **FileLocator.h** - Resolve file paths and URLs
  - LFN → PFN mapping
  - Protocol handling (local, xrootd, dcap, etc.)
- **InputFileCatalog.h** - Manage list of input files
  - Expand file lists
  - Handle fallback URLs

## Use Cases
- **Grid Jobs**: Access files via LFN
- **Site Configuration**: Site-specific file access
- **Multi-Protocol**: Support various storage protocols

## Dependencies
- **FWCore**: ServiceRegistry, Utilities
- **External**: tinyxml2 (XML parsing)

## Cross-References
- **FWCore/Sources** - Input sources use for file location
- **IOPool/Input** - File readers use catalog
