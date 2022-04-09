#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <vector>
#include <stack>
#include <set>
#include <map>
// #include "kuma/kuma.hpp"

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

#include "kuma.hpp"

#define NO_ARGS token(vector<token>{})

using namespace kuma;
using std::stringstream, std::string, std::getline, std::cin, std::cout, std::endl, std::vector, std::map, std::set, std::stack;
using std::pair;

// todo: state dict copy not ref

struct lexer {
  const char *input;
  unsigned long pos;
  map<string, token_type> op_map;
  map<string, pair<token_type, void *> > kw_map;

  lexer(const char* input)
    : input(input), pos(0), op_map(create_op_map()), kw_map(create_kw_map()) {};

  char eat() { return input[pos++]; }
  char look() { return input[pos]; }
  char peak() { return input[pos + 1]; }

  static map<string, pair<token_type, void *> > create_kw_map() {
    map<string, pair<token_type, void *> > kw_map;

    #define B(TOKEN_TYPE, VOID_PTR) pair<token_type, void *>{TOKEN_TYPE, VOID_PTR}
    #define A(TOKEN_TYPE) B(TOKEN_TYPE, nullptr)

    kw_map["if"] = A(OP_IF);
    kw_map["ifelse"] = A(OP_IF_ELSE);
    kw_map["while"] = A(OP_WHILE);
    kw_map["true"] = B(BOOLEAN, new bool(true));
    kw_map["false"] = B(BOOLEAN, new bool(false));
    kw_map["null"] = A(_NULL);

    return kw_map;
  }

  static map<string, token_type> create_op_map() {
    map<string, token_type> op_map;

    op_map["="] = OP_SET_EQUALS;
    op_map["+"] = OP_ADD;
    op_map["-"] = OP_SUBTRACT;
    // op_map["*"] = OP_MULTIPLY;
    // op_map["$/"] = OP_DIVIDE;

    op_map["["] = OP_START_ARRAY;
    op_map["]"] = OP_END_ARRAY;
    op_map["("] = OP_START_ARGS;
    op_map[")"] = OP_END_ARGS;
    // op_map["{"] = OP_START_SCOPE;
    // op_map["}"] = OP_END_SCOPE;
    op_map["!"] = OP_EXECUTE;
    // op_map[":"] = OP_PAIR;
    op_map["."] = OP_ACCESS;
    op_map[".."] = OP_RAW_ACCESS;

    op_map["<"] = OP_LESS_THAN;
    op_map[">"] = OP_GREATER_THAN;
    op_map["<="] = OP_LESS_EQUAL;
    op_map[">="] = OP_GREATER_EQUAL;
    op_map["=="] = OP_EQUAL_TO;
    op_map["!="] = OP_NOT_EQUAL_TO;
    op_map["&&"] = OP_AND;
    op_map["||"] = OP_OR;
    op_map["&"] = OP_BITWISE_AND;
    op_map["|"] = OP_BITWISE_OR;

    op_map["$"] = OP_VALUE_OF;
    // op_map["@"] = OP_REFERENCE;
    op_map["#"] = OP_POP;

    return op_map;
  }

