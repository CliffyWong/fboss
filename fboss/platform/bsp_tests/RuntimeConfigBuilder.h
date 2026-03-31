// Copyright (c) Meta Platforms, Inc. and affiliates.

#pragma once

#include <thrift/lib/cpp2/protocol/Serializer.h>
#include <string>
#include <tuple>
#include "fboss/platform/bsp_tests/gen-cpp2/bsp_tests_config_types.h"
#include "fboss/platform/bsp_tests/gen-cpp2/bsp_tests_runtime_config_types.h"
#include "fboss/platform/bsp_tests/gen-cpp2/fbiob_device_config_types.h"
#include "fboss/platform/platform_manager/gen-cpp2/platform_manager_config_types.h"

namespace facebook::fboss::platform::bsp_tests {
using facebook::fboss::platform::platform_manager::BspKmodsFile;
using facebook::fboss::platform::platform_manager::FanPwmCtrlConfig;
using facebook::fboss::platform::platform_manager::FpgaIpBlockConfig;
using facebook::fboss::platform::platform_manager::IdpromConfig;
using facebook::fboss::platform::platform_manager::LedCtrlConfig;
using facebook::fboss::platform::platform_manager::PlatformConfig;
using facebook::fboss::platform::platform_manager::PmUnitConfig;
using facebook::fboss::platform::platform_manager::PmUnitVersion;
using facebook::fboss::platform::platform_manager::SpiMasterConfig;
using facebook::fboss::platform::platform_manager::XcvrCtrlConfig;

class RuntimeConfigBuilder {
 public:
  RuntimeConfigBuilder() = default;
  virtual ~RuntimeConfigBuilder() = default;

  RuntimeConfig buildRuntimeConfig(
      const BspTestsConfig& testConfig,
      const PlatformConfig& pmConfig,
      const BspKmodsFile& kmods,
      const std::string& platformName);

 protected:
  std::tuple<std::string, std::string, int> getActualAdapter(
      const PlatformConfig& pmConfig,
      const std::string& sourceUnitName,
      const std::string& sourceBusName,
      const std::string& slotType);

 private:
  fbiob::AuxData createBaseAuxData(
      const FpgaIpBlockConfig& auxDev,
      fbiob::AuxDeviceType deviceType);
  fbiob::AuxData createLedAuxData(const LedCtrlConfig& ledCtrl);
  fbiob::AuxData createSysLedAuxData(const FpgaIpBlockConfig& sysLedCtrl);
  fbiob::AuxData createGpioAuxData(const FpgaIpBlockConfig& gpioChipConf);
  fbiob::AuxData createXcvrAuxData(const XcvrCtrlConfig& xcvrCtrl);
  fbiob::AuxData createFanAuxData(const FanPwmCtrlConfig& fanCtrl);
  fbiob::AuxData createSpiAuxData(const SpiMasterConfig& spiMaster);
  fbiob::AuxData createIdpromAuxData(const IdpromConfig& idpromConfig);
  facebook::fboss::platform::bsp_tests::I2CAdapter* findAdapter(
      std::map<std::string, facebook::fboss::platform::bsp_tests::I2CAdapter>&
          adapters,
      const std::string& unitName,
      const std::string& scopedName);
  std::map<std::string, PmUnitVersion> processIdpromDevices(
      const PlatformConfig& pmConfig,
      std::map<std::string, facebook::fboss::platform::bsp_tests::I2CAdapter>&
          adapters);

  virtual std::optional<PmUnitVersion> getPmunitVersions(
      const std::string& platformName,
      const std::string pmUnitName,
      const I2CAdapter& adapter,
      const I2CDevice& device,
      const IdpromConfig& idpromConfig);

  std::map<std::string, PmUnitConfig> resolvePmUnitConfigs(
      const PlatformConfig& pmConfig,
      std::map<std::string, PmUnitVersion> pmUnitVersions);

  std::optional<std::string> createEepromPathI2C(
      const IdpromConfig& idpromConfig,
      const I2CDevice& i2cDevice,
      const uint16_t i2cBusNum);

  std::optional<std::string> createEepromPathIoctl(
      const IdpromConfig& idpromConfig,
      const uint16_t i2cBusNum);
};

} // namespace facebook::fboss::platform::bsp_tests
