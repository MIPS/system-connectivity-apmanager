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

#ifndef APMANAGER_PROCESS_FACTORY_H_
#define APMANAGER_PROCESS_FACTORY_H_

#include <string>

#include <base/lazy_instance.h>

#include <brillo/process.h>

namespace apmanager {

class ProcessFactory {
 public:
  virtual ~ProcessFactory();

  // This is a singleton. Use ProcessFactory::GetInstance()->Foo().
  static ProcessFactory* GetInstance();

  virtual brillo::Process* CreateProcess();

 protected:
  ProcessFactory();

 private:
  friend struct base::DefaultLazyInstanceTraits<ProcessFactory>;

  DISALLOW_COPY_AND_ASSIGN(ProcessFactory);
};

}  // namespace apmanager

#endif  // APMANAGER_PROCESS_FACTORY_H_
