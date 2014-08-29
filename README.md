Z85
===

[![Build Status](https://travis-ci.org/artemkin/z85.svg?branch=master)](https://travis-ci.org/artemkin/z85)
[![Coverage Status](https://coveralls.io/repos/artemkin/z85/badge.png?branch=master)](https://coveralls.io/r/artemkin/z85?branch=master)

Z85 is a binary-to-text encoding library. 
It implements [ZeroMQ Base-85 Encoding Algorithm](http://rfc.zeromq.org/spec:32/Z85) and provides custom padding. Z85 is written in C and C++.

Install
-------

Just grab <code>z85.h</code> and <code>z85.c</code> to your project, compile and link.

Don't forget to take <code>z85.hpp</code> and <code>z85_impl.cpp</code> if you need C++ interface.

Usage
-----

### Hello World
```c
#include "stdio.h"
#include "string.h"
#include "z85/z85.h"

int main()
{
   char helloData[8] = "\x86\x4F\xD2\x6F\xB5\x59\xF7\x5B";

   // Encode binary to printable string
   char encBuf[10+1] = {}; // +1 for null terminating char
   size_t bytesEncoded = Z85_encode(helloData, encBuf, 8);

   // Decode printable string to binary
   char decBuf[8] = {};
   size_t bytesDecoded = Z85_decode(encBuf, decBuf, bytesEncoded);

   printf("%s", encBuf);

   if (bytesEncoded == 10 &&
       bytesDecoded == 8  &&
       !memcmp(helloData, decBuf, 8))
   {
     printf("!\n");
   }

   return 0;
}
```

Output
```
HelloWorld!
```
8 bytes of <code>helloData</code> are encoded into "HelloWorld" (10 ASCII symbols). The overhead of encoding is 25%.

<code>Z85_encode</code> and <code>Z85_decode</code> are implemented according to
[Z85 specification](http://rfc.zeromq.org/spec:32/Z85).
So, <code>Z85_encode</code> expects as input a binary string that length is divisible by 4 with no remainder, and
<code>Z85_decode</code> expects as input a printable string that length is divisible by 5 with no remainder.
It may be inconvenient, so the library provides functions that pad input strings to meet the above-mentioned requirements:
<code>Z85_encode_with_padding</code> and <code>Z85_decode_with_padding</code>.

### Hello World 2
```c
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "z85/z85.h"

char* encode(const char* src, size_t len)
{
   // allocate output buffer (+1 for null terminating char)
   char* dest = (char*)malloc(Z85_encode_with_padding_bound(len) + 1);

   if (len == 0)
   {
      dest[0] = '\0'; // write null terminating char
      return dest;
   }

   // encode the input buffer, padding it if necessary
   len = Z85_encode_with_padding(src, dest, len);

   if (len == 0) // something went wrong
   {
      free(dest);
      return NULL;
   }

   dest[len] = '\0'; // write null terminating char

   return dest;
}

int main()
{
   char* str = encode("\x86\x4F\xD2\x6F\xB5\x59\xF7\x5B", 8);

   if (str)
   {
      printf("%s\n", str);
      free(str);
   }

   return 0;
}
```

Output
```
4HelloWorld
```

<code>Z85_encode_with_padding_bound</code> is used to evaluate the size of output buffer.
This function returns exact size of the output buffer, so you do not need to shrink it after encoding.

The first symbol in encoded string ('4' in our example) stores a number of significant bytes contained in the remainder of input data.
Original Z85 algorithm can't encode byte sequence that length is not divisible by 4 with no remainder. In <code>Z85_encode_with_padding</code>
we pad the input remainder with '\0' bytes, encode the whole input with original algorithm and save a number of significat bytes 
in the reminder. '4' means no padding was even applied. '1', '2', '3' and '4' are possible values.

See <code>[z85.h](https://github.com/artemkin/z85/blob/master/src/z85.h)</code> for more details. It is well commented, so you can figure out
how to decode padded string by yourself.

Good luck!
