/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#define NQ_CLASS_NAME "NQArguments"
#define NQ_LOG_TAG NQ_CLASS_NAME

#include "config.h"
#include "libnetq/Arguments.h"

#include <libnetq/ObjectClass.h>
#include <libnetq/CType.h>
#include <libnetq/Limits.h>
#include <libnetq/Malloc.h>
#include <libnetq/UTF.h>
#include <libnetq/Log.h>
#include <libnetq/Assert.h>

#include <string.h>

#define NQ_ARG_MAX 4096

#define isArgSpace(c) ((c) == ' ' || (c) == '\t' || (c) == '\r' || (c) == '\n')

extern const NQObjectClass __NQArgumentsClass;

struct NQArguments {
  const NQObjectClass* class;
  NQArgs args;
};

static const char* s_argvEmpty[] = { NULL };
static NQArguments s_argumentsEmpty = {
  &__NQArgumentsClass,
  { 0, s_argvEmpty }
};

#ifdef NQ_OS_WINDOWS
static HINSTANCE s_hInstance = NULL;
static HINSTANCE s_hPrevInstance = NULL;
static PSTR s_lpCmdLine = NULL;
static PWSTR s_lpCmdLineW = NULL;
static INT s_iCmdShow = 0;
#endif

static NQArguments* s_arguments = &s_argumentsEmpty;

static const char* skipKey(const char* arg, const char* key)
{
  if (*arg != '-')
    return NULL;
  arg++;

  if (*arg != '-')
    return NULL;
  arg++;

  size_t len = strlen(key);
  if (memcmp(arg, key, len) != 0)
    return NULL;
  arg += len;

  return arg;
}

const char* NQArgGetStringOr(const char* arg, const char* key, const char* defval)
{
  arg = skipKey(arg, key);
  if (arg == NULL)
    return defval;

  if (*arg != '=')
    return defval;
  arg++;

  return arg;
}

int32_t NQArgGetInt32Or(const char* arg, const char* key, int32_t defval)
{
  int32_t result;
  if (NQArgGetInt32(arg, key, &result))
    return result;
  return defval;
}

int64_t NQArgGetInt64Or(const char* arg, const char* key, int64_t defval)
{
  int64_t result;
  if (NQArgGetInt64(arg, key, &result))
    return result;
  return defval;
}

uint32_t NQArgGetUint32Or(const char* arg, const char* key, uint32_t defval)
{
  uint32_t result;
  if (NQArgGetUint32(arg, key, &result))
    return result;
  return defval;
}

uint64_t NQArgGetUint64Or(const char* arg, const char* key, uint64_t defval)
{
  uint64_t result;
  if (NQArgGetUint64(arg, key, &result))
    return result;
  return defval;
}

bool NQArgHasKey(const char* arg, const char* key)
{
  arg = skipKey(arg, key);
  if (arg == NULL)
    return false;

  return (*arg == '=' || *arg == '\0');
}

bool NQArgHasKeyOnly(const char* arg, const char* key)
{
  arg = skipKey(arg, key);
  if (arg == NULL)
    return false;

  return (*arg == '\0');
}

bool NQArgGetBool(const char* arg, const char* key, bool* result)
{
  arg = skipKey(arg, key);
  if (arg == NULL)
    return false;

  if (*arg == '\0') {
    if (result) *result = true;
    return true;
  }

  if (*arg != '=')
    return false;
  arg++;

  if (NQIsEnabled(arg)) {
    if (result) *result = false;
    return true;
  }

  if (NQIsDisabled(arg)) {
    if (result) *result = false;
    return true;
  }

  return false;
}

bool NQArgGetInt32(const char* arg, const char* key, int32_t* result)
{
  int64_t num;
  if (!NQArgGetInt64(arg, key, &num))
    return false;

  if (num < NQ_INT32_MIN || NQ_INT32_MAX < num)
    return false;

  if (result)
    *result = (int32_t)num;

  return true;
}

bool NQArgGetInt64(const char* arg, const char* key, int64_t* result)
{
  const char* val = NQArgGetStringOr(arg, key, NULL);
  if (val == NULL)
    return false;

  char* end;
  long long num = strtoll(val, &end, 10);
  if (*end != '\0')
    return false;

  if (result)
    *result = (int64_t)num;

  return true;
}

bool NQArgGetUint32(const char* arg, const char* key, uint32_t* result)
{
  uint64_t num;
  if (!NQArgGetUint64(arg, key, &num))
    return false;

  if (UINT32_MAX < num)
    return false;

  if (result)
    *result = (uint32_t)num;

  return true;
}


bool NQArgGetUint64(const char* arg, const char* key, uint64_t* result)
{
  const char* val = NQArgGetStringOr(arg, key, NULL);
  if (val == NULL)
    return false;

  char* end;
  unsigned long long num = strtoull(val, &end, 10);
  if (*end != '\0')
    return false;

  if (result)
    *result = (uint64_t)num;

  return true;
}

