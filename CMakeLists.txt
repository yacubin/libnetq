cmake_minimum_required(VERSION 3.12)
project(libnetq VERSION 1.0.11
  DESCRIPTION "The cross-platform library"
  HOMEPAGE_URL "https://github.com/libnetq/libnetq"
  LANGUAGES "C"
  )

message(STATUS "Project ${PROJECT_NAME}/${PROJECT_VERSION} (${PROJECT_DESCRIPTION})")
message(STATUS "URL: ${PROJECT_HOMEPAGE_URL}")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

include(GNUInstallDirs)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckFunctionExists)

macro (LNQ_OPTION_DEFINE _name _description _type _value)
  set(LNQ_${_name} "${_value}" CACHE STRING "${_description}")
  if ("${_type}" STREQUAL "ENUM")
    if (NOT "${ARGN}" STREQUAL "")
      set_property(CACHE LNQ_${_name} PROPERTY STRINGS "${_value};${ARGN}")
    endif ()
  endif ()
endmacro ()

macro (LNQ_FEATURE_DEFINE _name _description)
  LNQ_OPTION_DEFINE(${_name} "${_description}" ENUM "AUTO" "ON" "OFF")
  if (NOT LNQ_${_name} MATCHES "AUTO|ON|OFF|FALSE|TRUE|0|1")
    message(FATAL_ERROR "Wrong '${LNQ_${_name}}' value for LNQ_${_name}")
  endif ()
endmacro ()

macro (LNQ_ADD_LIBRARY _name)
  string(TOUPPER ${_name} _library_upper_name)
  set(HAVE_${_library_upper_name} FALSE)
  set(WITH_${_library_upper_name} OFF)
  find_package(${_name})
  if (${_name}_FOUND)
    set(HAVE_${_library_upper_name} TRUE)
  endif ()
  if (LNQ_WITH_${_library_upper_name} OR HAVE_${_library_upper_name})
    LNQ_FEATURE_DEFINE(WITH_${_library_upper_name} "Allow compilation with ${_name} library")
    if (NOT LNQ_WITH_${_library_upper_name} STREQUAL "AUTO")
      set(WITH_${_library_upper_name} ${LNQ_WITH_${_library_upper_name}})
    elseif (LNQ_LIBRARY_TYPE STREQUAL "SHARED")
      set(WITH_${_library_upper_name} ON)
    endif ()
  endif ()
endmacro ()

LNQ_OPTION_DEFINE(LIBRARY_TYPE "Choose the library type: SHARED, STATIC, OBJECT" ENUM SHARED STATIC OBJECT)
LNQ_OPTION_DEFINE(CHECK_LEVEL "Runtime check in debug mode only" NUMBER 0)
LNQ_OPTION_DEFINE(ENV_LOGFD "File descriptor name used for logging output" STRING "")
LNQ_FEATURE_DEFINE(USE_EXPORT "Enable symbol export as a shared library")

if (NOT LNQ_USE_EXPORT STREQUAL AUTO)
  set(NQ_USE_EXPORT ${LNQ_USE_EXPORT})
elseif (LNQ_LIBRARY_TYPE STREQUAL "SHARED")
  set(NQ_USE_EXPORT ON)
else ()
  set(NQ_USE_EXPORT OFF)
endif ()

set(NQ_CHECK_LEVEL ${LNQ_CHECK_LEVEL})
set(NQ_ENV_LOGFD ${LNQ_ENV_LOGFD})

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

set(HAVE_PTHREAD FALSE)
set(WITH_PTHREAD OFF)
find_package(Threads)
if(Threads_FOUND)
  set(HAVE_PTHREAD TRUE)
