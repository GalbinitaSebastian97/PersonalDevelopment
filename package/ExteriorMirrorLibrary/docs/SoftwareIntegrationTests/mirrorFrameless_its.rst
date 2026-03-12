
Frameless fold is triggered
***************************

Purpose
-------

.. comment::

   Verify the successful initiation, execution, and completion of the Frameless Fold movement, reaches to the Soft Stop threshold without system errors.

Test description
----------------s

.. test::
   :c_Test_Precondition:    1. Software is in steady-state execution.
                            2. Valid positioning feedback is active for both Horizontal and Vertical axes via EmblCtrl_Read_PositioningStatus.
                            3. System state is IDLE (no active motor movement).
                            4. Parameter embl_ctrl_FramelessFoldPositionOffSet is configured to 0.5V.
                            5. No mechanical blockage is reported on the Horizontal axis via EmblCtrl_FoldMotor_Read_StopReason.
   :c_Test_Procedure:       1. Trigger a Fold Request by setting EmblCtrl_Read_embl_MirrorFoldCmd to MIRRFLD_COMMAND_FOLD.
   :c_Test_Expected_Results:    1. Software execution pauses at the designated EmblCtrl_Read_embl_MirrorFoldCmd breakpoint, confirming signal reception.
                                2. The system initiates motor actuation for the folding sequence.
                                3. Motor movement terminates autonomously upon reaching the calculated Soft Stop threshold.
                                4. Current glass coordinates are continuously updated on EmblCtrl_ActPos_Write_GlassPosX and EmblCtrl_ActPos_Write_GlassPosY.
   :c_Test_Postcondition:         n/a
   :c_Test_Comment:
   :c_TC_Maturity:                Accepted
   :p_Link_Software_Architecture: #TBD#
   :p_Link_Integration_Requirement: 

   **Pre-Condition:**

    1. Software is in steady-state execution.
    2. Valid positioning feedback is active for both Horizontal and Vertical axes via EmblCtrl_Read_PositioningStatus.
    3. System state is IDLE (no active motor movement).
    4. Parameter embl_ctrl_FramelessFoldPositionOffSet is configured to 0.5V.
    5. No mechanical blockage is reported on the Horizontal axis via EmblCtrl_FoldMotor_Read_StopReason.

   **Test Procedure:**

    1. Trigger a Fold Request by setting EmblCtrl_Read_embl_MirrorFoldCmd to MIRRFLD_COMMAND_FOLD.

   **Expected Result:**

    1. Software execution pauses at the designated EmblCtrl_Read_embl_MirrorFoldCmd breakpoint, confirming signal reception.
    2. The system initiates motor actuation for the folding sequence.
    3. Motor movement terminates autonomously upon reaching the calculated Soft Stop threshold.
    4. Current glass coordinates are continuously updated on EmblCtrl_ActPos_Write_GlassPosX and EmblCtrl_ActPos_Write_GlassPosY.


Frameless unfold is triggered
*****************************

Purpose
-------

.. comment::

   Verify that the system correctly retrieves the glass positions previously stored in NvM and uses them as target setpoints during an Unfold sequence.
   
Test description
----------------

.. test::
   :c_Test_Precondition:    1. Software is in steady-state execution.
                            2. Mirror is in the Folded state.
                            3. Valid positioning feedback is active via EmblCtrl_Read_PositioningStatus.
                            4. NvM contains previously persisted glass coordinates (e.g., X=1500, Y=1500).
   :c_Test_Procedure:       1. Trigger an Unfold command by setting the interface EmblCtrl_Read_embl_MirrorFoldCmd to MIRRFLD_COMMAND_UNFOLD.
   :c_Test_Expected_Results:    1. The system retrieves the saved coordinates (1500, 1500) from NvM.
                                2. The Unfold sequence initiates and moves both axes toward the retrieved NvM setpoints.
                                3. The movement terminates upon reaching the exact persisted coordinates.
                                4. MirrorFoldCmdAbortReason is set to ABORT_REASON_POS_REACHED.
   :c_Test_Postcondition:         n/a
   :c_Test_Comment: This test ensures the Unfold logic prioritizes NvM data over default parameters when valid data exists.
   :c_TC_Maturity:                Accepted
   :p_Link_Software_Architecture: #TBD#
   :p_Link_Integration_Requirement: 

   **Pre-Condition:**

    1. Software is in steady-state execution.
    2. Mirror is in the Folded state.
    3. Valid positioning feedback is active via EmblCtrl_Read_PositioningStatus.
    4. NvM contains previously persisted glass coordinates (e.g., X=1500, Y=1500).

   **Test Procedure:**

    1. Trigger an Unfold command by setting the interface EmblCtrl_Read_embl_MirrorFoldCmd to MIRRFLD_COMMAND_UNFOLD.

   **Expected Result:**

    1. The system retrieves the saved coordinates (1500, 1500) from NvM.
    2. The Unfold sequence initiates and moves both axes toward the retrieved NvM setpoints.
    3. The movement terminates upon reaching the exact persisted coordinates.
    4. MirrorFoldCmdAbortReason is set to ABORT_REASON_POS_REACHED.