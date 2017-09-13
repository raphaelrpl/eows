#include "generator.hpp"

// EOWS Core
#include "../../core/utils.hpp"

static const std::string allowed_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";


eows::auth::nonce_generator::nonce_generator(int length)
  : random_(static_cast<unsigned int>(eows::core::unix_timestamp(nullptr))),
    length_(length)
{
}

std::string eows::auth::nonce_generator::generate()
{
  std::uniform_int_distribution<> distr(0, static_cast<int>(allowed_chars.size() - 1));
  std::string result;
  result.reserve(length());
  std::generate_n(std::back_inserter(result), length(), [&]() { return allowed_chars[distr(random_)]; } );
  return result;
}

const int&eows::auth::nonce_generator::length() const
{
  return length_;
}

void eows::auth::nonce_generator::set_length(const int length)
{
  length_ = length;
}
