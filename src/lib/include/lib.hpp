#pragma once
#include <string>

bool is_palindrome_permutation(const std::string& in);
bool is_palindrome_permutation_array(const std::string& in);
bool is_palindrome_permutation_parallel(const std::string& in,
                                        const std::size_t max_threads = 12);
