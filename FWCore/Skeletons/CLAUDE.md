# FWCore/Skeletons

## Purpose
Code generation templates for creating new modules, packages, and subsystems.

## Key Components

- **mkTemplates/** - Template files for:
  - EDProducer skeleton
  - EDAnalyzer skeleton
  - EDFilter skeleton
  - ESProducer skeleton (conditions)
  - Package skeleton
- **Python scripts** - Code generators

## Command-Line Tools
- `mkedanlzr MyAnalyzer` - Create analyzer
- `mkedproducer MyProducer` - Create producer
- `mkedfltr MyFilter` - Create filter
- `mkedlpr MyLooper` - Create looper
- `mkesprod MyESProducer` - Create EventSetup producer

## Generated Files
- Header (.h)
- Implementation (.cc)
- BuildFile.xml
- Python configuration example

## Use Cases
- **New Modules**: Quick start for new algorithms
- **Standard Structure**: Consistent code organization
- **Best Practices**: Templates follow framework conventions

## Cross-References
- **FWCore/Framework** - Templates use framework base classes
- All packages - Use skeletons to create new modules
