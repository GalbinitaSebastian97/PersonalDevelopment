Exterior Mirror Library
=======================

.. contents:: Table of Contents
   :depth: 2
   :local:

----

What is this project?
---------------------

This is a personal project I started to bring together everything I have learned
throughout my career in automotive software development. The goal was simple: take
a real-world problem — controlling a frameless exterior mirror — and build it the
right way, with a modern architecture that could actually survive in a production
environment.

Frameless mirrors are a bit different from traditional ones. Without a physical
frame to stop them, the software needs to be smart about where the mirror moves,
when it is allowed to move, and what happens when something goes wrong. That is
exactly what this library handles.

At its core, the ``ExteriorMirrorLibrary`` is a C++ library that manages the
fold and unfold behaviour of a frameless mirror. It takes care of remembering
where the mirror was last positioned, deciding whether a movement request should
be accepted or rejected, and communicating that information to other parts of
the system cleanly and reliably.

----

A bit of context: Classic vs. Adaptive AUTOSAR
-----------------------------------------------

If you are not familiar with AUTOSAR, it is essentially the industry standard
for structuring automotive software. There are two flavours of it: **Classic**,
which runs on microcontrollers with very tight real-time constraints, and
**Adaptive**, which runs on more powerful processors and is closer to how modern
Linux-based applications are built.

This project deliberately sits in between the two. The control logic itself is
written in the spirit of Classic AUTOSAR — cyclic tasks, deterministic state
machines, NvM persistence — while the communication layer takes inspiration from
Adaptive AUTOSAR, using a service-oriented approach where a provider exposes
functionality and a consumer uses it without needing to know how it works
internally.

The reason for this hybrid approach is intentional. It reflects the reality of
many automotive projects today, where teams are transitioning from one world to
the other and need software that can live in both.

----

What has been built so far
--------------------------

Mirror fold control
~~~~~~~~~~~~~~~~~~~

The heart of the library is the ``FramelessFoldController``. It runs as a cyclic
task and handles everything related to moving the mirror:

- Accepting or rejecting fold and unfold requests depending on what the mirror
  is currently doing.
- Evaluating whether a movement should be aborted — for example, if the motor
  gets blocked or a manual adjustment is requested mid-operation.
- Triggering the actual movement with the correct target positions on both axes.

Persistent memory
~~~~~~~~~~~~~~~~~

Mirrors need to remember things across power cycles — specifically, where the
mirror was sitting before it was folded, so it can return to exactly that spot
when unfolded. The ``PersistencyManager`` handles this. It manages reading and
writing that data to non-volatile memory, with timeout handling and state
tracking to make sure nothing gets corrupted during a write.

Service-oriented communication
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

One of the bigger challenges in this project was making the controller accessible
across process boundaries — meaning a separate application running on the same
system can send fold/unfold requests and read back the mirror status without
having direct access to the controller itself.

This is solved through a **Skeleton/Proxy** pattern inspired by Adaptive AUTOSAR:

- The **Skeleton** lives on the provider side. It wraps the controller and
  exposes its functionality as a service.
- The **Proxy** lives on the consumer side. It gives any application a clean
  interface to call ``requestFold()``, ``requestUnfold()``, and read back
  positions and status — without knowing anything about what is running underneath.

Under the hood, the two processes communicate through a **POSIX shared memory
channel**. The consumer writes commands into shared memory, the provider picks
them up, executes them, and writes the results back. Status fields like target
positions and abort reasons are kept up to date every cycle so the consumer can
always read the latest state.

Build and deployment
~~~~~~~~~~~~~~~~~~~~

The project builds with **CMake** and runs inside a **Docker** container based
on Ubuntu, making it fully reproducible regardless of the host machine. A
**PowerShell** script ties everything together — building the image, running the
binaries, and distributing the output.

----

How it is structured
--------------------

.. code-block:: text

   package/ExteriorMirrorLibrary/
   ├── src/                   # Core control logic (controller, persistency, mocks)
   ├── adaptive_services/     # Skeleton and Proxy implementation
   ├── service_provider/      # Provider entry point (main)
   ├── service_consumer/      # Consumer entry point (test client)
   ├── dataTypes/             # Shared type definitions
   └── config/                # Mirror parameters and configuration

----

How to build and run
--------------------

Using Docker (recommended)
~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

   # Build the Docker image
   docker build -t exterior-mirror-lib .

   # Run the container (starts both provider and consumer)
   docker run --rm exterior-mirror-lib

