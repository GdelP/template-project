#include "lib.hpp"
#include <gtest/gtest.h>
#include <algorithm>
#include <random>

// Quick workaround to avoid creating an abstract interface
#define is_palindrome_permutation is_palindrome_permutation_parallel

TEST(IsPalindromePermutation, EmptyString) {
  // empty case: the word reads the same forward and backwards
  EXPECT_TRUE(is_palindrome_permutation(""));
}

TEST(IsPalindromePermutation, OneLetter) {
  for(char c = 'a'; c<'z'; ++c) {
    {
      SCOPED_TRACE("lower letter");
      std::string str;
      str.push_back(c);
      EXPECT_TRUE(is_palindrome_permutation(str));
    }
    {
      SCOPED_TRACE("capital letter");
      std::string str;
      str.push_back(std::toupper(c, std::locale()));
      EXPECT_TRUE(is_palindrome_permutation(str));
    }
  }
}

TEST(IsPalindromePermutation, TwoLetters) {
  {
    SCOPED_TRACE("Same letter repeated twice");
    for(char c = 'a'; c<'z'; ++c) {
      {
        SCOPED_TRACE("lower,lower");
        std::string str;
        str.push_back(c);
        str.push_back(c);
        EXPECT_TRUE(is_palindrome_permutation(str));
      }
      {
        SCOPED_TRACE("lower,capital");
        std::string str;
        str.push_back(c);
        str.push_back(std::toupper(c, std::locale()));
        EXPECT_TRUE(is_palindrome_permutation(str));
      }
      {
        SCOPED_TRACE("capital,lower");
        std::string str;
        str.push_back(std::toupper(c, std::locale()));
        str.push_back(c);
        EXPECT_TRUE(is_palindrome_permutation(str));
      }
      {
        SCOPED_TRACE("capital,capital");
        std::string str;
        str.push_back(std::toupper(c, std::locale()));
        str.push_back(std::toupper(c, std::locale()));
        EXPECT_TRUE(is_palindrome_permutation(str));
      }
    }
  }
  {
    SCOPED_TRACE("Two different letters");
    for(char c = 'a'; c<'z'; ++c) {
      for(char c2 = 'a'; c2<'z'; ++c2) {
        if (c != c2) {
          std::string str;
          str.push_back(c);
          str.push_back(c2);
          EXPECT_FALSE(is_palindrome_permutation(str));
        }
      }
    }
  }
}

TEST(IsPalindromePermutation, AllThreeLetterPalindromes) {
  for(char c = 'a'; c<'z'; ++c) {
    for(char c2 = c; c2<'z'; ++c2) {
      constexpr auto bool_values = {true, false};
      for(const bool firstUpper : bool_values) {
        for(const bool secondUpper : bool_values) {
          for(const bool thirdUpper : bool_values) {
            std::string palindrome;
            palindrome.push_back(firstUpper
                                 ? std::toupper(c,std::locale())
                                 : c);
            palindrome.push_back(secondUpper
                                 ? std::toupper(c2,std::locale())
                                 : c2);
            palindrome.push_back(thirdUpper
                                 ? std::toupper(c,std::locale())
                                 : c);
            EXPECT_TRUE(is_palindrome_permutation(palindrome));

            std::string copy{ palindrome };
            while(std::next_permutation(palindrome.begin(), palindrome.end())) {
              EXPECT_TRUE(is_palindrome_permutation(palindrome));
            }
            copy = palindrome;
            while(std::prev_permutation(palindrome.begin(), palindrome.end())) {
              EXPECT_TRUE(is_palindrome_permutation(palindrome));
            }
          }
        }
      }
    }
  }
}

TEST(IsPalindromePermutation, SomeCapitalLetters) {
  EXPECT_TRUE(is_palindrome_permutation("Tact Coa"));
  EXPECT_TRUE(is_palindrome_permutation("tact coa"));
  EXPECT_TRUE(is_palindrome_permutation("tAcT cOa"));
}

namespace {
  constexpr char const*const kExample{ "tact coa" };
  // TODO: subsitute manual calculation with automated one
  constexpr std::size_t kExampleSize{ 8 };
  const std::locale locale{ std::locale() };
}

TEST(IsPalindromePermutation, Example) {
  EXPECT_TRUE(is_palindrome_permutation(kExample));

  // all capital letters
  std::string copy;
  copy.reserve(kExampleSize);
  for(std::size_t i=0; i<kExampleSize; ++i) {
    const char c = kExample[i];
    copy.push_back(std::toupper(c, locale));
  }
  EXPECT_TRUE(is_palindrome_permutation(copy));
}

TEST(IsPalindromePermutation, Example_OneCapitalLetter) {
  for(std::size_t i=0; i<kExampleSize; ++i) {
    std::string copy{ kExample };
    copy[i] = std::toupper(copy[i], locale);
    EXPECT_TRUE(is_palindrome_permutation(copy));
  }
}

