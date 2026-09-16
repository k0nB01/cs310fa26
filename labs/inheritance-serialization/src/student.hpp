#pragma once
#include "person.hpp"
#include <vector>
class Student : public Person {
  int grad_year_;
  std::vector<std::string> courses_;
public:
  Student(int id, std::string name, std::string email, int grad_year,
          std::vector<std::string> courses = {});
  std::string role() const override { return "Student"; }
  nlohmann::json to_json() const override;
  YAML::Node to_yaml() const override;
  std::string csv_header() const override;
  std::string csv_row() const override;
};
