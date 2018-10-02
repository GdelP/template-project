#include "lib.hpp"
#include <iostream>
#include <boost/noncopyable.hpp>

struct dummy : boost::noncopyable {
  // empty
};

bool my_func() {
  usleep(1);
  return true;
}
