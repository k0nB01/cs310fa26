#include "person.hpp"
#include <regex>
Person::Person(int id, std::string name, std::string email)
  : id_(id), name_(std::move(name)), email_(std::move(email)) {
  if (id_ <= 0) throw ValidationError("id must be positive");
  if (name_.empty()) throw ValidationError("name is required");
  static const std::regex re(R"(^[^@\s]+@[^@\s]+\.[^@\s]+$)");
  if (!std::regex_match(email_, re)) throw ValidationError("Invalid email: " + email_);
}
