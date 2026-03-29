:baseID: <SWComponent>Req.<SWUnit>

:lastUsedID: 

<SWUnit> Software Requirements
]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]


:baseID: <SWComponent>Req.<SWUnit>.Configuration


Configuration
=============

.. comment:: 

   =======================================    =========================================================
   Build Option                               Description
   =======================================    =========================================================
   SWR_EXAMPLE_BUILD_OPTION                   Description of the example build option.
   =======================================    =========================================================


:baseID: <SWComponent>Req.<SWUnit>.InputsAndOutputs


Inputs and Outputs
==================

.. requirement::
   :c_This_is_a: functional_requirement
   :0_Module_Type: SW_Requirements
   :c_Implementation_Status: implemented
   :c_Information_Security: internal
   :c_Maturity: project_accepted
   :c_Verification_Method: SWT
   :c_Sil: QM
   :c_Release:
   :p_Link_SYR_Id:
   :p_Link_System_Architecture:
   :p_Link_Software_Architecture:

   +--------------------------------------+-----------------------+-----------------+-----------------+----------------------------+-------------------------------------------+
   | Input Signal Name                    | Unit/Type             | Range           | Min. resolution | Init value                 | Description                               |
   +======================================+=======================+=================+=================+============================+===========================================+
   | s_ExampleInputSignal                 | uint16                | [0,65535]       | 1/256           | 0u                         | Example Description of ExampleInputSignal |
   +--------------------------------------+-----------------------+-----------------+-----------------+----------------------------+-------------------------------------------+

.. requirement::
   :c_This_is_a: functional_requirement
   :0_Module_Type: SW_Requirements
   :c_Implementation_Status: implemented
   :c_Information_Security: internal
   :c_Maturity: project_accepted
   :c_Verification_Method: SWT
   :c_Sil: QM
   :c_Release:
   :p_Link_SYR_Id:
   :p_Link_System_Architecture:
   :p_Link_Software_Architecture:

   +--------------------------------------+-----------------------+-----------------+-----------------+----------------------------+--------------------------------------------+
   | Output Signal Name                   | Unit/Type             | Range           | Min. resolution | Init value                 | Description                                |
   +======================================+=======================+=================+=================+============================+============================================+
   | s_ExampleOutputSignal                | boolean               | [0,1]           | 1               | 0u                         | Example Description of ExampleOutputSignal |
   +--------------------------------------+-----------------------+-----------------+-----------------+----------------------------+--------------------------------------------+


:baseID: <SWComponent>Req.<SWUnit>.Parameters


Parameters
==========

.. requirement::
   :c_This_is_a: functional_requirement
   :0_Module_Type: SW_Requirements
   :c_Implementation_Status: implemented
   :c_Information_Security: internal
   :c_Maturity: project_accepted
   :c_Verification_Method: SWT
   :c_Sil: QM
   :c_Release:
   :p_Link_SYR_Id:
   :p_Link_System_Architecture:
   :p_Link_Software_Architecture:

   +------------------------------------+-------------+------------------+-----------------+---------------+---------------------------------------------------------------+
   | Parameter Name                     | Unit        | Range            | Min. resolution | Init value    | Description                                                   |
   +====================================+=============+==================+=================+===============+===============================================================+
   | p_ExampleParameter                 | no_unit     | [0,0xF]          | 1               | 2             | Description of <SWUnit>_ExampleParameter                      |
   +------------------------------------+-------------+------------------+-----------------+---------------+---------------------------------------------------------------+


:baseID: <SWComponent>Req.<SWUnit>


<Software Unit> - Generic approach
====================================


Example Functionality
---------------------

.. requirement::
   :c_This_is_a: #TBD#
   :0_Module_Type: SW_Requirements
   :c_Implementation_Status: implemented
   :c_Information_Security: internal
   :c_Maturity: project_accepted
   :c_Verification_Method: SWT
   :c_Sil: QM
   :c_Release:
   :p_Link_SYR_Id:
   :p_Link_System_Architecture:
   :p_Link_Software_Architecture:

   Requirement 1 of Example Functionality. Examples of requirement descriptions: 
   
   1) If the Body Control Unit receives a parking request, then the Body Control Unit shall check if a human or animal is inside the vehicle;
   2) If the Body Control Unit recognizes that no human or animal is inside the vehicle, then the Body Control Unit shall invoke the blinker 
      with a blink sequence specified in requirement Req_x.



.. requirement::
   :c_This_is_a: #TBD#
   :0_Module_Type: SW_Requirements
   :c_Implementation_Status: implemented
   :c_Information_Security: internal
   :c_Maturity: project_accepted
   :c_Verification_Method: SWT
   :c_Sil: QM
   :c_Release:
   :p_Link_SYR_Id:
   :p_Link_System_Architecture:
   :p_Link_Software_Architecture:

   Requirement 2 of Example Functionality.


...