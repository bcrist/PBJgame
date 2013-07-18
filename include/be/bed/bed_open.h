// Copyright (c) 2013 Benjamin Crist
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

///////////////////////////////////////////////////////////////////////////////
/// \file   be/bed/bed_open.h
/// \author Benjamin Crist
///
/// \brief  be::bed::Bed non-member functions.
/// \details These are separated from bed.h to reduce compile time since many
/// utilize \c boost::filesystem headers and they are not needed for most usage
/// of beds (after they are opened).

#ifndef BE_BED_BED_OPEN_H_
#define BE_BED_BED_OPEN_H_

#include "be/bed/bed.h"

#include <boost/filesystem.hpp>

#include <unordered_map>

namespace be {
namespace bed {

bool specify(const Id& bed_id, const boost::filesystem::path& path);
Id specify(const boost::filesystem::path& path);
void directory(const boost::filesystem::path& path);

const boost::filesystem::path& getPath(const Id& bed_id);

std::unordered_map<Id, boost::filesystem::path> getPaths();

std::shared_ptr<Bed> open(const Id& bed_id);
std::shared_ptr<Bed> openWritable(const Id& bed_id);

} // namespace bed
} // namespace be

#endif
