
Problem: Static Variable Isolation in Multi-Process Docker Containers
======================================================================

**Issue Description:**

In a Docker container environment with multiple processes, static variables 
are NOT shared between processes. Each process maintains its own isolated 
memory space with its own copy of static variables.

**Visual Scenario:**

When a provider process initializes a static variable ``s_serviceInstance`` 
to point to a skeleton object, the consumer process cannot access this 
initialization. The consumer process has its own separate copy of 
``s_serviceInstance`` which remains ``nullptr``.

**Root Cause:**

Static variables have process local scope. They are linked at compile time 
to each executable. When multiple processes are spawned (even within the 
same container), each receives its own independent memory address space 
(PROCESS MEMORY A, PROCESS MEMORY B, etc.).

**Consequences:**

- Provider process modifications to static variables are invisible to consumer processes
- Attempting to access shared state through static variables results in default 
    initialization (e.g., ``nullptr``) in other processes
- IPC synchronization cannot be achieved through static variable mutation alone

**Solution Approach:**

To share state between processes, use inter-process communication (IPC) 
mechanisms such as:

- Shared memory segments (``shm_open``, ``mmap``)
- Message queues
- Named pipes (FIFOs)
- Socket-based communication
- Memory-mapped files

┌─────────────────────────────────────────────────────────────────┐
│                     Docker Container                            │ 
│                                                                 │
│  ┌──────────────────────┐      ┌──────────────────────┐         │
│  │  Provider Process    │      │  Consumer Process    │         │
│  │  (main_service_      │      │  (main_service_      │         │
│  │   provider.cpp)      │      │   consumer.cpp)      │         │
│  │                      │      │                      │         │
│  │  s_serviceInstance   │  !   │  s_serviceInstance   │         │
│  │  = &skeleton         │ ───► │  = nullptr           │         │
│  │                      │      │                      │         │
│  │  [PROCESS MEMORY A]  │      │  [PROCESS MEMORY B]  │         │
│  └──────────────────────┘      └──────────────────────┘         │
│                                                                 │
│  Each process has its OWN copy of static variables!             │
│  They CANNOT see each other's memory.                           │
└─────────────────────────────────────────────────────────────────┘

Even though both executables run in the same Docker container, they are separate processes with isolated memory spaces.

**Key Concepts:**
 - **Process** : An independent running program with its own memory space.
 - **Virtual Memory** : Each process sees its own "virtual" memory addresses(0x0000 to 0xFFFF)
 - **Memory Isolation** : Process A cannot read/write Process B's memory(security feature)
 - **Static Variable** : A variable that exists once per process, stored in that process's memory space.
 - **Docker Container** : A lightweight isolated environment - but processess inside still have separate memory!

┌─────────────────────────────────────────────────────────────────────────────┐
│                           DOCKER CONTAINER                                  │
│                                                                             │
│  ┌─────────────────────────────────┐   ┌─────────────────────────────────┐  │
│  │      PROVIDER PROCESS           │   │      CONSUMER PROCESS           │  │
│  │    (main_service_provider)      │   │    (main_service_consumer)      │  │
│  │                                 │   │                                 │  │
│  │  ┌───────────────────────────┐  │   │  ┌───────────────────────────┐  │  │
│  │  │    PROCESS MEMORY         │  │   │  │    PROCESS MEMORY         │  │  │
│  │  │                           │  │   │  │                           │  │  │
│  │  │  s_serviceInstance ────────────────────► CANNOT ACCESS!         │  │  │
│  │  │  = 0x7fff1234             │  │ ! │  │  s_serviceInstance        │  │  │
│  │  │  (valid pointer)          │  │   │  │  = nullptr                │  │  │
│  │  │                           │  │   │  │  (separate copy!)         │  │  │
│  │  └───────────────────────────┘  │   │  └───────────────────────────┘  │  │
│  │                                 │   │                                 │  │
│  └─────────────────────────────────┘   └─────────────────────────────────┘  │
│                                                                             │
│  ════════════════════════════════════════════════════════════════════════   │
│                    MEMORY ISOLATION BARRIER                                 │
│         (Each process has its own virtual address space)                    │
│  ════════════════════════════════════════════════════════════════════════   │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘


Docker Container Shared Memory Architecture
=============================================

