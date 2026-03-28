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

.. reqAttributeEnumType:: c_Architectural_Element_Type
    :design_decision: 0                  /* Denotes that the DOORS object is a design decisions that the architect took and documented in the architecture. */
    :category: 1                         /* Denotes that the DOORS object is a category (model element type) in the Rhapsody architecture model. */
    :package: 2                          /* Denotes that the DOORS object is a package in the Rhapsody architecture model. */
    :diagram: 3                          /* Denotes that the DOORS object is a diagram in the Rhapsody architecture model. */
    :table: 4                            /* Denotes that the DOORS object is a table in the Rhapsody architecture model. */
    :matrix: 5                           /* Denotes that the DOORS object is a matrix in the Rhapsody architecture model. */
    :component: 6                        /* Denotes that the DOORS object corresponds to a physical component in the Rhapsody architecture model. */
    :component_configuration: 7          /* reserved for future use */
    :requirement_refinement_rationale: 8 /* Denotes that the DOORS object corresponds to one (of several) refinements that the architect defines in the Rhapsody model, if a requirement cannot be                                                 allocated unambiguously to one physical component. */
    :sw_unit: 9                          /*Denotes that the DOORS object corresponds to a software unit in the Rhapsody architecture model.*/
    :release: 10                         /* Defines a concrete release with label (e.g., A-Sample) and date etc. */
    :release_planning: 11                /*Contains the status of one Architectural Activity.*/
    :verification_criterion: 12          /*Denotes that this element provides additional information for the system integration tests for the corresponding integration step*/
    :interface: 13                      

.. reqAttributeEnumType:: c_Technology
    :RF:                   0 /* Radio Frequency */
    :not_applicable:       1 /* Used for requirements which do not have an associated Technology area */

.. reqAttributeEnumType:: c_Auto_Man
    :manual: 0                /*TC will be executed manually (TC isn't automated)*/
    :to_automate: 1           /*TC is currently manual but is planned to be automated*/
    :in_implementation: 2     /*TC will be automated (TC is currently in work and not executable)*/
    :semi_automated: 3        /*TC will be executed partially automatic; manual execution still needed*/
    :automatic: 4             /*TC will be executed automatically (TC is currently automated and executable)*/

.. reqAttributeEnumType:: c_Binding_Regulation
    :#TBD#:           0 /*Default setting: The object has not been classified yet according to regulation status. */
    :LTR:             1 /*The requirement covers a Legal Technical Regulation*/
    :OBD:             2 /*The requirement is OBD (On Board Diagnosis)  relevant*/
    :Standard:        3 /*The requirement covers a Standard*/
    :no_regulation:   4 /*The requirement is not forced by a Legal Technical Regulation.*/
    :N/A:             5 /*A classification is not applicable, because the object is not a requirement (e.g. Heading, ...). Requirements shall not be classified with N/A.*/

.. reqAttributeEnumType:: c_Compliance
    :Yes:                0 /*this value indicates the compliance with customer requests*/
    :Yes_with_deviation: 1 /*this value indicates the compliance with customer requests, but it will not be implemented exactly as the customer requested. Deviations shall be stated in c_Remark attribute.*/
    :No:                 2 /*this value indicates the non compliance with customer requests. It requires a reason in c_Remark.*/
    :unclear:            3 /*this value indicates that requests are in discussion with customer. It requires a reason in c_Remark.*/
    :N/A:                4 /*in case of c_Compliance the value "N/A" has two meanings: 1) The object is not a requirement; 2) The object is a requirement, but it is not requested to AUMOVIO (addressed to another supplier)*/
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
   :proposed:                  1 /* the requirement / test case is worked out and ready for project review */
   :in_work:                   2 /* the requirement / test case is under change */
   :project_accepted:          3 /* the requirement / test case is reviewed and accepted by the project team */
   :stakeholder_accepted:      4 /* the requirement / test case has positively passed the review with the customer */
   :invalid:                   5 /* the requirement / test case has been removed by project team or customer, or was explicitly set to invalid (for instance instead of deleting it). */

.. reqAttributeEnumType:: c_Problem_Source
    :internal:              0 /* problem was found internally */
    :external:              1 /* problem was found externally (e.g. by customer) */

