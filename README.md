🚗 Frameless Mirror Control – Functional Architecture
------------------------------------------------------
📌 Overview
-----------
This project documents the functional architecture and control logic behind a frameless automotive mirror folding system. It focuses on how fold/unfold commands are processed, validated, and executed safely while preserving user adjustments.

The system ensures:

 * Safe motor actuation

 * Position-aware movement (X/Y axes)

 * Fault detection and handling

 * Persistence of user-defined mirror positions

🧠 System Architecture
----------------------
<img width="1060" height="578" alt="image" src="https://github.com/user-attachments/assets/08ce71bb-5911-4472-8756-0a47cf4c0bfa" />
The system is composed of multiple interacting modules:

1. Application Layer

 * Handles user requests (fold/unfold)

 * Validates initial preconditions

 * Issues high-level commands

2. Subsystem Control

 * Manages motor preconditions

 * Executes fold/unfold logic

 * Interfaces with movement control

3. Movement Control Layer

 * Processes real-time motor behavior

 * Reads sensor inputs (positions, voltage, block detection)

 * Ensures smooth and safe movement execution

🔄 Functional Flow
------------------
▶️ Fold Request (Infold)

 * Read fold request

 * Check preconditions

 * Motor readiness

 * Voltage validity

 * Move mirror

 * First along Y-axis → center position

 * Then along X-axis → soft stall position

◀️ Unfold Request (Outfold)

 * Retrieve stored mirror position from NvM

 * Move mirror

 * First along X-axis → saved position

 * Then along Y-axis → saved position

💾 Position Memory (NvM)

If the user manually adjusts the mirror:

 * The system stores X and Y positions

 * On the next unfold request:

 * Mirror returns to user-defined position

🔧 Key Signals & Inputs
------------------------

Signals | Description | 
------- | ----------- |
FoldRequest | User input for fold/unfold |
MirrorCommandFold | Command sent to movement control |
XGlassPos | Mirror position feedback
YGlassPos | Mirror position feedback
StatusMirrorGlassPosition | Position validity
FoldMotorBlockStatus | Obstruction detection
Obstruction detection | System voltage check

⚙️ Mechanical Folding Reference
--------------------------------

📄 **[View Full Architecture PDF](./package/ExteriorMirrorLibrary/docs/Arhitecture/FramelessMirror.pdf)**

<img width="857" height="379" alt="image" src="https://github.com/user-attachments/assets/3098a7b6-0795-4064-a37c-0991856768d6" />
<img width="1051" height="739" alt="image" src="https://github.com/user-attachments/assets/4e043b27-1d97-4319-be89-06ee2f3d2511" />
<img width="745" height="518" alt="image" src="https://github.com/user-attachments/assets/ed4445d7-722e-4fe2-a0aa-43f2c4133d45" />


📚 Documentation
----------------
The project includes detailed technical documentation:

* **[Architecture Diagram (PDF)](./package/ExteriorMirrorLibrary/docs/Arhitecture/FramelessMirror.pdf)** - Frameless mirror system architecture
* **[Software Requirements](./package/ExteriorMirrorLibrary/docs/SoftwareRequirements/mirrorFrameless_swr.rst)** - System requirements specification
* **[Software Detail Design](./package/ExteriorMirrorLibrary/docs/SoftwareDetailDesign/mirrorFrameles_sdd.rst)** - Detailed design documentation
* **[Integration Tests](./package/ExteriorMirrorLibrary/docs/SoftwareIntegrationTests/mirrorFrameless_its.rst)** - Test specifications

🚀 Future Improvements
-----------------------
 * Add simulation or test cases

 * Integrate CAN communication layer

 * Extend to memory profiles (multiple users)

 * Add diagnostics reporting (DTCs)

🤝 Contribution
---------------
This project represents an evolving architecture. Feedback, improvements, and discussions are welcome.