bool NQArgGetBoolOr(const char* arg, const char* key, bool defval)
{
  bool result;
  if (NQArgGetBool(arg, key, &result))
    return result;
  return defval;
}

size_t NQArgs_count(NQArgs* thiz)
{
  return (size_t)thiz->argc;
}

const char** NQArgs_items(NQArgs* thiz)
{
  return thiz->argv;
}

const char* NQArgs_at(NQArgs* thiz, size_t index)
{
  if (index <= NQ_INT_MAX && (int)index < thiz->argc)
    return thiz->argv[index];
  return NULL;
}

bool NQArgs_has(NQArgs* thiz, const char* value)
{
  int i;

  for (i = 0; i < thiz->argc; i++) {
    if (!strcmp(thiz->argv[i], value))
      return true;
  }

  return false;
}

bool NQArgs_hasLKey(NQArgs* thiz, const char* value)
{
  int i;

  for (i = 0; i < thiz->argc; i++) {
    const char* p1 = thiz->argv[i];

    if (*p1 != '-')
      continue;
    p1++;

    if (*p1 != '-')
      continue;
    p1++;

    if (*p1 == '=' || *p1 == '\0')
      continue;

    const char* p2 = value;
    if (*p1 != *p2)
      continue;

    for(;;) {
      p1++;
      p2++;

      if (*p1 != *p2) {
        if (*p1 == '=' && *p2 == '\0')
          return true;
        break;
      }

      if (*p1 == '\0')
        return true;
    }
  }

  return false;
}

static size_t parseCommandLineUTF16(const uint16_t* str, char* buffer, char** argv, int* argc)
{
  const uint16_t* start = str;
  uint8_t* p = (uint8_t*)buffer;

  int n = 0;
  size_t size = 0;
  bool isQm = false;
  bool isSlash = false;
  bool isText = false;
  char* text = NULL;
  size_t sz;

  for (;;) {
    NQUChar ch = NQUCharNext16(start, NULL, (const uint16_t**)&start);
    if (ch == '\0') {
      if (isSlash) {
        sz = (p == NULL) ? NQUCharSize8('\\') : NQUCharPush8(p, NULL, &p, '\\');
        if (sz == 0) {
          size = 0;
          break;
        }
        size += sz;
      }
      if (isText) {
        if (p != NULL)
          *p++ = '\0';
        if (argv != NULL)
          argv[n] = text;
        n++;
        size++;
      }
      break;
    }

    if (!NQIsUChar(ch))
      break;

    if (isQm) {
       if (!isSlash && ch == '\\')
         isSlash = true;
       else if (!isSlash && ch == '\"') {
        isQm = false;
        isSlash = false;
      }
      else {
        if (isSlash) {
          isSlash = false;
          if (ch != '\"') {
            sz = (p == NULL) ? NQUCharSize8('\\') : NQUCharPush8(p, NULL, &p, '\\');
            if (sz == 0) {
              size = 0;
              break;
            }
            size += sz;
          }
        }
        sz = (p == NULL) ? NQUCharSize8(ch) : NQUCharPush8(p, NULL, &p, ch);
        if (sz == 0) {
          size = 0;
          break;
        }
        size += sz;
      }
      continue;
    }

    if (ch == '\"') {
      isQm = true;
      if (!isText) {
        text = (char*)p;
        isText = true;
      }
      continue;
    }

    if (isArgSpace(ch)) {
      if (isText) {
        if (p != NULL)
          *p++ = '\0';
        if (argv != NULL)
          argv[n] = text;
        n++;
        size++;
        isText = false;
      }
      continue;
    }

    if (!isText) {
      text = (char*)p;
      isText = true;
    }

    sz = (p == NULL) ? NQUCharSize8(ch) : NQUCharPush8(p, NULL, &p, ch);
    if (sz == 0) {
      size = 0;
      break;
    }
    size += sz;
  }

  if (argc != NULL)
    *argc = (size != 0) ? n : 0;

  return size;
}

NQArguments* NQArguments_create(int argc, const char* argv[])
{
  // TODO: For windows platform convert locale to utf8
  int i;

  size_t classSize = NQ_ALIGN_UP(sizeof(NQArguments), sizeof(char*));
  size_t argvSize = (size_t)(argc + 1) * sizeof(char*);

  size_t charSize = 0;
  for (i = 0; i < argc; i++)
    charSize +=  strlen(argv[i]) + 1;

  NQArguments* thiz = (NQArguments*)NQMalloc(classSize + argvSize + charSize);
  if (thiz == NULL)
    return NULL;

  const char** pargv = (const char**)(void*)((char*)thiz + classSize);
  char* pchar = (char*)((char*)pargv + argvSize);

  const char** pargvStart = pargv;
  char* pcharStart = pchar;
  for (i = 0; i < argc; i++) {
    const char* arg = argv[i];
    *pargvStart++ = pcharStart;
    for (;;) {
      *pcharStart = *arg;
      if (*pcharStart++ == '\0')
        break;
      arg++;
    }
  }
  *pargvStart++ = NULL;

  NQ_ASSERT((char*)pargvStart == (char*)pchar);
  NQ_ASSERT(((char*)thiz + classSize + argvSize + charSize) == pcharStart);

  thiz->class = &__NQArgumentsClass;
  thiz->args.argc = argc;
  thiz->args.argv = pargv;

  return thiz;
}

