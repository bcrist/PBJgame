///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/sw/import.cpp
/// \author Benjamin Crist
///
/// \brief  Functions for importing various stuff - fonts, models, etc.

#include "pbj/sw/import.h"

#include "pugixml.hpp"

#include <iostream>

namespace pbj {
namespace sw {

std::unique_ptr<gfx::TextureFont> parseBmFontXml(const sw::ResourceId& id, const be::ConstHandle<gfx::Texture> texture, const void* data, size_t size)
{
    pugi::xml_document doc;

    pugi::xml_parse_result result = doc.load_buffer(data, size);

    if (result.status != pugi::xml_parse_status::status_ok)
    {
        std::string status;
        switch (result.status)
        {
            case pugi::xml_parse_status::status_bad_attribute:          status = "bad_attribute"; break;
            case pugi::xml_parse_status::status_bad_cdata:              status = "bad_cdata"; break;
            case pugi::xml_parse_status::status_bad_comment:            status = "bad_comment"; break;
            case pugi::xml_parse_status::status_bad_doctype:            status = "bad_doctype"; break;
            case pugi::xml_parse_status::status_bad_end_element:        status = "bad_end_element"; break;
            case pugi::xml_parse_status::status_bad_pcdata:             status = "bad_pcdata"; break;
            case pugi::xml_parse_status::status_bad_pi:                 status = "bad_pi"; break;
            case pugi::xml_parse_status::status_bad_start_element:      status = "bad_start_element"; break;
            case pugi::xml_parse_status::status_end_element_mismatch:   status = "end_element_mismatch"; break;
            case pugi::xml_parse_status::status_file_not_found:         status = "file_not_found"; break;
            case pugi::xml_parse_status::status_internal_error:         status = "internal_error"; break;
            case pugi::xml_parse_status::status_io_error:               status = "io_error"; break;
            case pugi::xml_parse_status::status_out_of_memory:          status = "out_of_memory"; break;
            case pugi::xml_parse_status::status_unrecognized_tag:       status = "unrecognized_tag"; break;
            default:                                                    status = std::to_string(result.status); break;
        }

        PBJ_LOG(VWarning) << "Failed to load XML BMFont!" << PBJ_LOG_NL
                          << "     Pugi Status: " << status << PBJ_LOG_NL
                          << "Pugi Description: " << result.description() << PBJ_LOG_END;

        throw std::runtime_error("Could not load BMFont XML spec.");
    }

    pugi::xml_node el_font = doc.document_element();
    pugi::xml_node el_common = el_font.child("common");
    pugi::xml_node el_chars = el_font.child("chars");

    pugi::xml_attribute at_lineHeight = el_common.attribute("lineHeight");
    pugi::xml_attribute at_base = el_common.attribute("base");
    pugi::xml_attribute at_scale_w = el_common.attribute("scaleW");
    pugi::xml_attribute at_scale_h = el_common.attribute("scaleH");

    ivec2 scale(at_scale_w.as_int(), at_scale_h.as_int());
    I32 y_offset = at_base.as_int();

    if (scale.x <= 0 || scale.y <= 0)
    {
        PBJ_LOG(VWarning) << "Failed to load XML BMFont: invalid scaling!" << PBJ_LOG_NL
                          << "Common scaleW: " << scale.x << PBJ_LOG_NL
                          << "Common scaleH: " << scale.y << PBJ_LOG_END;

        throw std::runtime_error("Could not load BMFont XML spec: invalid scaling.");
    }

    std::vector<gfx::TextureFontCharacter> chars;

    F32 cap_height(0);
    bool x_cap_found(false);

    for (auto i(el_chars.begin()), end(el_chars.end()); i != end; ++i)
    {
        if (std::string(i->name()) == "char")
        {
            pugi::xml_node& el_char = *i;
            pugi::xml_attribute at_page = el_char.attribute("page");
            if (at_page.as_int() == 0)
            {
                    //<char id="-1" x="114" y="11" width="3" height="9" xoffset="0" yoffset="1" xadvance="4" />
                pugi::xml_attribute at_id = el_char.attribute("id");
                pugi::xml_attribute at_x = el_char.attribute("x");
                pugi::xml_attribute at_y = el_char.attribute("y");
                pugi::xml_attribute at_width = el_char.attribute("width");
                pugi::xml_attribute at_height = el_char.attribute("height");
                pugi::xml_attribute at_xoffset = el_char.attribute("xoffset");
                pugi::xml_attribute at_yoffset = el_char.attribute("yoffset");
                pugi::xml_attribute at_xadvance = el_char.attribute("xadvance");

                gfx::TextureFontCharacter ch;

                ch.character_advance = at_xadvance.as_float();
                ch.character_offset = vec2(at_xoffset.as_float(), y_offset - at_yoffset.as_float());
                ch.codepoint = at_id.as_uint();
                ch.texture_dimensions = vec2(at_width.as_float(), at_height.as_float());
                ch.texture_offset = vec2(at_x.as_float(), at_y.as_float());

                if (ch.codepoint < 0)
                    ch.codepoint = gfx::TextureFontCharacter::cp_invalid;

                chars.push_back(ch);

                if (!x_cap_found && ch.codepoint >= 'A' && ch.codepoint <= 'Z')
                {
                    if (ch.codepoint == 'X')
                        x_cap_found = true;

                    cap_height = ch.character_offset.y;
                }
            }
        }
    }

    gfx::TextureFont* font = new gfx::TextureFont(id, texture, cap_height, chars.begin(), chars.end());

    return std::unique_ptr<gfx::TextureFont>(font);
}

} // namespace pbj::sw
} // namespace pbj