  vector<token> lex() {
    vector<token> tokens = {};
    // set<char> exclude_op_chars = {'{', '}'};
    set<char> op_chars;
    for (auto pair : op_map)
      for (char c : pair.first)
        if (!isalpha(c))
          op_chars.insert(c);

    map<char, char> escape = {{'n', '\n'}, {'\'', '\''}, {'t', '\t'}};
    unsigned long scope_lvl = 0;
    for (char c = look(); c != '\0'; c = look()) {
      stringstream stream;
      if (isalpha(c)) {
        while (isalpha(look())) stream << eat();
        string id = stream.str();
        if (!kw_map.count(id)) tokens.push_back(token(IDENTIFIER, new string(stream.str())));
        else {
          tokens.push_back(token(kw_map[id].first, kw_map[id].second));
        }
      } else if (isnumber(c)) {
        while (isnumber(look())) stream << eat();
        tokens.push_back(token(atoi(stream.str().c_str())));
      } else if (c == '{') {
        // skips over op_chars for special scope behavior
        // ^^^^^^ for nesting
        // l=0
        // s={x x4+=}!
        // {{x x4+=}!}!
        //            ^
        do {
          if (look() == '{') {
            char chr = eat();
            if (++scope_lvl > 1) stream << chr;
          } else if (look() == '}') {
            char chr = eat();
            if (--scope_lvl > 0) stream << chr;
          } else if (scope_lvl > 0)
            stream << eat();
        } while (scope_lvl != 0 && look() != '\0');
        // vvvvv ERROR
        if (scope_lvl != 0) break;
        string src = stream.str();
        vector<token> scope_tokens = lexer(src.c_str()).lex();
        tokens.push_back(token(SCOPE, new vector<token>(scope_tokens)));
      } else if (op_chars.count(c)) {
        while (op_chars.count(look())) {
          string temp_op = stream.str();
          string new_temp_op = string(temp_op);
          new_temp_op += look();
          // no need for spaces between operators!
          // downside: every op must exist up to the whole op
          // e.g.: <=>
          // < and <= must exist, otherwise this logic breaks down
          if (
            op_map.count(temp_op) &&
            !op_map.count(new_temp_op)
          ) {
            tokens.push_back(token(op_map[temp_op]));
            stream.str(string());
          }
          stream << eat();
        }
        string op = stream.str();
        if (op_map.count(op)) {
          tokens.push_back(token(op_map[op]));
        } else return tokens;
        // ERROR ^^^^^^^^^^^^ UNKNOWN OPERATOR
      } else if (c == '\'') {
        eat();
        while (look() != '\'') {
          char chr = eat();
          if (chr == '\\')
            chr = escape[eat()];
          stream << chr;
        }
        eat();
        tokens.push_back(token(stream.str()));
      } else {
        eat();
      }
    }
    tokens.push_back(token(_EOF));
    return tokens;
  }
};

struct parser {
  unsigned long pos;
  vector<token> input;
  state &k;
  stack<token> &stk;

  parser(vector<token> input, state &k) : pos(0), input(input), k(k), stk(k.stack) {};

  token eat() { token t = stk.top(); stk.pop(); return t; }
  token look() { return stk.top(); }

  token ieat() { return input[pos++]; }
  token ilook() { return input[pos]; }

  token valueof(token val) {
    switch (val.type) {
      case IDENTIFIER: {
        string var_name = val.as_str();
        // cout << "tryna see " << var_name << endl;
        if (!k.dict.count(var_name)) {
          // cout << "warning: nonexistant variable " << var_name << "; returning null" << endl;
          val = token();
          break; // YOU NEED TO ERROR HERE, UNKNOWN VARIABLE
        }
        val = k.dict[var_name];
        break;
      }
      default: break;
    }
    return val;
  }

  void exec_native_func(string func_name, token args) {
    if (func_name == "print" || func_name == "println" || func_name == "input") {
      for (token t : args.as_arr())
        cout << t.val_to_str(true);
      if (func_name == "println") cout << endl;
      if (func_name == "input") {
        string str;
        getline(cin, str);
        stk.push(token(str));
      }
    } else if (func_name == "helloworld") {
      cout << "Hello, world!" << endl;
    } else if (func_name == "die") {
      stk.push(token(_EOF));
    } else if (func_name == "dumpDict") {
      cout << "DICT: ";
      for (auto pair : k.dict) {
        cout << "\t" << pair.first << "=" << pair.second.to_string() << endl;
      }
      cout << endl;
    } else if (func_name == "dumpStack") {
      stack<token> temp;
      cout << "STK: ";
      while (!stk.empty()) {
        temp.push(look());
        cout << "\t" << eat().to_string() << endl;
      }
      while (!temp.empty()) {
        stk.push(temp.top());
        temp.pop();
      }
      cout << endl;
    } else if (func_name == "sleep") {
      for (token t : args.as_arr()) {
        if (t.type != NUMBER) continue;
        std::this_thread::sleep_for(std::chrono::milliseconds(t.as_num()));
      }
    } else if (func_name == "random") {
      stk.push((bool)(rand() % 2));
    } else if (func_name == "min") {
      int min_value = INT_MAX;
      for (token t : args.as_arr()) {
        if (t.type != NUMBER) continue;
        min_value = std::min(min_value, t.as_num());
      }
      stk.push(min_value);
    } else if (func_name == "max") {
      int max_value = INT_MIN;
      for (token t : args.as_arr()) {
        if (t.type != NUMBER) continue;
        max_value = std::max(max_value, t.as_num());
      }
      stk.push(max_value);
    } else if (func_name == "termRows") {
      struct winsize w;
      ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
      stk.push((int)w.ws_row);
    } else if (func_name == "termCols") {
      struct winsize w;
      ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
      stk.push((int)w.ws_col);
    } else if (func_name == "import") {
      for (token t : args.as_arr()) {
        if (t.type != STRING) continue;
        std::ifstream file(t.as_str());
        stringstream buffer;
        buffer << file.rdbuf();

        // cout << "DUMP DICT ";
        // for (auto pair : k.dict) {
        //   cout << pair.first << "=" << pair.second.to_string() << ",";
        // }
        parser(
          lexer(
            buffer.str().c_str()
          ).lex(),
          k
        ).parse();

        if (!stk.empty() && look().type == _EOF)
          eat();
        // cout << "DUMP DICT ";
        // for (auto pair : k.dict) {
        //   cout << pair.first << "=" << pair.second.to_string() << ",";
        // }
      }
    } else {
      // error
      cout << "Hello from " << func_name << "!" << endl;
    }
  }

