#include "student.hpp"
#include "csv.hpp"
Student::Student(int id, std::string name, std::string email, int grad_year,
                 std::vector<std::string> courses)
  : Person(id, std::move(name), std::move(email)), grad_year_(grad_year), courses_(std::move(courses)) {
  if (grad_year_ < 2000) throw ValidationError("grad_year too small");
}
nlohmann::json Student::to_json() const {
  return {{"role",role()},{"id",id_},{"name",name_},{"email",email_},
          {"grad_year",grad_year_},{"courses",courses_}};
}
YAML::Node Student::to_yaml() const {
  YAML::Node n;
  n["role"] = role(); n["id"] = id_; n["name"] = name_; n["email"] = email_;
  n["grad_year"] = grad_year_;
  n["courses"] = YAML::Node(YAML::NodeType::Sequence);
  for (const auto& c : courses_) n["courses"].push_back(c);
  return n;
}
std::string Student::csv_header() const { return "role,id,name,email,grad_year,courses"; }
std::string Student::csv_row() const {
  return csv_escape(role()) + "," + std::to_string(id_) + "," + csv_escape(name_) + "," +
    csv_escape(email_) + "," + std::to_string(grad_year_) + "," + csv_list(courses_);
}
