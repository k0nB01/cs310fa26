#include <catch2/catch_test_macros.hpp>
#include "student.hpp"
#include "instructor.hpp"
#include "csv.hpp"
#include <memory>
#include <type_traits>
static_assert(std::is_abstract_v<Person>);
static_assert(std::is_abstract_v<Serializable>);
static_assert(std::has_virtual_destructor_v<Person>);
TEST_CASE("Student JSON preserves every field") {
  Student s(10,"Alice","alice@uni.edu",2027,{"CS101","HIST110"});
  nlohmann::json expected = {{"role","Student"},{"id",10},{"name","Alice"},{"email","alice@uni.edu"},{"grad_year",2027},{"courses",{"CS101","HIST110"}}};
  CHECK(s.to_json() == expected);
  CHECK(nlohmann::json::parse(s.json_text()) == expected);
}
TEST_CASE("Instructor JSON preserves every field") {
  Instructor i(11,"Bob","bob@uni.edu","C-210",{"CS101"});
  nlohmann::json expected = {{"role","Instructor"},{"id",11},{"name","Bob"},{"email","bob@uni.edu"},{"office","C-210"},{"teaches",{"CS101"}}};
  CHECK(nlohmann::json::parse(i.json_text()) == expected);
}
TEST_CASE("YAML round trips both concrete types") {
  Student s(10,"Alice","alice@uni.edu",2027,{"CS101","HIST110"});
  Instructor i(11,"Bob","bob@uni.edu","C-210",{"CS101"});
  auto sy = YAML::Load(s.yaml_text()); auto iy = YAML::Load(i.yaml_text());
  CHECK(sy["role"].as<std::string>() == "Student");
  CHECK(sy["id"].as<int>() == 10);
  CHECK(sy["name"].as<std::string>() == "Alice");
  CHECK(sy["email"].as<std::string>() == "alice@uni.edu");
  CHECK(sy["grad_year"].as<int>() == 2027);
  CHECK(sy["courses"].as<std::vector<std::string>>() == std::vector<std::string>{"CS101","HIST110"});
  CHECK(iy["role"].as<std::string>() == "Instructor");
  CHECK(iy["id"].as<int>() == 11);
  CHECK(iy["name"].as<std::string>() == "Bob");
  CHECK(iy["email"].as<std::string>() == "bob@uni.edu");
  CHECK(iy["office"].as<std::string>() == "C-210");
  CHECK(iy["teaches"][0].as<std::string>() == "CS101");
}
TEST_CASE("Empty collections remain arrays and sequences") {
  Student s(1,"A","a@uni.edu",2000); Instructor i(2,"B","b@uni.edu","R1");
  CHECK(s.to_json()["courses"] == nlohmann::json::array());
  CHECK(i.to_json()["teaches"] == nlohmann::json::array());
  CHECK(s.to_yaml()["courses"].IsSequence()); CHECK(s.to_yaml()["courses"].size() == 0);
  CHECK(i.to_yaml()["teaches"].IsSequence()); CHECK(i.to_yaml()["teaches"].size() == 0);
}
TEST_CASE("CSV escaping handles special characters") {
  CHECK(csv_escape("") == ""); CHECK(csv_escape("plain") == "plain");
  CHECK(csv_escape("a,b") == "\"a,b\"");
  CHECK(csv_escape("a\"b") == "\"a\"\"b\"");
  CHECK(csv_escape("a\nb") == "\"a\nb\"");
  CHECK(csv_escape("a\rb") == "\"a\rb\"");
  CHECK(csv_escape("a\r\nb") == "\"a\r\nb\"");
}
TEST_CASE("CSV rows and headers match schemas") {
  Student s(12,"Eve, \"The Great\"","eve@uni.edu",2028,{"CS,101","AI\"Lab"});
  CHECK(s.csv_header() == "role,id,name,email,grad_year,courses");
  CHECK(s.csv_row() == "Student,12,\"Eve, \"\"The Great\"\"\",eve@uni.edu,2028,\"CS,101;AI\"\"Lab\"");
  Instructor i(2,"Bob","bob@uni.edu","Room, 1",{"CS101","CS102"});
  CHECK(i.csv_header() == "role,id,name,email,office,teaches");
  CHECK(i.csv_row() == "Instructor,2,Bob,bob@uni.edu,\"Room, 1\",CS101;CS102");
  CHECK(Student(1,"A","a@uni.edu",2000).csv_row() == "Student,1,A,a@uni.edu,2000,");
}
TEST_CASE("Validation rejects invalid inputs in both subclasses") {
  for (int id : {0,-1}) {
    CHECK_THROWS_AS(Student(id,"A","a@uni.edu",2026),ValidationError);
    CHECK_THROWS_AS(Instructor(id,"A","a@uni.edu","R1"),ValidationError);
  }
  CHECK_THROWS_AS(Student(1,"","a@uni.edu",2026),ValidationError);
  CHECK_THROWS_AS(Instructor(1,"","a@uni.edu","R1"),ValidationError);
  for (const auto& email : {"", "not-an-email", "a@@uni.edu", "a b@uni.edu", "a@uni"}) {
    CHECK_THROWS_AS(Student(1,"A",email,2026),ValidationError);
    CHECK_THROWS_AS(Instructor(1,"A",email,"R1"),ValidationError);
  }
  CHECK_THROWS_AS(Student(1,"A","a@uni.edu",1999),ValidationError);
  CHECK_NOTHROW(Student(1,"A","a@uni.edu",2000));
  CHECK_THROWS_AS(Instructor(1,"A","a@uni.edu",""),ValidationError);
}
TEST_CASE("Base interfaces dispatch without downcasts") {
  std::vector<std::unique_ptr<Person>> people;
  people.push_back(std::make_unique<Student>(1,"A","a@uni.edu",2026));
  people.push_back(std::make_unique<Instructor>(2,"B","b@uni.edu","R1"));
  for (const auto& p : people) {
    const Serializable& s = *p;
    CHECK(s.to_json()["role"] == p->role());
    CHECK(s.to_yaml()["role"].as<std::string>() == p->role());
    CHECK(s.csv_row().starts_with(p->role() + ","));
    CHECK(s.csv_header().starts_with("role,id,name,email,"));
    CHECK(p->id() > 0); CHECK_FALSE(p->name().empty()); CHECK_FALSE(p->email().empty());
  }
}
TEST_CASE("Invalid UTF-8 JSON text raises SerializationError") {
  Student s(1,std::string(1,static_cast<char>(0xff)),"a@uni.edu",2026);
  CHECK_THROWS_AS(s.json_text(),SerializationError);
}
class BrokenYaml : public Student {
public:
  using Student::Student;
  YAML::Node to_yaml() const override { throw YAML::RepresentationException(YAML::Mark::null_mark(),"test failure"); }
};
TEST_CASE("YAML failures become SerializationError") {
  BrokenYaml s(1,"A","a@uni.edu",2026);
  CHECK_THROWS_AS(s.yaml_text(),SerializationError);
}
