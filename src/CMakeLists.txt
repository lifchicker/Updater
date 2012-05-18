
add_subdirectory(tests)

find_package (Qt4 COMPONENTS QtCore QtGui QtNetwork REQUIRED)
include (${QT_USE_FILE})

include (GenerateCppResourceFile)

#set (QT_USE_QTXML TRUE)
set (QT_USE_QTNETWORK TRUE)

set (SOURCES
  main.cpp
  updateclient.cpp
)

set (HEADERS
  updateclient.h
)

set (RESOURCE_PATH
  resources)

include_directories (${CMAKE_BINARY_DIR} ${QT_INCLUDE_DIR} ${QT_QTNETWORK_INCLUDE_DIR})

qt4_wrap_cpp (MOC_SOURCES ${HEADERS})

if (WIN32)
        set(EXE_FLAGS WIN32 resources/updater.rc)
endif()

add_executable (updater ${EXE_FLAGS} ${MOC_SOURCES} ${HEADERS} ${SOURCES})

if (UNIX)
  set (UNIX_LIBS pthread dl)
endif ()

target_link_libraries (
  updater
  ${QT_LIBRARIES}
  ${QT_QTNETWORK_LIBRARY}
  ${UNIX_LIBS}
)

install(TARGETS updater RUNTIME DESTINATION bin)

