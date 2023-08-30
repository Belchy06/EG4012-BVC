set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

set(OVC_TEST_SOURCES
    "misc/lenna.cpp"
    "util/ppm.cpp"
    "entropy/entropy_test.cpp"
    "wavelet/wavelet_test.cpp"
    "partition/partition_test.cpp"
    "encode/encode_test.cpp"
    "decode/decode_test.cpp"
    "e2e/e2e_test.cpp"
    "resilience/resilience_test.cpp"
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

add_library(ovc_test_lib STATIC ${OVC_TEST_SOURCES})
target_compile_options(ovc_test_lib PRIVATE ${cxx_flags})
target_include_directories(ovc_test_lib PUBLIC . ../src)
target_link_libraries(ovc_test_lib PRIVATE ovc_enc ovc_dec)

add_executable(ovc_test ${OVC_TEST_SOURCES})
target_compile_options(ovc_test PRIVATE ${cxx_flags})
target_include_directories(ovc_test PUBLIC . ../src)
target_link_libraries(ovc_test LINK_PUBLIC ovc_enc ovc_dec)
add_test(ovc_test ovc_test)