@startuml
|Application|
start
:T0: User presses button;

:T1: Call proxy.requestFold();

|Skeleton|
:T1: Receive skeleton.requestFold();

:T2: Call controller.requestFold();

|Controller|
:T2: Store request;
note right
  FramelessFoldCmd_e = FOLD
  (State stored!)
end note

:T3: Return true;

|Skeleton|
:T3: Return result to proxy;

|Application|
:T3: Request accepted!;

note 
  ... time passes (next cycle) ...
end note

|Controller|
:T4: MainTask() called;

:T5: Execute mirrorControl();
note right
  Reads: FramelessFoldCmd_e
  Detects: FOLD command!
end note

:T6: Call triggerFoldMovement();
note right
  - Calculate positions
  - Send motor commands
end note

:T7: Update state;
note right
  FramelessFoldActiveCmd_e = FOLD
  FramelessAutoAdjCmd_e = ON
end note

note
  ... movement in progress ...
end note

:T8: MainTask() called again;
:Execute mirrorControl();
:Check movement status;

:T9: Movement complete!;
note right
  FramelessFoldAbortReason_e = POS_REACHED
  FramelessFoldCmd_e = IDLE
end note

|Skeleton|
:T10: Call skeleton.processCycle();
:Detect abort reason change;

:T11: Notify subscribers;

|Application|
:T11: Event received!;
note right
  "Fold complete"
end note

stop

@enduml