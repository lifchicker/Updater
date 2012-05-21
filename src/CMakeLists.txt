
add_subdirectory(tests)

find_package (Qt4 COMPONENTS QtCore QtGui QtNetwork REQUIRED)
include (${QT_USE_FILE})

include (GenerateCppResourceFile)

set (QT_USE_QTNETWORK TRUE)

set (SOURCES_UPDATECLIENT
  updateclient.cpp
)

set (HEADERS_UPDATECLIENT
  updateclient.h
)

set (RESOURCE_PATH
  resources)

include_directories (${CMAKE_BINARY_DIR} ${QT_INCLUDE_DIR} ${QT_QTNETWORK_INCLUDE_DIR})

qt4_wrap_cpp (MOC_SOURCES ${HEADERS_UPDATECLIENT})

if (WIN32)
        set(EXE_FLAGS WIN32 resources/updater.rc)
endif()

add_library (updateclient ${MOC_SOURCES} ${HEADERS_UPDATECLIENT} ${SOURCES_UPDATECLIENT})

if (UNIX)
  set (UNIX_LIBS pthread dl)
endif ()

target_link_libraries (
  updateclient
  ${QT_LIBRARIES}
  ${QT_QTNETWORK_LIBRARY}
  ${UNIX_LIBS}
)

add_executable (updater ${EXE_FLAGS} ${HEADERS_UPDATECLIENT} main.cpp)

target_link_libraries (
  updater
  updateclient
)

install(TARGETS updater RUNTIME DESTINATION bin)

