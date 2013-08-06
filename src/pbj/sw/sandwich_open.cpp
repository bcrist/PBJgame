///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/sw/sandwich_open.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::sw::Sandwich non-member functions for
///         opening database files (sandwiches).

#include "pbj/sw/sandwich_open.h"

#include <dirent.h>

#include <iostream>
#include <algorithm>

namespace pbj {
namespace sw {
namespace {

struct SandwichInfo
{
   std::string path;
   std::weak_ptr<Sandwich> sandwich;
};

typedef std::unordered_map<Id, SandwichInfo> sw_map_t;

sw_map_t sandwiches;

SandwichInfo* getSWI(const Id& sandwich_id)
{
   auto i(sandwiches.find(sandwich_id));
   if (i == sandwiches.end())
      return nullptr;

   return &i->second;
}

} // namespace pbj::sw::(anon)

void readDirectory(const std::string& path)
{
    dirent *ent;
                
    DIR* dir = opendir(path.c_str());
    if (dir != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            switch (ent->d_type)
            {
                case DT_REG:
                    {
                        std::string fullpath = path + ent->d_name;
                        std::string extension = fullpath.substr(fullpath.length() - 3, 3);
                        std::transform(extension.begin(), extension.end(), extension.begin(), tolower);

                        if (extension == ".sw") // SW for sandwich (i.e. PB & J Sandwich)
                        {
                            try
                            {
                                Sandwich sw(fullpath, true);
                                Id swid = sw.getId();

                                if (swid == Id())
                                    throw std::runtime_error("Sandwich has no Id!");

                                SandwichInfo swi;
                                swi.path = fullpath;

                                sandwiches[swid] = swi;
                            }
                            catch (const db::Db::error& e)
                            {
                                PBJ_LOG(VWarning) << "Database error while opening sandwich!"  << PBJ_LOG_NL
                                                  << "     Path: " << fullpath << PBJ_LOG_NL
                                                  << "Exception: " << e.what() << PBJ_LOG_NL
                                                  << "      SQL: " << e.sql() << PBJ_LOG_END;
                            }
                            catch (const std::exception& e)
                            {
                                PBJ_LOG(VWarning) << "Exception while opening sandwich!"  << PBJ_LOG_NL
                                                  << "     Path: " << fullpath << PBJ_LOG_NL
                                                  << "Exception: " << e.what() << PBJ_LOG_END;
                            }

                        }
                        break;
                    }

                default:
                    //non-file
                    break;
            }
        }
        closedir(dir);
    }
    else
    {
        /* Could not open directory */
        PBJ_LOG(VWarning) << "Could not open directory!" << PBJ_LOG_NL
                          << "Path: " << path << PBJ_LOG_END;
    }
}

std::vector<Id> getSandwichIds()
{
    std::vector<Id> ids;

    for (auto& swi : sandwiches)
        ids.push_back(swi.first);

    return std::move(ids);
}

std::shared_ptr<Sandwich> open(const Id& id)
{
    SandwichInfo* swi = getSWI(id);
   
    if (!swi)
    {
        PBJ_LOG(VWarning) << "Attempted to open unknown sandwich!" << PBJ_LOG_NL
                          << "Sandwich ID: " << id << PBJ_LOG_END;
        return std::shared_ptr<Sandwich>();
    }
   
    std::shared_ptr<Sandwich> ptr(swi->sandwich.lock());

    if (!ptr)   // previous instance has already been destroyed
    {
        ptr.reset(new Sandwich(swi->path, true));
        swi->sandwich = std::weak_ptr<Sandwich>(ptr);
    }

    return std::move(ptr);
}

std::shared_ptr<Sandwich> openWritable(const Id& id)
{
    SandwichInfo* swi = getSWI(id);

    if (!swi)
    {
        PBJ_LOG(VWarning) << "Attempted to open unknown sandwich!" << PBJ_LOG_NL
                          << "Sandwich ID: " << id << PBJ_LOG_END;
        return std::shared_ptr<Sandwich>();
    }
   
    return std::shared_ptr<Sandwich>(new Sandwich(swi->path, false));
}

} // namespace be::bed
} // namespace be
