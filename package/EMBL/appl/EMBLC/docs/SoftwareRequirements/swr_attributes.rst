#These attributes are reuired in case of exporting the requirements to DOORS, as they are used in the DOORS module. 
#The attributes are defined here and then used in the requirements.

.. reqAttributeEnumType:: c_discipline
   :#TBD#:            0
   :LS:               1
   :SW:               2
   :EE:               3
   :ME:               4
   :manufacturing:    5
   :testing:          6
   :quality:          7
   :N/A:              8

.. reqAttributeEnumType:: c_sil
   :#TBD#:            0
   :QM:               1
   :QM(A):            2
   :QM(B):            3
   :QM(C):            4
   :QM(D):            5
   :ASIL A:           6
   :N/A:              7

.. reqAttributeEnumType:: c_maturity
   :#TBD#:                     0
   :proposed:                  1
   :in_work:                   2
   :project_accepted:          3
   :stakeholder_accepted:      4
   :invalid:                   5
   :N/A:                       6

.. reqAttributeEnumType:: c_implementation_status
   :#TBD#:                  0
   :not_implemented:        1
   :in_implementation:      2
   :implemented:            3
   :N/A:                    4

.. reqAttributeEnumType:: c_test_severity
   :#TBD#:        0
   :top:          1
   :high:         2
   :medium:       3
   :low:          4
   :N/A:          5

.. reqAttributeEnumType:: c_verification_method
   :#TBD#:        0
   :review:       1
   :VT:           2
   :VIT:          3
   :SyT:          4
   :SyIT:         5
   :SWT:          6
   :SWIT:         7
   :SWMT:         8
   :EET:          9
   :EEIT:         10
   :EEMT:         11
   :MDT:          12
   :N/A:          13

.. reqAttributeEnumType:: c_Priority
   :#TBD#:    0
   :T:        1
   :H:        2
   :M:        3
   :L:        4
   :n/a:      5

.. reqAttributeEnumType:: c_Information_Security
   :#TBD#:        0
   :N/A:          1
   :confidential: 2
   :internal:     3
   :public:       4

.. reqAttributeEnumType:: c_release
   :#TBD#:             0
   :N/A:               1
   :V1.0.0:            2
   :V2.0.0:            3
   :V3.0.0:            4
   :V4.0.0:            5
   :V5.0.0:            6
   :V6.0.0:            7
   :V7.0.0:            8
   :V9.0.0:            9
   :V17.0.0:           10
   :V19.0.0:           11

.. reqattribute:: c_Priority
   :type: c_Priority
   :defaultValue: M

.. reqattribute:: c_Sil
   :type: c_sil
   :defaultValue: QM

.. reqattribute:: c_Discipline
   :type: c_discipline
   :defaultValue: SW

.. reqattribute:: c_Verification_Method
   :type: c_verification_method
   :defaultValue: SWT

.. reqattribute:: c_Maturity
   :type: c_maturity
   :defaultValue: project_accepted

.. reqattribute:: c_Implementation_Status
   :type: c_implementation_status
   :defaultValue: implemented

.. reqattribute:: c_Test_Severity
   :type: c_test_severity
   :defaultValue: medium

.. reqattribute:: c_Information_Security
   :type: c_Information_Security
   :defaultValue: internal

.. reqattribute:: p_Link_System_Architecture
   :type: Text

.. reqattribute:: p_Link_Software_Architecture
   :type: Text

.. reqattribute:: p_Link_SYR_Id
   :type: Text

.. reqattribute:: c_Definition_Name
   :type: Text

.. reqattribute:: Description
   :type: Text