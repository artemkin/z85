
#include <cstring>
#include <cstdlib>
#include <cstddef>

#include "lest.hpp"
#include "z85.h"
#include "z85.hpp"

using namespace std;


const string c_badfood = "baadfoodbaadfoodbaadfoodbaadfood";

// Checks for buffer overruns
class strict_buf
{
   string m_buf;

public:
   explicit strict_buf(size_t i_size)
      : m_buf(c_badfood + string(i_size + 1, '\0') + c_badfood)
   {
   }

   char* p()
   {
      return &m_buf[0] + c_badfood.size();
   }

   std::string data() const
   {
      size_t count = c_badfood.size();

      return m_buf.substr(count, m_buf.size() - 2 * count - 1);
   }

   void check_buffer()
   {
      size_t count = c_badfood.size();

      const bool strict_buf_bad_food =
         m_buf.substr(0, count)           != c_badfood ||
         m_buf[m_buf.size()-1-count]      != '\0'      ||
         m_buf.substr(m_buf.size()-count) != c_badfood;

      EXPECT(strict_buf_bad_food == false);
   }
};

template<typename Fn>
void with_strict_buf(size_t bufSize, Fn f)
{
   strict_buf buf(bufSize);
   f(buf);
   buf.check_buffer();
}

template<typename Fn>
void for_random_data(size_t divisibleBy, Fn f)
{
   srand(0); //magic seed)

   std::string data;

   for (size_t i = 0; i < 10000; ++i)
   {
      const char ch = rand() % 256;
      data += ch;

      if (data.size() % divisibleBy == 0)
      {
         f(data);
      }
   }
}

