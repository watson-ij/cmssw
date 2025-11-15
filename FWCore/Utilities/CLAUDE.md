# FWCore/Utilities

## Purpose
Foundational utility classes and types used throughout the CMSSW framework. This is the lowest-level FWCore package providing essential types, exception handling, and common utilities.

## Key Components

### Exception Handling
- **Exception.h** (`interface/Exception.h:1`) - Base `cms::Exception` class for all CMSSW exceptions
- **EDMException.h** (`interface/EDMException.h:1`) - Framework-specific exception types
- Provides categorized error handling with context information

### Type System
- **TypeID.h** (`interface/TypeID.h:1`) - Runtime type identification wrapper around `std::type_info`
- **TypeIDBase.h** - Base type identification
- **FriendlyName.h** - Generates human-readable type names from C++ template types
- **WrappedClassName.h** - Class name wrapper utilities

### Token System (Type-Safe Product Access)
- **EDGetToken.h** (`interface/EDGetToken.h:1`) - Type-safe token for getting products from Events
- **EDPutToken.h** (`interface/EDPutToken.h:1`) - Type-safe token for putting products into Events
- **ESGetToken.h** - Token for accessing EventSetup (conditions) data
- **InputTag.h** (`interface/InputTag.h:1`) - Human-readable product identifier (module:instance:process)
- Tokens provide compile-time type safety and runtime validation

### Identifiers
- **StreamID.h** (`interface/StreamID.h:1`) - Identifies concurrent event processing streams
- **RunIndex.h**, **LuminosityBlockIndex.h** - Sequential indices for Run/Lumi blocks
- **BranchType.h** (`interface/BranchType.h:1`) - Enum for Event/Run/Lumi/ProcessBlock types
- **ProductKindOfType.h** - Product type classification

### Core Utilities
- **Digest.h** - MD5 hash computation
- **Guid.h** - UUID/GUID generation
- **Signal.h** - Signal/slot implementation
- **ReusableObjectHolder.h** - Thread-safe object pooling
- **propagate_const.h** - Const-correctness propagation wrapper
- **Algorithms.h** - STL algorithm extensions
- **Exception.h** - Stack trace utilities

### Framework Support
- **ProductLabels.h** - Product label handling
- **ProductResolverIndex.h** - Index for product lookup
- **WaitingTaskList.h** - Asynchronous task completion notification
- **transform.h** - Range transformations
- **bit_cast.h** - Type-punning utilities

## Dependencies
- **External**: boost, Intel TBB (oneapi::tbb), fmt, libuuid, md5
- **Internal**: None (foundational package)

## Integration Points
Used by virtually every other package in CMSSW:
- **FWCore/Framework** - Uses tokens, types, exceptions
- **FWCore/ParameterSet** - Uses InputTag, Exception
- **FWCore/PluginManager** - Uses TypeID
- **DataFormats/** - Uses TypeID for product registration
- All user modules use EDGetToken/EDPutToken

## Architecture Notes
- Header-only utilities where possible for efficiency
- Extensive use of templates for type safety
- Thread-safe implementations throughout
- Minimal dependencies to avoid circular deps
- C++17 standard required

## Key Design Patterns
1. **Token Pattern**: Type-safe product access without string lookups
2. **Exception Categories**: Structured error reporting with context
3. **Type Erasure**: TypeID wraps std::type_info with value semantics
4. **RAII**: Resource management throughout
