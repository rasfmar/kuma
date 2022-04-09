#include "kuma.hpp"

using namespace kuma;

std::string kuma::type_to_str(token_type type) {
  switch (type) {
    case _EOF: return "EOF";
    case _NULL: return "NULL";
    case IDENTIFIER: return "IDENTIFIER";
    case NUMBER: return "NUMBER";
    case STRING: return "STRING";
    case ARRAY: return "ARRAY";
    case BOOLEAN: return "BOOLEAN";
    case SCOPE: return "SCOPE";
    case OP_SET_EQUALS: return "OP_SET_EQUALS";
    case OP_ADD: return "OP_ADD";
    case OP_SUBTRACT: return "OP_SUBTRACT";
    case OP_START_ARRAY: return "OP_START_ARRAY";
    case OP_END_ARRAY: return "OP_END_ARRAY";
    case OP_START_ARGS: return "OP_START_ARGS";
    case OP_END_ARGS: return "OP_END_ARGS";
    case OP_START_SCOPE: return "OP_START_SCOPE";
    case OP_END_SCOPE: return "OP_END_SCOPE";
    case OP_EXECUTE: return "OP_EXECUTE";
    case OP_ACCESS: return "OP_ACCESS";
    case OP_RAW_ACCESS: return "OP_RAW_ACCESS";
    case OP_IF: return "OP_IF";
    case OP_IF_ELSE: return "OP_IF_ELSE";
    case OP_WHILE: return "OP_WHILE";
    case OP_LESS_THAN: return "OP_LESS_THAN";
    case OP_GREATER_THAN: return "OP_GREATER_THAN";
    case OP_LESS_EQUAL: return "OP_LESS_EQUAL";
    case OP_GREATER_EQUAL: return "OP_GREATER_EQUAL";
    case OP_EQUAL_TO: return "OP_EQUAL_TO";
    case OP_NOT_EQUAL_TO: return "OP_NOT_EQUAL_TO";
    case OP_AND: return "OP_AND";
    case OP_OR: return "OP_OR";
    case OP_BITWISE_AND: return "OP_BITWISE_AND";
    case OP_BITWISE_OR: return "OP_BITWISE_OR";
    case OP_VALUE_OF: return "OP_VALUE_OF";
    case OP_POP: return "OP_POP";
    default: return "UNKNOWN";
  }
}

token::token()
  : type(_NULL), value(nullptr) {};
token::token(int num)
  : type(NUMBER), value(new int(num)) {};
token::token(std::string str)
  : type(STRING), value(new std::string(str)) {};
token::token(std::vector<token> arr)
  : type(ARRAY), value(new std::vector<token>(arr)) {};
token::token(bool bl)
  : type(BOOLEAN), value(new bool(bl)) {};
token::token(token_type type)
  : type(type), value(nullptr) {};
token::token(token_type type, void *value)
  : type(type), value(value) {};
token::~token() {};

std::string token::type_to_str() {
  return kuma::type_to_str(type);
}

std::string token::val_to_str(bool raw) {
  std::stringstream stream;
  switch (type) {
    case STRING:
      if (!raw) stream << "'";
      stream << as_str();
      if (!raw) stream << "'";
      break;
    case IDENTIFIER:
      stream << as_str();
      break;
    case NUMBER:
      stream << as_num();
      break;
    case ARRAY: {
      stream << "[";
      std::vector<token> arr = as_arr();
      for (unsigned long i = 0; i < arr.size(); ++i)
        stream << arr[i].val_to_str() << (i != arr.size() - 1 ? (raw ? " " : ", ") : "");
      stream << "]";
      break;
    }
    case BOOLEAN:
      stream << (as_bool() ? "true" : "false");
      break;
    case SCOPE: {
      stream << "{";
      std::vector<token> arr = as_arr();
      for (unsigned long i = 0; i < arr.size(); ++i)
        stream << arr[i].to_string() << (i != arr.size() - 1 ? " " : "");
      stream << "}";
      break;
    }
    default: break;
  }
  return stream.str(); 
}

std::string token::to_string() {
  std::stringstream stream;
  std::string val = val_to_str();
  stream << type_to_str();
  if (!val.empty())
    stream << "::" << val;
  return stream.str();
}

std::string token::as_str() {
  return *(std::string *)value;
}

int token::as_num() {
  return *(int *)value;
}

std::vector<token> token::as_arr() {
  return *(std::vector<token> *)value;
}

bool token::as_bool() {
  return *(bool *)value;
}

state::state()
  : stack(std::stack<token>()),
    dict(std::map<std::string, token>({
      // {"true", token(BOOLEAN, new bool(true))},
      // {"false", token(BOOLEAN, new bool(false))},
      // {"null", token()}
    }))
  {};

state::state(std::stack<token> stack, std::map<std::string, token> dict)
  : stack(stack), dict(std::map<std::string, token>(dict)) {};

state::~state() {};