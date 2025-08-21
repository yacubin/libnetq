#include "config.h"
#include "libnetq/HttpBuilder.h"

#include <libnetq/Assert.h>
#include <libnetq/HttpVersion.h>
#include <libnetq/HttpHeader.h>
#include <libnetq/HttpMethod.h>
#include <libnetq/String.h>

static bool NQHttpBuilder_write(NQHttpBuilder* thiz, const char* characters)
{
  uint16_t newSize = thiz->size;
  NQ_ASSERT(thiz->data[newSize]== '\0');

  for (;;) {
    if (newSize >= sizeof(thiz->data)) {
      return false;
    }
    thiz->data[newSize] = *characters;
    if (*characters == '\0')
      break;
    newSize++;
    characters++;
  }

  thiz->size = newSize;
  return true;
}

static bool NQHttpBuilder_writen(NQHttpBuilder* thiz, const char* characters, size_t length)
{
  if (length == 0) {
    return true;
  }

  size_t newSize = thiz->size + length;
  if (sizeof(thiz->data) <= newSize) {
    return false;
  }

  memcpy(thiz->data + thiz->size, characters, length);
  thiz->data[newSize] = '\0';
  thiz->size = newSize;
  return true;
}

bool NQHttpBuilder_init(NQHttpBuilder* thiz)
{
  thiz->data[0] = '\0';
  thiz->size = 0;
  return true;
}

void NQHttpBuilder_finalize(NQHttpBuilder* thiz)
{
}

bool NQHttpBuilder_addRequest(NQHttpBuilder* thiz, const char* method, const char* url, const char* version)
{
  if (!NQHttpBuilder_write(thiz, method))
    return false;

  if (!NQHttpBuilder_write(thiz, " "))
    return false;

  if (!NQHttpBuilder_write(thiz, url))
    return false;

  if (!NQHttpBuilder_write(thiz, " "))
    return false;

  if (!NQHttpBuilder_write(thiz, version))
    return false;

  if (!NQHttpBuilder_write(thiz, NQ_HTTP_CRLF))
    return false;

  return true;
}

bool NQHttpBuilder_addGetRequest(NQHttpBuilder* thiz, const char* url)
{
  if (!NQHttpBuilder_write(thiz, NQ_HTTP_GET " "))
    return false;

  if (!NQHttpBuilder_write(thiz, url))
    return false;

  if (!NQHttpBuilder_write(thiz, " " NQ_HTTP11_VERSION NQ_HTTP_CRLF))
    return false;

  return true;
}

bool NQHttpBuilder_addStatusLine(NQHttpBuilder* thiz, const char* version, const char* code, const char* reason)
{
  if (!NQHttpBuilder_write(thiz, version))
    return false;

  if (!NQHttpBuilder_write(thiz, " "))
    return false;

  if (!NQHttpBuilder_write(thiz, code))
    return false;

  if (!NQHttpBuilder_write(thiz, " "))
    return false;

  if (!NQHttpBuilder_write(thiz, reason))
    return false;

  if (!NQHttpBuilder_write(thiz, NQ_HTTP11_VERSION NQ_HTTP_CRLF))
    return false;

  return true;
}

bool NQHttpBuilder_addStatusCode(NQHttpBuilder* thiz, uint16_t code)
{
  switch (code) {
  case 101:
    return NQHttpBuilder_write(thiz, NQ_HTTP11_VERSION " 101 Switching Protocols" NQ_HTTP_CRLF);
  }

  return false;
}

bool NQHttpBuilder_addHeader(NQHttpBuilder* thiz, const char* name, const char* value)
{
  if (!NQHttpBuilder_write(thiz, name))
    return false;

  if (!NQHttpBuilder_write(thiz, ": "))
    return false;

  if (!NQHttpBuilder_write(thiz, value))
    return false;

  if (!NQHttpBuilder_write(thiz, NQ_HTTP_CRLF))
    return false;

  return true;
}

bool NQHttpBuilder_finish(NQHttpBuilder* thiz)
{
  return NQHttpBuilder_write(thiz, NQ_HTTP_CRLF);
}
