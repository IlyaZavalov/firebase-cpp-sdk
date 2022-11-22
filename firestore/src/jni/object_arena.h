/*
 * Copyright 2022 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FIREBASE_FIRESTORE_SRC_JNI_OBJECT_ARENA_H_
#define FIREBASE_FIRESTORE_SRC_JNI_OBJECT_ARENA_H_

#include "firestore/src/jni/object.h"

namespace firebase {
namespace firestore {
namespace jni {

/** `ObjectArena` serves as a local HashMap. */
class ObjectArena : public Object {
 public:
  using Object::Object;

  static void Initialize(Env& env, Loader& loader);

  static ObjectArena& GetInstance();

  Local<Object> Get(Env& env, int64_t key);
  int64_t Put(Env& env, const Object& value);
  void Remove(Env& env, int64_t key);
  int64_t Dup(Env& env, int64_t key);

 private:
  int64_t next_key_ = 0;
};

}  // namespace jni
}  // namespace firestore
}  // namespace firebase

#endif  // FIREBASE_FIRESTORE_SRC_JNI_OBJECT_ARENA_H_