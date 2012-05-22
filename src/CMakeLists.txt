
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

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

set (SOURCES_MAINAPPLICATION
  mainapplication.cpp
)

set (HEADERS_MAINAPPLICATION
  mainapplication.h
)

set (RESOURCE_PATH
  resources)

include_directories (${CMAKE_BINARY_DIR} ${QT_INCLUDE_DIR} ${QT_QTNETWORK_INCLUDE_DIR})

qt4_wrap_cpp (MOC_SOURCES ${HEADERS_UPDATECLIENT} ${HEADERS_MAINAPPLICATION})

if (WIN32)
        set(EXE_FLAGS WIN32 resources/updater.rc)
endif()

add_library (updateclient ${MOC_SOURCES} ${HEADERS_UPDATECLIENT} ${HEADERS_MAINAPPLICATION} ${SOURCES_UPDATECLIENT} ${SOURCES_MAINAPPLICATION})

if (UNIX)
  set (UNIX_LIBS pthread dl)
endif ()

target_link_libraries (
  updateclient
  ${QT_LIBRARIES}
  ${QT_QTNETWORK_LIBRARY}
  ${UNIX_LIBS}
)

add_executable (updater ${EXE_FLAGS} ${HEADERS_UPDATECLIENT} ${HEADERS_MAINAPPLICATION} main.cpp)

target_link_libraries (
  updater
  updateclient
)

install(TARGETS updater RUNTIME DESTINATION bin)

