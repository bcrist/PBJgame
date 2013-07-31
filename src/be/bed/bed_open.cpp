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
/// \file   be/bed/bed_open.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of be::bed::Bed non-member functions for opening
///         beds.

#include "be/bed/bed_open.h"

#include <iostream>

namespace be {
namespace bed {
namespace {

struct BedInfo
{
   boost::filesystem::path path;
   std::weak_ptr<Bed> bed;
};

typedef std::unordered_map<Id, BedInfo> bed_map_t;

boost::filesystem::path empty_path;
bed_map_t beds;

BedInfo* getBI(const Id& bed_id)
{
   auto i(beds.find(bed_id));
   if (i == beds.end())
      return nullptr;

   return &i->second;
}

} // namespace be::bed::(anon)

///////////////////////////////////////////////////////////////////////////////
/// \brief  Specifies that the provided bed ID should represent the SQLite
///         database located at the provided path.
///
/// \details If the provided path contains no filename portion, a warning will
///         be emitted and false will be returned.  If the bed ID has already
///         been specified using a different path, the old path will remain the
///         canonical path, a warning will be emitted, and false will be
///         returned.
///
/// \param  bed_id The Id we are specifying.
/// \param  path The path to the database file of this bed.
/// \return \c true if the bed Id is now associated with the provided path.
bool specify(const Id& bed_id, const boost::filesystem::path& path)
{
   try
   {
      if (!path.has_filename())
      {
         BE_LOG(VWarning) << "Could not specify bed: path has no filename!" << BE_LOG_NL
                          << "            ID: " << bed_id << BE_LOG_NL
                          << "Specified Path: " << path << BE_LOG_END;
         return false;
      }
      

      auto insert_result = beds.insert(std::make_pair(bed_id, BedInfo()));
      boost::filesystem::path& map_path = insert_result.first->second.path;

      if (insert_result.second)
      {
         // the insertion was successful
         map_path = path;
         map_path.make_preferred();
      }
      else
      {
         // the entry already existed in the map
         boost::filesystem::path preferred(path);
         preferred.make_preferred();

         if (map_path != preferred)
         {
            BE_LOG(VWarning) << "Bed ID/path collision!" << BE_LOG_NL
                             << "            ID: " << bed_id << BE_LOG_NL
                             << " Existing Path: " << map_path << BE_LOG_NL
                             << "Specified Path: " << preferred << BE_LOG_END;
            return false;
         }
      }
   }
   catch (const boost::filesystem::filesystem_error& ex)
   {
      BE_LOG(VWarning) << "Exception specifying bed path " << path << BE_LOG_NL
                       << "            ID: " << bed_id << BE_LOG_NL
                       << "Specified Path: " << path << BE_LOG_NL
                       << "     Exception: " << ex.what() << BE_LOG_END;
      return false;
   }
   return true;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Automatically calculates a bed ID for the given database path and
///         specifies that that ID should be associated with that path.
///
/// \details If the specification fails, a warning will be emitted and a
///         \c std::runtime_error will be thrown.
///
///         The ID is calculated by hashing the path string.  Windows paths
///         (using '\' as a directory separator) will be converted to generic
///         paths (using '/') before hashing, but the system's preferred
///         directory separator will still be used when opening the bed.
///         Relative paths are not canonicalized, so the use of '.' and '..'
///         elements should be avoided if possible, as they may result in
///         multiple IDs referring to the same database file.
///
/// \param  path The path to the database file of this bed.
/// \return The ID calculated for the given path.
Id specify(const boost::filesystem::path& path)
{
   Id id(path.generic_string());
   if (specify(id, path))
      return id;
   throw std::runtime_error("Could not specify bed path!");
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Scans through all items in the provided directory path and
///         specifies an ID for each item that is not a directory and whose
///         extension is ".bed".
///
/// \details The scan is not recursive.
///
///         The ID is calculated in the same manner as
///         specify(const boost::filesystem::path& path), but no exceptions are
///         thrown if the specification fails (but a warning may still be
///         emitted).
///
///         If the path specified does not exist, directory() will attempt to
///         create it.  If the directory cannot be created, or it already
///         exists but is not a directory, or a filesystem error occurs, a
///         warning will be emitted.
///
/// \param  path The path to the directory to scan.
void directory(const boost::filesystem::path& path)
{
   try
   {
      if (boost::filesystem::exists(path) || boost::filesystem::create_directories(path))
      {
         if (boost::filesystem::is_directory(path))
         {
            for (boost::filesystem::directory_iterator i(path), end; i != end; ++i)
            {
               boost::filesystem::path path = i->path();

               if (!is_directory(path) && path.extension() == ".bed")
                  specify(Id(path.generic_string()), path);
            }
         }
         else
            BE_LOG(VWarning) << "Can't scan directory: path is not a directory!" << BE_LOG_NL
                             << "Specified Path: " << path << BE_LOG_END;
      }
      else
         BE_LOG(VWarning) << "Can't scan directory: directory couldn't be created!" << BE_LOG_NL
                          << "Specified Path: " << path << BE_LOG_END;
   }
   catch (const boost::filesystem::filesystem_error& ex)
   {
      BE_LOG(VWarning) << "Exception scanning directory!" << BE_LOG_NL
                       << "Specified Path: " << path << BE_LOG_NL
                       << "     Exception: " << ex.what() << BE_LOG_END;
   }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the path that would be used to construct a bed when
///         open(const Id& bed_id) is called.
///
/// \details If no path has been specified for the provided ID, the path object
///         returned will be empty.
///
/// \param  bed_id The Id of the bed to get the path of.
/// \return The path of the database file associated with the bed.
const boost::filesystem::path& getPath(const Id& bed_id)
{
   auto i(beds.find(bed_id));
   if (i == beds.end())
      return empty_path;

   return i->second.path;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves a container mapping bed IDs to the path which has been
///         specified for that ID.
///
/// \details This should generally only be used for debugging purposes, as it
///         creates the entire unordered_map from data stored in an internal
///         data structure every time it is called.
///
/// \return A map of all currently specified Ids and the paths they correspond
///         to.
std::unordered_map<Id, boost::filesystem::path> getPaths()
{
   std::unordered_map<Id, boost::filesystem::path> paths;
   for (auto i(beds.begin()), end(beds.end()); i != end; ++i)
      paths[i->first] = i->second.path;

   return std::move(paths);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves a shared_ptr to a read-only bed object that was
///         constructed from the path associated with the provided ID.
///
/// \details Multiple calls to open() using the same ID will return shared_ptrs
///         to the same object (assuming the shared_ptr is not invalidated in
///         between calls).
///
///         A path must be specified for a particular ID before that bed can be
///         opened.  If no path has been specified for the requested ID, a
///         warning will be emitted and an empty shared_ptr will be returned.
///
/// \param  bed_id The Id of the pre-specified bed.
/// \return A shared_ptr to the open read-only bed.
std::shared_ptr<Bed> open(const Id& bed_id)
{
   BedInfo* bi = getBI(bed_id);
   
   if (!bi)
   {
      BE_LOG(VWarning) << "Attempted to open unspecified bed!" << BE_LOG_NL
                       << "ID: " << bed_id << BE_LOG_END;
      return std::shared_ptr<Bed>();
   }
   
   std::shared_ptr<Bed> ptr(bi->bed.lock());

   if (!ptr)   // previous instance has already been destroyed
   {
      ptr.reset(new Bed(bed_id, bi->path.string(), true));
      bi->bed = std::weak_ptr<Bed>(ptr);
   }

   return std::move(ptr);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves a shared_ptr to a writable bed object constructed from
///         the path associated with the provided ID.
///
/// \details Multiple calls to openWritable() using the same ID will always
///         return shared_ptrs to different bed objects.
///
///         A path must be specified for a particular ID before that bed can be
///         opened.  If no path has been specified for the requested ID, a
///         warning will be emitted and an empty shared_ptr will be returned.
///
/// \param  bed_id The Id of the pre-specified bed.
/// \return A shared_ptr to the open bed.
std::shared_ptr<Bed> openWritable(const Id& bed_id)
{
   BedInfo* bi = getBI(bed_id);

   if (!bi)
   {
      BE_LOG(VWarning) << "Attempted to open unspecified bed!" << BE_LOG_NL
                       << "ID: " << bed_id << BE_LOG_END;
      return std::shared_ptr<Bed>();
   }
   
   return std::shared_ptr<Bed>(new Bed(bed_id, bi->path.string(), false));
}

} // namespace be::bed
} // namespace be
