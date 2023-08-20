set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

set(OVC_COMMON_SOURCES
    "ovc_common/bitstream/bitstream.h"
    "ovc_common/bitstream/bitstream.cpp"
    "ovc_common/linked_list/node.h"
    "ovc_common/linked_list/linked_list.h"
    "ovc_common/entropy/huffman/huffman_node.h"
    "ovc_common/entropy/huffman/huffman_node.cpp"
    "ovc_common/math/vector.h"
    "ovc_common/math/matrix.h"
    "ovc_common/math/matrix.cpp"
    "ovc_common/spiht/spiht.h"
    "ovc_common/spiht/spiht.cpp"
    "ovc_common/wavelet/wavelet.h"
    "ovc_common/wavelet/wavelet.cpp"
    "ovc_common/wavelet/haar/haar.h"
    "ovc_common/wavelet/haar/haar.cpp"
    "ovc_common/wavelet/biorthogonal/biorthogonal.h"
    "ovc_common/wavelet/biorthogonal/biorthogonal.cpp"
    "ovc_common/wavelet/coiflets/coiflets.h"
    "ovc_common/wavelet/coiflets/coiflets.cpp"
    "ovc_common/wavelet/symlets/symlets.h"
    "ovc_common/wavelet/symlets/symlets.cpp"
    "ovc_common/wavelet/daubechies/daubechies.h"
    "ovc_common/wavelet/daubechies/daubechies.cpp"
    "ovc_common/wavelet/reverse_biorthogonal/reverse_biorthogonal.h"
    "ovc_common/wavelet/reverse_biorthogonal/reverse_biorthogonal.cpp")

set(OVC_DEC_SOURCES
    "ovc_dec/entropy/arithmetic/decoder.h"
    "ovc_dec/entropy/arithmetic/decoder.cpp"
    "ovc_dec/entropy/huffman/decoder.h"
    "ovc_dec/entropy/huffman/decoder.cpp"
    "ovc_dec/entropy/skip/decoder.h"
    "ovc_dec/entropy/skip/decoder.cpp"
    "ovc_dec/entropy/decoder.h"
    "ovc_dec/entropy/decoder_factory.h"
    "ovc_dec/entropy/decoder_factory.cpp"
    "ovc_dec/wavelet/biorthogonal/recomposer.h"
    "ovc_dec/wavelet/biorthogonal/recomposer.cpp"
    "ovc_dec/wavelet/coiflets/recomposer.h"
    "ovc_dec/wavelet/coiflets/recomposer.cpp"
    "ovc_dec/wavelet/daubechies/recomposer.h"
    "ovc_dec/wavelet/daubechies/recomposer.cpp"
    "ovc_dec/wavelet/haar/recomposer.h"
    "ovc_dec/wavelet/haar/recomposer.cpp"
    "ovc_dec/wavelet/reverse_biorthogonal/recomposer.h"
    "ovc_dec/wavelet/reverse_biorthogonal/recomposer.cpp"
    "ovc_dec/wavelet/skip/recomposer.h"
    "ovc_dec/wavelet/skip/recomposer.cpp"
    "ovc_dec/wavelet/symlets/recomposer.h"
    "ovc_dec/wavelet/symlets/recomposer.cpp"
    "ovc_dec/wavelet/recomposer.h"
    "ovc_dec/wavelet/recomposer_factory.h"
    "ovc_dec/wavelet/recomposer_factory.cpp"
    "ovc_dec/partition/offset_zerotree/departitioner.h"
    "ovc_dec/partition/offset_zerotree/departitioner.cpp"
    "ovc_dec/partition/zerotree_preserving/departitioner.h"
    "ovc_dec/partition/zerotree_preserving/departitioner.cpp"
    "ovc_dec/partition/skip/departitioner.h"
    "ovc_dec/partition/skip/departitioner.cpp"
    "ovc_dec/partition/departitioner_factory.h"
    "ovc_dec/partition/departitioner_factory.cpp"
    "ovc_dec/spiht/spiht/decoder.h"
    "ovc_dec/spiht/spiht/decoder.cpp"
    "ovc_dec/spiht/skip/decoder.h"
    "ovc_dec/spiht/skip/decoder.cpp"
    "ovc_dec/spiht/decoder_factory.h"
    "ovc_dec/spiht/decoder_factory.cpp"
    "ovc_dec/ovc_dec.h"
    "ovc_dec/ovc_dec.cpp")

