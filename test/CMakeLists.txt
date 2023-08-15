set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

set(BVC_TEST_SOURCES
    "entropy/entropy_test.h"
    "entropy/entropy_test.cpp"
    "wavelet/wavelet_test.h"
    "wavelet/wavelet_test.cpp"
    "main.cpp")

if(MSVC)
  set(cxx_flags /GS /W3 /WX /Za /EHsc)
elseif(CMAKE_COMPILER_IS_GNUCXX)
  set(cxx_flags -Wall -Wshadow -Werror -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -Wno-sign-compare -fexceptions)
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8)
    set(cxx_flags ${cxx_flags} -Wno-error=missing-braces)
  endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(cxx_flags -Wshadow -Werror -fexceptions)
endif()

add_library(bvc_test_lib STATIC ${BVC_TEST_SOURCES})
target_compile_options(bvc_test_lib PRIVATE ${cxx_flags})
target_include_directories(bvc_test_lib PUBLIC . ../src)
target_link_libraries(bvc_test_lib PRIVATE bvc_enc bvc_dec)

add_executable(bvc_test ${BVC_TEST_SOURCES})
target_compile_options(bvc_test PRIVATE ${cxx_flags})
target_include_directories(bvc_test PUBLIC . ../src)
target_link_libraries(bvc_test LINK_PUBLIC bvc_enc bvc_dec)
add_test(bvc_test bvc_test)