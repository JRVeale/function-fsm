//Example 1: A simple Finite State Machine. Toggles the built-in LED whenever
//a single char is received by Serial.

#include <FunctionFSM.h>

//fsm triggers
enum Trigger {
  TOGGLE_SWITCH
  //You can add more triggers here...
};

//fsm state functions
void a_on_enter() {
  digitalWrite(LED_BUILTIN, LOW);
}

void b_on_enter() {
  digitalWrite(LED_BUILTIN, HIGH);
}

//fsm states
//define states with FunctionState state(&func_on_enter_state, &func_in_state, &func_on_exit_state)
//you can use nullptr where no function is required

FunctionState state_a(&a_on_enter, nullptr, nullptr);
FunctionState state_b(&b_on_enter, nullptr, nullptr);
//You can have as many states as you'd like


//fsm
//define the fsm with the state it will start in

FunctionFsm fsm(&state_a);
//You can run as many state machines as you'd like


void initfsm() {
//define transitions between states with
//myfsm.add_transition(&state_to_transition_from, &state_to_transition_to, &trigger, &func_on_transition
//you can use nullptr where no transition function is required

//it's useful to group all the add_transitions together so they can be easily called as needed

  fsm.add_transition(&state_a, &state_b, TOGGLE_SWITCH, nullptr);
  fsm.add_transition(&state_b, &state_a, TOGGLE_SWITCH, nullptr);

}

void setup() {
  Serial.begin(19200);
  Serial.flush();
  
  pinMode(LED_BUILTIN,OUTPUT);
  
  //init fsm
  initfsm();
}

void loop() {

  fsm.run_machine();
  
  if(Serial.available()){
    Serial.println("\r\n");
    Serial.read();
        
    //trigger global fsm transition
    fsm.trigger(TOGGLE_SWITCH);
  }
}
