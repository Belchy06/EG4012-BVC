set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

set(BVC_COMMON_SOURCES
    "bvc_common/bitstream/bitstream.h"
    "bvc_common/bitstream/bitstream.cpp"
    "bvc_common/linked_list/node.h"
    "bvc_common/linked_list/linked_list.h"
    "bvc_common/entropy/huffman/huffman_node.h"
    "bvc_common/entropy/huffman/huffman_node.cpp"
    "bvc_common/math/vector.h"
    "bvc_common/math/matrix.h"
    "bvc_common/math/matrix.cpp"
    "bvc_common/spiht/spiht.h"
    "bvc_common/spiht/spiht.cpp"
    "bvc_common/wavelet/wavelet.h"
    "bvc_common/wavelet/wavelet.cpp"
    "bvc_common/wavelet/haar/haar.h"
    "bvc_common/wavelet/haar/haar.cpp"
    "bvc_common/wavelet/biorthogonal/biorthogonal.h"
    "bvc_common/wavelet/biorthogonal/biorthogonal.cpp"
    "bvc_common/wavelet/coiflets/coiflets.h"
    "bvc_common/wavelet/coiflets/coiflets.cpp")

set(BVC_DEC_SOURCES
    "bvc_dec/entropy/arithmetic/decoder.h"
    "bvc_dec/entropy/arithmetic/decoder.cpp"
    "bvc_dec/entropy/huffman/decoder.h"
    "bvc_dec/entropy/huffman/decoder.cpp"
    "bvc_dec/entropy/entropy_decoder.h"
    "bvc_dec/entropy/entropy_decoder_factory.h"
    "bvc_dec/entropy/entropy_decoder_factory.cpp"
    "bvc_dec/wavelet/haar/recomposer.h"
    "bvc_dec/wavelet/haar/recomposer.cpp"
    "bvc_dec/wavelet/biorthogonal/recomposer.h"
    "bvc_dec/wavelet/biorthogonal/recomposer.cpp"
    "bvc_dec/wavelet/coiflets/recomposer.h"
    "bvc_dec/wavelet/coiflets/recomposer.cpp"
    "bvc_dec/wavelet/wavelet_recomposer.h"
    "bvc_dec/wavelet/wavelet_recomposer_factory.h"
    "bvc_dec/wavelet/wavelet_recomposer_factory.cpp"
    "bvc_dec/partition/offset_zerotree/departitioner.h"
    "bvc_dec/partition/offset_zerotree/departitioner.cpp"
    "bvc_dec/partition/zerotree_preserving/departitioner.h"
    "bvc_dec/partition/zerotree_preserving/departitioner.cpp"
    "bvc_dec/partition/departitioner_factory.h"
    "bvc_dec/partition/departitioner_factory.cpp"
    "bvc_dec/bvc_dec.h"
    "bvc_dec/bvc_dec.cpp")

set(BVC_ENC_SOURCES
    "bvc_enc/entropy/arithmetic/encoder.h"
    "bvc_enc/entropy/arithmetic/encoder.cpp"
    "bvc_enc/entropy/huffman/encoder.h"
    "bvc_enc/entropy/huffman/encoder.cpp"
    "bvc_enc/entropy/entropy_encoder.h"
    "bvc_enc/entropy/entropy_encoder_factory.h"
    "bvc_enc/entropy/entropy_encoder_factory.cpp"
    "bvc_enc/wavelet/haar/decomposer.h"
    "bvc_enc/wavelet/haar/decomposer.cpp"
    "bvc_enc/wavelet/biorthogonal/decomposer.h"
    "bvc_enc/wavelet/biorthogonal/decomposer.cpp"
    "bvc_enc/wavelet/coiflets/decomposer.h"
    "bvc_enc/wavelet/coiflets/decomposer.cpp"
    "bvc_enc/wavelet/wavelet_decomposer.h"
    "bvc_enc/wavelet/wavelet_decomposer_factory.h"
    "bvc_enc/wavelet/wavelet_decomposer_factory.cpp"
    "bvc_enc/partition/offset_zerotree/partitioner.h"
    "bvc_enc/partition/offset_zerotree/partitioner.cpp"
    "bvc_enc/partition/zerotree_preserving/partitioner.h"
    "bvc_enc/partition/zerotree_preserving/partitioner.cpp"
    "bvc_enc/partition/partitioner_factory.h"
    "bvc_enc/partition/partitioner_factory.cpp"
    "bvc_enc/spiht/encoder.h"
    "bvc_enc/spiht/encoder.cpp"
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