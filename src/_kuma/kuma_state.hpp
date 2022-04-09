#ifndef __KUMA_STATE
#define __KUMA_STATE

#include <stack>
#include <map>
#include "kuma_token.hpp"

struct kuma_state {
  std::stack<kuma_token*> *stack;
  std::map<std::string, kuma_token*> *map;
  kuma_state();
  // ~kuma_state();
};

#endif