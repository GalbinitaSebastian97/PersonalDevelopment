:baseID: <SWComponent>ITS

:lastUsedID: 

============================================
<SWComponent> Integration Test Specification
============================================


Details
-------

.. table::

   +-------------------------+---------------------------------+
   | Maturity                | Valid                           |
   +-------------------------+---------------------------------+
   | Author(s)               | Galbinita Sebastian             |
   +-------------------------+---------------------------------+
   | Release Authority       | Galbinita Sebastian             |
   +-------------------------+---------------------------------+
   | Distribution            | project                         |
   +-------------------------+---------------------------------+
   | Security Classification | For internal Use Only           |
   +-------------------------+---------------------------------+


Test Log
--------

.. comment::

   Test Log Table:

   .. table::

        +----------------------+----------------------+---------------------------------+
        | Functionality        | Checkpoint Reference | Target Software Release Version |
        +======================+======================+=================================+
        | <SWComponent>        | Lib Release Tag      | Test is done for which Release  |
        +----------------------+----------------------+---------------------------------+
        | Software Tester      | Department           | Date of Test Completion         |
        +----------------------+----------------------+---------------------------------+
        | Tester Name          | Tester Department    | Date when test is finished      |
        +----------------------+----------------------+---------------------------------+


Introduction
------------

.. comment::

   This document describes the recommended integration tests to perform when integrating the <SWComponent> library into an application project.
   This document is a generic template suitable for most projects. It should still be tailored to the specific application project.


Abbreviations and Definitions
-----------------------------



References
----------

.. comment::

   References Table:

   .. table::

      +------+---------------------------------------------------------------------+---------------+----------------------------+
      | No.  | Document Name                                                       | Date/Revision | Link (if applicable)       |
      +======+=====================================================================+===============+============================+
      | /R1/ |                                                                     |               |                            |               
      +------+---------------------------------------------------------------------+---------------+----------------------------+
      | /R2/ |                                                                     |               |                            |
      |      |                                                                     |               |                            |
      +------+---------------------------------------------------------------------+---------------+----------------------------+


Test Environment
----------------

.. comment::

   This chapter defines the test environment.



Hardware
~~~~~~~~

.. comment::

   Hardware Specifications.


Default Preconditions
~~~~~~~~~~~~~~~~~~~~~



Test Case Formalism
-------------------

.. comment::

   Test case types are defined here according "CA0505005 Testing Method" /R1/.


Test Type Identifier
~~~~~~~~~~~~~~~~~~~~

.. table::

   +-------------------------+----------------------------------+-------------------------------------+
   | Test Technique Type     | Test Technique                   | Comment                             |
   +=========================+==================================+=====================================+
   | Systematic Technique    | Sequence Diagram Testing (SeqDT) |                                     |
   +-------------------------+----------------------------------+-------------------------------------+



Test Case Specification
-----------------------



Purpose
~~~~~~~

.. comment:: 

   Check that <functionality> is performed and/or works as intended.


Test Description
~~~~~~~~~~~~~~~~

.. test:: 

   :c_Test_Precondition:      - Precondition 1;
                              - Precondition 2;

                                ...
                                
   :c_Test_Procedure:         1)Step 1 of test procedure.
                              2)Step 2 of test procedure.
                              3)...         


   :c_Test_Expected_Results:  1)Expected result 1.
                              2)Expected result 2.
                              3)...


   :c_Test_Postcondition:     Test postconditions.
   :c_Test_Comment:           Test comment.
   :c_TC_Maturity:            
   :c_Priority:               
   :c_Auto_Man:               
   :p_Link_Software_Architecture:   
   :p_Link_Integration_Requirement:  

   **Pre-Condition:**

   - Precondition 1;
   - Precondition 2;

     ...

   **Test Procedure:**

   1) Step 1 of test procedure;
   2) Step 2 of test procedure;

      ...

   **Expected Result:**

   1) Expected result 1;
   2) Expected result 2;

      ...

   **Comments:**

   - Test Comment.




