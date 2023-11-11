#ifndef CONTEXT_H
#define CONTEXT_H
#include <State.h>
#include <Arduino.h>

class State; // Forward declaration of State

class Context
{
  /**
   * @var State A reference to the current state of the Context.
   */

private:
  State *state_;

public:
  Context(State *state) : state_(nullptr)
  {
    this->transition_to(state);
  };

  ~Context()
  {
    delete state_;
  }

  /**
   * The Context allows changing the State object at runtime.
   */

  void transition_to(State *state);

  /**
   * The Context delegates part of its behavior to the current State object.
   */

  void do_work();

  void command_go();

  void command_stop();

  void timeout();

  void on_operational();
};

#endif