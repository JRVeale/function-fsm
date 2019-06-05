/*------------------------------------------------*\
 * FunctionFSM                                    *
 * By JRVeale                                     *
 * 7th May 2019                                   *
 *                                                *
 * An FSM libary based upon arduino-fsm that uses *
 * std::function<void()> instead of raw function  *
 * pointers. This will allow FSMs to be used      *
 * within parent classes.                         *
 *                                                *
 * (c) James Veale 2019                           *
\*================================================*/

#include "FunctionFSM.h"

FunctionState::FunctionState(std::function<void()> on_enter,
                std::function<void()> on_state,
                std::function<void()> on_exit)
: on_enter{std::move(on_enter)},
  on_state{std::move(on_state)},
  on_exit{std::move(on_exit)}
{}

FunctionFsm::FunctionFsm(FunctionState* initial_state)
: m_current_state(initial_state),
  m_transitions(),
  m_initialized(false)
{}

FunctionFsm::~FunctionFsm(){
}

void FunctionFsm::add_transition(FunctionState* state_from,
                                 FunctionState* state_to,
                                 int event,
                                 std::function<void()> on_transition){
  if(state_from == NULL || state_to == NULL) return;
  
  Transition transition = FunctionFsm::create_transition(state_from,
                                                         state_to,
                                                         event,
                                                         on_transition);
  //Replaced pointers and realloc with std::vector
  m_transitions.push_back(transition);
}

void FunctionFsm::add_timed_transition(FunctionState* state_from,
                                       FunctionState* state_to,
                                       unsigned long interval,
                                       std::function<void()> on_transition){
  if (state_from == NULL || state_to == NULL) return;
  
  Transition transition = FunctionFsm::create_transition(state_from,
                                                         state_to,
                                                         0,
                                                         on_transition);
  
  TimedTransition timed_transition;
  timed_transition.transition = transition;
  timed_transition.start = std::chrono::high_resolution_clock::now();
  timed_transition.interval = interval;
  
  m_timed_transitions.push_back(timed_transition);
}

FunctionFsm::Transition FunctionFsm::create_transition(FunctionState* state_from,
                                                       FunctionState* state_to,
                                                       int event,
                                                       std::function<void()> on_transition){
  Transition t;
  t.state_from = state_from;
  t.state_to = state_to;
  t.event = event;
  t.on_transition = on_transition;
  
  return t;
}

void FunctionFsm::trigger(int event){
  if(m_initialized){
    //Find the transition with the current state and given event.
    for (auto& t : m_transitions){
      if (t.state_from == m_current_state && t.event == event) {
        FunctionFsm::make_transition(&t);
        return;
      }
    }
  }
}

void FunctionFsm::check_timed_transitions(){
  for (auto& tt : m_timed_transitions){
    if(tt.transition.state_from == m_current_state){
      std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
      if (std::chrono::duration_cast<std::chrono::milliseconds>(now - tt.start).count() >= tt.interval){
        FunctionFsm::make_transition(&(tt.transition));
        tt.start = std::chrono::high_resolution_clock::now();
      }
    }
  }
}

void FunctionFsm::run_machine(){
  //first run must execute first state "on_enter"
  if (!m_initialized){
    m_initialized = true;
    if (!!m_current_state->on_enter) { //if nullptr (see [func.wrap.func] paragraph 8 in standard)
      m_current_state->on_enter();
    }
  }
  
  if (!!m_current_state->on_state) {  //if nullptr (see [func.wrap.func] paragraph 8 in standard)
    m_current_state->on_state();
  }
  
  FunctionFsm::check_timed_transitions();
}

void FunctionFsm::make_transition(Transition* transition) {
  //Execute handlers in the correct order
  if (!!transition->state_from->on_exit) {  //if not nullptr (see [func.wrap.func] paragraph 8 in standard)
    transition->state_from->on_exit();
  }
  
  if (!!transition->on_transition) {  //if not nullptr (see [func.wrap.func] paragraph 8 in standard)
    transition->on_transition();
  }
  
  if (!!transition->state_to->on_enter) {
    transition->state_to->on_enter();
  }
  
  m_current_state = transition->state_to;
  
  //Initialise all timed transitions from m_current_state
  std::chrono::time_point<std::chrono::high_resolution_clock> now =  std::chrono::high_resolution_clock::now();
  for (auto& tt : m_timed_transitions) {
    if(tt.transition.state_from == m_current_state){
      tt.start = now;
    }
  }
}



















//nosey
