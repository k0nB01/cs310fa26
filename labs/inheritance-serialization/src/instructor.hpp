#pragma once
#include "person.hpp"
#include <vector>
class Instructor : public Person {
  std::string office_;
  std::vector<std::string> teaches_;
public:
  Instructor(int id, std::string name, std::string email, std::string office,
             std::vector<std::string> teaches = {});
  std::string role() const override { return "Instructor"; }
  nlohmann::json to_json() const override;
  YAML::Node to_yaml() const override;
  std::string csv_header() const override;
  std::string csv_row() const override;
};
