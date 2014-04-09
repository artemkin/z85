
#include "lest.hpp"
#include "z85.hpp"

using namespace std;

const lest::test specification[] =
{
   "Hello world!", []
   {
      EXPECT(z85::encode(string("\x86\x4F\xD2\x6F\xB5\x59\xF7\x5B")) == "HelloWorld");
   },
};

int main()
{
   return lest::run(specification);
}

