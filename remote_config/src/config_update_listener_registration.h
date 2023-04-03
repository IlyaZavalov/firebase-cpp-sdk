// Copyright 2023 Google LLC
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

#ifndef FIREBASE_REMOTE_CONFIG_SRC_LISTENER_REGISTRATION_H_
#define FIREBASE_REMOTE_CONFIG_SRC_LISTENER_REGISTRATION_H_

#include "include/firebase/remote_config.h"

namespace firebase {
namespace remote_config {

//  Calling Remove stops the listener from receiving
//  config updates and unregisters itself. If remove is called and no other
//  listener registrations remain, the connection to the Remote Config backend
//  is closed. Subsequently calling AddOnConfigUpdate will re-open the
//  connection.
class ConfigUpdateListenerRegistration {
 public:
  ConfigUpdateListenerRegistration(
      std::function<void(ConfigUpdate&&, RemoteConfigError)>
          config_update_listener,
      RemoteConfigInternal remoteConfigInternal);

  ~ConfigUpdateListenerRegistration();

  // Removes the listener being tracked by this
  // ConfigUpdateListenerRegistration. After the initial call, subsequent calls
  // to Remove have no effect.
  void Remove();

 private:
  std::function<void(ConfigUpdate&&, RemoteConfigError)> config_update_listener;
  RemoteConfigInternal remoteConfigInternal;
};

}  // namespace remote_config
}  // namespace firebase

#endif  // FIREBASE_REMOTE_CONFIG_SRC_LISTENER_REGISTRATION_H_