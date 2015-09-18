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

#include "apmanager/shill_manager.h"

#include <base/bind.h>
#include <chromeos/dbus/service_constants.h>
#include <chromeos/errors/error.h>

using std::string;

namespace apmanager {

ShillManager::ShillManager() {}

ShillManager::~ShillManager() {}

void ShillManager::Init(const scoped_refptr<dbus::Bus>& bus) {
  CHECK(!manager_proxy_) << "Already init";
  manager_proxy_.reset(new org::chromium::flimflam::ManagerProxy(bus));
  // This will connect the name owner changed signal in DBus object proxy,
  // The callback will be invoked as soon as service is avalilable. and will
  // be cleared after it is invoked. So this will be an one time callback.
  manager_proxy_->GetObjectProxy()->WaitForServiceToBeAvailable(
      base::Bind(&ShillManager::OnServiceAvailable, base::Unretained(this)));
  // This will continuously monitor the name owner of the service. However,
  // it does not connect the name owner changed signal in DBus object proxy
  // for some reason. In order to connect the name owner changed signal,
  // either WaitForServiceToBeAvaiable or ConnectToSignal need to be invoked.
  // Since we're not interested in any signals from Shill proxy,
  // WaitForServiceToBeAvailable is used.
  manager_proxy_->GetObjectProxy()->SetNameOwnerChangedCallback(
      base::Bind(&ShillManager::OnServiceNameChanged, base::Unretained(this)));
}

void ShillManager::ClaimInterface(const string& interface_name) {
  CHECK(manager_proxy_) << "Proxy not initialize yet";
  chromeos::ErrorPtr error;
  if (!manager_proxy_->ClaimInterface(kServiceName, interface_name, &error)) {
    // Ignore unknown object error (when shill is not running). Only report
    // internal error from shill.
    if (error->GetCode() != DBUS_ERROR_UNKNOWN_OBJECT) {
      LOG(ERROR) << "Failed to claim interface from shill: "
                 << error->GetCode() << " " << error->GetMessage();
    }
  }
  claimed_interfaces_.insert(interface_name);
}

void ShillManager::ReleaseInterface(const string& interface_name) {
  CHECK(manager_proxy_) << "Proxy not initialize yet";
  chromeos::ErrorPtr error;
  if (!manager_proxy_->ReleaseInterface(kServiceName, interface_name, &error)) {
    // Ignore unknown object error (when shill is not running). Only report
    // internal error from shill.
    if (error->GetCode() != DBUS_ERROR_UNKNOWN_OBJECT) {
      LOG(ERROR) << "Failed to release interface from shill: "
                 << error->GetCode() << " " << error->GetMessage();
    }
  }
  claimed_interfaces_.erase(interface_name);
}

void ShillManager::OnServiceAvailable(bool service_available) {
  LOG(INFO) << "OnServiceAvailabe " << service_available;
  // Nothing to be done if proxy service not available.
  if (!service_available) {
    return;
  }
  // Claim all interfaces from shill DBus service in case this is a new
  // instance.
  for (const auto& interface : claimed_interfaces_) {
    chromeos::ErrorPtr error;
    if (!manager_proxy_->ClaimInterface(kServiceName, interface, &error)) {
      LOG(ERROR) << "Failed to claim interface from shill: "
                 << error->GetCode() << " " << error->GetMessage();
    }
  }
}

void ShillManager::OnServiceNameChanged(const string& old_owner,
                                         const string& new_owner) {
  LOG(INFO) << "OnServiceNameChanged old " << old_owner
            << " new " << new_owner;
  // Nothing to be done if no owner is attached to the shill service.
  if (new_owner.empty()) {
    return;
  }
  OnServiceAvailable(true);
}

}  // namespace apmanager
