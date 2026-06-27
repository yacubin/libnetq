/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_DBC_DBCJ1939_H
#define _LIBNETQ_DBC_DBCJ1939_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#define kNQDBCJ1939Broadcast 0xFFu
typedef uint32_t NQDBCJ1939Id;

NQ_EXPORT uint32_t NQDBCJ1939Mkpdu(uint8_t p, uint32_t pgn, uint8_t da, uint8_t sa);
NQ_EXPORT uint32_t NQDBCJ1939Mkpdu1(uint8_t p, uint32_t pgn, uint8_t da, uint8_t sa);

NQ_EXPORT uint8_t NQDBCJ1939GetP(uint32_t id);
NQ_EXPORT uint8_t NQDBCJ1939GetR(uint32_t id);
NQ_EXPORT uint8_t NQDBCJ1939GetDP(uint32_t id);
NQ_EXPORT uint8_t NQDBCJ1939GetPF(uint32_t id);
NQ_EXPORT bool NQDBCJ1939IsPDU1(uint32_t id);
NQ_EXPORT bool NQDBCJ1939IsPDU2(uint32_t id);
NQ_EXPORT uint8_t NQDBCJ1939GetSA(uint32_t id);
NQ_EXPORT bool NQDBCJ1939SetSA(uint32_t* id, uint8_t sa);
NQ_EXPORT uint8_t NQDBCJ1939GetDA(uint32_t id);
NQ_EXPORT bool NQDBCJ1939SetDA(uint32_t*, uint8_t da);
NQ_EXPORT uint32_t NQDBCJ1939GetPGN(uint32_t id);

NQ_EXPORT uint32_t NQDBCJ1939kDM1DTC(int spn, uint8_t fmi, uint8_t oc);

/*
   Protocol data unit
*/
typedef union NQDBCJ1939PDU {
  uint32_t id;

  struct {
    uint32_t pad : 2;
    uint32_t p : 3;   // Priority
    uint32_t r : 1;   // Reserved
    uint32_t dp : 2;  // Data Page
    uint32_t pf : 8;  // PDU Format
    uint32_t ps : 8;  // PDU Specific
    uint32_t sa : 8;  // Source Address
  } pdu;

  struct {
    uint32_t pad : 2;
    uint32_t p : 3;  // Priority
    uint32_t r : 1;  // Reserved
    uint32_t dp : 2; // Data Page
    uint32_t pf : 8; // PDU Format
    uint32_t da : 8; // Destination Address
    uint32_t sa : 8; // Source Address
  } pdu1;

  struct {
    uint32_t pad : 2;
    uint32_t p : 3;  // Priority
    uint32_t r : 1;  // Reserved
    uint32_t dp : 2; // Data Page
    uint32_t pf : 8; // PDU Format
    uint32_t ge : 8; // Group Extension
    uint32_t sa : 8; // Source Address
  } pdu2;

} NQDBCJ1939PDU;

NQ_EXPORT void NQDBCJ1939PDU_init(NQDBCJ1939PDU*, uint32_t id);
NQ_EXPORT uint8_t NQDBCJ1939PDU_version(const NQDBCJ1939PDU*);
NQ_EXPORT uint8_t NQDBCJ1939PDU_priority(const NQDBCJ1939PDU*);
NQ_EXPORT uint32_t NQDBCJ1939PDU_pgn(const NQDBCJ1939PDU*);
NQ_EXPORT uint8_t NQDBCJ1939PDU_sa(const NQDBCJ1939PDU*);
NQ_EXPORT uint8_t NQDBCJ1939PDU_da(const NQDBCJ1939PDU*);

typedef struct NQDBCJ1939Lamps {
  uint8_t mil : 2;
  uint8_t rsl : 2;
  uint8_t awl : 2;
  uint8_t pl : 2;
} NQDBCJ1939Lamps;

typedef struct NQDBCJ1939DTC {
  int spn;
  uint8_t fmi;
  uint8_t oc;
} NQDBCJ1939DTC;

#define NQDBCJ1939_DM1_PGN 65226u

typedef struct NQDBCJ1939DM1 {
  uint16_t size;
  uint8_t data[400];
} NQDBCJ1939DM1;

NQ_EXPORT void NQDBCJ1939DM1_init(NQDBCJ1939DM1* dm1);
NQ_EXPORT void NQDBCJ1939DM1_setMIL(NQDBCJ1939DM1* dm1, uint8_t state, uint8_t flash);
NQ_EXPORT void NQDBCJ1939DM1_setRSL(NQDBCJ1939DM1* dm1, uint8_t state, uint8_t flash);
NQ_EXPORT void NQDBCJ1939DM1_setAWL(NQDBCJ1939DM1* dm1, uint8_t state, uint8_t flash);
NQ_EXPORT void NQDBCJ1939DM1_setPL(NQDBCJ1939DM1* dm1, uint8_t state, uint8_t flash);
NQ_EXPORT bool NQDBCJ1939DM1_addDTC(NQDBCJ1939DM1* dm1, int spn, uint8_t fmi, uint8_t oc);
NQ_EXPORT size_t NQDBCJ1939DM1_read(const NQDBCJ1939DM1* dm1, void* data, size_t size);

#define NQDBCJ1939_TP_PACKEGE_MAX 256u
#define NQDBCJ1939_TP_DATA_SIZE_MAX ((NQDBCJ1939_TP_PACKEGE_MAX - 1u) * 7u)

#define kNQDBCCanFrameSizeMax 8u
typedef struct NQDBCCanFrame {
  uint32_t id;
  uint8_t size;
  uint8_t __reserve[3];
  uint8_t data[kNQDBCCanFrameSizeMax];
} NQDBCCanFrame;

NQ_EXPORT void NQDBCCanFrame_init(NQDBCCanFrame* frame, uint32_t id, const uint8_t* data, size_t size);
NQ_EXPORT bool NQDBCCanFrame_append(NQDBCCanFrame* frame, const uint8_t* data, size_t size);
NQ_EXPORT void NQDBCCanFrame_dump(NQDBCCanFrame* frame, char* buffer, size_t n);

typedef struct NQDBCJ1939TPBuilder {
  uint8_t p;
  uint8_t sa;
  uint8_t da;
  uint8_t sn;
  uint16_t nbyte;
  NQDBCCanFrame frames[NQDBCJ1939_TP_PACKEGE_MAX];
} NQDBCJ1939TPBuilder;

NQ_EXPORT void NQDBCJ1939TPBuilder_init(NQDBCJ1939TPBuilder* builder, uint8_t p, uint32_t pgn, uint8_t da, uint8_t sa);
NQ_EXPORT bool NQDBCJ1939TPBuilder_append(NQDBCJ1939TPBuilder* builder, const uint8_t* data, size_t size);
NQ_EXPORT size_t NQDBCJ1939TPBuilder_frameCount(const NQDBCJ1939TPBuilder* builder);
NQ_EXPORT const NQDBCCanFrame* NQDBCJ1939TPBuilder_getFrameByIndex(const NQDBCJ1939TPBuilder* builder, size_t index);

enum {
  kNQDBCJ1939CalcRuleAEBS2,
  kNQDBCJ1939CalcRuleXBR,
  kNQDBCJ1939CalcRuleSAS,
  kNQDBCJ1939CalcRuleCN,
};

NQ_EXPORT bool NQDBCJ1939IsValidChecksum(int rule, const NQDBCCanFrame* frame);
NQ_EXPORT bool NQDBCJ1939UpdateChecksum(int rule, NQDBCCanFrame* frame, uint8_t counter);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _LIBNETQ_DBC_DBCJ1939_H */