set(OVC_ENC_SOURCES
    "ovc_enc/entropy/arithmetic/encoder.h"
    "ovc_enc/entropy/arithmetic/encoder.cpp"
    "ovc_enc/entropy/huffman/encoder.h"
    "ovc_enc/entropy/huffman/encoder.cpp"
    "ovc_enc/entropy/skip/encoder.h"
    "ovc_enc/entropy/skip/encoder.cpp"
    "ovc_enc/entropy/encoder.h"
    "ovc_enc/entropy/encoder_factory.h"
    "ovc_enc/entropy/encoder_factory.cpp"
    "ovc_enc/wavelet/biorthogonal/decomposer.h"
    "ovc_enc/wavelet/biorthogonal/decomposer.cpp"
    "ovc_enc/wavelet/coiflets/decomposer.h"
    "ovc_enc/wavelet/coiflets/decomposer.cpp"
    "ovc_enc/wavelet/daubechies/decomposer.h"
    "ovc_enc/wavelet/daubechies/decomposer.cpp"
    "ovc_enc/wavelet/haar/decomposer.h"
    "ovc_enc/wavelet/haar/decomposer.cpp"
    "ovc_enc/wavelet/reverse_biorthogonal/decomposer.h"
    "ovc_enc/wavelet/reverse_biorthogonal/decomposer.cpp"
    "ovc_enc/wavelet/skip/decomposer.h"
    "ovc_enc/wavelet/skip/decomposer.cpp"
    "ovc_enc/wavelet/symlets/decomposer.h"
    "ovc_enc/wavelet/symlets/decomposer.cpp"
    "ovc_enc/wavelet/decomposer.h"
    "ovc_enc/wavelet/decomposer_factory.h"
    "ovc_enc/wavelet/decomposer_factory.cpp"
    "ovc_enc/partition/offset_zerotree/partitioner.h"
    "ovc_enc/partition/offset_zerotree/partitioner.cpp"
    "ovc_enc/partition/zerotree_preserving/partitioner.h"
    "ovc_enc/partition/zerotree_preserving/partitioner.cpp"
    "ovc_enc/partition/skip/partitioner.h"
    "ovc_enc/partition/skip/partitioner.cpp"
    "ovc_enc/partition/partitioner_factory.h"
    "ovc_enc/partition/partitioner_factory.cpp"
    "ovc_enc/spiht/spiht/encoder.h"
    "ovc_enc/spiht/spiht/encoder.cpp"
    "ovc_enc/spiht/skip/encoder.h"
    "ovc_enc/spiht/skip/encoder.cpp"
    "ovc_enc/spiht/encoder_factory.h"
    "ovc_enc/spiht/encoder_factory.cpp"
    "ovc_enc/ovc_enc.h"
    "ovc_enc/ovc_enc.cpp")

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
  if(OVC_TARGET_ARCH STREQUAL "x86" AND MINGW)
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
  if(OVC_TARGET_ARCH STREQUAL "x86" AND MINGW)
    list(APPEND cxx_base_flags "-mxsave")
  endif()
endif()
set(cxx_default ${cxx_base_flags} ${cxx_exception_flags})
set(cxx_strict ${cxx_strict_flags})
set(linker_flags "")

set(ovc_dec_src_extra "")
set(ovc_enc_src_extra "")

# ovc_common
add_library (ovc_common OBJECT ${OVC_COMMON_SOURCES})
target_compile_options(ovc_common PRIVATE ${cxx_default} ${cxx_strict})
target_include_directories(ovc_common PUBLIC .)

# ovc_enc
add_library(ovc_enc ${OVC_ENC_SOURCES} $<TARGET_OBJECTS:ovc_common> ${ovc_enc_src_extra})
set_target_properties(ovc_enc PROPERTIES OUTPUT_NAME "ovcenc")
target_compile_options(ovc_enc PRIVATE ${cxx_default} ${cxx_strict})
target_include_directories (ovc_enc PUBLIC .)
target_link_libraries(ovc_enc INTERFACE ${linker_flags} PUBLIC Threads::Threads)

# ovc_dec
add_library(ovc_dec ${OVC_DEC_SOURCES} $<TARGET_OBJECTS:ovc_common> ${ovc_dec_src_extra})
set_target_properties(ovc_dec PROPERTIES OUTPUT_NAME "ovcdec")
target_compile_options(ovc_dec PRIVATE ${cxx_default} ${cxx_strict})
target_include_directories (ovc_dec PUBLIC .)
target_link_libraries(ovc_dec INTERFACE ${linker_flags} PUBLIC Threads::Threads)


install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/ovc_enc/ovcenc.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/ovc_dec/ovcdec.h"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(TARGETS ovc_enc ovc_dec DESTINATION "${CMAKE_INSTALL_LIBDIR}")