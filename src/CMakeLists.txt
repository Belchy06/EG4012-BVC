set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

set(BVC_COMMON_SOURCES
    "bvc_common/format.h"
    "bvc_common/bitstream.h"
    "bvc_common/bitstream.cpp"
    "bvc_common/entropy/entropy.h"
    "bvc_common/entropy/huffman/huffman.h"
    "bvc_common/entropy/huffman/node.h"
    "bvc_common/entropy/huffman/linked_list.h"
    "bvc_common/entropy/huffman/huffman_node.h"
    "bvc_common/entropy/huffman/huffman_node.cpp"
    "bvc_common/util/vector.h"
    "bvc_common/util/matrix.h"
    "bvc_common/util/matrix.cpp"
    "bvc_common/wavelet/wavelet.h"
    "bvc_common/wavelet/wavelet.cpp"
    "bvc_common/wavelet/haar/haar_config.h"
    "bvc_common/wavelet/haar/haar.h"
    "bvc_common/wavelet/haar/haar.cpp"
    "bvc_common/wavelet/biorthogonal/biorthogonal_config.h"
    "bvc_common/wavelet/biorthogonal/biorthogonal.h"
    "bvc_common/wavelet/biorthogonal/biorthogonal.cpp"
    "bvc_common/wavelet/coiflets/coiflets_config.h"
    "bvc_common/wavelet/coiflets/coiflets.h"
    "bvc_common/wavelet/coiflets/coiflets.cpp")

set(BVC_DEC_SOURCES
    "bvc_dec/entropy_coding/abac/abac_decoder.h"
    "bvc_dec/entropy_coding/abac/abac_decoder.cpp"
    "bvc_dec/entropy_coding/huffman/huffman_decoder.h"
    "bvc_dec/entropy_coding/huffman/huffman_decoder.cpp"
    "bvc_dec/entropy_coding/entropy_decoder.h"
    "bvc_dec/entropy_coding/entropy_decoder_factory.h"
    "bvc_dec/entropy_coding/entropy_decoder_factory.cpp"
    "bvc_dec/wavelet/haar/haar_reconstructor.h"
    "bvc_dec/wavelet/haar/haar_reconstructor.cpp"
    "bvc_dec/wavelet/biorthogonal/biorthogonal_reconstructor.h"
    "bvc_dec/wavelet/biorthogonal/biorthogonal_reconstructor.cpp"
    "bvc_dec/wavelet/coiflets/coiflets_reconstructor.h"
    "bvc_dec/wavelet/coiflets/coiflets_reconstructor.cpp"
    "bvc_dec/wavelet/wavelet_reconstructor.h"
    "bvc_dec/wavelet/wavelet_reconstructor_factory.h"
    "bvc_dec/wavelet/wavelet_reconstructor_factory.cpp"
    "bvc_dec/result.h"
    "bvc_dec/picture.h"
    "bvc_dec/nal.h"
    "bvc_dec/config.h"
    "bvc_dec/bvc_dec.h"
    "bvc_dec/bvc_dec.cpp")

set(BVC_ENC_SOURCES
    "bvc_enc/entropy_coding/abac/abac_encoder.h"
    "bvc_enc/entropy_coding/abac/abac_encoder.cpp"
    "bvc_enc/entropy_coding/huffman/huffman_encoder.h"
    "bvc_enc/entropy_coding/huffman/huffman_encoder.cpp"
    "bvc_enc/entropy_coding/entropy_encoder.h"
    "bvc_enc/entropy_coding/entropy_encoder_factory.h"
    "bvc_enc/entropy_coding/entropy_encoder_factory.cpp"
    "bvc_enc/wavelet/haar/haar_decomposer.h"
    "bvc_enc/wavelet/haar/haar_decomposer.cpp"
    "bvc_enc/wavelet/biorthogonal/biorthogonal_decomposer.h"
    "bvc_enc/wavelet/biorthogonal/biorthogonal_decomposer.cpp"
    "bvc_enc/wavelet/coiflets/coiflets_decomposer.h"
    "bvc_enc/wavelet/coiflets/coiflets_decomposer.cpp"
    "bvc_enc/wavelet/wavelet_decomposer.h"
    "bvc_enc/wavelet/wavelet_decomposer_factory.h"
    "bvc_enc/wavelet/wavelet_decomposer_factory.cpp"
    "bvc_enc/result.h"
    "bvc_enc/nal.h"
    "bvc_enc/config.h"
    "bvc_enc/bvc_enc.h"
    "bvc_enc/bvc_enc.cpp")

