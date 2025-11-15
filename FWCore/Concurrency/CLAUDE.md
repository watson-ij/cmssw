# FWCore/Concurrency

## Purpose
Thread-safe concurrency primitives and task scheduling infrastructure for multi-threaded event processing. Provides the building blocks for CMSSW's parallel execution model built on Intel TBB.

## Key Components

### Task Queues
- **SerialTaskQueue.h** (`interface/SerialTaskQueue.h:1`) - Serializes access to shared resources
  - Ensures tasks accessing the same resource execute one at a time
  - Built on TBB task queue
  - Usage: Protect non-thread-safe resources (e.g., ROOT files, legacy code)
  - Methods: `push(task)`, `pushAndWait(task)`
- **SerialTaskQueueChain.h** - Chain of serial queues
  - Multiple resources must be locked together
- **LimitedTaskQueue.h** (`interface/LimitedTaskQueue.h:1`) - Limits concurrent execution
  - Max N tasks running simultaneously
  - Usage: Limit GPU kernel launches, network connections

### Asynchronous Task Coordination
- **WaitingTaskList.h** (`interface/WaitingTaskList.h:1`) - Notifies multiple waiting tasks
  - Implements "promise" pattern for async operations
  - One "done" signal → wakes all waiting tasks
  - Methods: `add(task)`, `doneWaiting(exception_ptr)`
- **WaitingTask.h** (`interface/WaitingTask.h:1`) - Task with completion notification
  - Base class for async tasks
  - Holds exception_ptr for error propagation
- **WaitingTaskHolder.h** (`interface/WaitingTaskHolder.h:1`) - RAII wrapper for WaitingTask
  - Automatic done notification
  - Exception-safe
- **WaitingTaskWithArenaHolder.h** - Task with TBB arena context
  - Ensures task runs in correct TBB arena

### TBB Thread Pool Management
- **ThreadsController.h** (`interface/ThreadsController.h:1`) - Manages TBB thread pool
  - Sets number of threads
  - Creates TBB task arena
- **FunctorTask.h** - Wraps lambda/functor as TBB task
- **FunctorWaitingTask.h** - Async functor task

### Thread-Safe Containers
- **ThreadSafeAddOnlyContainer.h** - Lock-free append-only container
  - Thread-safe insertions
  - No removals (immutable after growth)
- **SerialTaskQueueChain.h** - Ordered queue chain

### Async Operations Support
- **WaitingTaskWithArenaHolder.h** - GPU/external work support
  - Captures TBB arena for callback context
- **WaitingTaskList.h** - Coordinates multiple consumers of async result

## Concurrency Model

### CMSSW Parallelism Strategy
1. **Stream Parallelism**: Multiple independent event streams (4-8 typically)
2. **Module Parallelism**: Modules within stream execute as tasks
3. **Task-Based**: Intel TBB task scheduler, not thread-per-module

### Module Concurrency Models
- **stream::EDProducer**: Automatic parallelism, one instance per stream
- **global::EDProducer**: Shared instance, must be thread-safe
- **ExternalWork**: Async work (GPU, network), uses WaitingTaskHolder

### SerialTaskQueue Use Cases
- **ROOT I/O**: ROOT is not thread-safe, serialize file access
- **Legacy Code**: Non-thread-safe algorithms
- **External Libraries**: Libraries without thread-safety guarantees

## Dependencies
- **FWCore/Utilities** - Exception handling
- **External**: Intel TBB (oneapi::tbb)

## Integration Points

### Used By
- **FWCore/Framework** - Core scheduling and module execution
  - EventProcessor uses for stream coordination
  - ProductResolver uses for lazy loading
- **IOPool** - File I/O serialization
- **Modules with ExternalWork** - GPU modules, async I/O

### Uses
- **FWCore/Utilities** - Exception propagation
- **TBB**: task, task_group, task_arena

## Key Design Patterns

1. **Serial Queue Pattern** (protect shared resource):
   ```cpp
   class MyProducer {
     edm::SerialTaskQueue queue_;
     NonThreadSafeResource resource_;

   public:
     void produce(edm::Event& e, edm::EventSetup const&) {
       queue_.pushAndWait([&]() {
         resource_.doWork();  // Serialized access
       });
     }
   };
   ```

2. **Async Completion Pattern** (GPU module):
   ```cpp
   void MyGPUProducer::acquire(edm::Event const& e,
                                edm::EventSetup const&,
                                edm::WaitingTaskWithArenaHolder holder) {
     launchKernel([holder]() mutable {
       holder.doneWaiting(nullptr);  // Signal completion
     });
   }
   ```

3. **Waiting Task List** (one producer, many consumers):
   ```cpp
   class CachedProductProducer {
     edm::WaitingTaskList waitingTasks_;
     std::atomic<bool> cached_{false};

   public:
     void produce(edm::Event& e, edm::EventSetup const&, WaitingTaskHolder holder) {
       if (cached_) {
         holder.doneWaiting(nullptr);
       } else {
         waitingTasks_.add(holder);
         if (startComputation()) {
           // ... compute ...
           waitingTasks_.doneWaiting(nullptr);  // Wakes all waiters
         }
       }
     }
   };
   ```

## Common Usage Patterns

### Protecting Non-Thread-Safe Code
```cpp
class MyModule {
  edm::SerialTaskQueue queue_;

public:
  void produce(Event& e, EventSetup const&) {
    queue_.pushAndWait([&]() {
      legacyNonThreadSafeFunction();
    });
  }
};
```

### Async External Work
```cpp
class MyGPUProducer : public edm::global::EDProducer<edm::ExternalWork> {
public:
  void acquire(Event const& e, EventSetup const&, WaitingTaskWithArenaHolder holder) override {
    auto data = e.get(inputToken_);
    gpuAsync(data, [holder]() mutable {
      holder.doneWaiting(nullptr);
    });
  }

  void produce(Event& e, EventSetup const&) override {
    // acquire() completed, get GPU result
    auto result = getGPUResult();
    e.put(outputToken_, std::move(result));
  }
};
```

### Error Propagation
```cpp
void MyModule::asyncWork(WaitingTaskHolder holder) {
  try {
    doWork();
    holder.doneWaiting(nullptr);  // Success
  } catch (...) {
    holder.doneWaiting(std::current_exception());  // Propagate exception
  }
}
```

## Architecture Notes
- Built on Intel TBB (Threading Building Blocks)
- Work-stealing task scheduler
- TBB arena per process
- Exception propagation via exception_ptr
- RAII wrappers prevent resource leaks

## Performance Considerations
- **SerialTaskQueue overhead**: Minimal, but serializes execution
- **Task granularity**: Too fine → overhead, too coarse → poor scaling
- **TBB arena**: Shared across all streams, work-stealing balances load
- **Lock-free where possible**: ThreadSafeAddOnlyContainer

## Thread Safety Guarantees
- All primitives are thread-safe
- WaitingTaskHolder automatically handles exceptions
- SerialTaskQueue ensures mutual exclusion
- No deadlocks from queue chains (DAG ordering)

## Debugging Tips
- TBB debugger integration
- Exception propagation preserves stack traces
- Deadlocks: Check SerialTaskQueueChain ordering
- Race conditions: Use thread sanitizer

## Cross-References
- **FWCore/Framework** - Uses extensively for scheduling
- **FWCore/Utilities** - Exception types
- **HeterogeneousCore** - GPU modules use ExternalWork pattern
- **IOPool** - File I/O uses SerialTaskQueue
- **DataFormats** - Thread-safe data products
