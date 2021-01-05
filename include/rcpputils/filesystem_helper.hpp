// Copyright (c) 2019, Open Source Robotics Foundation, Inc.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the copyright holder nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

// This file is originally from:
// https://github.com/ros/pluginlib/blob/1a4de29fa55173e9b897ca8ff57ebc88c047e0b3/pluginlib/include/pluginlib/impl/filesystem_helper.hpp

/*! \file filesystem_helper.hpp
 * \brief Cross-platform filesystem helper functions and additional emulation of [std::filesystem](https://en.cppreference.com/w/cpp/filesystem).
 *
 * Note: The functionality here is implemented as a thin wrapper around a 3rd-party header library
 * to provide equivalent std::filesystem functionality (see 'ghc/filesystem.hpp' in this directory).
 * Once std::filesystem is supported on all ROS2 platforms, this class can be deprecated/removed in
 * favor of the built-in functionality.
 */

#ifndef RCPPUTILS__FILESYSTEM_HELPER_HPP_
#define RCPPUTILS__FILESYSTEM_HELPER_HPP_

#include <filesystem>

namespace rcpputils
{
namespace fs
{

/**
 * \brief Drop-in replacement for [std::filesystem::path](https://en.cppreference.com/w/cpp/filesystem/path).
 *
 * It must conform to the same standard described and cannot include methods that are not
 * incorporated there.
 */
using path = std::filesystem::path;

/**
 * \brief Check if the path is a regular file.
 *
 * \param p The path to check
 * \return True if the path exists, false otherwise.
 */
inline bool is_regular_file(const path & p) noexcept
{
  return std::filesystem::is_regular_file(p);
}

/**
 * \brief Check if the path is a directory.
 *
 * \param p The path to check
 * \return True if the path is an existing directory, false otherwise.
 */
inline bool is_directory(const path & p) noexcept
{
  return std::filesystem::is_directory(p);
}

/**
 * \brief Get the file size of the path.
 *
 * \param p The path to get the file size of.
 * \return The file size in bytes.
 *
 * \throws std::sytem_error
 */
inline uint64_t file_size(const path & p)
{
  return std::filesystem::file_size(p);
}

/**
 * \brief Check if a path exists.
 *
 * \param path_to_check The path to check.
 * \return True if the path exists, false otherwise.
 */
inline bool exists(const path & path_to_check)
{
  return std::filesystem::exists(path_to_check);
}


/**
 * \brief Get a path to a location in the temporary directory, if it's available.
 *
 * \return A path to a directory for storing temporary files and directories.
 */
inline path temp_directory_path()
{
  return std::filesystem::temp_directory_path();
}

/**
 * \brief Return current working directory.
 *
 * \return The current working directory.
 *
 * \throws std::system_error
 */
inline path current_path()
{
  return std::filesystem::current_path();
}

/**
 * \brief Create a directory with the given path p.
 *
 * This builds directories recursively and will skip directories if they are already created.
 * \return Return true if the directory is created, false otherwise.
 */
inline bool create_directories(const path & p)
{
  return std::filesystem::create_directories(p);
}

/**
 * \brief Remove the file or directory at the path p.
 *
 * \param p The path of the object to remove.
 * \return true if the file exists and it was successfully removed, false otherwise.
 */
inline bool remove(const path & p)
{
  return std::filesystem::remove(p);
}

/**
 * \brief Remove the directory at the path p and its content.
 *
 * Additionally to \sa remove, remove_all removes a directory and its containing files.
 *
 * \param The path of the directory to remove.
 * \return true if the directory exists and it was successfully removed, false otherwise.
 */
inline bool remove_all(const path & p)
{
  return std::filesystem::remove_all(p);
}

/**
 * \brief Remove extension(s) from a path.
 *
 * An extension is defined as text starting from the end of a path to the first period (.) character.
 *
 * \param file_path The file path string.
 * \param n_times The number of extensions to remove if there are multiple extensions.
 * \return The path object.
 */
inline path remove_extension(const path & file_path, int n_times = 1)
{
  path new_path(file_path);
  for (int i = 0; i < n_times; i++) {
    const auto new_path_str = new_path.string();
    const auto last_dot = new_path_str.find_last_of('.');
    if (last_dot == std::string::npos) {
      return new_path;
    }
    new_path = path(new_path_str.substr(0, last_dot));
  }
  return new_path;
}

}  // namespace fs
}  // namespace rcpputils

#endif  // RCPPUTILS__FILESYSTEM_HELPER_HPP_
