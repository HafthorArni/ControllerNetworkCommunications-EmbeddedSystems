#ifndef STATE_H
#define STATE_H
#include <Context.h>

class Context; // Forward declaration of context

class State
{
  /**
   * @var Context
   */

protected:
  Context *context_;

public:

  virtual ~State();

  void set_context(Context *context);

  virtual void on_do() = 0;
  virtual void on_entry() = 0;
  virtual void on_exit() = 0;
  virtual void on_command_go() = 0;
  virtual void on_command_stop() = 0;
  virtual void on_timeout() = 0;

  // ...

};

#endif