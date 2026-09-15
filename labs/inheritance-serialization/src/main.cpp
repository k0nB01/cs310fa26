#include "student.hpp"
#include "instructor.hpp"
#include <iostream>
#include <memory>
#include <vector>
int main() {
  try {
    std::vector<std::unique_ptr<Person>> people;
    people.push_back(std::make_unique<Student>(1,"Ada","ada@uni.edu",2026,std::vector<std::string>{"CS101","MATH200"}));
    people.push_back(std::make_unique<Instructor>(2,"Grace","grace@uni.edu","Room 314",std::vector<std::string>{"CS101"}));
    nlohmann::json j = nlohmann::json::array();
    YAML::Node y(YAML::NodeType::Sequence);
    for (const auto& p : people) { j.push_back(p->to_json()); y.push_back(p->to_yaml()); }
    try { std::cout << "JSON:\n" << j.dump(2) << "\n\nYAML:\n" << YAML::Dump(y) << '\n'; }
    catch (const nlohmann::json::exception& e) { throw SerializationError(e.what()); }
    catch (const YAML::Exception& e) { throw SerializationError(e.what()); }
    for (const auto& p : people)
      std::cout << "\nCSV (" << p->role() << "):\n" << p->csv_header() << '\n' << p->csv_row() << '\n';
  } catch (const ValidationError& e) { std::cerr << "ValidationError: " << e.what() << '\n'; return 2;
  } catch (const SerializationError& e) { std::cerr << "SerializationError: " << e.what() << '\n'; return 3;
  } catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; }
}