endif ()
if (HAVE_PTHREAD)
  set(WITH_PTHREAD ON)
  if (CMAKE_USE_PTHREADS_INIT AND NOT APPLE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
  endif ()
endif ()

set(HAVE_OLE32 FALSE)
set(WITH_OLE32 OFF)
check_include_file(combaseapi.h HAVE_COMBASEAPI_H)
if (HAVE_COMBASEAPI_H)
  set(HAVE_OLE32 TRUE)
endif ()
if (LNQ_WITH_OLE32 OR HAVE_OLE32)
  LNQ_FEATURE_DEFINE(WITH_OLE32 "Allow compilation with Ole32 library")
  if (NOT LNQ_WITH_OLE32 STREQUAL "AUTO")
    set(WITH_OLE32 ${LNQ_WITH_OLE32})
  elseif (LNQ_LIBRARY_TYPE STREQUAL "SHARED" OR WIN32)
    set(WITH_OLE32 ON)
  endif ()
endif ()

set(HAVE_ADVAPI32 FALSE)
set(WITH_ADVAPI32 OFF)
if (WIN32)
  check_include_files("windows.h;wincrypt.h" HAVE_WINCRYPT_H)
endif ()
if (HAVE_WINCRYPT_H)
  set(HAVE_ADVAPI32 TRUE)
endif ()
if (LNQ_WITH_ADVAPI32 OR HAVE_ADVAPI32)
  LNQ_FEATURE_DEFINE(WITH_ADVAPI32 "Allow compilation with Advapi32 library")
  if (NOT LNQ_WITH_ADVAPI32 STREQUAL "AUTO")
    set(WITH_ADVAPI32 ${LNQ_WITH_ADVAPI32})
  elseif (LNQ_LIBRARY_TYPE STREQUAL "SHARED" OR WIN32)
    set(WITH_ADVAPI32 ON)
  endif ()
endif ()

LNQ_ADD_LIBRARY(ZLIB)
LNQ_ADD_LIBRARY(OpenSSL)
LNQ_ADD_LIBRARY(Jansson)
LNQ_ADD_LIBRARY(CJSON)
LNQ_ADD_LIBRARY(PCRE2)
LNQ_ADD_LIBRARY(CivetWeb)
LNQ_ADD_LIBRARY(LibEvent)
LNQ_ADD_LIBRARY(LibUV)
LNQ_ADD_LIBRARY(Expat)
LNQ_ADD_LIBRARY(AtomicOps)
LNQ_ADD_LIBRARY(LibBase58)
LNQ_ADD_LIBRARY(Secp256k1)

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
void memory_barrier(void) { __sync_synchronize(); }
int atomic_add(int* i) { return __sync_fetch_and_add(i, 1); }
int mutex_trylock(int* m) { return __sync_lock_test_and_set(m, 1); }
void mutex_unlock(int* m) { __sync_lock_release (m); }
int main() { return 0; }
" HAVE_SYNC_BUILTINS)

check_c_source_compiles ("
int atomic_add(int i) { return __atomic_fetch_add(&i, 1, __ATOMIC_SEQ_CST); }
int atomic_cmpxchg(int i, int j, int k) { return __atomic_compare_exchange_n(&i, &j, k, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST); }
int main(void) { return 0; }
" HAVE_ATOMIC_BUILTINS)

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
  libnetq/asset/BinaryAsset.h
  libnetq/asset/FileSystemAsset.h
  libnetq/asset/PlatformAsset.h

  libnetq/checksum/CRC32.h

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

  libnetq/crypto/CryptoAlgorithm.h
  libnetq/crypto/CryptoHash.h
  libnetq/crypto/CryptoHashAdvapi32.h
  libnetq/crypto/CryptoHashOpenSSL.h
  libnetq/crypto/CryptoHashStub.h
  libnetq/crypto/CryptoHashSysLinux.h
  libnetq/crypto/HMAC_SHA1.h
  libnetq/crypto/MD5.h
  libnetq/crypto/Secp256k1.h
  libnetq/crypto/SecureErase.h
  libnetq/crypto/SHA1.h
  libnetq/crypto/SHA256.h

  libnetq/fs/Stat.h

  libnetq/io/ByteBufferChannel.h
  libnetq/io/ByteBufferWriter.h
  libnetq/io/DataReader.h
  libnetq/io/FileWriter.h
  libnetq/io/IOWriter.h
  libnetq/io/NsVarWriter.h
  libnetq/io/ReadWrapper.h
  libnetq/io/StreamCallbacks.h
  libnetq/io/WriteWrapper.h

  libnetq/json/JSON.h
  libnetq/json/JSONTypes.h
  libnetq/json/JSONWriter.h

  libnetq/net/ChunkBuffer.h
  libnetq/net/NetBufferQueue.h
  libnetq/net/SpyModule.h
  libnetq/net/SpyModuleList.h
  libnetq/net/SpyServer.h

  libnetq/random/CryptoRandom.h
  libnetq/random/UnlimitedRandom.h

  libnetq/regex/Regex.h

  libnetq/string/StringStorage.h

  libnetq/web/HttpStatistics.h
  libnetq/web/JsonRpcTypes.h
  libnetq/web/UrlHost.h
  libnetq/web/WebRequest.h
  libnetq/web/WebResponse.h
  libnetq/web/WebServer.h
  libnetq/web/WebSocket.h
  libnetq/web/WebWriter.h

  libnetq/tls/CipherSuite.h

  libnetq/wasm/Opcodes.def
  libnetq/wasm/Module.h
  libnetq/wasm/ModuleTypes.h

  libnetq/xml/XMLParser.h

  libnetq/Abort.h
  libnetq/Allocator.h
  libnetq/Application.h
  libnetq/ApplicationAndroid.h
  libnetq/ApplicationCallbacks.h
  libnetq/AppMain.h
  libnetq/Arguments.h
  libnetq/Array.h
  libnetq/ASCII.def
  libnetq/Assert.h
  libnetq/Asset.h
  libnetq/Atomic.h
  libnetq/AutoReleaseCF.h
  libnetq/Base16.h
  libnetq/Base58.h
  libnetq/Base64.h
  libnetq/Basic.h
  libnetq/BitOps.h
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
  libnetq/HttpStatus.def
  libnetq/HttpStatus.h
  libnetq/HttpVersion.h
  libnetq/Image.h
  libnetq/ImageBuilder.h
  libnetq/JSON.h
  libnetq/KeyboardEvent.h
  libnetq/KeyVal.h
  libnetq/Library.h
  libnetq/Limits.h
  libnetq/List.h
  libnetq/Leb128.h
  libnetq/Log.h
  libnetq/LogCircular.h
  libnetq/Looper.h
  libnetq/LooperSource.h
  libnetq/LooperTypes.h
  libnetq/Malloc.h
  libnetq/Math.h
  libnetq/MediaType.h
  libnetq/MimeTypes.def
  libnetq/Module.h
  libnetq/MotionEvent.h
  libnetq/MouseEvent.h
  libnetq/Mutex.h
  libnetq/Network.h
  libnetq/NetworkLooper.h
  libnetq/Notifier.h
  libnetq/Once.h
  libnetq/OS.h
  libnetq/Path.h
  libnetq/PlatformLooper.h
  libnetq/PlatformPoll.h
  libnetq/PlatformSocket.h
  libnetq/Poll.h
  libnetq/PollVector.h
  libnetq/PrimitiveStorage.h
  libnetq/PrimitiveType.h
  libnetq/Random.h
  libnetq/Rect.h
  libnetq/RefCount.h
  libnetq/Regex.h
  libnetq/RWLock.h
  libnetq/Signal.h
  libnetq/Sleep.h
  libnetq/Socket.h
  libnetq/SocketHandle.h
  libnetq/Sprintf.h
  libnetq/String.h
  libnetq/StringList.h
  libnetq/StringPrint.h
  libnetq/Strtox.h
  libnetq/Tarball.h
  libnetq/Thread.h
  libnetq/ThreadData.h
  libnetq/Time.h
  libnetq/TimerQueue.h
  libnetq/Types.h
  libnetq/URL.h
  libnetq/UrlPath.h
  libnetq/UrlQuery.h
  libnetq/UTF.h
  libnetq/UUID.h
  libnetq/VA.h
  libnetq/WakeOnLan.h
  libnetq/WebSocketCalcAccept.h
  libnetq/WebSocketFrame.h
  libnetq/WebSocketTypes.h
  libnetq/Window.h
  libnetq/WindowEvent.h
  libnetq/WindowManager.h
  libnetq/Zip.h
  )

set(libnetq_SOURCES
  libnetq/asset/BinaryAsset.c
  libnetq/asset/FileSystemAsset.c
  libnetq/asset/PlatformAsset.c

  libnetq/checksum/CRC32Stub.c
  libnetq/checksum/CRC32SysLinux.c
  libnetq/checksum/CRC32ZLib.c

  libnetq/crypto/CryptoHashAdvapi32.c
  libnetq/crypto/CryptoHashOpenSSL.c
  libnetq/crypto/CryptoHashSysLinux.c
  libnetq/crypto/HMAC_SHA1.c
  libnetq/crypto/MD5OpenSSL.c
  libnetq/crypto/Secp256k1.c
  libnetq/crypto/Secp256k1Stub.c
  libnetq/crypto/SHA1OpenSSL.c
  libnetq/crypto/SHA256OpenSSL.c

  libnetq/fs/Stat.c

  libnetq/io/ByteBufferChannel.c
  libnetq/io/ByteBufferWriter.c
  libnetq/io/FileWriter.c
  libnetq/io/IOWriter.c
  libnetq/io/NsVarWriter.c
  libnetq/io/ReadWrapper.c
  libnetq/io/WriteWrapper.c

  libnetq/json/JSON.c
  libnetq/json/JSONCJSON.c
  libnetq/json/JSONJansson.c
  libnetq/json/JSONStub.c
  libnetq/json/JSONWriter.c

  libnetq/net/ChunkBuffer.c
  libnetq/net/NetBufferQueue.c
  libnetq/net/SpyModuleList.c
  libnetq/net/SpyServer.c

  libnetq/random/CryptoRandomOpenSSL.c
  libnetq/random/CryptoRandomPlatform.c
  libnetq/random/UnlimitedRandom.c

  libnetq/regex/RegexPcre2.c
  libnetq/regex/RegexStub.c

  libnetq/string/StringStorage.c

  libnetq/wasm/Module.c
  libnetq/wasm/ModuleTypes.c

  libnetq/web/HttpStatistics.c
  libnetq/web/UrlHost.c
  libnetq/web/WebResponse.c
  libnetq/web/WebServer.c

  libnetq/xml/XMLParserExpat.c
  libnetq/xml/XMLParserStub.c

  libnetq/Abort.c
  libnetq/Application.c
  libnetq/ApplicationAndroid.c
  libnetq/Arguments.c
  libnetq/Array.c
  libnetq/Asset.c
  libnetq/Atomic.c
  libnetq/Base16.c
  libnetq/Base58LibBase58.c
  libnetq/Base58Stub.c
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
  libnetq/NetworkLooper.c
  libnetq/OncePosix.c
  libnetq/OnceSysLinux.c
  libnetq/OnceWin.c
  libnetq/Path.c
  libnetq/PlatformLooperCF.c
  libnetq/PlatformLooperStub.c
  libnetq/PlatformLooperWin.c
  libnetq/PlatformPoll.c
  libnetq/Poll.c
  libnetq/PollVector.c
  libnetq/PrimitiveStorage.c
  libnetq/PrimitiveType.c
  libnetq/Rect.c
  libnetq/RefCount.c
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
  libnetq/Time.c
  libnetq/TimerQueue.c
  libnetq/URL.c
  libnetq/UrlPath.c
  libnetq/UrlQuery.c
  libnetq/UTF.c
  libnetq/UUID.c
  libnetq/Version.c
  libnetq/WakeOnLan.c
  libnetq/WebSocketCalcAccept.c
  libnetq/WebSocketFrame.c
  libnetq/Window.c
  libnetq/WindowManager.c
  libnetq/Zip.c
  )

set(libnetq_CMAKEFILES
  cmake/FindAmiVideo.cmake
  cmake/FindApr.cmake
  cmake/FindAtomicOps.cmake
  cmake/FindBZip2.cmake
  cmake/FindCivetWeb.cmake
  cmake/FindCJSON.cmake
  cmake/FindDBC.cmake
  cmake/FindExpat.cmake
  cmake/FindGIF.cmake
  cmake/FindIFF.cmake
  cmake/FindILBM.cmake
  cmake/FindJansson.cmake
  cmake/FindJBig.cmake
  cmake/FindJBig2Enc.cmake
  cmake/FindJSONC.cmake
  cmake/FindLeptonica.cmake
  cmake/FindLibArchive.cmake
  cmake/FindLibBase58.cmake
  cmake/FindLibEvent.cmake
  cmake/FindLibHeif.cmake
  cmake/FindLibJpegTurbo.cmake
  cmake/FindLibNetQ.cmake
  cmake/FindLibUV.cmake
  cmake/FindLZ4.cmake
  cmake/FindLZO.cmake
  cmake/FindMHD.cmake
  cmake/FindNanoSVG.cmake
  cmake/FindOpenJP2.cmake
  cmake/FindPCRE2.cmake
  cmake/FindQuickJS.cmake
  cmake/FindSecp256k1.cmake
  cmake/FindTidy.cmake
  cmake/FindTIFF.cmake
  cmake/FindWebP.cmake
  cmake/FindZSTD.cmake
  )

set(libnetq_DATAFILES
  config.h.in
  libnetq/Features.h.in
  libnetq/Version.h.in
  LICENSE
  README.md
  )

set(libnetq_INCLUDES
  "${CMAKE_CURRENT_BINARY_DIR}/generated"
  "${CMAKE_CURRENT_SOURCE_DIR}"
  )

set(libnetq_LIBRARIES
  ${CMAKE_DL_LIBS}
  )

set(libnetq_DEFINITIONS
  )

set(libnetq_GENERATED
  )

set_source_files_properties(${libnetq_HEADERS} PROPERTIES HEADER_FILE_ONLY TRUE)

if (WITH_OPENSSL)
  set(NQ_USE_OPENSSL_CRYPTOHASH 1)
elseif (WITH_ADVAPI32)
  set(NQ_USE_ADVAPI32_CRYPTOHASH 1)
else ()
  set(NQ_USE_STUB_CRYPTOHASH 1)
  message("! CryptoHash has no implementation")
endif ()

if (WITH_OPENSSL)
  set(NQ_USE_OPENSSL_MD5 1)
elseif (NQ_USE_STUB_CRYPTOHASH)
  message("! MD5 has no implementation")
endif ()

if (WITH_OPENSSL)
  set(NQ_USE_OPENSSL_SHA1 1)
elseif (NQ_USE_STUB_CRYPTOHASH)
  message("! SHA1 has no implementation")
endif ()

if (WITH_OPENSSL)
  set(NQ_USE_OPENSSL_SHA256 1)
elseif (NQ_USE_STUB_CRYPTOHASH)
  message("! SHA256 has no implementation")
endif ()

if (WITH_OPENSSL)
  set(NQ_USE_OPENSSL_CRYPTORAND 1)
else ()
  set(NQ_USE_PLATFORM_CRYPTORAND 1)
endif ()

if (WITH_ZLIB)
  set(NQ_USE_ZLIB_CRC32 1)
else ()
  set(NQ_USE_STUB_CRC32 1)
  message("! CRC32 has no implementation")
endif ()

if (WITH_JANSSON)
  set(NQ_USE_JANSSON_JSON 1)
elseif (WITH_CJSON)
  set(NQ_USE_CJSON_JSON 1)
else ()
  set(NQ_USE_STUB_JSON 1)
  message("! JSON has no implementation")
endif ()

if (WITH_PCRE2)
  set(NQ_USE_PCRE2_REGEX 1)
else ()
  set(NQ_USE_STUB_REGEX 1)
  message("! Regex has no implementation")
endif ()

if (WITH_EXPAT)
  set(NQ_USE_EXPAT_XMLPARSER 1)
else ()
  set(NQ_USE_STUB_XMLPARSER 1)
  message("! XMLParser has no implementation")
endif ()

if (WITH_ATOMICOPS)
  set(NQ_USE_ATOMICOPS_ATOMIC 1)
else ()
  set(NQ_USE_NATIVE_ATOMIC 1)
endif ()

if (WITH_LIBBASE58)
  set(NQ_USE_LIBBASE58_BASE58 1)
else ()
  set(NQ_USE_STUB_BASE58 1)
  message("! Base58 has no implementation")
endif ()

if (WITH_SECP256K1)
  set(NQ_USE_SECP256K1_SECP256K1 1)
else ()
  set(NQ_USE_STUB_SECP256K1 1)
  message("! Secp256k1 has no implementation")
endif ()

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

if (WITH_PTHREAD)
  list(APPEND libnetq_LIBRARIES Threads::Threads)
endif ()

if (WITH_ZLIB)
  list(APPEND libnetq_LIBRARIES ZLIB::ZLIB)
endif ()

if (WITH_OLE32)
  list(APPEND libnetq_LIBRARIES ole32)
endif ()

if (WITH_ADVAPI32)
  list(APPEND libnetq_LIBRARIES advapi32)
endif ()

if (WITH_OPENSSL)
  list(APPEND libnetq_LIBRARIES OpenSSL::SSL OpenSSL::Crypto)
endif ()

if (WITH_JANSSON)
  list(APPEND libnetq_LIBRARIES Jansson::Jansson)
endif ()

if (WITH_CJSON)
  list(APPEND libnetq_LIBRARIES CJSON::CJSON)
endif ()

if (WITH_PCRE2)
  list(APPEND libnetq_LIBRARIES PCRE2::PCRE2)
endif ()

if (WITH_EXPAT)
  list(APPEND libnetq_LIBRARIES Expat::Expat)
endif ()

if (WITH_ATOMICOPS)
  list(APPEND libnetq_LIBRARIES AtomicOps::AtomicOps)
endif ()

if (WITH_CIVETWEB)
  list(APPEND libnetq_LIBRARIES CivetWeb::CivetWeb)
endif ()

if (WITH_LIBBASE58)
  list(APPEND libnetq_LIBRARIES Bitcoin::LibBase58)
endif ()

if (WITH_SECP256K1)
  list(APPEND libnetq_LIBRARIES Bitcoin::Secp256k1)
endif ()

configure_file(config.h.in "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY)
configure_file(libnetq/Version.h.in "${CMAKE_CURRENT_BINARY_DIR}/generated/libnetq/Version.h" @ONLY)
configure_file(libnetq/Features.h.in "${CMAKE_CURRENT_BINARY_DIR}/generated/libnetq/Features.h" @ONLY)

list(APPEND libnetq_GENERATED
  "${CMAKE_CURRENT_BINARY_DIR}/config.h"
  "${CMAKE_CURRENT_BINARY_DIR}/generated/libnetq/Version.h"
  "${CMAKE_CURRENT_BINARY_DIR}/generated/libnetq/Features.h"
  )

source_group(TREE "${CMAKE_CURRENT_BINARY_DIR}" FILES ${libnetq_GENERATED})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${libnetq_HEADERS} ${libnetq_SOURCES} ${libnetq_CMAKEFILES} ${libnetq_DATAFILES})
add_library(libnetq ${LNQ_LIBRARY_TYPE} ${libnetq_GENERATED} ${libnetq_HEADERS} ${libnetq_SOURCES} ${libnetq_CMAKEFILES} ${libnetq_DATAFILES})
target_include_directories(libnetq PUBLIC "$<BUILD_INTERFACE:${libnetq_INCLUDES}>")
target_include_directories(libnetq PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
target_compile_definitions(libnetq PUBLIC "${libnetq_DEFINITIONS}")
target_link_libraries(libnetq PUBLIC ${libnetq_LIBRARIES})
set_target_properties(libnetq PROPERTIES LINKER_LANGUAGE "C")
set_target_properties(libnetq PROPERTIES PREFIX "" IMPORT_PREFIX "")
set_target_properties(libnetq PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(libnetq PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(LibNetQ::LibNetQ ALIAS libnetq)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/generated/libnetq/Version.h"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/libnetq/Features.h"
  DESTINATION
    "${CMAKE_INSTALL_INCLUDEDIR}/libnetq"
  )

foreach (_header ${libnetq_HEADERS})
  get_filename_component(_directory "${_header}" DIRECTORY)
  install(FILES "${_header}" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${_directory}")
endforeach ()

install(FILES ${libnetq_CMAKEFILES} DESTINATION "${CMAKE_INSTALL_DATADIR}/libnetq/cmake")
install(FILES ${libnetq_CMAKEFILES} DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/libnetq")

install(TARGETS libnetq
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  )

message(STATUS "Build type . . . . . . . . . . . . . . . . . ${LNQ_LIBRARY_TYPE}")
message(STATUS "PThread  . . . . . . . . . . . . . . . . . . ${WITH_PTHREAD} (HAVE ${HAVE_PTHREAD})")
message(STATUS "ZLib . . . . . . . . . . . . . . . . . . . . ${WITH_ZLIB} (HAVE ${HAVE_ZLIB})")
message(STATUS "Ole32  . . . . . . . . . . . . . . . . . . . ${WITH_OLE32} (HAVE ${HAVE_OLE32})")
message(STATUS "Advapi32 . . . . . . . . . . . . . . . . . . ${WITH_ADVAPI32} (HAVE ${HAVE_ADVAPI32})")
message(STATUS "OpenSSL  . . . . . . . . . . . . . . . . . . ${WITH_OPENSSL} (HAVE ${HAVE_OPENSSL})")
message(STATUS "Jansson  . . . . . . . . . . . . . . . . . . ${WITH_JANSSON} (HAVE ${HAVE_JANSSON})")
message(STATUS "cJSON  . . . . . . . . . . . . . . . . . . . ${WITH_CJSON} (HAVE ${HAVE_CJSON})")
message(STATUS "PCRE2  . . . . . . . . . . . . . . . . . . . ${WITH_PCRE2} (HAVE ${HAVE_PCRE2})")
message(STATUS "CivetWeb . . . . . . . . . . . . . . . . . . ${WITH_CIVETWEB} (HAVE ${HAVE_CIVETWEB})")
message(STATUS "LibEvent . . . . . . . . . . . . . . . . . . ${WITH_LIBEVENT} (HAVE ${HAVE_LIBEVENT})")
message(STATUS "LibUV  . . . . . . . . . . . . . . . . . . . ${WITH_LIBUV} (HAVE ${HAVE_LIBUV})")
message(STATUS "Expat  . . . . . . . . . . . . . . . . . . . ${WITH_EXPAT} (HAVE ${HAVE_EXPAT})")
message(STATUS "AtomicOps  . . . . . . . . . . . . . . . . . ${WITH_ATOMICOPS} (HAVE ${HAVE_ATOMICOPS})")
message(STATUS "LibBase58  . . . . . . . . . . . . . . . . . ${WITH_LIBBASE58} (HAVE ${HAVE_LIBBASE58})")
message(STATUS "Secp256k1  . . . . . . . . . . . . . . . . . ${WITH_SECP256K1} (HAVE ${HAVE_SECP256K1})")
