#pragma once
#include "serializable.hpp"
#include <utility>
class Person : public Serializable {
protected:
  int id_;
  std::string name_, email_;
public:
  Person(int id, std::string name, std::string email);
  virtual std::string role() const = 0;
  int id() const { return id_; }
  const std::string& name() const { return name_; }
  const std::string& email() const { return email_; }
};
