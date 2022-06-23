// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_C_RECV_TABLE_H_
#define HAVOC_C_RECV_TABLE_H_

#include <cstdint>

enum class ESendPropType : int32_t {
  INT = 0,
  FLOAT,
  VECTOR,
  VECTOR2D,
  STRING,
  ARRAY,
  DATATABLE,
  INT64,
  SENDPROPTYPEMAX
};

struct CDataVariant {
  union {
    float Float;
    int32_t Int;
    char *String;
    void *Data;
    float Vector[3];
    int64_t Int64;
  };

  ESendPropType type;
};

struct CRecvProp;
struct CRecvProxyData {
  CRecvProp *prop;
  CDataVariant value;
  int32_t element;
  int32_t objectID;
};

struct CRecvTable {
  CRecvProp *props;
  int32_t count;
  void *decoder;
  char *name;
  bool isInitialized;
  bool inMainList;
};

struct CRecvProp {
  char *name;
  ESendPropType type;
  int32_t flags;
  int32_t stringBufferSize;
  bool isInsideArray;
  const void *extraData;
  CRecvProp *prop;
  void *arrayLengthProxyFn;
  void *proxyFn;
  void *dataTableProxyFn;
  CRecvTable *table;
  unsigned int offset;
  int32_t stride;
  int32_t elements;
  const char *parentPropName;
};

#endif  // HAVOC_C_RECV_TABLE_H_
