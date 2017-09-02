#include <iostream>
#include <cassert>

#include "readers.h"
#include "readers_util.h"

#define ASSERT_EQ(expected, actual) { \
    auto __expected = expected; \
    auto __actual = actual; \
    if (__expected != __actual) { \
        std::cerr << __FILE__ << ":" << __LINE__ << ": Assertion error" << std::endl; \
        std::cerr << "\texpected: " << __expected << " (= " << #expected << ")" << std::endl; \
        std::cerr << "\tgot: " << __actual << " (= " << #actual << ")" << std::endl; \
        std::terminate(); \
    } \
}

// shortcuts for tests
std::unique_ptr<Reader> MakeR(const std::string& data) {
    return std::unique_ptr<StringReader>(new StringReader(data));
}

void TestStringReader() {
    ASSERT_EQ("", ReadAll(MakeR("").get()));
    ASSERT_EQ("a", ReadAll(MakeR("a").get()));

    std::string big_string(1 << 15, 'f');
    ASSERT_EQ(big_string, ReadAll(MakeR(big_string).get()));
}

void TestLimitReader() {
    LimitReader l1(MakeR(""), 0);
    ASSERT_EQ("", ReadAll(&l1));

    LimitReader l2(MakeR(" "), 0);
    ASSERT_EQ("", ReadAll(&l2));

    LimitReader l3(MakeR("ab"), 1);
    ASSERT_EQ("a", ReadAll(&l3));

    srand(42);
    std::string big_string(500, '\0');
    for (char& c : big_string) { c = 'A' + (rand() % 10); }

    LimitReader l4(MakeR(big_string), 123);
    ASSERT_EQ(big_string.substr(0, 123), ReadAll(&l4));
}

TeeReader MakeTee(std::vector<std::string> chunks) {
    std::vector<std::unique_ptr<Reader>> readers;
    for (auto c : chunks) {
        readers.emplace_back(MakeR(c));
    }
    return TeeReader(std::move(readers));
}

void TestTeeReader() {
    TeeReader t1({});
    ASSERT_EQ("", ReadAll(&t1));

    TeeReader t2 = MakeTee({""});
    ASSERT_EQ("", ReadAll(&t2));

    TeeReader t3 = MakeTee({"abc"});
    ASSERT_EQ("abc", ReadAll(&t3));

    TeeReader t4 = MakeTee({"abc", "def", "g"});
    ASSERT_EQ("abcdefg", ReadAll(&t4));

    TeeReader t5 = MakeTee({"abc", "", "def"});
    ASSERT_EQ("abcdef", ReadAll(&t5));

    TeeReader t6 = MakeTee({"abc", "", "", "", "" "def", "", ""});
    ASSERT_EQ("abcdef", ReadAll(&t6));

    srand(42);
    std::string big_string(500, '\0');
    for (char& c : big_string) { c = 'A' + (rand() % 10); }

    TeeReader t7 = MakeTee({ big_string, big_string, big_string });
    ASSERT_EQ(big_string + big_string + big_string, ReadAll(&t7));
}

void TestHexReader() {
    HexDecodingReader h1(MakeR(""));
    ASSERT_EQ("", ReadAll(&h1));

    HexDecodingReader h2(MakeR("616263"));
    ASSERT_EQ("abc", ReadAll(&h2));

    std::string big_hex = "74657374666f6f626172";
    std::string answer = "testfoobar";
    for (int i = 0; i < 6; ++i) {
        big_hex += big_hex;
        answer += answer;
    }

    HexDecodingReader h3(MakeR(big_hex));
    ASSERT_EQ(answer, ReadAll(&h3));
}


int main() {
    TestStringReader();
    TestLimitReader();
    TestTeeReader();
    TestHexReader();

    return 0;
}
