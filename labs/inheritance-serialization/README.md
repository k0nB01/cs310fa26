# Inheritance and serialization lab

C++20 Person hierarchy with JSON, YAML, CSV, exceptions, and Catch2 tests.

## Build and run

From this directory (requires CMake >= 3.20, a C++20 compiler, Git, and network access for the pinned dependencies):

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j2
ctest --test-dir build --output-on-failure
./build/app
```

## Design

Serializable and Person are abstract. Student and Instructor implement the same serialization contract. The virtual destructor supports ownership through unique_ptr<Person>; the demo dispatches every format without downcasts. Type-specific fields and CSV headers are preserved. Empty collections serialize as JSON arrays and YAML sequences.

ValidationError is thrown for nonpositive IDs, empty names, malformed email addresses, graduation years before 2000, and empty instructor offices. The lab's deliberately simple email regex is not a complete email-address standard validator.

json_text() and yaml_text() translate library serialization exceptions to SerializationError. The demo also translates errors while emitting its combined JSON/YAML output. Tests cover invalid UTF-8 at JSON emission and an injected YAML failure. Allocation and other unexpected failures remain standard exceptions.

CSV quotes fields containing commas, double quotes, carriage returns, or newlines and doubles embedded quotes. Course names are joined with semicolons before field escaping, as specified by the lab. This course-list convention is not reversible if a course itself contains a semicolon; no CSV deserializer is provided. CSV headers differ by concrete type, so the demo prints separate tables.

Tests cover both types and all formats, validation boundaries, empty collections, escaping, polymorphism, and serialization exceptions.
