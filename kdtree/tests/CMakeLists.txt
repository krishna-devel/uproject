project(tests)

configure_file(basic_tests/data/sample_data.csv basic_tests/data/sample_data.csv)
configure_file(basic_tests/data/dummy_data.csv basic_tests/data/dummy_data.csv)
add_subdirectory(lib/googlemock)
add_subdirectory(basic_tests)
