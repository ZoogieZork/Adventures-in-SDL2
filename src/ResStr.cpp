
/* ResStr.cpp
 *
 * Copyright (C) 2013 Michael Imamura
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

#include "StdAfx.h"

#include <fstream>

#include "Exception.h"

#include "ResStr.h"

namespace AISDL {

ResStr::ResStr(const std::string &filename) :
	filename(filename)
{
	Reload();
}

/**
 * Create a new resource string.
 * @param filename The file to load the string from.
 * @return The resource string.
 */
std::shared_ptr<ResStr> ResStr::Load(const std::string &filename)
{
	//TODO: Keep weak_ptrs to returned assets for ReloadAll().
	return std::make_shared<ResStr>(filename);
}

/**
 * Reload the string from the file it came from.
 */
void ResStr::Reload()
{
	// Fast byte-for-byte string read.
	// Adapted from:
	// http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in) {
		in.seekg(0, std::ios::end);
		s.resize(static_cast<std::string::size_type>(in.tellg()));
		in.seekg(0, std::ios::beg);
		in.read(&s[0], s.size());
		in.close();
	}
	else {
		throw Exception("Failed to read: " + filename);
	}
}

}  // namespace AISDL

