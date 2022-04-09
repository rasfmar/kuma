#include <string>
#include <vector>
#include <sstream>
#include <stack>
#include <map>

namespace kuma {
  enum token_type {
    _EOF,
    _NULL,
    IDENTIFIER,
    NUMBER,
    STRING,
    ARRAY,
    BOOLEAN,
    SCOPE,
    OP_SET_EQUALS,
    OP_ADD,
    OP_SUBTRACT,
    OP_START_ARRAY,
    OP_END_ARRAY,
    OP_START_ARGS,
    OP_END_ARGS,
    OP_START_SCOPE,
    OP_END_SCOPE,
    OP_EXECUTE,
    OP_ACCESS,
    OP_RAW_ACCESS,
    OP_IF,
    OP_IF_ELSE,
    OP_WHILE,
    OP_LESS_THAN,
    OP_GREATER_THAN,
    OP_LESS_EQUAL,
    OP_GREATER_EQUAL,
    OP_EQUAL_TO,
    OP_NOT_EQUAL_TO,
    OP_AND,
    OP_OR,
    OP_BITWISE_AND,
    OP_BITWISE_OR,
    OP_VALUE_OF,
    OP_POP,
  };


  std::string type_to_str(token_type type);

  struct token {
    token_type type;
    void *value;

    token();
    token(int num);
    token(std::string str);
    token(std::vector<token> arr);
    token(bool bl);
    token(token_type type);
    token(token_type type, void *value);
    ~token();

    std::string type_to_str();
    std::string val_to_str(bool raw = false);
    std::string to_string();

    std::string as_str();
    int as_num();
    std::vector<token> as_arr();
    bool as_bool();
  };

  struct state {
    std::stack<token> stack;
    std::map<std::string, token> dict;
    state();
    state(std::stack<token> stack, std::map<std::string, token> dict);
    ~state();
  };
};