target_compile_definitions(${PROJECT_NAME} PUBLIC CLAPACK_USED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "../Common/CControl/src/*.c"
)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    "../Common/CControl/src/*.h"
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/**/*.h")
foreach(header ${HEADERS})
    get_filename_component(dir ${header} DIRECTORY)
    list(APPEND INCLUDE_DIRS ${dir})
endforeach()
list(REMOVE_DUPLICATES INCLUDE_DIRS)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${SOURCES})

#message(PROJECT_INCLUDE_DIR="${INCLUDE_DIRS}")
#message(PROJECT_SOURCES="${SOURCES}")