TEST(IsPalindromePermutation, Example_TwoCapitalLetters) {
  for(std::size_t i=0; i<kExampleSize; ++i) {
    for(std::size_t j=i; j<kExampleSize; ++j) {
      std::string copy{ kExample };
      copy[i] = std::toupper(copy[i], locale);
      copy[j] = std::toupper(copy[j], locale);
      EXPECT_TRUE(is_palindrome_permutation(copy));
    }
  }
}

TEST(IsPalindromePermutation, NoSpaces) {
  EXPECT_TRUE(is_palindrome_permutation("TactCoa"));
  EXPECT_TRUE(is_palindrome_permutation("tactcoa"));
  EXPECT_TRUE(is_palindrome_permutation("tAcTcOa"));
}

TEST(IsPalindromePermutation, OneSpace) {
  for(std::size_t i=0; i<kExampleSize; ++i) {
    std::string example{ kExample };
    example.insert(std::next(example.begin(), i), ' ');
    EXPECT_TRUE(is_palindrome_permutation(example));
  }
}
TEST(IsPalindromePermutation, TwoSpaces) {
  for(std::size_t i=0; i<kExampleSize; ++i) {
    for(std::size_t j=i; j<kExampleSize; ++j) {
      std::string example{ kExample };
      example.insert(std::next(example.begin(), i), ' ');
      example.insert(std::next(example.begin(), j+1), ' ');
      EXPECT_TRUE(is_palindrome_permutation(example));
    }
  }
}

TEST(IsPalindromePermutation, WeirdSpacing) {
  EXPECT_TRUE(is_palindrome_permutation(" TactCoa"));
  EXPECT_TRUE(is_palindrome_permutation("tactcoa "));
  EXPECT_TRUE(is_palindrome_permutation(" tAcTcOa "));
  EXPECT_TRUE(is_palindrome_permutation(" tAc T c Oa "));
}

struct Palindromes : public ::testing::TestWithParam<std::tuple<std::size_t,
                                                               std::size_t>> {
public:
  std::string generate_random_string(const std::size_t size) {
    constexpr std::size_t range = 'z' - 'a';
    std::string ret;

    ret.reserve(size);
    for (std::size_t i = 0; i < size; ++i) {
      char c = 'a' + (random()%range);
      ret.push_back(c);
    }

    return ret;
  }

  std::string generate_random_palindrome(const std::size_t size) {
    std::string palin = generate_random_string(size);
    palin.reserve(size*2);
    std::reverse_copy(palin.begin(), palin.end(), std::back_inserter(palin));
    return palin;
  }

  std::size_t repetitions() const {
    return std::get<0>(GetParam()); }
  std::size_t string_size() const {
    return std::get<1>(GetParam()); }

protected:
  std::default_random_engine random;
};

TEST_P(Palindromes, AlreadyPalindromes) {
  for (std::size_t i=0; i < repetitions(); ++i) {
    const auto str = generate_random_palindrome(string_size());
    EXPECT_TRUE(is_palindrome_permutation(str)) << str;
  }
}

TEST_P(Palindromes, OffByOne) {
  for (std::size_t i=0; i < repetitions(); ++i) {
    std::string str = generate_random_palindrome(string_size());
    str.insert(std::next(str.begin(), (random() % str.size())),
               'a');
    EXPECT_TRUE(is_palindrome_permutation(str)) << str;
    str.insert(std::next(str.begin(), (random() % str.size())),
               'b');
    EXPECT_FALSE(is_palindrome_permutation(str)) << str;
  }
}

INSTANTIATE_TEST_CASE_P(PalindromesTest, Palindromes,
                        ::testing::Combine
                        (::testing::Values(3, 10,100),
                         ::testing::Values(3, 10,1000,10000,20000)));

TEST(IsPalindromePermutation, NonPalindromesByOne) {
  EXPECT_FALSE(is_palindrome_permutation("albcddcb"));
  EXPECT_FALSE(is_palindrome_permutation("bcdldcba"));
  EXPECT_FALSE(is_palindrome_permutation("abdndcba"));
  EXPECT_FALSE(is_palindrome_permutation("abcdldca"));
  EXPECT_FALSE(is_palindrome_permutation("abcddvcbak"));
  EXPECT_FALSE(is_palindrome_permutation("abcddcbbaK"));
  EXPECT_FALSE(is_palindrome_permutation("abcd dcbaKD "));
}

TEST(IsPalindromePermutation, NoRepeatedLetters) {
  EXPECT_FALSE(is_palindrome_permutation("abcdefghijklmnopqrstuvwxyz"));
}

TEST(IsPalindromePermutation, OrderedLongStrings) {
  // long string
  auto alphabet = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                   'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                   's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
  std::string lo;
  constexpr int kAlphabetTimes{ 100 };
  for (std::size_t i=0; i<kAlphabetTimes; ++i) {
    for (auto c : alphabet) {
      lo += c;
    }
  }
  EXPECT_TRUE(is_palindrome_permutation(lo));

  // even longer string
  constexpr int kAlphabetTimesBigger{ 100000 };
  for (std::size_t i=0; i<kAlphabetTimesBigger; ++i) {
    for (auto c : alphabet) {
      lo += c;
    }
  }
  EXPECT_TRUE(is_palindrome_permutation(lo));
}
