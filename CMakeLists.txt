cmake_minimum_required(VERSION 3.12)
project(libnetq VERSION 1.0.2
  DESCRIPTION "The cross-platform library"
  HOMEPAGE_URL "https://github.com/libnetq/libnetq"
  LANGUAGES "C"
  )

message(STATUS "Project ${PROJECT_NAME}/${PROJECT_VERSION} (${PROJECT_DESCRIPTION})")
message(STATUS "URL: ${PROJECT_HOMEPAGE_URL}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GNUInstallDirs)
include(CheckIncludeFile)
include(CheckFunctionExists)

macro (LNQ_OPTION_DEFINE _name _description _value)
  set(LNQ_${_name} "${_value}" CACHE STRING "${_description}")
  if (NOT "${ARGN}" STREQUAL "")
    set_property(CACHE LNQ_${_name} PROPERTY STRINGS "${_value};${ARGN}")
  endif ()
endmacro ()

macro (LNQ_FEATURE_DEFINE _name _description)
  LNQ_OPTION_DEFINE(${_name} "${_description}" "Default" "Enable" "Disable")
endmacro ()

LNQ_OPTION_DEFINE(LIBRARY_TYPE "Choose the library type: SHARED, STATIC, OBJECT" SHARED STATIC OBJECT)
LNQ_FEATURE_DEFINE(USE_EXPORT "Enable symbol export as a shared library")

option(LNQ_PUBLIC_GENERATED_DIR "Allow config.h to be populate in include directories" OFF)
set(LNQ_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")

if (LNQ_USE_EXPORT STREQUAL "Enable")
  set(NQ_USE_EXPORT ON)
endif ()

math(EXPR NQ_VERSION_CODE "${PROJECT_VERSION_MAJOR} * 10000000 + ${PROJECT_VERSION_MINOR} * 100000 + ${PROJECT_VERSION_PATCH} * 1000")

if ((MINGW OR UNIX) AND NOT APPLE)
  set(_POSIX_SOURCE   1)
  set(_POSIX_1_SOURCE 1)
  set(_POSIX_C_SOURCE 200112L)
  list(APPEND CMAKE_REQUIRED_DEFINITIONS
    -D_POSIX_SOURCE=${_POSIX_SOURCE}
    -D_POSIX_1_SOURCE=${_POSIX_1_SOURCE}
    -D_POSIX_C_SOURCE=${_POSIX_C_SOURCE}
    )
endif ()

find_package(Threads)
if (Threads_FOUND AND CMAKE_USE_PTHREADS_INIT AND NOT APPLE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
endif ()

set(WITH_ZLIB OFF)
find_package(ZLIB)
if (ZLIB_FOUND)
  LNQ_FEATURE_DEFINE(WITH_ZLIB "Allow compilation with ZLIB library")
  set(HAVE_ZLIB ON)
  if (LNQ_WITH_ZLIB STREQUAL "Enable")
    set(WITH_ZLIB ON)
  endif ()
endif ()

set(WITH_OLE32 OFF)
set(WITH_FOUND FALSE)
set(CMAKE_REQUIRED_LIBRARIES_SAVE ${CMAKE_REQUIRED_LIBRARIES})
list(APPEND CMAKE_REQUIRED_LIBRARIES ole32)
check_include_file(combaseapi.h HAVE_COMBASEAPI_H)
if (HAVE_COMBASEAPI_H)
  LNQ_FEATURE_DEFINE(WITH_OLE32 "Allow compilation with OLE32 library")
  set(OLE32_FOUND TRUE)
  if (LNQ_WITH_OLE32 STREQUAL "Enable")
    set(WITH_OLE32 ON)
  endif ()
endif ()
set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES_SAVE})

if (LNQ_LIBRARY_TYPE STREQUAL "SHARED")
  if (LNQ_WITH_ZLIB STREQUAL "Default")
    set(WITH_ZLIB ON)
  endif ()
  if (LNQ_WITH_OLE32 STREQUAL "Default")
    set(WITH_OLE32 ON)
  endif ()
endif ()

if (WITH_ZLIB)
  list(APPEND CMAKE_REQUIRED_INCLUDES "${ZLIB_INCLUDE_DIR}")
  list(APPEND CMAKE_REQUIRED_LIBRARIES "${ZLIB_LIBRARY}")
endif ()

check_include_file(stddef.h HAVE_STDDEF_H)
check_include_file(stdio.h HAVE_STDIO_H)
check_include_file(stdint.h HAVE_STDINT_H)
check_include_file(stdlib.h HAVE_STDLIB_H)
check_include_file(string.h HAVE_STRING_H)
check_include_file(strings.h HAVE_STRINGS_H)
check_include_file(memory.h HAVE_MEMORY_H)
check_include_file(ifaddrs.h HAVE_IFADDRS_H)
check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(time.h HAVE_TIME_H)
check_include_file(unistd.h HAVE_UNISTD_H)
check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(sys/stat.h HAVE_SYS_STAT_H)

check_function_exists(pthread_yield HAVE_PTHREAD_YIELD)
check_function_exists(pthread_setname_np HAVE_PTHREAD_SETNAME_NP)
check_function_exists(pthread_getschedparam HAVE_PTHREAD_GETSCHEDPARAM)
check_function_exists(pthread_setschedparam HAVE_PTHREAD_SETSCHEDPARAM)
check_function_exists(pthread_condattr_setclock HAVE_PTHREAD_CONDATTR_SETCLOCK)
check_function_exists(pthread_attr_set_qos_class_np HAVE_PTHREAD_ATTR_SET_QOS_CLASS_NP)
check_function_exists(AAsset_getLength64 HAVE_AASSET_GETLENGTH64)
check_function_exists(getlogin_r HAVE_GETLOGIN_R)

check_c_source_compiles("
#define __EXTENSIONS__ 1

#if ${HAVE_STDIO_H}0
# include <stdio.h>
#endif

#if ${HAVE_SYS_TYPES_H}0
# include <sys/types.h>
#endif

#if ${HAVE_SYS_STAT_H}0
# include <sys/stat.h>
#endif

#if ${HAVE_STDLIB_H}0
# include <stdlib.h>
#endif

#if ${HAVE_STDDEF_H}0
# include <stddef.h>
#endif

#if ${HAVE_MEMORY_H}0
# include <memory.h>
#endif

#if ${HAVE_STRING_H}0
# include <string.h>
#endif

#if ${HAVE_STRINGS_H}0
# include <strings.h>
#endif

#if ${HAVE_INTTYPES_H}0
# include <inttypes.h>
#endif

#if ${HAVE_STDINT_H}0
# include <stdint.h>
#endif

#if ${HAVE_UNISTD_H}0
# include <unistd.h>
#endif

int main ()
{
  return 0;
}
" HAVE_EXTENSIONS)
if (HAVE_EXTENSIONS)
  set(__EXTENSIONS__ 1)
  set(_ALL_SOURCE 1)
  set(_GNU_SOURCE 1)
  set(_POSIX_PTHREAD_SEMANTICS 1)
  set(_TANDEM_SOURCE 1)
endif ()

check_c_source_compiles("
#if ${HAVE_SYS_TYPES_H}0
# include <sys/types.h>
#endif

#if ${HAVE_TIME_H}0
# include <time.h>
#endif

int main ()
{
  struct tm tm;
  time_t t;
  localtime_r(&t, &tm);
  return tm.tm_yday;
}" HAVE_LOCALTIME_R)

check_c_source_compiles("
#if ${HAVE_SYS_TYPES_H}0
# include <sys/types.h>
#endif

#if ${HAVE_TIME_H}0
# include <time.h>
#endif

int main ()
{
  struct tm tm;
  time_t t;

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
  localtime_s(&tm, &t);
#else
  localtime_s(&t, &tm);
#endif

  return tm.tm_yday;
}
" HAVE_LOCALTIME_S)

check_c_source_compiles("
#if ${HAVE_SYS_TYPES_H}0
# include <sys/types.h>
#endif

#if ${HAVE_TIME_H}0
# include <time.h>
#endif

int main ()
{
  struct tm tm;
  tm.tm_gmtoff;
  return 0;
}
" HAVE_TM_GMTOFF)

check_c_source_compiles("
void memory_barrier (void) { __sync_synchronize (); }
int atomic_add (int* i) { return __sync_fetch_and_add (i, 1); }
int mutex_trylock (int* m) { return __sync_lock_test_and_set (m, 1); }
void mutex_unlock (int* m) { __sync_lock_release (m); }
int main() { return 0; }
" HAVE_INTEL_ATOMIC_PRIMITIVES)

check_c_source_compiles("
#define _GNU_SOURCE 1
#include <pthread.h>
int main () { return pthread_setname_np(\"TestThreadName\"); }
" HAVE_PTHREAD_SETNAME_NP_1ARG)

check_c_source_compiles("
#define _GNU_SOURCE 1
#include <pthread.h>
int main () { return pthread_setname_np(pthread_self(), \"TestThreadName\"); }
" HAVE_PTHREAD_SETNAME_NP_2ARG)

if (HAVE_PTHREAD_SETNAME_NP AND NOT (HAVE_PTHREAD_SETNAME_NP_1ARG OR HAVE_PTHREAD_SETNAME_NP_2ARG))
  message(FATAL_ERROR "Unknown arguments in function pthread_setname_np")
endif ()

set(DEFINE_MAP_INLINE "/* #undef inline */")
set(DEFINE_MAP___INLINE__ "#define inline __inline__")
set(DEFINE_MAP___INLINE "#define inline __inline")
foreach (_iter INLINE __INLINE__ __INLINE)
  set(DEFINE_INLINE "${DEFINE_MAP_${_iter}}")
  check_c_source_compiles("
  ${DEFINE_INLINE}
  typedef int foo_t;
  static inline foo_t static_foo() { return 0; }
  inline foo_t foo() { return 0; }
  int main() { return static_foo(); }
  " HAVE_${_iter})
  if (HAVE_${_iter})
    break ()
  endif ()
  unset(DEFINE_INLINE)
endforeach ()
if (NOT DEFINE_INLINE)
  message(FATAL_ERROR "An inline function does not exist.")
endif ()

set(libnetq_HEADERS
  libnetq/cpu/alpha.h
  libnetq/cpu/amd64.h
  libnetq/cpu/arm.h
  libnetq/cpu/arm64.h
  libnetq/cpu/blackfin.h
  libnetq/cpu/convex.h
  libnetq/cpu/epiphany.h
  libnetq/cpu/hppa.h
  libnetq/cpu/ia64.h
  libnetq/cpu/m68k.h
  libnetq/cpu/mips.h
  libnetq/cpu/ppc.h
  libnetq/cpu/pyramid.h
  libnetq/cpu/riscv.h
  libnetq/cpu/rs6k.h
  libnetq/cpu/s370.h
  libnetq/cpu/s390.h
  libnetq/cpu/sh.h
  libnetq/cpu/sparc.h
  libnetq/cpu/tms320.h
  libnetq/cpu/tms470.h
  libnetq/cpu/x86.h
  libnetq/cpu/z.h

  libnetq/web/HttpStatistics.h

  libnetq/tls/CipherSuite.h

  libnetq/Abort.h
  libnetq/Allocator.h
  libnetq/Application.h
  libnetq/ApplicationAndroid.h
  libnetq/ApplicationCallbacks.h
  libnetq/AppMain.h
  libnetq/Arguments.h
  libnetq/Array.h
  libnetq/Assert.h
  libnetq/Asset.h
  libnetq/AssetBinary.h
  libnetq/AssetSystem.h
  libnetq/Atomic.h
  libnetq/AutoReleaseCF.h
  libnetq/Base16.h
  libnetq/Base64.h
  libnetq/Basic.h
  libnetq/BufferBuilder.h
  libnetq/BuildInfo.h
  libnetq/ByteBuffer.h
  libnetq/ByteOrder.h
  libnetq/CoherentLooper.h
  libnetq/ColorSpace.h
  libnetq/Compiler.h
  libnetq/Cond.h
  libnetq/ConstExpr.h
  libnetq/CPU.h
  libnetq/CStrBase.h
  libnetq/CStrConverter.h
  libnetq/CType.h
  libnetq/DescSet.h
  libnetq/Dir.h
  libnetq/DispatchQueue.h
  libnetq/Endian.h
  libnetq/Env.h
  libnetq/ErrorCode.h
  libnetq/Event.h
  libnetq/EventTarget.h
  libnetq/EventWakeup.h
  libnetq/Executable.h
  libnetq/Export.h
  libnetq/FileHandle.h
  libnetq/FileSystem.h
  libnetq/GetComputerName.h
  libnetq/GetSineWave.h
  libnetq/GetUserName.h
  libnetq/Hash.h
  libnetq/HexViewer.h
  libnetq/HttpBuilder.h
  libnetq/HttpHeader.h
  libnetq/HttpMethod.h
  libnetq/HttpMultiPartBuilder.h
  libnetq/HttpMultiPartParser.h
  libnetq/HttpVersion.h
  libnetq/Image.h
  libnetq/ImageBuilder.h
  libnetq/JSONWriter.h
  libnetq/KeyboardEvent.h
  libnetq/KeyVal.h
  libnetq/Library.h
  libnetq/Limits.h
  libnetq/Leb128.h
  libnetq/Log.h
  libnetq/LogCircular.h
  libnetq/Looper.h
  libnetq/LooperSource.h
  libnetq/LooperTypes.h
  libnetq/Malloc.h
  libnetq/Math.h
  libnetq/MediaType.h
  libnetq/MotionEvent.h
  libnetq/MouseEvent.h
  libnetq/Mutex.h
  libnetq/Network.h
  libnetq/Object.h
  libnetq/ObjectClass.h
  libnetq/ObjectType.h
  libnetq/Once.h
  libnetq/OS.h
  libnetq/Path.h
  libnetq/PlatformLooper.h
  libnetq/Poll.h
  libnetq/PrimitiveStorage.h
  libnetq/PrimitiveType.h
  libnetq/Rect.h
  libnetq/RWLock.h
  libnetq/Signal.h
  libnetq/Sleep.h
  libnetq/Socket.h
  libnetq/SocketHandle.h
  libnetq/Sprintf.h
  libnetq/StreamCallbacks.h
  libnetq/String.h
  libnetq/StringList.h
  libnetq/StringPrint.h
  libnetq/Strtox.h
  libnetq/Tarball.h
  libnetq/Thread.h
  libnetq/ThreadData.h
  libnetq/Time.h
  libnetq/TimerQueue.h
  libnetq/TimeVal.h
  libnetq/Types.h
  libnetq/UnlimitedRandom.h
  libnetq/URL.h
  libnetq/UTF.h
  libnetq/UUID.h
  libnetq/VA.h
  libnetq/Version.h
  libnetq/WakeOnLan.h
  libnetq/WebSocketTypes.h
  libnetq/Window.h
  libnetq/WindowEvent.h
  libnetq/WindowManager.h
  libnetq/Zip.h
  )

set(libnetq_SOURCES
  libnetq/web/HttpStatistics.c

  libnetq/Abort.c
  libnetq/Application.c
  libnetq/ApplicationAndroid.c
  libnetq/Arguments.c
  libnetq/Array.c
  libnetq/AssetBinary.c
  libnetq/AssetSystem.c
  libnetq/Atomic.c
  libnetq/Base16.c
  libnetq/Base64.c
  libnetq/BufferBuilder.c
  libnetq/BuildInfo.c
  libnetq/ByteBuffer.c
  libnetq/CoherentLooper.c
  libnetq/CondPosix.c
  libnetq/CondSysLinux.c
  libnetq/CondWin.c
  libnetq/CPU.c
  libnetq/CStrBase.c
  libnetq/CStrConverter.c
  libnetq/CType.c
  libnetq/DescSet.c
  libnetq/Dir.c
  libnetq/DispatchQueue.c
  libnetq/Endian.c
  libnetq/Env.c
  libnetq/EventTarget.c
  libnetq/EventWakeup.c
  libnetq/Executable.c
  libnetq/FileHandle.c
  libnetq/FileHandlePosix.c
  libnetq/FileHandleSysLinux.c
  libnetq/FileHandleWin.c
  libnetq/FileSystem.c
  libnetq/GetComputerName.c
  libnetq/GetSineWave.c
  libnetq/GetUserName.c
  libnetq/Hash.c
  libnetq/HexViewer.c
  libnetq/HttpBuilder.c
  libnetq/HttpHeader.c
  libnetq/HttpMethod.c
  libnetq/HttpMultiPartBuilder.c
  libnetq/HttpMultiPartParser.c
  libnetq/Image.c
  libnetq/ImageBuilder.c
  libnetq/JSONWriter.c
  libnetq/KeyVal.c
  libnetq/Leb128.c
  libnetq/Library.c
  libnetq/Log.c
  libnetq/LogCircular.c
  libnetq/Looper.c
  libnetq/LooperAndroid.c
  libnetq/LooperCF.c
  libnetq/LooperMain.c
  libnetq/LooperSelect.c
  libnetq/LooperSource.c
  libnetq/Malloc.c
  libnetq/Math.c
  libnetq/MutexPosix.c
  libnetq/MutexSysLinux.c
  libnetq/MutexWin.c
  libnetq/Network.c
  libnetq/Object.c
  libnetq/OncePosix.c
  libnetq/OnceSysLinux.c
  libnetq/OnceWin.c
  libnetq/Path.c
  libnetq/PlatformLooperCF.c
  libnetq/PlatformLooperStub.c
  libnetq/PlatformLooperWin.c
  libnetq/Poll.c
  libnetq/PrimitiveStorage.c
  libnetq/PrimitiveType.c
  libnetq/Rect.c
  libnetq/RWLockPosix.c
  libnetq/RWLockSysLinux.c
  libnetq/RWLockWin.c
  libnetq/Signal.c
  libnetq/Sleep.c
  libnetq/Socket.c
  libnetq/SocketHandle.c
  libnetq/String.c
  libnetq/StringList.c
  libnetq/StringPrint.c
  libnetq/Tarball.c
  libnetq/ThreadDataPosix.c
  libnetq/ThreadDataWin.c
  libnetq/ThreadPosix.c
  libnetq/ThreadSysLinux.c
  libnetq/ThreadWin.c
  libnetq/TimerQueue.c
  libnetq/Time.c
  libnetq/TimeVal.c
  libnetq/UnlimitedRandom.c
  libnetq/URL.c
  libnetq/UTF.c
  libnetq/UUID.c
  libnetq/Version.c
  libnetq/WakeOnLan.c
  libnetq/Window.c
  libnetq/WindowManager.c
  libnetq/Zip.c
  )

set(libnetq_DATAFILES
  config.h.in
  LICENSE
  README.md
  )

set(libnetq_INCLUDES
  "${CMAKE_CURRENT_SOURCE_DIR}"
  )

set(libnetq_LIBRARIES
  ${CMAKE_DL_LIBS}
  )

set(libnetq_DEFINITIONS
  )

set(libnetq_GENERATED
  "${LNQ_GENERATED_DIR}/config.h"
  )

if (CMAKE_SYSTEM_NAME MATCHES "Android")
  list(APPEND libnetq_LIBRARIES android log)
endif ()

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
  list(APPEND libnetq_LIBRARIES "-framework Foundation")
endif ()

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  #set(_XOPEN_SOURCE 600)
  list(APPEND libnetq_LIBRARIES m)
endif ()

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
  list(APPEND libnetq_DEFINITIONS _WIN32_WINNT=0x0600)
  list(APPEND libnetq_LIBRARIES ws2_32 wsock32)
endif ()

if (Threads_FOUND)
  list(APPEND libnetq_LIBRARIES Threads::Threads)
endif ()

if (WITH_ZLIB)
  list(APPEND libnetq_LIBRARIES ZLIB::ZLIB)
endif ()

if (WITH_OLE32)
  list(APPEND libnetq_LIBRARIES ole32)
endif ()

configure_file(config.h.in "${LNQ_GENERATED_DIR}/config.h" @ONLY)

if (LNQ_PUBLIC_GENERATED_DIR)
  list(APPEND libnetq_INCLUDES "${LNQ_GENERATED_DIR}")
endif ()

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${libnetq_HEADERS} ${libnetq_SOURCES} ${libnetq_DATAFILES})
add_library(libnetq ${LNQ_LIBRARY_TYPE} ${libnetq_GENERATED} ${libnetq_HEADERS} ${libnetq_SOURCES} ${libnetq_DATAFILES})
target_include_directories(libnetq PUBLIC "$<BUILD_INTERFACE:${libnetq_INCLUDES}>")
target_include_directories(libnetq PRIVATE "${LNQ_GENERATED_DIR}")
target_compile_definitions(libnetq PUBLIC "${libnetq_DEFINITIONS}")
target_link_libraries(libnetq PUBLIC ${libnetq_LIBRARIES})
set_target_properties(libnetq PROPERTIES PREFIX "")
set_target_properties(libnetq PROPERTIES OUTPUT_NAME "${libnetq_BUILD_NAME}")
set_target_properties(libnetq PROPERTIES LINKER_LANGUAGE "C")
set_target_properties(libnetq PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(libnetq PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(LibNetQ::LibNetQ ALIAS libnetq)

foreach (_header ${libnetq_HEADERS})
  get_filename_component(_directory "${_header}" DIRECTORY)
  install(FILES "${_header}" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${_directory}")
endforeach ()

install(TARGETS libnetq
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  )

message(STATUS "Build type . . . . . . . . . . . . . . . . . ${LNQ_LIBRARY_TYPE}")
message(STATUS "Pthread found . . . . . . . . . . . . . . .  ${Threads_FOUND}")

message(STATUS "ZLib found . . . . . . . . . . . . . . . . . ${ZLIB_FOUND}")
if (DEFINED LNQ_WITH_ZLIB)
  message(STATUS "ZLib dependency . . . . . . . . . . . . . .  ${WITH_ZLIB} (${LNQ_WITH_ZLIB})")
endif ()

message(STATUS "Ole32 found . . . . . . . . . . . . . . . .  ${OLE32_FOUND}")
if (DEFINED LNQ_WITH_OLE32)
  message(STATUS "Ole32 dependency . . . . . . . . . . . . . . ${WITH_OLE32} (${LNQ_WITH_OLE32})")
endif ()
