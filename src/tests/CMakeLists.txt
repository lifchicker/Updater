project (updater-tests)

cmake_minimum_required(VERSION 2.8)

include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/..")

find_package (Qt4 COMPONENTS QtCore QtGui QtNetwork REQUIRED)
include (${QT_USE_FILE})

# Install data files required by unit tests
set(TEST_FILES
  test.py
)

foreach(TEST_FILE ${TEST_FILES})
  execute_process(
    COMMAND
	"${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_FILE}" "${CMAKE_CURRENT_BINARY_DIR}"
  )
endforeach()

# Add unit test binaries
add_executable(TestCopyFile
  TestCopyFile.cpp
)

target_link_libraries(TestCopyFile
  updateclient
)

add_executable(TestCopyFiles
  TestCopyFiles.cpp
)

target_link_libraries(TestCopyFiles
  updateclient
)

add_executable(TestMoveFile
  TestMoveFile.cpp
)

target_link_libraries(TestMoveFile
  updateclient
)
