Z85
===

Z85 is a binary-to-text encoding library. 
It implements [ZeroMQ Base-85 Encoding Algorithm](http://rfc.zeromq.org/spec:32/Z85) and provides custom padding. Z85 is written in C89.

Install
-------

Just grab <code>z85.h</code> and <code>z85.c</code> to your project, compile and link.

Usage
-----

HelloWorld!

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

<code>Z85_encode</code> and <code>Z85_decode</code> are implemented according to
[Z85 specification](http://rfc.zeromq.org/spec:32/Z85).
So, <code>Z85_encode</code> expects as input a binary string that length is divisible by 4 with no remainder, and
<code>Z85_decode</code> expects as input a printable string that length is divisible by 5 with no remainder.
It may be inconvenient, so the library provides functions that pad input strings to meet the above-mentioned requirements:
<code>Z85_encode_with_padding</code> and <code>Z85_decode_with_padding</code>.