NQArguments* NQArguments_parse(const char* s)
{
  return NULL;
}

NQArguments* NQArguments_parseUTF16(const uint16_t* str)
{
  int argcSave;
  size_t sizeSave = parseCommandLineUTF16(str, NULL, NULL, &argcSave);
  size_t argvSizeInBytes = (argcSave + 1) * sizeof(char*);
  size_t sizeInBytes = argvSizeInBytes + sizeSave;

  size_t classSize = NQ_ALIGN_UP(sizeof(NQArguments), sizeof(char*));
  NQArguments* thiz = (NQArguments*)NQMalloc(classSize + sizeInBytes);
  if (thiz == NULL)
    return NULL;

  thiz->class = &__NQArgumentsClass;

  if (argcSave == 0) {
    thiz->args.argc = 0;
    thiz->args.argv = s_argvEmpty;
  }
  else {
    thiz->args.argv = (const char**)((char*)thiz + classSize);
    char* buffer = (char*)thiz->args.argv + argvSizeInBytes;
    size_t size = parseCommandLineUTF16(str, buffer, (char**)thiz->args.argv, &thiz->args.argc);
    NQ_ASSERT(thiz->args.argc == argcSave);
    NQ_ASSERT_UNUSED(size, size == sizeSave);
    thiz->args.argv[thiz->args.argc] = NULL;
  }

  return thiz;
}

void NQArguments_destroy(NQArguments* thiz)
{
  NQFree(thiz);
}

size_t NQArguments_count(NQArguments* thiz)
{
  return NQArgs_count(&thiz->args);
}

const char** NQArguments_items(NQArguments* thiz)
{
  return NQArgs_items(&thiz->args);
}

const char* NQArguments_at(NQArguments* thiz, size_t index)
{
  return NQArgs_at(&thiz->args, index);
}

bool NQArguments_has(NQArguments* thiz, const char* value)
{
  return NQArgs_has(&thiz->args, value);
}

bool NQArguments_hasLKey(NQArguments* thiz, const char* value)
{
  return NQArgs_hasLKey(&thiz->args, value);
}

void NQMainArgumentsInit(int argc, const char* argv[])
{
  NQArguments* arguments = NQArguments_create(argc, argv);
  if (arguments != NULL)
    s_arguments = arguments;
}

#ifdef NQ_OS_WINDOWS

void NQMainArgumentsInitWin(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT iCmdShow)
{
  s_hInstance = hInstance;
  s_hPrevInstance = hPrevInstance;
  s_lpCmdLine = lpCmdLine;
  s_lpCmdLineW = NULL;
  s_iCmdShow = iCmdShow;

  PWSTR str = GetCommandLineW();
  NQArguments* arguments = NQArguments_parseUTF16((uint16_t*)str);
  s_arguments = (arguments != NULL) ? arguments : &s_argumentsEmpty;
}

void NQMainArgumentsInitWinW(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, INT iCmdShow)
{
  s_hInstance = hInstance;
  s_hPrevInstance = hPrevInstance;
  s_lpCmdLine = NULL;
  s_lpCmdLineW = lpCmdLine;
  s_iCmdShow = iCmdShow;

  PWSTR str = GetCommandLineW();
  NQArguments* arguments = NQArguments_parseUTF16((uint16_t*)str);
  s_arguments = (arguments != NULL) ? arguments : &s_argumentsEmpty;
}

HINSTANCE NQGetInstance()
{
  if (s_hInstance == NULL)
    s_hInstance = GetModuleHandleW(NULL);
  return s_hInstance;
}

HINSTANCE NQGetPrevInstance()
{
  return s_hPrevInstance;
}

INT NQGetCmdShow()
{
  return s_iCmdShow;
}

#endif

void NQMainArgumentsFinalize()
{
  if (s_arguments != &s_argumentsEmpty) {
    NQFree(s_arguments);
    s_arguments = &s_argumentsEmpty;
  }
}

NQArguments* NQGetMainArguments()
{
  return s_arguments;
}

const NQObjectClass __NQArgumentsClass = {
  NQArgumentsObjectType,
  NQ_CLASS_NAME,
  NQ_VERSION_CODE,
  (NQObjectReleaseCallback)NQArguments_destroy,
};
