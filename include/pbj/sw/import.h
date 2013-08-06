///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/sw/import.h
/// \author Benjamin Crist
///
/// \brief  Functions for importing various stuff - fonts, models, etc.

#ifndef PBJ_SW_IMPORT_H_
#define PBJ_SW_IMPORT_H_

#include "pbj/gfx/texture_font.h"

#include <memory>

namespace pbj {
namespace sw {

std::unique_ptr<gfx::TextureFont> parseBmFontXml(const sw::ResourceId& id, const be::ConstHandle<gfx::Texture> texture, const void* data, size_t size);

} // namespace pbj::sw
} // namespace pbj

#endif
