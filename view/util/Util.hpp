/*
 *  Util.hpp
 *  Created on: Dec 08, 2017
 *      Author: Thomas Wiemann
 *
 *  Copyright (c) 2017 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace asteroids
{

/**
 * @brief Get the extension of a file path
 * @param filename File name or path
 * @return The file extension of \ref filename
 *
 * Examples:
 *
 * "dir/file.jpg" becomes "jpg".
 * "dir/file-without-extension" becomes "" (the empty string).
 */
std::string GetExtensionFromFileName(std::string filename);

/**
 * @brief Removes the last components from a path
 * @param filename Path to process (usually to a file)
 * @return The given path, without the last component (usually a file)
 *
 * Example:
 *
 * "../test/bla.xml" becomes "../test/".
 */
std::string GetPathFromFileName(std::string filename);

} // namespace asteroids


#endif // TEXTUREFACTORY_H
