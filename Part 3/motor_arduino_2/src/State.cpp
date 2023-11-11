#include <State.h>


  State::~State()
  {
  }

  void State::set_context(Context *context)
  {
    this->context_ = context;
  }


  // ...
