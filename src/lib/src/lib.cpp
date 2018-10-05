#include "lib.hpp"
#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <boost/noncopyable.hpp>
#include <boost/core/ignore_unused.hpp>

namespace {
  const std::locale locale{ std::locale() };
}
/**
 * Time cost: O(N), where N is in.size(); bc/ count.size() <= in.size()
 * Spatial cost: O(1) | possible values of a character
 * Parallelisation?
 *   locking over operator[] would cause great contention
 *   generate multiple hash maps in parallel and merge them at the end
 * Alternative implementation:
 *   we could use a fixed-size array instead of an unordered_map
 *   that would be easier to parallelise, even with contention: several threads could share data-structure
 */
bool is_palindrome_permutation(const std::string& in) {
  // TODO: Could be a fixed-size array
  //       That would be slower to initialize but faster to use
  std::unordered_map<char, std::size_t> count;

  for (const char c : in ) {
    if (!std::isspace(c, locale)) {
      count[std::tolower(c, locale)]++;
    }
  }

  // if we see two letters with an odd count, we cannot make a palindrome
  bool seen_odd{ false };
  for (auto [key, value] : count) {
    boost::ignore_unused(key);
    if (value%2 != 0) {
      if (seen_odd)
        return false;
      seen_odd = true;
    }
  }

  // otherwise, we can
  return true;
}

bool is_palindrome_permutation_array(const std::string& in) {
  // TODO: Can we extract this info from the standard library?
  constexpr std::size_t kPossibleCharValues{ 255 };
  std::array<std::size_t, kPossibleCharValues> count;
  for(std::size_t i = 0; i < kPossibleCharValues; ++i) {
    count[i] = 0; }

  std::size_t index{ 0 };
  /*  for(; (index+7)<in.size(); index+=8) {
    if (!std::isspace(in[index], locale)) {
      count[std::tolower(in[index], locale)]++;
    }
    if (!std::isspace(in[index+1], locale)) {
      count[std::tolower(in[index+1], locale)]++;
    }
    if (!std::isspace(in[index+2], locale)) {
      count[std::tolower(in[index+2], locale)]++;
    }
    if (!std::isspace(in[index+3], locale)) {
      count[std::tolower(in[index+3], locale)]++;
    }
    if (!std::isspace(in[index+4], locale)) {
      count[std::tolower(in[index+4], locale)]++;
    }
    if (!std::isspace(in[index+5], locale)) {
      count[std::tolower(in[index+5], locale)]++;
    }
    if (!std::isspace(in[index+6], locale)) {
      count[std::tolower(in[index+6], locale)]++;
    }
    if (!std::isspace(in[index+7], locale)) {
      count[std::tolower(in[index+7], locale)]++;
    }
    }*/
  for(; index<in.size(); ++index) {
    if (!std::isspace(in[index], locale)) {
      count[std::tolower(in[index], locale)]++;
    }
  }

  // if we see two letters with an odd count, we cannot make a palindrome
  bool seen_odd{ false };
  for (auto value : count) {
    if (value%2 != 0) {
      if (seen_odd)
        return false;
      seen_odd = true;
    }
  }

  // otherwise, we can
  return true;
}

bool is_palindrome_permutation_parallel(const std::string& in,
                                        const std::size_t max_threads) {
  // TODO: Can we extract this info from the standard library?
  constexpr std::size_t kPossibleCharValues{ 255 };

  std::vector<std::thread> workers;
  std::vector<std::array<std::size_t, kPossibleCharValues>> data;

  workers.reserve(max_threads);
  data.reserve(max_threads);
  //  std::cout << "\nAlgorithm over a string of size " << in.size()
  //            << " uses buckets of size " << bucket_size << std::endl;

  for (std::size_t index{0}; index<max_threads; ++index) {
    data.emplace_back();
    auto& count = data.back();
    workers.push_back(std::thread([index, max_threads, &in, &count]() {
          const std::size_t bucket_size =
            std::clamp<std::size_t>(std::ceil(float(in.size()) / float(max_threads)),
                                    1, in.size());
          const std::size_t start =
            std::clamp<std::size_t>(bucket_size * index, 0, in.size());
          const std::size_t end =
            std::clamp<std::size_t>(bucket_size * (index+1), 0, in.size());
          //          std::cout << "Thread " << index <<
          //            " -> [" << start << ", " << end << "]" << std::endl;

          for(std::size_t i = 0; i < kPossibleCharValues; ++i) {
            count[i] = 0; }

          std::size_t index{ start };
          for(; index<end; ++index) {
            if (!std::isspace(in[index], locale)) {
              count[std::tolower(in[index], locale)]++;
            }
          }}));
    usleep(10);
  }

  for(auto& th : workers) {
    th.join();
  }

  // if we see two letters with an odd count, we cannot make a palindrome
  bool seen_odd{ false };
  for (std::size_t index{0}; index < kPossibleCharValues; ++index) {
    std::size_t value{ 0 };
    for (auto& d : data) {
      value += d[index];
    }
    if (value%2 != 0) {
      if (seen_odd) {
        return false;
      }
      seen_odd = true;
    }
  }

  // otherwise, we can
  return true;
}
