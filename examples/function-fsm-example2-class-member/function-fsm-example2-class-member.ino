//Example 2: Objects that hold a FunctionFSM as an instance variable.

#include <FunctionFSM.h>

class MyClass {
private:
  //class variables
  char a = 'a';
  char b = 'b';
  unsigned long last_print = 0;
  unsigned long interval = 1000;
  
  
  //fsm triggers
  enum Trigger {
    TOGGLE_SWITCH
    //can add more triggers here...
  };

  //fsm state functions
  void a_on_enter(){
    Serial.print("Entering a: ");
  }
  void a_on() {
    if ( millis() - last_print > interval ){
      Serial.print(a);
      last_print = millis();
    }
  }
  void a_on_exit() { Serial.println(" - exitting a. "); }
  void a_on_trans_b() { Serial.println("Moving from a to b."); }
  
  void b_on_enter() {
    Serial.print("Entering b: ");
  }
  void b_on() {
    if ( millis() - last_print > interval ){
      Serial.print(b);
      last_print = millis();
    }
  }
  void b_on_exit() { Serial.println(" - exitting b. "); }
  void b_on_trans_a( ){ Serial.println("Moving from b to a."); }
  
  //fsm states (initialise in MyClass constructor)
  FunctionState state_a;
  FunctionState state_b;
  
  //fsm (initialise with MyClass constructor)
  FunctionFsm fsm;
  
public:
  MyClass()
  : state_a([this]() { a_on_enter(); },
            [this]() { a_on(); },
            [this]() { a_on_exit(); }),
            
    state_b([this]() { b_on_enter(); },
            [this]() { b_on(); },
            [this]() { b_on_exit(); }),
    
    fsm(&state_a)
  {}
  
  void init() {
    //add triggers
    fsm.add_transition(&state_a, &state_b, TOGGLE_SWITCH, [this]() { a_on_trans_b(); } );
    fsm.add_transition(&state_b, &state_a, TOGGLE_SWITCH, [this]() { b_on_trans_a(); } );
	
	//other class init stuff...
  }
  
  void update() {
    fsm.run_machine();
	
	//other class update stuff
  }
  
  void toggleFsm(){
    fsm.trigger(TOGGLE_SWITCH);
  }
  
};

MyClass myclass;


void setup() {
  Serial.begin(19200);
  Serial.flush();
    
  //init class fsm (its states control Serial prints)
  myclass.init();
  
}

void loop() {
  
  myclass.update();
  
  if(Serial.available()) {
    Serial.println("\r\n");
    Serial.read();
    
    //trigger class fsm transition
    myclass.toggleFsm();
  }
}
