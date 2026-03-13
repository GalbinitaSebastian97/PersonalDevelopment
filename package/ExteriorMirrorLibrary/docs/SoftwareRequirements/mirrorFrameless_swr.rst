
#TBD:Make Id's auto generated to better find requirmetns

Frameless Mirror Fold Control
=============================

Configuration
-------------

.. comment::

   =======================================    ==============================================================
   Build Option                               Description
   =======================================    ==============================================================
   FRAMELESS_MIRROR_SUPPORT                   Enable/Disable frameless mirror fold functionality.
   =======================================    ==============================================================

Inputs and Outputs
------------------

.. requirement::
   :c_This_is_a: functional_requirement
   :c_Implementation_Status: implemented
   :c_Verification_Method: SWT
   :c_Information_Security: public

   +--------------------------------------+---------------------------+-----------------+-----------------+------------------------------+------------------------------------------+
   | Input Signal Name                    | Unit/Type                 | Range           | Min. resolution | Init value                   | Description                              |
   +======================================+===========================+=================+=================+==============================+==========================================+
   | MirrorFoldCmd                        | t_emblMirrorFoldCmd       | [0, 2]          | 1               | MIRRFLD_COMMAND_IDLE         | Mirror fold command                      |
   +--------------------------------------+---------------------------+-----------------+-----------------+------------------------------+------------------------------------------+
   | MirrorGlassManAdjCmd                 | t_emblGlassManualAdjustCmd| [0, 4]          | 1               | GLASS_ADJUST_CMD_NO_REQUEST  | Mirror glass manual adjust command       |
   +--------------------------------------+---------------------------+-----------------+-----------------+------------------------------+------------------------------------------+

.. requirement::
   :c_This_is_a: functional_requirement
   :c_Implementation_Status: implemented
   :c_Maturity: project_accepted
   :c_Verification_Method: SWT
   :c_Information_Security: public

   +--------------------------------------+-----------------------+-----------------+-----------------+----------------------------+-------------------------------------------------+
   | Output Signal Name                   | Unit/Type             | Range           | Min. resolution | Init value                 | Description                                     |
   +======================================+=======================+=================+=================+============================+=================================================+
   | MirrorFoldCmdAbortReason             | t_emblAbortReason     | [0, 12]         | 1               | ABORT_REASON_NONE          | Abort reason for the mirror fold command        |
   +--------------------------------------+-----------------------+-----------------+-----------------+----------------------------+-------------------------------------------------+

Parameters
----------

.. requirement::
   :c_This_is_a: functional_requirement
   :c_Implementation_Status: implemented
   :c_Maturity: project_accepted
   :c_Verification_Method: SWT
   :c_Information_Security: public

   +----------------------------------------+-----------------------+-----------------+-----------------+----------------------+------------------------------------------------+
   | Parameter Name                         | Unit/Type             | Range           | Min. resolution | Init value           | Description                                    |
   +========================================+=======================+=================+=================+======================+================================================+
   | FramelessFoldPositionOffSetPosition    | uin8                  | [0, 255]        | 0.2V            | 0.5                  | Soft stop position offset.                     |  
   +----------------------------------------+-----------------------+-----------------+-----------------+----------------------+------------------------------------------------+
   | FramelessDrivePosY                     | uint16                | [0, 65535]      | 0V              | 2.5                  | Default driving position for axis Y in case the|
   |                                        |                       |                 |                 |                      | position from NvM is not set                   |
   +----------------------------------------+-----------------------+-----------------+-----------------+----------------------+------------------------------------------------+
   | FramelessDrivePosX                     | uint16                | [0, 65535]      | 0V              | 1                    | Default driving position for axis X in case the|
   |                                        |                       |                 |                 |                      | position from NvM is not set                   |
   +----------------------------------------+-----------------------+-----------------+-----------------+----------------------+------------------------------------------------+

Requirements
------------

