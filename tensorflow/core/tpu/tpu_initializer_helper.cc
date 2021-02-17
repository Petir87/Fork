/* Copyright 2021 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "tensorflow/core/tpu/tpu_initializer_helper.h"

#if defined(LIBTPU_ON_GCE)
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#endif  // LIBTPU_ON_GCE

#include "absl/strings/str_split.h"
#include "absl/synchronization/mutex.h"
#include "tensorflow/core/platform/logging.h"

namespace tensorflow {
namespace tpu {

bool TryAcquireTpuLock() {
#if defined(LIBTPU_ON_GCE)
  static absl::Mutex* mu = new absl::Mutex();
  absl::MutexLock l(mu);

  static bool attempted_file_open = false;
  static bool should_load_library = false;

  if (!attempted_file_open) {
    should_load_library = true;

    // if the TPU_HOST_BOUNDS env var is set, that means we are loading each
    // chip in a different process and thus multiple libtpu loads are OK.
    if (getenv("TPU_HOST_BOUNDS") == nullptr) {
      int fd = open("/tmp/libtpu_lockfile", O_CREAT | O_RDWR);

      // This lock is held until the process exits intentionally. The underlying
      // TPU device will be held on until it quits.
      if (lockf(fd, F_TLOCK, 0) != 0) {
        LOG(WARNING) << "libtpu.so already in used by another process. Not "
                        "attempting to load libtpu.so in this process.";
        should_load_library = false;
      } else {
        should_load_library = true;
      }
    } else {
      LOG(INFO) << "TPU_HOST_BOUNDS is set, allowing multiple libtpu.so loads.";
      should_load_library = true;
    }
  }

  return should_load_library;
#else  // LIBTPU_ON_GCE
  return false;
#endif
}

std::pair<std::vector<std::string>, std::vector<const char*>>
GetLibTpuInitArguments() {
  // We make copies of the arguments returned by getenv because the memory
  // returned may be altered or invalidated by further calls to getenv.
  std::vector<std::string> args;
  std::vector<const char*> arg_ptrs;

  // Retrieve arguments from environment if applicable.
  char* env = getenv("LIBTPU_INIT_ARGS");
  if (env != nullptr) {
    // TODO(frankchn): Handles quotes properly if necessary.
    args = absl::StrSplit(env, ' ');
  }

  arg_ptrs.reserve(args.size());
  for (int i = 0; i < args.size(); ++i) {
    arg_ptrs.push_back(args[i].data());
  }

  return {args, arg_ptrs};
}

}  // namespace tpu
}  // namespace tensorflow