.. reqAttributeEnumType:: c_implementation_status
   :#TBD#:                  0
   :not_implemented:        1
   :in_implementation:      2
   :implemented:            3
   :N/A:                    4

.. reqAttributeEnumType:: c_Result_Test_Run
    :passed:                0  /* Test case has passed successfully */
    :failed:                1  /* Test case has failed */
    :not_done:              2  /* Test case was not executed */
    :TC_complaint:          3  /* Test case could not be executed properly */

.. reqAttributeEnumType:: c_Security_and_Privacy
    :#TBD#:                 0 /* Default setting: The object was not yet classified according to SPR. */
    :SPR:                   1 /* The requirement is security or privacy relevant */
    :not_SPR:               2 /* The requirement is not security or privacy relevant */
    :N/A:                   3 /* A classification according to SPR does not make sense, because it is no binding requirement (e.g. Headings,...). Requirements are not allowed to be classified with NA.*/

.. reqAttributeEnumType:: c_test_severity
   :#TBD#:        0
   :top:          1
   :high:         2
   :medium:       3
   :low:          4
   :N/A:          5

.. reqAttributeEnumType:: c_verification_method
   :#TBD#:        0
   :review:       1 /*This value specifies the verification method of review. It shall be used to check that the requirement is fulfilled or the architectural element is realized.*/
   :VT:           2 /*This value specifies the test environment of a Vehicle Verification or Vehicle System Verification to be used for verifying the requirement. The verification method for this test environment is testing.*/
   :VIT:          3 /* This value specifies the test environment of a Vehicle Integration Verification or Vehicle System Integration Verification to be used for verifying the corresponding architectural element. The verification method for this test environment is testing.*/
   :SyT:          4 /* This value specifies the test environment of a System Verification to be used for verifying the system requirement. The verification method for this test environment is testing.*/
   :SyIT:         5 /* This value specifies the test environment of System Integration Verification used to verify the integration of system elements as part of the system architecture. The verification method for this test environment is testing.This value is used for architectural elements (interfaces, models, diagrams, design decisions...). */
   :SWT:          6 /* This value specifies the test environment of Software Verification to be used for verifying the software requirement. The verification method for this test environment is testing.*/
   :SWIT:         7 /* This value specifies the test environment of Software Integration Verification used to verify the integration of software elements as part of the software architecture and detailed design. The verification method for this test environment is testing. This value is used for architectural elements (interfaces, models, diagrams, design decisions...). */
   :SWMT:         8 /* This value specifies the test environment of Software Module Verification to be used for verifying the software component. The verification method for this test environment is testing.*/
   :EET:          9 /* This value specifies the test environment of Electrical Engineering Verification to be used for verifying the requirement. The verification method for this test environment is testing.*/
   :EEIT:         10 /* This value specifies the test environment of Electrical Engineering Integration Verification to be used for verifying the architectural design (modules and their interaction/interfaces). The verification method for this test environment is testing.*/
   :EEMT:         11 /* This value specifies the test environment of Electrical Engineering Module Verification to be used for verifying the module design (architecture and details). The verification method for this test environment is testing.*/
   :MDT:          12 /* This value specifies the test environment of Mechanical Engineering Verification to be used for verifying the design (architecture and details) and requirements. The verification method for this test environment is testing.*/
   :N/A:          13
   :DV:           14 /* This value specifies that the requirement shall be tested with Design Validation (using prototype samples). */
   :PV:           15 /* This value specifies that the requirement shall be tested with Product Validation (using pre-series production samples). */
   :PT:           16 /* This value specifies that the requirement shall be tested with Production Test (by testing the production line). */
   :measurement:  17 /* This value specifies the verification method of using measurements on a physical sample of a design to verify the intended characteristics of a design. Examples: measure amplification factor, measure dimensions, CPU load */
   :analysis:     18 /* This value specifies the verification method of (manually) setting up an analysis with documentation of each calculation/processing step and simplifications applied to ensure that a design will function correctly in given operating environment conditions. */
   :simulation:   19 /* Examples: worst case analysis, tolerance chain analysis, NvM lifetime based on expected write cycles */

