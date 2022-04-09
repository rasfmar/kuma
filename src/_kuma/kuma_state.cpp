#include "kuma_state.hpp"

kuma_state::kuma_state() {
  this->stack = new std::stack<kuma_token*>();
  this->map = new std::map<std::string, kuma_token*>();
}