**Overview**
Shared Memory is a special memory region that the operating system creates outside of any single process. Multiple processes can then "map" this region into their own address space.
This diagram illustrates inter-process communication (IPC) within a Docker container using shared memory regions to enable process synchronization and data exchange.

**Brief**
A Provider Process writes data to a shared memory region that a Consumer Process reads from, allowing asynchronous communication without direct dependencies.

**Previous Problems Addressed**
1. **Process Isolation**: Processes running in containers cannot communicate directly; they need a shared data structure.
2. **Race Conditions**: Without synchronization mechanisms, processes attempting to access resources simultaneously could cause conflicts.
3. **Service Discovery**: Consumer processes had no way to detect when provider services became available.
4. **Data Persistence**: Temporary data needed to persist across process boundaries without file I/O overhead.

**Solution**
- **Shared Memory Region**: A dedicated memory space accessible by all container processes eliminates the need for file-based or network-based IPC.
- **Asynchronous Communication**: Provider writes availability flags (e.g., `serviceAvailable = true`) and configuration data (e.g., `instanceId = "/mirror_fold"`) that Consumer processes can poll or wait for.
- **Atomic Operations**: Shared memory allows processes to safely exchange state information with minimal latency.
- **In-Memory Data Storage**: Critical metadata is stored directly in shared memory, avoiding serialization/deserialization overhead.

**Key Components**
- Provider Process: Initializes and writes service availability state
- Consumer Process: Reads and reacts to provider state changes
- Shared Memory Region: Central hub containing service metadata and status flags

┌─────────────────────────────────────────────────────────────────────────────┐
│                           DOCKER CONTAINER                                  │
│                                                                             │
│  ┌─────────────────────────────────┐   ┌─────────────────────────────────┐  │
│  │      PROVIDER PROCESS           │   │      CONSUMER PROCESS           │  │
│  │                                 │   │                                 │  │
│  │  ┌───────────────────────────┐  │   │  ┌───────────────────────────┐  │  │
│  │  │    PROCESS MEMORY         │  │   │  │    PROCESS MEMORY         │  │  │
│  │  │                           │  │   │  │                           │  │  │
│  │  │  registry* ───────────────────────────► registry*               │  │  │
│  │  │      │                    │  │   │  │      │                    │  │  │
│  │  └──────┼────────────────────┘  │   │  └──────┼────────────────────┘  │  │
│  │         │                       │   │         │                       │  │
│  └─────────┼───────────────────────┘   └─────────┼───────────────────────┘  │
│            │                                     │                          │
│            │         BOTH POINT TO SAME          │                          │
│            │            MEMORY!                  │                          │
│            ▼                                     ▼                          │
│  ┌──────────────────────────────────────────────────────────────────────┐   │
│  │                     SHARED MEMORY REGION                             │   │
│  │                    (/dev/shm/mirror_fold_service_registry)           │   │
│  │                                                                      │   │
│  │    ┌─────────────────────────────────────────────────────────────┐   │   │
│  │    │  SharedServiceRegistry                                      │   │   │
│  │    │  ├── serviceAvailable: true                                 │   │   │
│  │    │  ├── instanceId: "/sebastianGalbinita/embl/mirror_fold"     │   │   │
│  │    │  └── providerPid: 1234                                      │   │   │
│  │    └─────────────────────────────────────────────────────────────┘   │   │
│  │                                                                      │   │
│  │    Provider WRITES here ──────► Consumer READS here                  │   │
│  │                                                                      │   │
│  └──────────────────────────────────────────────────────────────────────┘   │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘


How Shared Memory Works(Step by Step)
*************************************

┌─────────────────────────────────────────────────────────────────────────────┐
│                         SHARED MEMORY LIFECYCLE                              │
└─────────────────────────────────────────────────────────────────────────────┘

STEP 1: Provider Creates/Opens Shared Memory
═══════════════════════════════════════════

    Provider Process                         Operating System
    ────────────────                         ────────────────
          │                                        │
          │  shm_open("/mirror_fold_registry",     │
          │           O_CREAT | O_RDWR, 0666)      │
          │ ─────────────────────────────────────► │
          │                                        │
          │    Creates /dev/shm/mirror_fold_registry
          │    (a special file in memory)          │
          │                                        │
          │  ◄───────────────────────────────────  │
          │    Returns: file descriptor (fd=3)     │
          │                                        │


