#include "VFDInterface.hpp"
#include <cstdlib>

void setCursor(int x, int y) {
  char buf1[5];
  itoa(y+1, buf1, 10);
  char buf2[5];
  itoa(x+1, buf2, 10);
  Serial1.print((String)""+(char)27+"["+buf1+";"+buf2+"H");
}

void clear() {
  Serial1.print((String)""+(char)27+"[2J");
}