  void exec_scope(token scope, token args) {
    vector<token> all_args = args.as_arr();
    // passing clones of stack and dict
    // we want the actual stack to be accessible not a clone thoooo soooo uhhhh
    // so ref to stack and clone of dict
    // hmm
    // state sub_k = state(k.stack, k.dict);
    // temporarily defining/redefining these variables as necessary on k instead of sub_k
    char arg_list[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (unsigned long i = 0; i < std::min(all_args.size(), strlen(arg_list)); ++i)
      k.dict[string(1, arg_list[i])] = all_args[i];
    // cout << "ARG DUMP" << endl;
    // for (token t : all_args)
    //   cout << t.to_string() << ",";
    // cout << endl;
    // cout << "DICT DUMP" << endl;
    // for (auto pair : k.dict)
    //   cout << pair.first << "=" << pair.second.to_string() << ",";
    // pass k here temporarily to allow access to stack and dict
    // means no args thoooo :(
    parser(scope.as_arr(), k).parse();
  }

  void exec_func(token func, token args) {
    if (func.type == IDENTIFIER) {
      string func_name = func.as_str();
      if (k.dict.count(func_name) && k.dict[func_name].type == SCOPE)
        exec_scope(k.dict[func_name], args);
      else exec_native_func(func_name, args);
    } else if (func.type == SCOPE) exec_scope(func, args);
    // else error
    return;
  }

  void parse() {
    for (token t = ilook(); t.type != _EOF; t = ilook()) {
      // cout << "DUMP DICT ";
      // for (auto pair : k.dict) {
      //   cout << pair.first << "=" << pair.second.to_string() << ",";
      // }
      // cout << endl;
      // cout << "STACK LOOK " << (!stk.empty() ? look().to_string() : "empty") << endl;
      switch (t.type) {
        case _NULL:
        case IDENTIFIER:
        case NUMBER:
        case STRING:
        case ARRAY: //?
        case BOOLEAN:
        case SCOPE:
        case OP_START_ARRAY:
        case OP_START_ARGS:
          stk.push(t);
          break;

        case OP_SET_EQUALS: {
          if (stk.size() < 2) break;
          // cout << "SETTING" << endl;
          token val = valueof(eat());
          // if @, get another to find reference to set to
          // if id, check dict, otherwise error
          // just break out for now
          token key = eat();
          if (key.type != IDENTIFIER) break;
          k.dict[key.as_str()] = val;
          break;
        }

        case OP_ADD: {
          if (stk.size() < 2) break;
          token val_two = valueof(eat());
          token val_one = valueof(eat());
          if (val_one.type == NUMBER) {
            int num_one = val_one.as_num();
            int num_two = (val_two.type == NUMBER ? val_two.as_num() : 0);
            stk.push(token(num_one + num_two));
          } else if (val_one.type == STRING) {
            // todo: conversion from int to string with an official cast function
            string str_one = val_one.as_str();
            string str_two = (val_two.type == STRING ? val_two.as_str() : std::to_string(val_two.as_num()));
            stk.push(token(str_one + str_two));
          } else {
            stk.push(token());
          }
          break;
        }

        case OP_SUBTRACT: {
          if (stk.size() < 2) break;
          token val_two = valueof(eat());
          token val_one = valueof(eat());
          // todo: decide if push null? probs
          if (val_one.type != NUMBER || val_two.type != NUMBER)
            break;
          int num_one = val_one.as_num();
          int num_two = val_two.as_num();
          stk.push(token(num_one - num_two));
          break;
        }

        case OP_END_ARRAY: {
          if (stk.size() < 1) break;
          vector<token> array = {};
          while (!stk.empty() && look().type != OP_START_ARRAY)
            array.push_back(valueof(eat()));
            // ^^^^^^ detrimental when it comes to @?
          if (!stk.empty() && look().type == OP_START_ARRAY) eat();
          stk.push(token(array));
          break;
        }

        case OP_END_ARGS: {
          // 2 args, ( and func name
          if (stk.size() < 2) break;
          // make arg array
          vector<token> array = {};
          while (!stk.empty() && look().type != OP_START_ARGS)
            array.push_back(valueof(eat()));
          // ^^^^^^ detrimental when it comes to @?
          if (!stk.empty() && look().type == OP_START_ARGS) eat();
          token args = token(array);
          exec_func(eat(), args);
          break;
        }

        case OP_EXECUTE:
          if (stk.size() < 1) break;
          exec_func(eat(), NO_ARGS);
          break;
        
        case OP_ACCESS: case OP_RAW_ACCESS: {
          if (stk.size() < 2) break;
          // use valueof here with ..
          token key = eat();
          if (t.type == OP_RAW_ACCESS) key = valueof(key);
          token obj = valueof(eat());
          if (obj.type != ARRAY /*&& obj.type != STRING*/) break;

          vector<token> arr = obj.as_arr();
          int arr_len = arr.size();
          // if (obj.type == STRING) {
          //   vector<string> new_obj;
          //   for (char )
          // }
          // another line that separates . from ..
          if (t.type != OP_RAW_ACCESS && key.type == IDENTIFIER) key.type = STRING;
          if (key.type == STRING) {
            if (key.as_str() == "length")
              stk.push(token(arr_len));
            else break; // ERROR
          } else if (key.type == NUMBER) {
            int num = key.as_num();
            // only works for arrays, diff for objects
            // at some point, arrays will just be objects like 0:thing 1:thing 2:thing
            if (num < arr_len && num >= 0)
              stk.push(arr[num]);
            else break; // ERROR
          } else break; // ERROR? though objs can have anything as keys soooo
          break;
        }

        case OP_IF: {
          // {bool}{code}
          if (stk.size() < 2) break;
          token if_code = valueof(eat());
          token conditional = valueof(eat());
          // not taking any chances, only identifiers or scopes
          if (if_code.type != SCOPE || conditional.type != SCOPE) break;
          exec_func(conditional, NO_ARGS);

          // stack being empty is truthy or falsey? hmm
          // truthy: for(;;){}, no conditional = truthy
          // falsey: null would be falsey
          // going with truthy for now
          // also not having valueof here could be interesting

          token top = stk.empty() ? token() : valueof(eat());

          if (
            (top.type == STRING || top.type == SCOPE || top.type == ARRAY) || // make empty string falsey?
            (top.type == NUMBER && top.as_num() != 0) ||
            (top.type == BOOLEAN && top.as_bool())
          ) exec_func(if_code, NO_ARGS);

          break;
        }

        case OP_IF_ELSE: {
          if (stk.size() < 3) break;

          token else_code = valueof(eat());
          token if_code = valueof(eat());
          token conditional = valueof(eat());
          if (else_code.type != SCOPE || if_code.type != SCOPE || conditional.type != SCOPE)
            break;

          exec_func(conditional, NO_ARGS);
          token top = stk.empty() ? token() : valueof(eat());

          if (
            (top.type == STRING || top.type == SCOPE || top.type == ARRAY) || // make empty string falsey?
            (top.type == NUMBER && top.as_num() != 0) ||
            (top.type == BOOLEAN && top.as_bool())
          ) exec_func(if_code, NO_ARGS);
          else exec_func(else_code, NO_ARGS);

          break;
        }

        case OP_WHILE: {
          if (stk.size() < 2) break;

          token while_code = valueof(eat());
          token conditional = valueof(eat());
          if (while_code.type != SCOPE || conditional.type != SCOPE) break;
          
          for (;;) {
            exec_func(conditional, NO_ARGS);
            token top = stk.empty() ? token() : valueof(eat());
            if (
              (top.type == STRING || top.type == SCOPE || top.type == ARRAY) || // make empty string falsey?
              (top.type == NUMBER && top.as_num() != 0) ||
              (top.type == BOOLEAN && top.as_bool())
            ) {
              exec_func(while_code, NO_ARGS);
            } else break;
          }

          break;
        }

        #define AR_OP(TOKEN_TYPE, OPERATION, EXPECTED_TOKEN_TYPE, EXPECTED_TYPE, CAST_FUNC) \
          case TOKEN_TYPE: { \
            if (stk.size() < 2) break; \
            token val_two = valueof(eat()); \
            token val_one = valueof(eat()); \
            if (val_one.type != EXPECTED_TOKEN_TYPE || val_two.type != EXPECTED_TOKEN_TYPE) { \
              stk.push(token(false)); \
              break; \
            } \
            EXPECTED_TYPE thing_one = val_one.CAST_FUNC(); \
            EXPECTED_TYPE thing_two = val_two.CAST_FUNC(); \
            stk.push(token(thing_one OPERATION thing_two)); \
            break; \
          }

        AR_OP(OP_LESS_THAN, <, NUMBER, int, as_num)
        AR_OP(OP_GREATER_THAN, >, NUMBER, int, as_num)
        AR_OP(OP_LESS_EQUAL, <=, NUMBER, int, as_num)
        AR_OP(OP_GREATER_EQUAL, >=, NUMBER, int, as_num)
        AR_OP(OP_BITWISE_AND, &, NUMBER, int, as_num)
        AR_OP(OP_BITWISE_OR, |, NUMBER, int, as_num)
        AR_OP(OP_AND, &&, BOOLEAN, bool, as_bool)
        AR_OP(OP_OR, ||, BOOLEAN, bool, as_bool)

        #define EQ_OP(TOKEN_TYPE, OPERATION, DEFAULT) \
          case TOKEN_TYPE: { \
            if (stk.size() < 2) break; \
            token val_two = valueof(eat()); \
            token val_one = valueof(eat()); \
            if (val_one.type != val_two.type) { \
              stk.push(token(DEFAULT)); \
              break; \
            } \
            if (val_one.type == STRING) { \
              string str_one = val_one.as_str(); \
              string str_two = val_two.as_str(); \
              stk.push(token(str_one OPERATION str_two)); \
            } else if (val_one.type == NUMBER) { \
              int num_one = val_one.as_num(); \
              int num_two = val_two.as_num(); \
              stk.push(token(num_one OPERATION num_two)); \
            } else if (val_one.type == BOOLEAN) { \
              bool bool_one = val_one.as_bool(); \
              bool bool_two = val_two.as_bool(); \
              stk.push(token(bool_one OPERATION bool_two)); \
            } else if (val_one.type == _NULL) { \
              stk.push(token(!DEFAULT)); \
            } else stk.push(token(DEFAULT)); \
            break; \
          }

        EQ_OP(OP_EQUAL_TO, ==, false)
        EQ_OP(OP_NOT_EQUAL_TO, !=, true)

        case OP_VALUE_OF:
          if (stk.size() < 1) break;
          stk.push(valueof(eat()));
          break;

        case OP_POP:
          if (stk.size() < 1) break;
          stk.pop();
          break;

        default:
          cout << "encountered: " << t.to_string() << endl;
          break;
      }
      if (!stk.empty() && look().type == _EOF)
        break;
      ieat();
    }
  }
};

int main(int argc, char *argv[]) {
  string input;

  if (argc == 1) {
    input = "helloworld!";
  } else {
    std::ifstream file(argv[1]);
    stringstream buffer;
    buffer << file.rdbuf();
    input = buffer.str();
  }

  lexer lex = lexer(input.c_str());
  vector<token> tokens = lex.lex();

  // for (token t : tokens) {
  //   cout << t.to_string() << endl;
  // }
  // cout << "-----------" << endl;

  state k = state();
  parser prs = parser(tokens, k);
  prs.parse();

  // cout << "-----------" << endl;
  // for (auto pair : k.dict) {
  //   cout << pair.first << " = " << pair.second.val_to_str() << endl;
  // }

  return 0;
}










