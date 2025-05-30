cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(task_3 VERSION ${PROJECT_VESRION})

#option(WITH_BOOST_TEST "Whether to build Boost test" ON)

#configure_file(version.h.in version.h)

add_executable(task_3_cli main.cpp)
add_library(task_3 my_container.cpp)

set_target_properties(task_3_cli task_3 PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(task_3
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(task_3_cli PRIVATE
    task_3
)

# if(WITH_BOOST_TEST)
#     find_package(Boost COMPONENTS unit_test_framework REQUIRED)
#     add_executable(test_version test_version.cpp)

#     set_target_properties(test_version PROPERTIES
#         CXX_STANDARD 14
#         CXX_STANDARD_REQUIRED ON
#     )

#     set_target_properties(test_version PROPERTIES
#         COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
#         INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
#     )

#     target_link_libraries(test_version
#         ${Boost_LIBRARIES}
#         task_3
#     )
# endif()

# if (MSVC)
#     target_compile_options(task_3_cli PRIVATE
#         /W4
#     )
#     target_compile_options(task_3 PRIVATE
#         /W4
#     )

#     # if(WITH_BOOST_TEST)
#     #     target_compile_options(test_version PRIVATE
#     #         /W4
#     #     )
#     # endif()
# else ()
#     target_compile_options(task_3_cli PRIVATE
#         -Wall -Wextra -pedantic -Werror
#     )
#     target_compile_options(task_3 PRIVATE
#         -Wall -Wextra -pedantic -Werror
#     )
#     # if(WITH_BOOST_TEST)
#     #     target_compile_options(test_version PRIVATE
#     #         -Wall -Wextra -pedantic -Werror
#     #     )
#     # endif()
# endif()

install(TARGETS task_3_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

# if(WITH_BOOST_TEST)
#     enable_testing()
#     add_test(test_version test_version)
# endif()