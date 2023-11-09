#include <Context.h>
#include <Arduino.h>


  /**
   * The Context allows changing the State object at runtime.
   */

  void Context::transition_to(State *state)
  {
    Serial.println("Context: Transition");
    if (this->state_ != nullptr)
    {
      this->state_->on_exit();
      delete this->state_;
    }

    this->state_ = state;
    this->state_->set_context(this);
    this->state_->on_entry();
  }

  /**
   * The Context delegates part of its behavior to the current State object.
   */

  void Context::do_work()
  {
    this->state_->on_do();
  }

  void Context::command_go()
  {
    this->state_->on_command_go();
  }

  void Context::command_stop()
  {
    this->state_->on_command_stop();
  }

  void Context::timeout()
  {
    this->state_->on_timeout();
  }

