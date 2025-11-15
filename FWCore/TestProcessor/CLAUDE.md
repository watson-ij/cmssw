# FWCore/TestProcessor

## Purpose
Simplified framework for unit testing modules without full job setup. Enables fast, isolated module tests.

## Key Components

- **TestProcessor.h** (`interface/TestProcessor.h:1`) - Lightweight event processor for tests
  - Minimal setup
  - Run single modules
  - Inject test data
- **Event.h** - Test event interface
- **EventSetupTestHelper.h** - Mock EventSetup for tests
- **ESProduceEntry.h** - Inject EventSetup data for tests

## Use Cases
- **Unit Tests**: Test modules in isolation
- **Fast Tests**: No framework initialization overhead
- **Mocking**: Inject test data easily

## Example
```cpp
edm::test::TestProcessor tester(R"_(
  process TEST = {
    source = EmptySource {}
    module test = MyProducer {
      param = 5
    }
  }
)_");

auto event = tester.test();
auto const& product = event.get<MyProduct>();
REQUIRE(product.value() == 5);
```

## Dependencies
- **FWCore**: Framework, ParameterSet, ServiceRegistry, Utilities
- **External**: boost, TBB

## Cross-References
- **FWCore/Framework** - Simplified version of this
- **FWCore/Integration** - Integration tests (heavier)
- Module unit tests in all packages