STEP 2: Provider Sets Size
══════════════════════════

    Provider Process                         Operating System
    ────────────────                         ────────────────
          │                                        │
          │  ftruncate(fd, sizeof(SharedServiceRegistry))
          │ ─────────────────────────────────────► │
          │                                        │
          │    Allocates 264 bytes                 │
          │    (bool + char[256] + int)            │
          │                                        │


STEP 3: Provider Maps Memory
════════════════════════════

    Provider Process                         Operating System
    ────────────────                         ────────────────
          │                                        │
          │  mmap(NULL, size, PROT_READ|WRITE,     │
          │       MAP_SHARED, fd, 0)               │
          │ ─────────────────────────────────────► │
          │                                        │
          │    Maps shared memory into             │
          │    provider's address space            │
          │                                        │
          │  ◄───────────────────────────────────  │
          │    Returns: pointer 0x7f1234000        │
          │                                        │

    Now provider can use this pointer like normal memory!


STEP 4: Provider Writes Data
════════════════════════════

    Provider Process                         Shared Memory
    ────────────────                         ─────────────
          │                                        │
          │  registry->serviceAvailable = true;    │
          │  strcpy(registry->instanceId, "...");  │
          │ ─────────────────────────────────────► │
          │                                    ┌───┴───┐
          │                                    │ true  │
          │                                    │ "/.." │
          │                                    │ 1234  │
          │                                    └───────┘


STEP 5: Consumer Opens SAME Shared Memory
═════════════════════════════════════════

    Consumer Process                         Operating System
    ────────────────                         ────────────────
          │                                        │
          │  shm_open("/mirror_fold_registry",     │
          │           O_CREAT | O_RDWR, 0666)      │
          │ ─────────────────────────────────────► │
          │                                        │
          │    Opens EXISTING shared memory        │
          │    (same name = same memory!)          │
          │                                        │
          │  ◄───────────────────────────────────  │
          │    Returns: file descriptor (fd=3)     │
          │                                        │


STEP 6: Consumer Maps and Reads
═══════════════════════════════

    Consumer Process                         Shared Memory
    ────────────────                         ─────────────
          │                                    ┌───────┐
          │  mmap(...) → pointer               │ true  │
          │                                    │ "/.." │
          │  if (registry->serviceAvailable)   │ 1234  │
          │ ◄──────────────────────────────────┴───────┘
          │                                        
          │  "Service found!"                      
          │                         

Why We Use Simple Types (Not Pointers or Complex Objects)
*********************************************************

┌─────────────────────────────────────────────────────────────────────────────┐
│                    SHARED MEMORY DATA STRUCTURE RULES                       │
└─────────────────────────────────────────────────────────────────────────────┘

  ✗ WRONG: Using pointers or std::string
  ═══════════════════════════════════════

    struct BrokenRegistry {
        bool serviceAvailable;
        std::string instanceId;      // ✗ BROKEN! std::string uses internal pointers
        IMirrorFoldService* service; // ✗ BROKEN! Pointer only valid in one process
    };

    Why it fails:
    ┌──────────────────────────┐     ┌──────────────────────────┐
    │  Provider Process        │     │  Consumer Process        │
    │                          │     │                          │
    │  std::string stores:     │     │  Reads from shared mem:  │
    │  ptr → 0x7fff1234        │     │  ptr → 0x7fff1234        │
    │         │                │     │         │                │
    │         ▼                │     │         ▼                │
    │  ┌──────────────┐        │     │  ┌──────────────┐        │
    │  │ "hello"      │        │     │  │ GARBAGE!     │        │
    │  │ (in provider │        │     │  │ (address     │        │
    │  │  memory)     │        │     │  │  doesn't     │        │
    │  └──────────────┘        │     │  │  exist here) │        │
    │                          │     │  └──────────────┘        │
    └──────────────────────────┘     └──────────────────────────┘


  ✓ CORRECT: Using fixed-size arrays and primitive types
  ═══════════════════════════════════════════════════════

    struct SharedServiceRegistry {
        bool serviceAvailable;       // ✓ Primitive type - stored directly
        char instanceId[256];        // ✓ Fixed array - data stored in-place
        int providerPid;             // ✓ Primitive type - stored directly
    };

    Why it works:
    ┌──────────────────────────────────────────────────────────┐
    │              SHARED MEMORY REGION                        │
    │                                                          │
    │  Offset 0:    [bool: true]             ← serviceAvailable│
    │  Offset 1:    ['/']['.']['.'][...]     ← instanceId      │
    │              (256 bytes, all in-place)                   │
    │  Offset 257:  [int: 1234]              ← providerPid     │
    │                                                          │
    │  ALL DATA IS STORED DIRECTLY IN SHARED MEMORY!           │
    │  No pointers, no external references.                    │
    └──────────────────────────────────────────────────────────┘

