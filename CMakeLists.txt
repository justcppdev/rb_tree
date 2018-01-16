cmake_minimum_required(VERSION 3.3)

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.19.123.tar.gz"
    SHA1 "57d07480686f82ddc916a5980b4f2a18e5954c2b"
)

project(rb_tree)
set(RB_TREE_VERSION_MAJOR 0)
set(RB_TREE_VERSION_MINOR 1)
set(RB_TREE_VERSION_PATCH 0)
set(RB_TREE_VERSION_TWEAK 0)
set(RB_TREE_VERSION ${RB_TREE_VERSION_MAJOR}.${RB_TREE_VERSION_MINOR}.${RB_TREE_VERSION_PATCH}.${RB_TREE_VERSION_TWEAK})
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" OFF)
option(BUILD_EXAMPLES "Build Examples" OFF)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

if(BUILD_TESTS)
	hunter_add_package(Catch)
	find_package(Catch CONFIG REQUIRED)

	enable_testing()

	file(GLOB ${PROJECT_NAME}_TEST_SOURCES tests/*.cpp)
	add_executable(tests ${${PROJECT_NAME}_TEST_SOURCES})
	target_link_libraries(tests ${PROJECT_NAME} Catch::Catch )

	add_test(NAME test_name COMMAND tests "-s" "-r" "compact" "--use-colour" "yes") 
endif()

if(BUILD_EXAMPLES)
	file(GLOB EXAMPLE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/examples/*.cpp")
	foreach(EXAMPLE_SOURCE ${EXAMPLE_SOURCES})
		get_filename_component(EXAMPLE_NAME ${EXAMPLE_SOURCE} NAME_WE)
		set(EXAMPLE_TARGET_NAME example_${EXAMPLE_NAME})
		add_executable(${EXAMPLE_TARGET_NAME} ${EXAMPLE_SOURCE})
		target_link_libraries(${EXAMPLE_TARGET_NAME} ${PROJECT_NAME})
		set_target_properties(${EXAMPLE_TARGET_NAME} PROPERTIES OUTPUT_NAME ${EXAMPLE_NAME})
		install(TARGETS ${EXAMPLE_TARGET_NAME}
			RUNTIME DESTINATION bin
		)
	endforeach(EXAMPLE_SOURCE ${EXAMPLE_SOURCES})
endif()

install(TARGETS rb_tree 
	EXPORT rb_tree-config
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)
install(EXPORT rb_tree-config DESTINATION cmake)

include(CPackConfig.cmake)
