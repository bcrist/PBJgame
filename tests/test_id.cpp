// Copyright (c) 2013 Benjamin Crist
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#include "be/id.h"
#include "pbj/_pbj.h"

#ifdef BE_TEST
#include "catch.hpp"

namespace Catch {

std::string toString(be::Id id)
{
   return id.to_string();
}

} // namespace Catch

TEST_CASE("bengine/Id", "Provides hashed string identifiers")
{
   REQUIRE(be::Id().value() == 0xCBF29CE484222325);
   REQUIRE(be::Id("").value() == 0xCBF29CE484222325);

   REQUIRE(be::Id(0).value() == 0);
   REQUIRE(be::Id(0x1).value() == 0x1);
   REQUIRE(be::Id(0x12).value() == 0x12);
   REQUIRE(be::Id(0x123).value() == 0x123);
   REQUIRE(be::Id(0x1234).value() == 0x1234);
   REQUIRE(be::Id(0x12345).value() == 0x12345);
   REQUIRE(be::Id(0x123456).value() == 0x123456);
   REQUIRE(be::Id(0x1234567).value() == 0x1234567);
   REQUIRE(be::Id(0x12345678).value() == 0x12345678);
   REQUIRE(be::Id(0x123456789).value() == 0x123456789);
   REQUIRE(be::Id(0x123456789A).value() == 0x123456789A);
   REQUIRE(be::Id(0x123456789AB).value() == 0x123456789AB);
   REQUIRE(be::Id(0x123456789ABC).value() == 0x123456789ABC);
   REQUIRE(be::Id(0x123456789ABCD).value() == 0x123456789ABCD);
   REQUIRE(be::Id(0x123456789ABCDE).value() == 0x123456789ABCDE);
   REQUIRE(be::Id(0x123456789ABCDEF).value() == 0x123456789ABCDEF);

   be::Id id_abcd("abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
   std::string alpha(id_abcd.to_string().substr(0, 17));
   std::transform(alpha.begin(), alpha.end(), alpha.begin(), tolower);
   REQUIRE(alpha == "#bb308fe4c0b0d33a");

   REQUIRE(be::Id(be::Id("159487")) == be::Id("159487"));

   be::Id("asdf");   // ensure that "asdf" has been recorded as the name of this Id if names are being recorded
   REQUIRE(be::Id(be::Id("asdf")).to_string() == be::Id(0x90285684421F9857).to_string());

#ifdef BE_ID_NAMES_ENABLED
   REQUIRE(be::Id(be::Id("306")).to_string() == "#5728871822492220:306");
#endif

   be::Id was_empty_id;
   REQUIRE((was_empty_id = be::Id("a;sldkfj ")) == be::Id("a;sldkfj "));

   REQUIRE(std::hash<be::Id>()(be::Id("Testing!")) == std::hash<be::Id>()(be::Id(0xED51CBED43EC088E)));


   REQUIRE(be::Id("Component").value()      == 0xa6d4361f17423080);
   REQUIRE(be::Id("bsc::Transform").value() == 0x60e3d88a09fd5395);
   REQUIRE(be::Id("bsc::Camera").value()    == 0x12b535f385b85c44);
   REQUIRE(be::Id("bsc::Scene").value()     == 0xb9984bd1f4c69801);
}

TEST_CASE("bengine/Id/Comparisons", "Comparison based on numerical value of hash")
{
   REQUIRE(  be::Id(0) == be::Id(0));
   REQUIRE(!(be::Id(0) == be::Id(1)));

   REQUIRE(  be::Id(65536) != be::Id(0));
   REQUIRE(!(be::Id(65536) != be::Id(65536)));

   REQUIRE(!(be::Id(0x100000000) < be::Id(0x0FFFFFFFF)));
   REQUIRE(!(be::Id(0x100000000) < be::Id(0x100000000)));
   REQUIRE(  be::Id(0x100000000) < be::Id(0x100000001));

   REQUIRE(  be::Id(0x100000000) > be::Id(0x0FFFFFFFF));
   REQUIRE(!(be::Id(0x100000000) > be::Id(0x100000000)));
   REQUIRE(!(be::Id(0x100000000) > be::Id(0x100000001)));

   REQUIRE(!(be::Id(0x100000000) <= be::Id(0x0FFFFFFFF)));
   REQUIRE(  be::Id(0x100000000) <= be::Id(0x100000000));
   REQUIRE(  be::Id(0x100000000) <= be::Id(0x100000001));

   REQUIRE(  be::Id(0x100000000) >= be::Id(0x0FFFFFFFF));
   REQUIRE(  be::Id(0x100000000) >= be::Id(0x100000000));
   REQUIRE(!(be::Id(0x100000000) >= be::Id(0x100000001)));
}

#endif