Using CMake directly
~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

   mkdir build && cd build
   cmake ..
   make

   # Run the provider in one terminal
   ./MirrorFoldServiceProvider

   # Run the consumer in another
   ./MirrorFoldServiceConsumer

----

Seeing it in action
-------------------

Below is the actual output from running both processes together. The provider
starts the controller and offers the service, and the consumer finds it, sends
a fold request, reads back the status, then sends an unfold request.

.. code-block:: text

   ========================================
     Mirror Fold Service Consumer
     Test Application
   ========================================
   [ServiceConsumer] Searching for service...
   [Proxy] Found service instance in shared memory: /sebastianGalbinita/embl/mirror_fold (Provider PID: 7)
   [ServiceConsumer] Creating proxy...
   [Proxy] Connected to service: /sebastianGalbinita/embl/mirror_fold
   [ServiceConsumer] Subscribing to events...
   [Proxy] Subscribed to abort reason events
   ========================================
   Commands:
     Will automatically test fold/unfold
     Press Ctrl+C to stop
   ========================================

   [ServiceConsumer] TEST 1: Requesting FOLD...
   [ServiceConsumer] Fold request result: Accepted

   [ServiceConsumer] Reading status...
     Current Command: 0
     X Target Position: 105
     Y Target Position: 500
     Abort Reason: 18

   [ServiceConsumer] TEST 2: Requesting UNFOLD...
   [ServiceConsumer] Unfold request result: Accepted

   [ServiceConsumer] Monitoring events... (Ctrl+C to exit)

What the output is telling us
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The fold and unfold requests are both accepted, which means the service
discovery, shared memory channel, and controller logic are all working
correctly end to end. A couple of the status values are worth explaining
because they might look unexpected at first glance.

**X Target Position: 105 and Y Target Position: 500** — These are the target
coordinates the controller calculated for the fold movement, derived from the
default position parameters loaded at startup. The X value includes a small
offset on top of the lower limit position, and Y lands at the midpoint between
the upper and lower vertical limits. Exactly as designed.

**Current Command: 0 (IDLE)** — This one is intentional for now. Once the fold
movement is triggered, the controller waits for a confirmation from the glass
adjustment module that the mirror actually reached its target. That module is not
implemented yet — it currently returns a hardcoded "finished" status immediately,
which means the command gets marked as complete and resets to IDLE in the same
cycle it was triggered. In other words, the movement completes so fast in
simulation that by the time the consumer reads the status a couple of seconds
later, it has already settled back to IDLE. Once real motor feedback is wired up,
this value will reflect the active command for as long as the mirror is actually
moving.

**Abort Reason: 18** — This is the ``ABORT_REASON_POS_REACHED`` value, which
means the controller considers the fold movement successfully completed. Even
though the glass adjustment module is stubbed out for now, this confirms the
state machine is running through the right path and landing in the correct final
state.

----

In the future ahead
-------------------

There is still a lot I want to do with this project. Some of it is about making
the architecture cleaner, some of it is about getting it closer to what a real
production codebase would look like.

**Cleaner IPC layer** — Right now the shared memory logic is a bit tangled into
the Proxy class. I want to pull it out into its own dedicated communication
layer, so the control logic and the transport mechanism have no knowledge of each
other. Swapping shared memory for sockets or SOME/IP in the future should require
zero changes to the controller.

**Real motor feedback** — Several parts of the state machine currently use
hardcoded placeholders where real motor status would come from. Wiring up actual
glass adjustment module feedback is the next step to make the fold/unfold
sequence behave as it would on a real vehicle.

**MISRA C++:2023 compliance** — Automotive software has strict coding guidelines.
Working toward MISRA compliance would make this library genuinely usable in a
production context, not just a solid proof of concept.

**Quality metrics and AUTOSAR composition** — Adding static analysis, code
coverage, and eventually an AUTOSAR composition description would round out the
project as a proper deliverable rather than just a working prototype.

**A /build delivery pipeline** — Automating the full build, test, and
distribution cycle into a clean output folder, the kind of thing you would hand
off to an integration team.

----

A note on the documentation
----------------------------

The architecture of this project is documented in reStructuredText throughout,
following professional-grade documentation practices. The intention is that
someone unfamiliar with the codebase could understand the design decisions and
the reasoning behind them without having to dig through the source.

----

*Built as a personal exploration of automotive software architecture.*
*Feedback and questions are welcome.*
