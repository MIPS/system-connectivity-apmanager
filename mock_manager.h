//
// Copyright (C) 2014 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef APMANAGER_MOCK_MANAGER_H_
#define APMANAGER_MOCK_MANAGER_H_

#include <string>

#include <base/macros.h>
#include <gmock/gmock.h>

#include "apmanager/manager.h"

namespace apmanager {

class MockManager : public Manager {
 public:
  MockManager();
  ~MockManager() override;

  MOCK_METHOD0(Start, void());
  MOCK_METHOD0(Stop, void());
  MOCK_METHOD1(RegisterDevice, void(scoped_refptr<Device> device));
  MOCK_METHOD0(GetAvailableDevice, scoped_refptr<Device>());
  MOCK_METHOD1(GetDeviceFromInterfaceName,
               scoped_refptr<Device>(const std::string& interface_name));
  MOCK_METHOD1(ClaimInterface, void(const std::string& interface_name));
  MOCK_METHOD1(ReleaseInterface, void(const std::string& interface_name));
  MOCK_METHOD1(RequestDHCPPortAccess, void(const std::string& interface));
  MOCK_METHOD1(ReleaseDHCPPortAccess, void(const std::string& interface));

 private:
  DISALLOW_COPY_AND_ASSIGN(MockManager);
};

}  // namespace apmanager

#endif  // APMANAGER_MOCK_MANAGER_H_
