# Make sure the compiler can find include files from our Apex library.
include_directories (${APEX_SOURCE_DIR}/src/apex ${APEX_HIP_EXTRA_INCLUDE})
include_directories(${APEX_SOURCE_DIR}/src/unit_tests/HIP)

# Make sure the linker can find the Apex library once it is built.
link_directories (${APEX_BINARY_DIR}/src/apex)
#link_directories (${APEX_BINARY_DIR}/src/apex_pthread_wrapper)

set(example_programs
    MatrixTranspose
   )

message(INFO "Using HIP libraries: ${HIP_LIBRARIES}")

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${APEX_HIP_C_FLAGS}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${APEX_HIP_CXX_FLAGS}")

foreach(example_program ${example_programs})
  set(sources ${example_program}.cpp)
  source_group("Source Files" FILES ${sources})
  add_executable("${example_program}_hip" ${sources})
  target_link_libraries ("${example_program}_hip" apex ${LIBS} hip::host)
    if (BUILD_STATIC_EXECUTABLES)
        set_target_properties("${example_program}_hip" PROPERTIES LINK_SEARCH_START_STATIC 1 LINK_SEARCH_END_STATIC 1)
    endif()
  add_dependencies ("${example_program}_hip" apex)
  add_dependencies (tests "${example_program}_hip")
  add_test ("test_${example_program}_hip" "${example_program}_hip")
endforeach()

# Make sure the linker can find the Apex library once it is built.
link_directories (${APEX_BINARY_DIR}/src/apex)