if(MSVC)
  set(cxx_base_flags /GS /W4 /WX /Za)
  set(cxx_exception_flags "-EHsc")
  set(cxx_strict_flags
    /we4254 # error: conversion, possible loss of data
    /we4296 # error: expression is always true/false
    /we4388 # error: signed/unsigned mismatch
    /we4389 # error: signed/unsigned mismatch
    /we4706 # error: assignment within conditional expression
    /wd4100 # disabled: unreferenced formal parameter
    /wd4018 # disabled: signed/unsigned mismatch
    /wd4127 # disabled: conditional expression is constant
    /wd4752 # disabled: found Intel(R) Advanced Vector Extensions; consider using /arch:AVX
  )
elseif(CMAKE_COMPILER_IS_GNUCXX)
  set(cxx_base_flags -Wall -Wshadow)
  set(cxx_exception_flags "-fexceptions")
  set(cxx_strict_flags -Werror -Wextra -Wunused -Wold-style-cast -Wlogical-op -Wpointer-arith
        -Wno-unused-parameter -Wno-missing-field-initializers)
  if(BVC_TARGET_ARCH STREQUAL "x86" AND MINGW)
    list(APPEND cxx_base_flags "-mxsave")
  endif()
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8)
    message(FATAL_ERROR "thread_local requires gcc >= 4.8")
    set(cxx_strict_flags ${cxx_strict_flags} -Wno-error=missing-braces)
  endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(cxx_base_flags -Wall -Wshadow)
  set(cxx_exception_flags "-fexceptions")
  set(cxx_strict_flags -Werror -Wextra -Wno-missing-braces -Wno-unused-const-variable -Wno-unused-parameter)
  if(BVC_TARGET_ARCH STREQUAL "x86" AND MINGW)
    list(APPEND cxx_base_flags "-mxsave")
  endif()
endif()
set(cxx_default ${cxx_base_flags} ${cxx_exception_flags})
set(cxx_strict ${cxx_strict_flags})
set(linker_flags "")

set(bvc_dec_src_extra "")
set(bvc_enc_src_extra "")

# bvc_common
add_library (bvc_common OBJECT ${BVC_COMMON_SOURCES})
target_compile_options(bvc_common PRIVATE ${cxx_default} ${cxx_strict})
target_include_directories(bvc_common PUBLIC .)

# bvc_enc
add_library(bvc_enc ${BVC_ENC_SOURCES} $<TARGET_OBJECTS:bvc_common> ${bvc_enc_src_extra})
set_target_properties(bvc_enc PROPERTIES OUTPUT_NAME "bvcenc")
target_compile_options(bvc_enc PRIVATE ${cxx_default} ${cxx_strict})
target_include_directories (bvc_enc PUBLIC .)
target_link_libraries(bvc_enc INTERFACE ${linker_flags} PUBLIC Threads::Threads)

# bvc_dec
add_library(bvc_dec ${BVC_DEC_SOURCES} $<TARGET_OBJECTS:bvc_common> ${bvc_dec_src_extra})
set_target_properties(bvc_dec PROPERTIES OUTPUT_NAME "bvcdec")
target_compile_options(bvc_dec PRIVATE ${cxx_default} ${cxx_strict})
target_include_directories (bvc_dec PUBLIC .)
target_link_libraries(bvc_dec INTERFACE ${linker_flags} PUBLIC Threads::Threads)


install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/bvc_enc/bvcenc.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/bvc_dec/bvcdec.h"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(TARGETS bvc_enc bvc_dec DESTINATION "${CMAKE_INSTALL_LIBDIR}")