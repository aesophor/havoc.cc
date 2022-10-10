// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_I_APP_SYSTEM_H_
#define HAVOC_I_APP_SYSTEM_H_

struct AppSystemInfo_t {
  const char* m_pModuleName;
  const char* m_pInterfaceName;
};

enum InitReturnVal_t {
  INIT_FAILED = 0,
  INIT_OK,

  INIT_LAST_VAL,
};

enum AppSystemTier_t {
  APP_SYSTEM_TIER0 = 0,
  APP_SYSTEM_TIER1,
  APP_SYSTEM_TIER2,
  APP_SYSTEM_TIER3,

  APP_SYSTEM_TIER_OTHER,
};

typedef void* (*CreateInterfaceFn) (const char*, int*);

class IAppSystem {
 public:
  virtual bool Connect(CreateInterfaceFn factory) = 0;
  virtual void Disconnect() = 0;

  // Here's where systems can access other interfaces implemented by this object
  // Returns nullptr if it doesn't implement the requested interface
  virtual void* QueryInterface(const char* pInterfaceName) = 0;

  virtual InitReturnVal_t Init() = 0;
  virtual void Shutdown() = 0;
  virtual const AppSystemInfo_t* GetDependencies() = 0;
  virtual AppSystemTier_t GetTier() = 0;
  virtual void Reconnect(CreateInterfaceFn factory, const char* pInterfaceName) = 0;
  virtual bool IsSingleton() = 0;
};

#endif  // HAVOC_I_APP_SYSTEM_H_
