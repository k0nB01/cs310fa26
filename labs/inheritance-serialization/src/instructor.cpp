#include "instructor.hpp"
#include "csv.hpp"
Instructor::Instructor(int id, std::string name, std::string email, std::string office,
                       std::vector<std::string> teaches)
  : Person(id, std::move(name), std::move(email)), office_(std::move(office)), teaches_(std::move(teaches)) {
  if (office_.empty()) throw ValidationError("office is required");
}
nlohmann::json Instructor::to_json() const {
  return {{"role",role()},{"id",id_},{"name",name_},{"email",email_},
          {"office",office_},{"teaches",teaches_}};
}
YAML::Node Instructor::to_yaml() const {
  YAML::Node n;
  n["role"] = role(); n["id"] = id_; n["name"] = name_; n["email"] = email_;
  n["office"] = office_;
  n["teaches"] = YAML::Node(YAML::NodeType::Sequence);
  for (const auto& c : teaches_) n["teaches"].push_back(c);
  return n;
}
std::string Instructor::csv_header() const { return "role,id,name,email,office,teaches"; }
std::string Instructor::csv_row() const {
  return csv_escape(role()) + "," + std::to_string(id_) + "," + csv_escape(name_) + "," +
    csv_escape(email_) + "," + csv_escape(office_) + "," + csv_list(teaches_);
}