.. requirement::
   :c_This_is_a: functional_requirement
   :c_Implementation_Status: implemented
   :c_Maturity: project_accepted
   :c_Verification_Method: SWT
   :c_Information_Security: public

    If a **Fold Request** is received during an active **Automatic** or **Manual Glass Adjustment**, and 
    the parameter *embl_FramelessFoldGlassRequestPriority* is configured to 0 (Glass Adjustment Priority), 
    the EMBL shall:
        - Set the MirrorFoldCmdAbortReason signal to *ABORT_REASON_LOW_PRIO*.
        - Reject the incoming **Fold Request**.

.. requirement::
   :c_This_is_a: functional_requirement
   :c_Implementation_Status: implemented
   :c_Maturity: project_accepted
   :c_Verification_Method: SWT
   :c_Information_Security: public

    Upon the **MirrorFoldCmd** signal transitioning to *MIRRFLD_COMMAND_FOLD*, the EMBL shall initiate mirror 
    movement by asserting the **EmblCtrl_SetMoveRequest** signal for the X-axis to reach the designated center position.

.. requirement::
   :c_This_is_a: functional_requirement
   :c_Implementation_Status: implemented
   :c_Maturity: project_accepted
   :c_Verification_Method: SWT
   :c_Information_Security: public

    Upon the Y-axis reaching the designated center position, the EMBL shall initiate mirror movement by asserting 
    the **EmblCtrl_SetMoveRequest** signal for the X-axis to transition the mirror to the Soft Stop fold position

.. comment::

    A **Soft Stop** is a calibrated threshold utilized during mirror folding to terminate motor actuation 
    prior to reaching the physical mechanical limit **Hard Block**. The EMBL maintains this safety 
    buffer by stopping the movement at a distance defined by the *embl_FramelessFoldPositionOffSet*
    parameter, thereby preventing mechanical stress and hardware wear.

.. requirement::
    :c_This_is_a: functional_requirement
    :c_Implementation_Status: implemented
    :c_Maturity: project_accepted
    :c_Verification_Method: SWT
    :c_Information_Security: public

    Upon the **MirrorFoldCmd** signal transitioning to *MIRRFLD_COMMAND_UNFOLD*, the EMBL shall initiate 
    mirror movement by asserting the **EmblCtrl_SetMoveRequest** signal for the X-axis to reach the 
    horizontal drive position retrieved from the NvM.

.. requirement::
    :c_This_is_a: functional_requirement
    :c_Implementation_Status: implemented
    :c_Maturity: project_accepted
    :c_Verification_Method: SWT
    :c_Information_Security: public

    Upon the X-axis reaching the target horizontal position, the EMBL shall initiate mirror movement 
    by asserting the **EmblCtrl_SetMoveRequest** signal for the Y-axis to reach the vertical drive 
    position retrieved from the NvM.

.. requirement::
    :c_This_is_a: functional_requirement
    :c_Implementation_Status: implemented
    :c_Maturity: project_accepted
    :c_Verification_Method: SWT
    :c_Information_Security: public

    Upon the successful arrival of both the X and Y axes at their respective target positions while 
    a **Fold** or **Unfold** command is active, the EMBL shall assert the **MirrorFoldCmdAbortReason** signal 
    to *ABORT_REASON_POS_REACHED* to signify sequence completion.

.. requirement::
   :c_This_is_a: functional_requirement
   :c_Implementation_Status: implemented
   :c_Maturity: project_accepted
   :c_Verification_Method: SWT
   :c_Information_Security: public

    Upon receiving a **Fold Command**, the EMBL shall persist the current horizontal and vertical 
    mirror glass potentiometer positions to the NvM, provided that a 
    **manual** or **automatic glass adjustment** has occurred since the completion of the previous fold movement.

.. requirement::
    :c_This_is_a: functional_requirement
    :c_Implementation_Status: implemented
    :c_Maturity: project_accepted
    :c_Verification_Method: SWT
    :c_Information_Security: public

    In the event that no valid horizontal or vertical drive positions are available 
    within the NvM, the EMBL shall utilize the default configuration parameters 
    *embl_FramelessDrivePosX* and *embl_FramelessDrivePosY* 
    as the target setpoints for the unfold movement.