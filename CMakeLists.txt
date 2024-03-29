project(wpn-lnotify)
cmake_minimum_required(VERSION 3.4.0)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../)
set(SO_LNOTIFY "wpn-lnotify")
set(SO_STDOUT "wpn-stdout")

file(GLOB SOURCE_STDOUT wpn-stdout.cpp)
file(GLOB HEADERS_STDOUT wpn-notify.h)

if (MSVC)
	set(NOTIFYLIBS Pathcch.lib runtimeobject.lib shlwapi.lib)	# Urlmon.lib
	file(GLOB SOURCE_LNOTIFY wpn-lnotify.cpp DesktopToast.cpp)
	file(GLOB HEADERS_LNOTIFY wpn-notify.h DesktopToast.h)
else (MSVC)
	set(NOTIFYLIBS notify)
	file(GLOB SOURCE_LNOTIFY wpn-lnotify.cpp)
	file(GLOB HEADERS_LNOTIFY wpn-notify.h)
endif (MSVC)

set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(${SO_LNOTIFY} SHARED ${SOURCE_LNOTIFY} ${HEADERS_LNOTIFY})
add_library(${SO_STDOUT} SHARED ${SOURCE_STDOUT} ${HEADERS_STDOUT})	
add_executable(wpn-lnotify-test test/main.cpp wpn-notify.h)
target_link_libraries(${SO_LNOTIFY} ${NOTIFYLIBS})
target_link_libraries(${SO_STDOUT})
target_include_directories(${SO_LNOTIFY} PUBLIC "${CMAKE_CURRENT_BINARY_DIR} ${HEADERS_LNOTIFY}")
target_include_directories(${SO_STDOUT} PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${HEADERS_STDOUT})


install(TARGETS ${PROJECT_NAME} DESTINATION bin)