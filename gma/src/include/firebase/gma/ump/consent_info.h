/*
 * Copyright 2023 Google LLC
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

#ifndef FIREBASE_GMA_SRC_INCLUDE_FIREBASE_GMA_UMP_CONSENT_INFO_H_
#define FIREBASE_GMA_SRC_INCLUDE_FIREBASE_GMA_UMP_CONSENT_INFO_H_

#include "firebase/app.h"
#include "firebase/future.h"
#include "firebase/gma/ump/types.h"
#include "firebase/internal/platform.h"

#if FIREBASE_PLATFORM_ANDROID
#include <jni.h>
#endif  // FIREBASE_PLATFORM_ANDROID

namespace firebase {
namespace gma {
/// @brief API for User Messaging Platform.
///
/// The User Messaging Platform (UMP) SDK is Google’s option to handle user
/// privacy and consent in mobile apps.
namespace ump {

namespace internal {
// Forward declaration for platform-specific data, implemented in each library.
class ConsentInfoInternal;
}  // namespace internal

/// This class contains all of the methods necessary for obtaining
/// consent from the user.
class ConsentInfo {
 public:
  /// Shut down the User Messaging Platform Consent SDK
  ~ConsentInfo();

  /// Initializes the User Messaging Platform Consent SDK.
  ///
  /// @param[in] app Any Firebase App instance.
  ///
  /// @param[out] init_result_out Optional: If provided, write the basic init
  /// result here. kInitResultSuccess if initialization succeeded, or
  /// kInitResultFailedMissingDependency on Android if there are Android
  /// dependencies missing.
  ///
  /// @return A pointer to the ConsentInfo instance if UMP was successfully
  /// initialized, nullptr otherwise. Each call to GetInstance() will return the
  /// same pointer; when you are finished using the SDK, you can delete the
  /// pointer and the UMP SDK will shut down.
  static ConsentInfo* GetInstance(const ::firebase::App& app,
                                  InitResult* init_result_out = nullptr);

#if FIREBASE_PLATFORM_ANDROID || defined(DOXYGEN)
  /// Initializes the User Messaging Platform Consent SDK without Firebase for
  /// Android.
  ///
  /// The arguments to GetInstance() are platform-specific so the caller must
  /// do something like this:
  /// @code
  /// #if defined(__ANDROID__)
  /// consent_info = firebase::gma::ump::ConsentInfo::GetInstance(jni_env,
  /// activity); #else consent_info = firebase::gma::ump::GetInstance(); #endif
  /// @endcode
  ///
  /// @param[in] jni_env JNIEnv pointer.
  /// @param[in] activity Activity used to start the application.
  /// @param[out] init_result_out Optional: If provided, write the basic init
  /// result here. kInitResultSuccess if initialization succeeded, or
  /// kInitResultFailedMissingDependency on Android if there are Android
  /// dependencies missing.
  ///
  /// @return A pointer to the ConsentInfo instance if UMP was successfully
  /// initialized, nullptr otherwise. Each call to GetInstance() will return the
  /// same pointer; when you are finished using the SDK, you can delete the
  /// pointer and the UMP SDK will shut down.
  static ConsentInfo* GetInstance(JNIEnv* jni_env, jobject activity,
                                  InitResult* init_result_out = nullptr);

#if !defined(DOXYGEN)
  // On Android, this convenience function exists so you can easily get the
  // existing ConsentInfo instance after it's first initialized. Returns nullptr
  // if no instance has been created yet; make sure you have called
  // GetInstance(JNIEnv*, jobject) first.
  static ConsentInfo* GetInstance();
#endif  // defined(DOXYGEN)
#endif  // FIREBASE_PLATFORM_ANDROID || defined(DOXYGEN)

#if !FIREBASE_PLATFORM_ANDROID || defined(DOXYGEN)
  /// Initializes User Messaging Platform for iOS without Firebase.
  ///
  /// @param[out] init_result_out Optional: If provided, write the basic init
  /// result here. kInitResultSuccess if initialization succeeded, or
  /// kInitResultFailedMissingDependency if a dependency is missing. On iOS,
  /// this will always return kInitResultSuccess, as missing dependencies would
  /// have caused a linker error at build time.
  ///
  /// @return A pointer to the ConsentInfo instance. Each call to GetInstance()
  /// will return the same pointer; when you are finished using the SDK, you can
  /// delete the pointer, and the UMP SDK will shut down.
  static ConsentInfo* GetInstance(InitResult* init_result_out = nullptr);
#endif  // !defined(__ANDROID__) || defined(DOXYGEN)

  /// The user’s consent status. This value is cached between app sessions and
  /// can be read before calling RequestConsentInfoUpdate().
  ConsentStatus GetConsentStatus();

  /// Requests consent information update. Must be called before loading a
  /// consent form. If successful, GetConsentStatus() will be updated.
  Future<void> RequestConsentInfoUpdate(const ConsentRequestParameters& params);

  /// Get the Future from the most recent call to RequestConsentInfoUpdate().
  Future<void> RequestConsentInfoUpdateLastResult();

  /// Consent form status. This value defaults to kConsentFormStatusUnknown and
  /// requires a call to RequestConsentInfoUpdate() to update.
  ConsentFormStatus GetConsentFormStatus();

  /// Loads a consent form. If successful, GetConsentFormStatus() will be
  /// updated.
  Future<void> LoadConsentForm();

  /// Get the Future from the most recent call to LoadConsentForm().
  Future<void> LoadConsentFormLastResult();

  /// Presents the full screen consent form using the given FormParent, which is
  /// defined as an Activity on Android and a UIViewController on iOS. The form
  /// will be dismissed and the Future will be completed after the user selects
  /// an option. GetConsentStatus() is updated when the Future is completed.
  Future<void> ShowConsentForm(FormParent parent);

  /// Get the Future from the most recent call to ShowConsentForm().
  Future<void> ShowConsentFormLastResult();

  /// If required, load and then show the consent form. You can call this
  /// instead of LoadConsentForm() and ShowConsentForm().
  Future<void> LoadAndShowConsentFormIfRequired(FormParent parent);

  /// Get the Future from the most recent call to
  /// LoadAndShowConsentFormIfRequired().
  Future<void> LoadAndShowConsentFormIfRequiredLastResult();

  /// Check whether the privacy options form needs to be displayed.
  /// This is updated by RequestConsentStatus().
  PrivacyOptionsRequirementStatus GetPrivacyOptionsRequirementStatus();

  /// If needed, show the privacy options form to the user. This allows them to
  /// revoke their consent.
  Future<void> ShowPrivacyOptionsForm(FormParent parent);

  /// Get the Future from the most recent call to ShowPrivacyOptionsForm().
  Future<void> ShowPrivacyOptionsFormLastResult();

  /// If this returns true, it is now safe to request ads. If not, do not show
  /// ads to the user. This is updated by RequestConsentStatus().
  bool CanRequestAds();

  /// Clears all consent state from persistent storage. This can be used in
  /// development to simulate a new installation.
  void Reset();

 private:
  ConsentInfo();
#if FIREBASE_PLATFORM_ANDROID
  // TODO(b/291622888) Implement Android-specific Initialize..
  InitResult Initialize(/* JNIEnv* jni_env, jobject activity */);
#else
  InitResult Initialize();
#endif  // FIREBASE_PLATFORM_ANDROID
  void Terminate();

  static ConsentInfo* s_instance_;

#if FIREBASE_PLATFORM_ANDROID
  JavaVM* java_vm() { return java_vm_; }
  JavaVM* java_vm_;
#endif

  // An internal, platform-specific implementation object that this class uses
  // to interact with the User Messaging Platform SDKs for iOS and Android.
  internal::ConsentInfoInternal* internal_;
};

}  // namespace ump
}  // namespace gma
}  // namespace firebase

#endif  // FIREBASE_GMA_SRC_INCLUDE_FIREBASE_GMA_UMP_CONSENT_INFO_H_