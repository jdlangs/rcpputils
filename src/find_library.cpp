// Copyright 2019 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "rcpputils/find_library.hpp"

#include <cassert>
#include <cstddef>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>

#include "rcutils/filesystem.h"
#include "rcutils/get_env.h"

#include "rcpputils/split.hpp"
#include "rcpputils/get_env.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

namespace rcpputils
{

namespace
{

#ifdef _WIN32
static constexpr char kPathVar[] = "PATH";
static constexpr char kPathSeparator = ';';
static constexpr char kSolibPrefix[] = "";
static constexpr char kSolibExtension[] = ".dll";
#elif __APPLE__
static constexpr char kPathVar[] = "DYLD_LIBRARY_PATH";
static constexpr char kPathSeparator = ':';
static constexpr char kSolibPrefix[] = "lib";
static constexpr char kSolibExtension[] = ".dylib";
#else
static constexpr char kPathVar[] = "LD_LIBRARY_PATH";
static constexpr char kPathSeparator = ':';
static constexpr char kSolibPrefix[] = "lib";
static constexpr char kSolibExtension[] = ".so";
#endif

}  // namespace

std::string find_library_path(const std::string & library_name)
{
  std::string search_path = get_env_var(kPathVar);
  std::vector<std::string> search_paths = rcpputils::split(search_path, kPathSeparator);

  #ifdef _WIN32
  // Support current directory to enable containerized deployments, like Hololens.
  DWORD len = GetCurrentDirectoryA(0, NULL) + 1;  // How much space is needed for current path?
  char* currentDir = new char[len];
  if (currentDir) {
    if (GetCurrentDirectoryA(len, currentDir) > 0) {
      search_paths.insert(search_paths.begin(), currentDir);
    }

    delete [] currentDir; // no longer needed.
  }
  #endif

  std::string filename = kSolibPrefix;
  filename += library_name + kSolibExtension;

  for (const auto & search_path : search_paths) {
    auto path = std::filesystem::path(search_path) / filename;
    if (rcutils_is_file(path.string().c_str())) {
      return path.string();
    }
  }
  return "";
}

}  // namespace rcpputils