const lest::test specification[] =
{
   "Hello world!", []
   {
      EXPECT(z85::encode(string("\x86\x4F\xD2\x6F\xB5\x59\xF7\x5B")) == "HelloWorld");
   },

   "Test strict buffer", []
   {
      {
         strict_buf buf_holder(0);
         char* buf = buf_holder.p();

         EXPECT(buf[-1] != '\0');
         EXPECT(buf[0]  == '\0');
         EXPECT(buf[1]  != '\0');
      }
      {
         strict_buf buf_holder(1);
         char* buf = buf_holder.p();

         EXPECT(buf[-1] != '\0');
         EXPECT(buf[0]  == '\0');
         EXPECT(buf[1]  == '\0');
         EXPECT(buf[2]  != '\0');
      }
   },

   "Test unsafe", []
   {
      auto test = [](const string& bin, const string& txt)
      {
         with_strict_buf(txt.size(), [&](strict_buf& txt_buf) {
         with_strict_buf(bin.size(), [&](strict_buf& bin_buf) {

         char* txt_end = Z85_encode_unsafe(bin.c_str(), bin.c_str() + bin.size(), txt_buf.p());
         char* bin_end = Z85_decode_unsafe(txt_buf.p(), txt_end, bin_buf.p());

         EXPECT((txt_end - txt_buf.p()) == (int)txt.size());
         EXPECT((bin_end - bin_buf.p()) == (int)bin.size());
         EXPECT(bin_buf.data() == bin);
         EXPECT(txt_buf.data() == txt);

         });});
      };

      test("", "");
      test("\x86\x4F\xD2\x6F\xB5\x59\xF7\x5B", "HelloWorld");
      test("\x8E\x0B\xDD\x69\x76\x28\xB9\x1D\x8F\x24\x55\x87\xEE\x95\xC5\xB0"
           "\x4D\x48\x96\x3F\x79\x25\x98\x77\xB4\x9C\xD9\x06\x3A\xEA\xD3\xB7",
           "JTKVSB%%)wK0E.X)V>+}o?pNmC{O&4W4b!Ni{Lh6");
   },

   "Test no padding", []
   {
      auto test = [](const string& bin, const string& txt)
      {
         with_strict_buf(txt.size(), [&](strict_buf& txt_buf) {
         with_strict_buf(bin.size(), [&](strict_buf& bin_buf) {

         size_t txt_written = Z85_encode(bin.c_str(), txt_buf.p(), bin.size());
         size_t bin_written = Z85_decode(txt_buf.p(), bin_buf.p(), txt_written);

         EXPECT(txt_written == txt.size());
         EXPECT(bin_written == bin.size());
         EXPECT(bin_buf.data() == bin);
         EXPECT(txt_buf.data() == txt);

         });});
      };

      test("", "");
      test("\x86\x4F\xD2\x6F", "Hello");
      test("\x86\x4F\xD2\x6F\xB5\x59\xF7\x5B", "HelloWorld");
      test("\x8E\x0B\xDD\x69\x76\x28\xB9\x1D\x8F\x24\x55\x87\xEE\x95\xC5\xB0"
           "\x4D\x48\x96\x3F\x79\x25\x98\x77\xB4\x9C\xD9\x06\x3A\xEA\xD3\xB7",
           "JTKVSB%%)wK0E.X)V>+}o?pNmC{O&4W4b!Ni{Lh6");
   },

   "Test with padding", []
   {
      auto test = [](const string& bin, const string& txt)
      {
         with_strict_buf(txt.size(), [&](strict_buf& txt_buf) {
         with_strict_buf(bin.size(), [&](strict_buf& bin_buf) {

         size_t txt_written = Z85_encode_with_padding(bin.c_str(), txt_buf.p(), bin.size());
         size_t bin_written = Z85_decode_with_padding(txt_buf.p(), bin_buf.p(), txt_written);

         EXPECT(txt_written == txt.size());
         EXPECT(bin_written == bin.size());
         EXPECT(bin_buf.data() == bin);
         EXPECT(txt_buf.data() == txt);

         });});
      };

      test("", "");
      test("\x86\x4F\xD2\x6F\xB5\x59\xF7\x5B", "4HelloWorld");
      test("\x8E\x0B\xDD\x69\x76\x28\xB9\x1D\x8F\x24\x55\x87\xEE\x95\xC5\xB0"
           "\x4D\x48\x96\x3F\x79\x25\x98\x77\xB4\x9C\xD9\x06\x3A\xEA\xD3\xB7",
           "4JTKVSB%%)wK0E.X)V>+}o?pNmC{O&4W4b!Ni{Lh6");
   },

   "Test no padding roundtrip", []
   {
      for_random_data(4, [](const string& bin)
      {
         const string txt     = z85::encode(bin);
         const string new_bin = z85::decode(txt);

         EXPECT(new_bin == bin);
      });
   },

   "Test with padding roundtrip", []
   {
      for_random_data(1, [](const string& bin)
      {
         const string txt     = z85::encode_with_padding(bin);
         const string new_bin = z85::decode_with_padding(txt);

         EXPECT(new_bin == bin);
      });
   },

   "Test Z85_encode_bound()", []
   {
      char buf[1300];
      std::string input;
      for (size_t i = 0; i < 1000; i += 4)
      {
         const size_t estimate = Z85_encode_bound(input.size());
         const size_t encoded = Z85_encode(input.c_str(), buf, input.size());
         EXPECT(estimate == encoded);
         input += i       % 256;
         input += (i + 1) % 256;
         input += (i + 2) % 256;
         input += (i + 3) % 256;
      }
   },

   "Test Z85_decode_bound()", []
   {
      char buf[1300];
      std::string input;
      for (size_t i = 0; i < 1000; i += 5)
      {
         const size_t estimate = Z85_decode_bound(input.size());
         const size_t decoded = Z85_decode(input.c_str(), buf, input.size());
         EXPECT(estimate == decoded);
         input += i       % 256;
         input += (i + 1) % 256;
         input += (i + 2) % 256;
         input += (i + 3) % 256;
         input += (i + 4) % 256;
      }
   },

   "Test Z85_encode_with_padding_bound()", []
   {
      char buf[1300];
      std::string input;
      for (size_t i = 0; i < 1000; ++i)
      {
         const size_t estimate = Z85_encode_with_padding_bound(input.size());
         const size_t encoded = Z85_encode_with_padding(input.c_str(), buf, input.size());
         EXPECT(estimate == encoded);
         input += i % 256;
      }
   },

   "Test Z85_decode_with_padding_bound()", []
   {
      char buf[1300];
      char buf2[1300];
      std::string input;
      for (size_t i = 0; i < 1000; ++i)
      {
         const size_t encoded = Z85_encode_with_padding(input.c_str(), buf, input.size());
         const size_t estimate = Z85_decode_with_padding_bound(buf, encoded);
         const size_t decoded = Z85_decode_with_padding(buf, buf2, encoded);
         EXPECT(estimate == decoded);
         input += i % 256;
      }
   },

   "Test Z85_encode()/Z85_decode() wrong input", []
   {
      char buf[100];
      EXPECT(Z85_encode(NULL, NULL, 5) == 0);
      EXPECT(Z85_encode("some binary data", NULL, 5) == 0);
      EXPECT(Z85_encode("some binary data", buf, 5) == 0);
      EXPECT(Z85_encode("some binary data", buf, 16) == 20);

      EXPECT(Z85_decode(NULL, NULL, 4) == 0);
      EXPECT(Z85_decode("some text.", NULL, 4) == 0);
      EXPECT(Z85_decode("some text.", buf, 4) == 0);
      EXPECT(Z85_decode("some text.", buf, 10) == 8);
   },

   "Test Z85_decode_with_padding() wrong tail bytes count", []
   {
      char buf[100];
      EXPECT(Z85_decode_with_padding("0HelloWorld", buf, 11) == 0);
      EXPECT(Z85_decode_with_padding("5HelloWorld", buf, 11) == 0);
      EXPECT(Z85_decode_with_padding("AHelloWorld", buf, 11) == 0);
      EXPECT(Z85_decode_with_padding("4HelloWorld", buf, 11) == 8);
   },

   "Test wrong input for z85:: functions", []
   {
      EXPECT(z85::encode_with_padding(NULL, 0) == "");
      EXPECT(z85::encode_with_padding("bin", 0) == "");
      EXPECT(z85::encode_with_padding(NULL, 4) == "");

      EXPECT(z85::decode_with_padding(NULL, 0) == "");
      EXPECT(z85::decode_with_padding("txt", 0) == "");
      EXPECT(z85::decode_with_padding(NULL, 5) == "");

      EXPECT(z85::encode(NULL, 0) == "");
      EXPECT(z85::encode("bin", 0) == "");
      EXPECT(z85::encode(NULL, 4) == "");

      EXPECT(z85::decode(NULL, 0) == "");
      EXPECT(z85::decode("txt", 0) == "");
      EXPECT(z85::decode(NULL, 5) == "");
   }
};

int main()
{
   return lest::run(specification);
}

