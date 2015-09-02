//
// Copyright (C) 2015 The Android Open Source Project
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

#ifndef APMANAGER_FIREWALL_MANAGER_H_
#define APMANAGER_FIREWALL_MANAGER_H_

#include <set>
#include <string>

#include <base/macros.h>
#include <base/memory/scoped_ptr.h>

#include "permission_broker/dbus-proxies.h"

// Class for managing required firewall rules for apmanager.
namespace apmanager {

class FirewallManager final {
 public:
  FirewallManager();
  ~FirewallManager();

  void Init(const scoped_refptr<dbus::Bus>& bus);

  // Request/release DHCP port access for the specified interface.
  void RequestDHCPPortAccess(const std::string& interface);
  void ReleaseDHCPPortAccess(const std::string& interface);

 private:
  // Setup lifeline pipe to allow the remote firewall server
  // (permission_broker) to monitor this process, so it can remove the firewall
  // rules in case this process crashes.
  bool SetupLifelinePipe();

  void OnServiceAvailable(bool service_available);
  void OnServiceNameChanged(const std::string& old_owner,
                            const std::string& new_owner);

  // This is called when a new instance of permission_broker is detected. Since
  // the new instance doesn't have any knowledge of previously port access
  // requests, re-issue those requests to permission_broker to get in sync.
  void RequestAllPortsAccess();

  // Request/release UDP port access for the specified interface and port.
  void RequestUdpPortAccess(const std::string& interface, uint16_t port);
  void ReleaseUdpPortAccess(const std::string& interface, uint16_t port);

  // DBus proxy for permission_broker.
  std::unique_ptr<org::chromium::PermissionBrokerProxy>
      permission_broker_proxy_;
  // File descriptors for the two end of the pipe use for communicating with
  // remote firewall server (permission_broker), where the remote firewall
  // server will use the read end of the pipe to detect when this process exits.
  int lifeline_read_fd_;
  int lifeline_write_fd_;

  // List of interfaces with DHCP port access.
  std::set<std::string> dhcp_access_interfaces_;

  DISALLOW_COPY_AND_ASSIGN(FirewallManager);
};

}  // namespace apmanager

#endif  // APMANAGER_FIREWALL_MANAGER_H_
