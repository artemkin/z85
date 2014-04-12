
#include <cstring>

#include "lest.hpp"
#include "z85.h"
#include "z85.hpp"

using namespace std;

void test_nopadding(const char* data, size_t dataSize, const char* text)
{
   const size_t bufSize = 32 * 1024;
   if (dataSize > bufSize * 0.7)
   {
      EXPECT(false);
      return;
   }

   EXPECT(dataSize % 4 == 0);
   char encodedBuf[bufSize] = {0};
   const size_t encodedBytes = Z85_encode(data, encodedBuf, dataSize);
   EXPECT(encodedBytes % 5 == 0);
   EXPECT(encodedBytes == (dataSize * 5 / 4));
   EXPECT(!strcmp(text, encodedBuf));

   char decodedBuf[bufSize] = {0};
   const size_t decodedBytes = Z85_decode(encodedBuf, decodedBuf, encodedBytes);
   EXPECT(decodedBytes % 4 == 0);
   EXPECT(decodedBytes == dataSize);
   EXPECT(!memcmp(data, decodedBuf, dataSize));

   char encodedBufWithPadding[bufSize] = {0};
   const size_t encodedBytesWithPadding = Z85_encode_with_padding(data, encodedBufWithPadding, dataSize);
   EXPECT(encodedBytes + (encodedBytes ? 1 : 0) == encodedBytesWithPadding); // +one byte for padding count
   EXPECT(encodedBytesWithPadding == 0 || encodedBufWithPadding[0] == '4');  // nothing should be padded, so 4 count
   EXPECT(!memcmp(encodedBuf, encodedBufWithPadding + 1, encodedBytes));

   char decodedBufWithPadding[bufSize] = {0};
   const size_t decodedBytesWithPadding = Z85_decode_with_padding(encodedBufWithPadding, decodedBufWithPadding, encodedBytesWithPadding);
   EXPECT(decodedBytesWithPadding % 4 == 0);
   EXPECT(decodedBytesWithPadding == dataSize);
   EXPECT(!memcmp(data, decodedBufWithPadding, dataSize));
}

const lest::test specification[] =
{
   "Hello world!", []
   {
      EXPECT(z85::encode(string("\x86\x4F\xD2\x6F\xB5\x59\xF7\x5B")) == "HelloWorld");
   },

   "Test unsafe", []
   {
      test_nopadding("", 0, "");
      test_nopadding("\x86\x4F\xD2\x6F\xB5\x59\xF7\x5B", 8, "HelloWorld");
      test_nopadding("\x8E\x0B\xDD\x69\x76\x28\xB9\x1D\x8F\x24\x55\x87\xEE\x95\xC5\xB0"
                     "\x4D\x48\x96\x3F\x79\x25\x98\x77\xB4\x9C\xD9\x06\x3A\xEA\xD3\xB7",
                     32, "JTKVSB%%)wK0E.X)V>+}o?pNmC{O&4W4b!Ni{Lh6");
   },
};

int main()
{
   return lest::run(specification);
}

