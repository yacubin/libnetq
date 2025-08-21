/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_ZIP_H
#define _LIBNETQ_ZIP_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ZIP_DATA_DESC_FLAG (1 << 3)

#define ZipLocalFileHeaderSig 0x04034b50
#define ZipLocalFileHeaderSize 30
struct ZipLocalFileHeader {
  uint32_t signature;
  uint16_t version;
  uint16_t flags;
  uint16_t compression;
  uint16_t lastTime;
  uint16_t lastDate;
  uint32_t crc32;
  uint32_t compressedSize;
  uint32_t uncompressedSize;
  uint16_t nameSize;
  uint16_t extraFieldSize;
  /*  <name>   */
  /*  <field>  */
};

#define ZipDataDescriptorSig 0x08074b50
#define ZipDataDescriptorSize 16
struct ZipDataDescriptor {
  uint32_t signature;
  uint32_t crc32;
  uint32_t compressedSize;
  uint32_t uncompressedSize;
};

#define ZipDataDescriptor64Size 24
struct ZipDataDescriptor64 {
  uint32_t signature;
  uint32_t crc32;
  uint64_t compressedSize;
  uint64_t uncompressedSize;
};

#define ZipCentralDirHeaderSig 0x02014b50
#define ZipCentralDirHeaderSize 46
struct ZipCentralDirHeader {
  uint32_t signature;
  uint16_t targetVersion;
  uint16_t minVersion;
  uint16_t flags;
  uint16_t compression;
  uint16_t lastTime;
  uint16_t lastDate;
  uint32_t crc32;
  uint32_t compressedSize;
  uint32_t uncompressedSize;
  uint16_t nameSize;
  uint16_t extraFieldSize;
  uint16_t commentSize;
  uint16_t diskNumberStart;
  uint16_t internalAttributes;
  uint32_t externalAttributes;
  uint32_t offset;
  /*  <name>   */
  /*  <field>  */
  /* <comment> */
};

#define ZipEndCentralDirSig 0x06054b50
#define ZipEndCentralDirSize 22
struct ZipEndCentralDir {
  uint32_t signature;
  uint16_t totalCurDisk;
  uint16_t totalDisks;
  uint16_t totalCurEntries;
  uint16_t totalEntries;
  uint32_t centralDirSize;
  uint32_t centralDirOffset;
  uint16_t commentLength;
  /* <comment> */
};

enum {
  ZIP_ZIP64_FIELD          = 0x0001,
  ZIP_TIMESTAMP_FIELD      = 0x5455,
  ZIP_UNIXSTAT_FIELD       = 0x7875,
};

#define	ZIP_MODIFY_TIME_BIT (1 << 0)
#define	ZIP_ACCESS_TIME_BIT (1 << 1)
#define	ZIP_CREATE_TIME_BIT (1 << 2)

#define ZIP_COMPRESSION_DEFLATED 8

struct ZipExtraFields {
  // 0x5455
  struct {
    uint8_t flags;
    uint32_t modification;
    uint32_t access;
    uint32_t creation;
  } timestamp;

  // 0x7875
  struct {
    uint32_t uid;
    uint32_t gid;
  } unixstat;
};

NQ_EXPORT bool ZipLocalFileHeader_parse(struct ZipLocalFileHeader* header, const uint8_t* start, const uint8_t* end, const uint8_t** pend);
NQ_EXPORT bool ZipDataDescriptor_parse(struct ZipDataDescriptor* desc, const uint8_t* start, const uint8_t* end, const uint8_t** pend);
NQ_EXPORT bool ZipDataDescriptor64_parse(const uint8_t* data, size_t size, struct ZipDataDescriptor64* result);
NQ_EXPORT void ZipExtraFields_process(const uint8_t* data, size_t size, struct ZipExtraFields* result);
NQ_EXPORT const uint8_t* ZipEndCentralDir_find(struct ZipEndCentralDir* self, const uint8_t* data, size_t size);
NQ_EXPORT bool ZipCentralDirHeader_parse(struct ZipCentralDirHeader* header, const uint8_t* start, const uint8_t* end, const uint8_t** pend);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_ZIP_H */