API Flow Diagrams
*****************

┌─────────────────────────────────────────────────────────────────────────────┐
│                      PROVIDER STARTUP SEQUENCE                              │
└─────────────────────────────────────────────────────────────────────────────┘

main_service_provider.cpp
         │
         ▼
┌─────────────────────────────────────┐
│ 1. Create Skeleton                  │
│    MirrorFoldServiceSkeleton        │
│    skeleton(instanceId);            │
└─────────────────────────────────────┘
         │
         ▼
┌─────────────────────────────────────┐
│ 2. Initialize Controller            │
│    skeleton.getController().Init(); │
└─────────────────────────────────────┘
         │
         ▼
┌─────────────────────────────────────┐
│ 3. Offer Service (Skeleton)         │
│    skeleton.offerService();         │
│                                     │
│    (Internal registration in        │
│     skeleton's own mechanism)       │
└─────────────────────────────────────┘
         │
         ▼
┌─────────────────────────────────────┐
│ 4. Register in Shared Memory        │    ┌─────────────────────────────┐
│    MirrorFoldServiceProxy::         │    │    SHARED MEMORY            │
│        offerService(&skeleton);     │───►│                             │
│                                     │    │  serviceAvailable = true    │
│    This writes to shared memory     │    │  instanceId = "/.../..."    │
│    so consumers can discover us!    │    │  providerPid = 1234         │
└─────────────────────────────────────┘    └─────────────────────────────┘
         │
         ▼
┌─────────────────────────────────────┐
│ 5. Run Event Loop                   │
│    while (running) {                │
│        // Handle requests           │
│    }                                │
└─────────────────────────────────────┘
         │
         ▼
┌─────────────────────────────────────┐
│ 6. Cleanup (on shutdown)            │    ┌─────────────────────────────┐
│    MirrorFoldServiceProxy::         │    │    SHARED MEMORY            │
│        stopOfferService();          │───►│                             │
│                                     │    │  serviceAvailable = false   │
└─────────────────────────────────────┘    └─────────────────────────────┘

Consumer Discovery Flow
***********************

┌─────────────────────────────────────────────────────────────────────────────┐
│                      CONSUMER DISCOVERY SEQUENCE                             │
└─────────────────────────────────────────────────────────────────────────────┘

main_service_consumer.cpp
         │
         ▼
┌─────────────────────────────────────┐
│ 1. Find Service                     │    ┌─────────────────────────────┐
│    auto handles =                   │    │    SHARED MEMORY            │
│      MirrorFoldServiceProxy::       │◄───│                             │
│        findService(instanceId);     │    │  serviceAvailable = true ✓  │
│                                     │    │  instanceId = "/.../..."    │
│    Reads from shared memory!        │    │  providerPid = 1234         │
└─────────────────────────────────────┘    └─────────────────────────────┘
         │
         ▼
┌─────────────────────────────────────┐
│ 2. Check if Found                   │
│    if (handles.empty()) {           │
│        // ERROR: No provider!       │──────► Line 33: "No service provider"
│        return;                      │
│    }                                │
└─────────────────────────────────────┘
         │ (if found)
         ▼
┌─────────────────────────────────────┐
│ 3. Create Proxy                     │
│    MirrorFoldServiceProxy           │
│        proxy(handles[0]);           │
└─────────────────────────────────────┘
         │
         ▼
┌─────────────────────────────────────┐
│ 4. Use Service                      │
│    proxy.requestFold();             │
│    proxy.requestUnfold();           │
│    ...                              │
└─────────────────────────────────────┘