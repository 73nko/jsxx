
#include "global_wasi.hpp"

#include <unistd.h>
#include <vector>

static JSValue write_to_stdout(JSValue thisArg, std::vector<JSValue> &args) {
  JSValue data = args[0];
  if (data.type() != JSValueType::STRING)
    return JSValue::undefined();
	std::string str = data.coerce_to_string();
  write(1, str.c_str(), str.size());
  return JSValue{true};
}

JSValue create_WASI_global() {
  char buf[1024];
  std::string input{};
  while (true) {
    auto n = read(0, buf, 1023);
    buf[n] = 0;
    input += std::string{buf};
    if (n < 1023)
      break;
  };

  JSValue global = JSValue::new_object(
      {{JSValue{"stdin"}, JSValue{input}},
       {JSValue{"write_to_stdout"}, JSValue::new_function(write_to_stdout)}});

  return global;
}
