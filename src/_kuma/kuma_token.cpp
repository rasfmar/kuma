#include "kuma_token.hpp"
#include <sstream>
#include <iostream>

kuma_token::kuma_token(kuma_type type, void *value) {
  // std::cout << (type == kuma_type::NUMBER ? *(long*)value : 123) << std::endl;
  this->type = type;
  this->value = value;
  // std::cout << (this->type == kuma_type::NUMBER ? *(long*)this->value : 456) << std::endl;
}

kuma_token::kuma_token() {
  kuma_token(kuma_type::_NULL, nullptr);
}

kuma_token::kuma_token(const std::string &&str) {
  kuma_token(kuma_type::STRING, new std::string(str));
}

kuma_token::kuma_token(const long &&num) {
  // std::cout << num << std::endl;
  kuma_token(kuma_type::NUMBER, new long(num));
}

kuma_token::kuma_token(std::string* const str) {
  kuma_token(kuma_type::STRING, str);
}

kuma_token::kuma_token(long* const num) {
  kuma_token(kuma_type::NUMBER, num);
}

std::string kuma_token::kuma_type_to_string(kuma_type type) {
  switch (type) {
    case _NULL: return "NULL";
    case IDENTIFIER: return "IDENTIFIER";
    case NUMBER: return "NUMBER";
    case STRING: return "STRING";
    case EQUALS: return "EQUALS";
    default: return "UNKNOWN";
  }
}

std::string kuma_token::to_string() {
  std::stringstream stream;
  stream << kuma_type_to_string(this->type);

  switch (this->type) {
    case IDENTIFIER:
      stream << "::" << *(std::string *)value;
      break;
    case NUMBER:
      stream << "::" << *(long *)value;
      break;
    case STRING:
      stream << "::" << *(std::string *)value;
      break;
    default:
      break;
  }

  std::string str;
  stream >> str;
  return str;
}
