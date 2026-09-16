#pragma once
#include "exceptions.hpp"
#include <string>
#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>
struct Serializable {
  virtual ~Serializable() = default;
  virtual nlohmann::json to_json() const = 0;
  virtual YAML::Node to_yaml() const = 0;
  virtual std::string csv_header() const = 0;
  virtual std::string csv_row() const = 0;
  std::string json_text() const {
    try { return to_json().dump(2); }
    catch (const nlohmann::json::exception& e) { throw SerializationError(e.what()); }
  }
  std::string yaml_text() const {
    try { return YAML::Dump(to_yaml()); }
    catch (const YAML::Exception& e) { throw SerializationError(e.what()); }
  }
};
