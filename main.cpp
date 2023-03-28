// #include <functional>
// #include <iostream>
// #include <string>

#include "main.hpp"

#include "lambda2/alpha.hpp"
#include "lambda2/is_free_var.hpp"

using namespace lambda2;
using namespace TPrelude;

struct BOOL {
  struct True {
    static const bool value = true;
  };
  struct False {
    static const bool value = false;
  };
};

struct vx {};
struct vy {};
using fn = lambda2::abs<var<vx>, app<var<vy>, var<vx>>>;

using x2 = is_free_var<var<vx>, var<vx>, BOOL>::type;
static const bool x2_v = x2::value;
int main() {
  // auto ma = None();
	// auto a =
  //   ma.match(
  //     []() { return false; }, 
  //     [](auto x) { return true; }
  //   );

  // auto b = False;

  // auto not_b = _not(b);

  //auto i = b.match([]() { return 0;}, []() { return 1;});

  //auto xs = List(1, true, "");

  //auto ys = Cons(5, Cons(true, Nil));

  //auto y = head(tail(ys));
  
  //auto len_ys = length(ys);
  /*
    auto a = 
      match(mx, 
        _None([]() { ... }, 
        _Some([](auto x) { ... })
      );
  */

  // std::cout << a << std::endl;		
  return 0;
}
