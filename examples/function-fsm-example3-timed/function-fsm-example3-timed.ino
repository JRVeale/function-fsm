//Example 3: Blink using a Finite State Machine with timed transitions

#include <FunctionFSM.h>

//fsm state functions
void a_on_enter(){
  digitalWrite(LED_BUILTIN, LOW);
}

void b_on_enter(){
  digitalWrite(LED_BUILTIN, HIGH);
}

//fsm states (initialise in MyClass constructor)
FunctionState state_a(&a_on_enter, nullptr, nullptr);
FunctionState state_b(&b_on_enter, nullptr, nullptr);

//fsm (initialise with MyClass constructor)
FunctionFsm fsm(&state_a);


void initfsm(){
//define timed transitions between states in a similar manner as triggered transitions
//myfsm.add_timed_transition(&state_from, &state_to, time_until_transition_ms, &func_on_transition
  fsm.add_timed_transition(&state_a, &state_b, 1000, nullptr);
  fsm.add_timed_transition(&state_b, &state_a, 500, nullptr);
  //You can mix regular and timed transitions to achieve timeouts simply
}

void setup(){
  Serial.begin(19200);
  Serial.flush();
  
  pinMode(LED_BUILTIN,OUTPUT);
  
  //init fsm
  initfsm();
}

void loop(){
  fsm.run_machine();
}
