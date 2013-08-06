///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/sw/sandwich_open.h
/// \author Benjamin Crist
///
/// \brief  pbj::sw::Sandwich non-member functions.

#ifndef PBJ_SW_SANDWICH_OPEN_H_
#define PBJ_SW_SANDWICH_OPEN_H_

#include "pbj/sw/sandwich.h"

#include <unordered_map>

namespace pbj {
namespace sw {

void readDirectory(const std::string& path);

std::vector<Id> getSandwichIds();

std::shared_ptr<Sandwich> open(const Id& id);
std::shared_ptr<Sandwich> openWritable(const Id& id);

} // namespace pbj::sw
} // namespace pbj

#endif
