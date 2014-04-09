/*
 * Copyright 2013 Stanislav Artemkin <artemkin@gmail.com>.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Implementation of 32/Z85 specification (http://rfc.zeromq.org/spec:32/Z85)
 * Source repository: http://github.com/artemkin/z85
 */

#include "z85.hpp"

#include <cassert>

#include "z85.h"


namespace z85
{

std::string encode_with_padding(const char* source, size_t inputSize)
{
   if (!source || inputSize == 0)
   {
      return std::string();
   }

   std::string buf;
   buf.resize(Z85_encode_with_padding_bound(inputSize));

   const size_t encodedBytes = Z85_encode_with_padding(source, &buf[0], inputSize);
   assert(encodedBytes == buf.size()); (void)encodedBytes;

   return buf;
}

std::string encode_with_padding(const std::string& source)
{
   return encode_with_padding(source.c_str(), source.size());
}

std::string decode_with_padding(const char* source, size_t inputSize)
{
   if (!source || inputSize == 0)
   {
      return std::string();
   }

   const size_t bufSize = Z85_decode_with_padding_bound(source, inputSize);
   if (bufSize == 0)
   {
      assert(!"wrong padding");
      return std::string();
   }

   std::string buf;
   buf.resize(bufSize);

   const size_t decodedBytes = Z85_decode_with_padding(source, &buf[0], inputSize);
   assert(decodedBytes == buf.size()); (void)decodedBytes;

   return buf;
}

std::string decode_with_padding(const std::string& source)
{
   return decode_with_padding(source.c_str(), source.size());
}

std::string encode(const char* source, size_t inputSize)
{
   if (!source || inputSize == 0)
   {
      return std::string();
   }

   std::string buf;
   buf.resize(Z85_encode_bound(inputSize));

   const size_t encodedBytes = Z85_encode(source, &buf[0], inputSize);
   if (encodedBytes == 0)
   {
      assert(!"wrong input size");
      return std::string();
   }

   return buf;
}

std::string encode(const std::string& source)
{
   return encode(source.c_str(), source.size());
}

std::string decode(const char* source, size_t inputSize)
{
   if (!source || inputSize == 0)
   {
      return std::string();
   }

   std::string buf;
   buf.resize(Z85_decode_bound(inputSize));

   const size_t decodedBytes = Z85_decode(source, &buf[0], inputSize);
   if (decodedBytes == 0)
   {
      assert(!"wrong input size");
      return std::string();
   }

   return buf;
}

std::string decode(const std::string& source)
{
   return decode(source.c_str(), source.size());
}

} // namespace z85

