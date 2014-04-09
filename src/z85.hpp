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

#pragma once

#include <stddef.h>
#include <string>

// Used to forbid implicit std::string construction from const char*
#if __cplusplus > 199711L // if C++11
   #define Z85_DELETE_FUNCTION_DEFINITION = delete
#else
   #define Z85_DELETE_FUNCTION_DEFINITION
#endif

namespace z85
{

/*******************************************************************************
 * ZeroMQ Base-85 encoding/decoding functions with custom padding              *
 *******************************************************************************/

/**
 * @brief Encodes 'inputSize' bytes from 'source'.
 *        If 'inputSize' is not divisible by 4 with no remainder, 'source' is padded.
 *
 * @param source in, input buffer (binary string to be encoded)
 * @param inputSize in, number of bytes to be encoded
 * @return printable string
 */
std::string encode_with_padding(const char* source, size_t inputSize);
std::string encode_with_padding(const std::string& source);

std::string encode_with_padding(const char*) Z85_DELETE_FUNCTION_DEFINITION;

/**
 * @brief Decodes 'inputSize' printable symbols from 'source',
 *        encoded with encode_with_padding().
 *
 * @param source in, input buffer (printable string to be decoded)
 * @param inputSize in, number of symbols to be decoded
 * @return decoded string
 */
std::string decode_with_padding(const char* source, size_t inputSize);
std::string decode_with_padding(const std::string& source);

std::string decode_with_padding(const char*) Z85_DELETE_FUNCTION_DEFINITION;


/*******************************************************************************
 * ZeroMQ Base-85 encoding/decoding functions (specification compliant)        *
 *******************************************************************************/

/**
 * @brief Encodes 'inputSize' bytes from 'source'.
 *        If 'inputSize' is not divisible by 4 with no remainder, empty string is retured.
 *
 * @param source in, input buffer (binary string to be encoded)
 * @param inputSize in, number of bytes to be encoded
 * @return printable string
 */
std::string encode(const char* source, size_t inputSize);
std::string encode(const std::string& source);

std::string encode(const char*) Z85_DELETE_FUNCTION_DEFINITION;

/**
 * @brief Decodes 'inputSize' printable symbols from 'source'.
 *        If 'inputSize' is not divisible by 5 with no remainder, empty string is returned.
 *
 * @param source in, input buffer (printable string to be decoded)
 * @param inputSize in, number of symbols to be decoded
 * @return decoded string
 */
std::string decode(const char* source, size_t inputSize);
std::string decode(const std::string& source);

std::string decode(const char*) Z85_DELETE_FUNCTION_DEFINITION;

} // namespace z85

#undef Z85_DELETE_FUNCTION_DEFINITION

