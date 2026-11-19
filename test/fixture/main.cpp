#if defined(_MSVC_LANG)
#define CPP_CI_CPLUSPLUS _MSVC_LANG
#else
#define CPP_CI_CPLUSPLUS __cplusplus
#endif

#ifdef CPP_CI_MIN_CPLUSPLUS
static_assert(CPP_CI_CPLUSPLUS >= CPP_CI_MIN_CPLUSPLUS, "CMAKE_CXX_STANDARD was not applied");
#endif

#ifdef CPP_CI_EXPECT_LIBCXX
#include <cstddef>
#ifndef _LIBCPP_VERSION
#error "expected libc++"
#endif
#endif

int main() {}
