/**
 * This is the unit test file for http/Header class
 */

#include "http/header.h"

#include <iostream>
#include "catch2/catch_test_macros.hpp"
#include "http/http_utils.h"
/* for convenience reason */
using Next::Http::CRLF;
using Next::Http::Header;

TEST_CASE("[http/header]") {
  SECTION("normal constructor method for header") {
    std::string k = "http_version";
    std::string v1 = "1.1";
    std::string v2 = "1.2";
    Header h1(k, v1);
    CHECK(h1.IsValid());
    CHECK(h1.GetKey() == k);
    CHECK(h1.GetValue() == v1);

    h1.SetValue(v2);
    CHECK(h1.GetValue() == v2);
    std::cout << h1.Serialize() << std::endl;
  }

  SECTION("serialize/deserialize a header from string") {
    std::string valid_header_line = "Host:GXJ";
    std::string invalid_header_line = "Hello& No Way!";
    Header h1(valid_header_line);
    Header h2(invalid_header_line);

    CHECK(h1.IsValid());
    CHECK(h1.GetKey() == "Host");
    CHECK(h1.GetValue() == "GXJ");

    // no colon in this header line, is not valid
    CHECK(!h2.IsValid());

    std::string serialized = h1.Serialize();
    std::cout << serialized << std::endl;
    CHECK(serialized == (valid_header_line + CRLF));
  }
}
