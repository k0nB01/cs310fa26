#pragma once
#include <string>
#include <vector>
inline std::string csv_escape(const std::string& s) {
  if (s.find_first_of(",\"\r\n") == std::string::npos) return s;
  std::string result = "\"";
  for (char c : s) { if (c == '\"') result += '\"'; result += c; }
  return result + "\"";
}
inline std::string csv_list(const std::vector<std::string>& values) {
  std::string result;
  for (std::size_t i = 0; i < values.size(); ++i) {
    if (i) result += ';';
    result += values[i];
  }
  return csv_escape(result);
}
