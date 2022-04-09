#ifndef __KUMA_TOKEN
#define __KUMA_TOKEN

#include <string>

enum kuma_type {
  _NULL,
  IDENTIFIER,
  NUMBER,
  STRING,
  EQUALS
};

struct kuma_token {
  void *value;
  kuma_type type;

  std::string to_string();
  static std::string kuma_type_to_string(kuma_type type);

  kuma_token();
  kuma_token(kuma_type type, void *value);

  kuma_token(const std::string &&str);
  kuma_token(const long &&num);

  kuma_token(std::string* const str);
  kuma_token(long* const num);

  // ~kuma_token();
};

#endif