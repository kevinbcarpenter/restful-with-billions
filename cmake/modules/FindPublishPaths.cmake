option(BinPath "Assign binary output paths" OFF)
if (BinPath)
set(BinPath get_filename_component(DIRECTORY ${BinPath} ABSOLUTE))
else (BinPath)
set(BinPath "${CMAKE_SOURCE_DIR}/bin")
endif (BinPath)

message(STATUS "Binary output path has been set to ${BinPath}")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BinPath}/${TARGET_COMPILER_NAME}/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BinPath}/${TARGET_COMPILER_NAME}/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BinPath}/${TARGET_COMPILER_NAME}/${CMAKE_BUILD_TYPE}/lib)
