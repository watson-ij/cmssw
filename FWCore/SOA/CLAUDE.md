# FWCore/SOA

## Purpose
Structure-of-Arrays (SoA) table support for efficient data layout. Improves CPU cache performance and enables vectorization.

## Key Components

- **Table.h** (`interface/Table.h:1`) - SoA table container
- **Column.h** - Column definition
- **TableView.h** - View interface for tables
- **RowView.h** - Row accessor (creates struct-like view)

## Memory Layout

**Traditional AoS (Array of Structs)**:
```
struct Particle { float pt, eta, phi; };
Particle particles[N]; // pt,eta,phi, pt,eta,phi, pt,eta,phi, ...
```

**SoA (Struct of Arrays)**:
```
Table<Column<float, "pt">, Column<float, "eta">, Column<float, "phi">> particles;
// Memory: pt,pt,pt,..., eta,eta,eta,..., phi,phi,phi,...
```

## Benefits
- **Cache efficiency**: Access single column without loading others
- **Vectorization**: Contiguous data enables SIMD
- **GPU-friendly**: Coalesced memory access

## Use Cases
- **Performance-critical code**: Reconstruction algorithms
- **GPU kernels**: Data transfer to/from GPU
- **Large collections**: Track collections, calorimeter hits

## Dependencies
- Header-only (no BuildFile.xml)

## Cross-References
- **DataFormats/** - Can use SoA layout for products
- **HeterogeneousCore** - GPU SoA data formats
- **RecoTracker** - Uses SoA for pixel tracks
