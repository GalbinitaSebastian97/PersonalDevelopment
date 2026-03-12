
Repositiory Top Level Folder Structure
======================================

| Folder | Description |
| :--- | :--- |
| **package** | Root folder for generic reusable software packages. A package may contain multiple software components. Each package is individually versioned and baselined. |
| **cmb** | *(In development)* Contains the CMB core and scripts used to run actions. |
| **app** | *(In development)* Contains `.vscode` templates, `econ` configurations/settings, CMB tools configurations, and the variants configuration to be built. |

Package List
============

The following software package is maintained within the repository. For further information on the package pelase follow the link.

[Exterior Mirror Basic Library](package/README.rst)

How to Build and Run (Step-by-Step)
----------------------------------

1. Run the provided PowerShell script in your terminal : .\build_and_run.ps1
2. (Optional) In case you want to explore the build environment inside the container: docker run -it frameless-fold-poc /bin/bash

🤝 Contribution
---------------
This project represents an evolving architecture. Feedback, improvements, and discussions are welcome.
