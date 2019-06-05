/*------------------------------------------------*\
 * FunctionFSM                                    *
 * By JRVeale                                     *
 * 7th May 2019                                   *
 *                                                *
 * An FSM libary based upon arduino-fsm that uses *
 * std::function<void()> instead of raw function  *
 * pointers. This will allow FSMs to be used      *
 * inside parent classes.                         *
 *                                                *
 * (c) James Veale 2019                           *
\*================================================*/

#pragma once

#include <functional>
#include <vector>
#include <chrono>

struct FunctionState {
  FunctionState(std::function<void()> on_enter,
                std::function<void()> on_state,
                std::function<void()> on_exit);
  std::function<void()> on_enter;
  std::function<void()> on_state;
  std::function<void()> on_exit;
};

class FunctionFsm {
public:
  FunctionFsm(FunctionState* initial_state);
  ~FunctionFsm();
  
  void add_transition(FunctionState* state_from,
                      FunctionState* state_to,
                      int event,
                      std::function<void()> on_transition);
  
  void add_timed_transition(FunctionState* state_from,
                            FunctionState* state_to,
                            unsigned long interval,
                            std::function<void()> on_transition);
  
  void check_timed_transitions();
  
  void trigger(int event);
  void run_machine();
  
  FunctionState& get_current_state(){
    return *m_current_state;
  }
  
  bool is_in_state(FunctionState &s){
    FunctionState current = get_current_state();
    return &s == m_current_state;
  }

private:
  struct Transition {
    FunctionState* state_from;
    FunctionState* state_to;
    int event;
    std::function<void()> on_transition;
  };
  
  struct TimedTransition {
    Transition transition;
		std::chrono::time_point<std::chrono::high_resolution_clock> start;
    unsigned long interval;
  };
  
  static Transition create_transition(FunctionState* state_from,
                                      FunctionState* state_to,
                                      int event,
                                      std::function<void()> on_transition);
  
  void make_transition(Transition* transition);

private:
  FunctionState* m_current_state;
  
  std::vector<Transition> m_transitions;
  std::vector<TimedTransition> m_timed_transitions;

  bool m_initialized;
};