.. reqAttributeEnumType:: c_Priority
   :#TBD#:    0
   :T:        1
   :H:        2
   :M:        3
   :L:        4
   :n/a:      5

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
   :defaultValue: QM /* refer to ISO 26262 for values definitions */ 

.. reqAttributeEnumType:: c_Discipline
    :SY:              0 /* Requirement is assigned to "System". This value is necessary to distinguish between system and discipline requirements. */
    :SW:              1 /* Requirement is assigned to development discipline SW */
    :EE:              2 /* Requirement is assigned to development discipline EE (Electrical Engineering) */
    :ME:              3 /* Requirement is assigned to development discipline ME (Mechanical Engineering) */
    :other:           4 /* Requirement is assigned to an area which is not covered by the other defined discipline values. This value  shall only be used for requirements which are not satisfied by the product itself (e.g. covered by a discipline like "process"). In case the value "other" is used, the development area which shall satisfy the requirement (PM, Testing, Quality, Manufacturing, Process) shall be documented in the c_Remark attribute. An evidence that the requirement is satisfied by the target area has to be available.
                           This assignment and coverage by the related development area shall be solved already in the STR as far as possible (principle: requests shall be forwarded to related development area as early as possible). However, it can be necessary to cover also these stakeholder requirements by system requirements, for instance:
                            - if stakeholder requirement is not completely satisfied by the assigned development area (e.g. stakeholder requirement not atomic)  
                            - if project decides to handle artifacts like customer provided Acceptance Tests as system requirements.
                           In an STR, the value "other" can be combined with SY or one discipline value, if the requirement impacts both areas. In this case, the SY or discipline specific part has to be handled in SYR or discipline requirements according to the general RE approach. Here the development areas shall be separated and the value "other" shall not be combined with the other discipline values anymore.*/

.. reqattribute:: c_Verification_Method
   :type: c_verification_method
   :defaultValue: SWT

.. reqattribute:: c_Maturity
   :type: c_maturity
   :defaultValue: project_accepted

.. reqattribute:: c_Implementation_Status
    :not_implemented:      0 /* the requirement is not yet implemented */
    :implemented:          1 /* the requirement has been implemented, module test has been performed. */

.. reqattribute:: c_Test_Severity
   :type: c_test_severity
   :defaultValue: medium

.. reqAttributeEnumType:: c_Test_Category
    :BounVal_Systematic:            0 /* Boundary Value Systematic */
    :EqCls_Systematic:              1 /* Equivalence classes */
    :SBT_Systematic:                2 /* State Based Testing */
    :ADT_Systematic:                3 /* Activity Diagram Testing */
    :DTT_Systematic:                4 /* Decision Table Testing */
    :BounValDoc_Systematic:         5 /* Boundary values (for equivalence classes) documented */
    :EqClsDoc_Systematic:           6 /* Equivalence classes documented */
    :CTE_Systematic:                7 /* Classification Tree Analysis / Classification Tree Editor */
    :CEA_Systematic:                8 /* Cause Effect Analysis */
    :OpScen_Systematic:             9 /* Operational Scenario Tests */
    :Stress_Robustness_Supporting: 10 /* Stress and Robustness Testing */
    :PerfTest_Supporting:          11 /* Performance Testing */
    :FaultIns_Supporting:          12 /* Fault insertion from FMEA / FMEDA */
    :BTB_Supporting:               13 /* Back to Back Testing */
    :CoAn_Supporting:              14 /* Coverage Analysis */
    :ErrGu_Intuitive:              15 /* Error Guessing */
    :Checklists_LessonsLearned:    16 /* Checklists and Lessons Learned */
    :FailTrack_LessonLearned:      17 /* SYAR coverage from own project */
    :FailAna_LessonLearned:        18 /* SYAR coverage from previous projects */
    :CTA_Special:                  19 /* Critical Timing Analysis */
    :ConTest_Special:              20 /* Conformance Testing */

.. reqAttributeEnumType:: c_Testmanagers_Choice
    :Yes:                           0 /* Execute in any case */
    :No:                            1 /* Do not execute in any case */
    :dont_care:                     2 /* No choice specified */

