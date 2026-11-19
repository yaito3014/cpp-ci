cmake_minimum_required(VERSION 3.23)

project(cpp_ci_fixture LANGUAGES CXX)

enable_testing()

add_executable(cpp_ci_fixture main.cpp)

# Lower bounds of the __cplusplus (or _MSVC_LANG) value for each standard; the
# bounds for 23 and 26 accept the provisional values compilers report.
set(CPP_CI_MIN_11 201103L)
set(CPP_CI_MIN_14 201402L)
set(CPP_CI_MIN_17 201703L)
set(CPP_CI_MIN_20 202002L)
set(CPP_CI_MIN_23 202003L)
set(CPP_CI_MIN_26 202303L)
if(DEFINED CMAKE_CXX_STANDARD)
    target_compile_definitions(
        cpp_ci_fixture
        PRIVATE CPP_CI_MIN_CPLUSPLUS=${CPP_CI_MIN_${CMAKE_CXX_STANDARD}}
    )
endif()

if(CMAKE_CXX_FLAGS MATCHES "-stdlib=libc\\+\\+")
    target_compile_definitions(cpp_ci_fixture PRIVATE CPP_CI_EXPECT_LIBCXX)
endif()

add_test(NAME cpp_ci_fixture COMMAND cpp_ci_fixture)