.. reqattribute:: c_Change_Request
   :type: Text

.. reqAttributeEnumType:: c_Information_Security
    :confidential:           0 /* Information whose disclosure may result in significant financial damage, legal consequence or damage to the companies reputation. */
    :internal:               1 /* these objects may contain some know-how or related information, which should not be provided to customer*/
    :public:                 2 /* access to this data is unlimited. It is used to build the Target Specification, which is actually the contract with the customer*/

.. reqAttributeEnumType:: c_This_is_a
    :#TBD#:                      0 /* Default setting: The object was not yet classified */
    :arhitectural_element:       1 /* this value is used for architectural elements (models, diagrams, design decisions...). To be used only in Architecture modules. */
    :functional_requirement:     2 /* this value is used for functional requirements objects. Functional requirements specify a function, a behavior that a system must be able to perform */
    :non_functional_requirement: 3 /* this value is used for non-functional requirements objects. Non-functional requirements can not be associated with a specific function of the system, they are valid for multiple functions or the whole system. They are generally properties which do not describe functionality but which have to be covered by the system (standards, performance, maintainability, reliability, reusability...) */
    :heading:                    4 /* this value is used for objects which are not requirements but just headings to structure the specification. */
    :comment:                    5 /* this value is used for objects which are not processed as requirements or test cases */
    :feature:                    6 /* this value is used for features. To be used only in Feature List modules. */
    :definition:                 7 /* Denotes that the DOORS object is a "Definition Object". In Definition Objects, values or terms are assigned to a placeholder which is used typically in several requirements. More details in main document CA0500035. */
    :constant:                   8 /* labels for values relevant for testing (e.g. test system related values, system delays) */
    :function:                   9 /* labels for the internal software functions. Might be necessary in verification criteria for the requirements which shall be tested at “white box” level by accessing SW internal functions, setting breakpoints, etc.*/
    :parameter:                  10 /* this value it's used in DFR modules to define parameter item types.  */
    :signal:                     11 /* A signal is a piece of information exchanged between system elements that represents a specific value or state at a given time.  */
    :variable:                   12 /* labels for the internal software variables. Might be necessary in verification criteria for the requirements which shall be tested at “white box” level by accessing SW internal variables. */
    :recommendation:             13 /* this value is used for recommendation objects, in case of generic/platform projects. Recommendation can be used to indicate that
                                      - among several possibilities, one is recommended as particularly suitable, without mentioning or excluding the others.
                                      - a certain course of action is preferred but not necessarily required.
    :reference:                  14 /* this value is used for making a reference to requirements which are defined in separate modules */
    :result:                     15 /* this value is used for result objects for transferred results of test cases (from test automation) */
    :test_case:                  16 /* this value is used for test case objects */
    :test_case_gen:              17 /* this value is used for test case objects generated by the TCA (from test case automation) */
    :test_idea:                  18 /* User scenario without implementation */
    :test_pattern:               19 /* Parametrized generic test scenario */
    :test_vector:                20 /* Test vestors/data for test cases */
    :test_step:                  21 /* stimulation and verification actions (testing related) */

.. reqattribute:: p_Link_System_Architecture
   :type: Text

.. reqattribute:: p_Link_Software_Architecture
   :type: Text

.. reqattribute:: p_Link_SYR_Id
   :type: Text

.. reqattribute:: c_Release
   :type: c_release
   :defaultValue: N/A

.. reqattribute:: c_Definition_Name
   :type: Text

.. reqattribute:: c_Definition_BriefDoc
   :type: Text

.. reqattribute:: c_Definition_Min_Value
   :type: Text

.. reqattribute:: c_Definition_Max_Value
   :type: Text

.. reqattribute:: c_Definition_Min_Resolution
   :type: Text

.. reqattribute:: c_Definition_Value
   :type: Text

.. reqattribute:: c_Definition_Default_Value
   :type: Text

.. reqattribute:: c_Definition_Constant_Value
   :type: Text

.. reqattribute:: c_Definition_Unit
   :type: Text

.. reqattribute:: Description
   :type: Text
