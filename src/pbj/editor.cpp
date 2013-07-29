// Copyright (c) 2013 PBJ^2 Productions
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
/// \file   pbj/editor.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::Editor functions.

#include "pbj/editor.h"

#include "pbj/gfx/texture_font.h"
#include "pbj/scene/ui_root.h"
#include "pbj/scene/ui_button.h"

#include <iostream>
#include <thread>

namespace pbj {

Editor::Editor()
    : engine_(getEngine()),
      batcher_(getEngine().getBatcher()),
      builtins_(getEngine().getBuiltIns()),
      window_(*getEngine().getWindow())
{
    initUI();

    window_.registerContextResizeListener([=](I32 width, I32 height) { onContextResized_(width, height); });

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

Editor::~Editor()
{
}

void Editor::initUI()
{
    const gfx::TextureFont& font = builtins_.getTextureFont(Id("TextureFont.default"));

    color3 btn_color(0.5f, 0.6f, 0.65f);

    bsc_[0].button_state = Id("__normal__");

    bsc_[0].font = font.getHandle();
    bsc_[0].background_color = color4(btn_color * 0.66f, 0.5f);
    bsc_[0].border_color = color4(btn_color, 0.6f);
    bsc_[0].text_color = color4(btn_color, 1.0f);
    bsc_[0].text_scale = vec2(1.0f, 1.0f);
    bsc_[0].border_width_left = 0.5f;
    bsc_[0].border_width_right = 0.5f;
    bsc_[0].border_width_top = 0.5f;
    bsc_[0].border_width_bottom = 0.5f;
    bsc_[0].margin_left = 0.5f;
    bsc_[0].margin_right = 1.5f;
    bsc_[0].margin_top = 0.5f;
    bsc_[0].margin_bottom = 1.5f;

    bsc_[1] = bsc_[0];
    bsc_[1].button_state = Id("__hovered__");
    bsc_[1].background_color.a = 0.7f;
    bsc_[1].text_color *= 1.3f;
    bsc_[1].text_color.a = 1.0f;
    bsc_[1].border_color.a = 1.0f;

    bsc_[2] = bsc_[1];
    bsc_[2].button_state = Id("__active__");
    bsc_[2].background_color *= 0.8f;
    bsc_[2].background_color.a = 0.7f;
    bsc_[2].text_color = bsc_[0].text_color;

    bsc_[2].border_width_left = 0.0f;
    bsc_[2].border_width_right = 0.0f;
    bsc_[2].border_width_top = 0.0f;
    bsc_[2].border_width_bottom = 0.0f;
    bsc_[2].margin_left = 1.0f;
    bsc_[2].margin_right = 0.0f;
    bsc_[2].margin_top = 1.0f;
    bsc_[2].margin_bottom = 0.0f;

    bsc_[3] = bsc_[0];
    bsc_[3].button_state = Id("__disabled__");
    bsc_[3].background_color.a = 0.3f;
    bsc_[3].text_color *= 0.6f;
    bsc_[3].text_color.a = 1.0f;
    bsc_[3].border_width_left = 0.0f;
    bsc_[3].border_width_right = 0.0f;
    bsc_[3].border_width_top = 0.0f;
    bsc_[3].border_width_bottom = 0.0f;
    bsc_[3].margin_left = 0.0f;
    bsc_[3].margin_right = 0.0f;
    bsc_[3].margin_top = 0.0f;
    bsc_[3].margin_bottom = 0.0f;

    bsc_[4] = bsc_[0];
    bsc_[4].button_state = Id("__focused__");
    bsc_[4].border_color.a = 0.9f;

    bsc_[5] = bsc_[1];
    bsc_[5].button_state = Id("__focused_hovered__");

    bsc_[6] = bsc_[2];
    bsc_[6].button_state = Id("__focused_active__");

    last_created_focusable_element_ = &scene_.ui.panel;

    scene::UIPanel* menu_ = new scene::UIPanel();
    scene_.ui.panel.addElement(std::unique_ptr<scene::UIElement>(menu_));
    ui_elements_[Id("menu")] = menu_;

    menu_->addElement(newButton_(Id("menu_b_databases"), "Databases", vec2(0, 0 * 30), vec2(100, 22), nullptr));
    menu_->addElement(newButton_(Id("menu_b_levels"),    "Levels",    vec2(0, 1 * 30), vec2(100, 22), nullptr));
    menu_->addElement(newButton_(Id("menu_b_world"),     "World",     vec2(0, 2 * 30), vec2(100, 22), nullptr));
    menu_->addElement(newButton_(Id("menu_b_objects"),   "Objects",   vec2(0, 3 * 30), vec2(100, 22), nullptr));
    menu_->addElement(newButton_(Id("menu_b_textures"),  "Textures",  vec2(0, 4 * 30), vec2(100, 22), nullptr));
    menu_->addElement(newButton_(Id("menu_b_settings"),  "Settings",  vec2(0, 13 * 30), vec2(100, 22), nullptr));
    menu_->addElement(newButton_(Id("menu_b_exit"),      "Exit",      vec2(0, 14 * 30), vec2(100, 22),
        [&]()
        {
            window_.requestClose();
        } ));

    static_cast<scene::UIButton*>(ui_elements_[Id("menu_b_settings")])->setDisabled(true);
    ui_elements_[Id("menu_b_exit")]->setNextFocusElement(ui_elements_[Id("menu_b_databases")]);

    //menu_->setFocused();

    ivec2 wnd_size(window_.getSize());
    onContextResized_(wnd_size.x, wnd_size.y);
}


void Editor::run()
{
    double last_frame_time = 0;
    double fps = 0;

    while (true)
    {
        double frame_start = glfwGetTime();
        
        glfwPollEvents();

        if (window_.isClosePending())
            break;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene_.ui.draw();
        batcher_.draw();

        GLenum gl_error;
        while ((gl_error = glGetError()) != GL_NO_ERROR)
        {
            PBJ_LOG(VWarning) << "OpenGL error while rendering frame!" << PBJ_LOG_NL
                                   << "Error Code: " << gl_error << PBJ_LOG_NL
                                   << "     Error: " << getGlErrorString(gl_error) << PBJ_LOG_END;
        }

        glfwSwapBuffers(window_.getGlfwHandle());

        double frame_time = last_frame_time = glfwGetTime() - frame_start;

        if (last_frame_time < (1.0 / 180.0))
        {
            std::this_thread::sleep_for(std::chrono::microseconds(int(1000000 * ((1.0 / 180.0) - last_frame_time))));
            frame_time = glfwGetTime() - frame_start;
        }

        fps = 1.0 / frame_time;
    }
}

void Editor::onContextResized_(I32 width, I32 height)
{
    scene_.ui.panel.setDimensions(vec2(width, height));

    // Place/scale menu panel correctly
    const ivec2 min_menu_size(800, 450);
    int menu_scale = std::max(1, std::min(width / min_menu_size.x, height / min_menu_size.y));
    vec2 menu_offset = vec2(I32(0.5 * (width - menu_scale * min_menu_size.x)),
                            I32(0.5 * (height - menu_scale * min_menu_size.y)));
    if (menu_offset.y > menu_offset.x)
        menu_offset.y = menu_offset.x;

    scene::UIPanel* menu_ = static_cast<scene::UIPanel*>(ui_elements_[Id("menu")]);

    menu_->setPosition(menu_offset);
    menu_->setScale(vec2(menu_scale, menu_scale));
}

std::unique_ptr<scene::UIElement> Editor::newButton_(const Id& id,
                                                     const std::string& text,
                                                     const vec2& position,
                                                     const vec2& dimensions,
                                                     const std::function<void()>& callback)
{
    scene::UIButton* btn = new scene::UIButton();
    ui_elements_[id] = btn;
    if (last_created_focusable_element_)
        last_created_focusable_element_->setNextFocusElement(btn);

    last_created_focusable_element_ = btn;

    btn->setText(text);
    btn->setPosition(position);
    btn->setDimensions(dimensions);
    
    btn->setFocusedState(Id("__focused__"));
    //btn->setFocusedHoveredState(Id("__focused_hovered__"));
    //btn->setFocusedActiveState(Id("__focused_active__"));

    for (int i = 0; i < 7; ++i)
    {
        scene::UIButtonStateConfig bsc = bsc_[i];
        bsc.click_callback = callback;
        btn->setStateConfig(bsc);
    }

    return std::unique_ptr<scene::UIElement>(btn);
}

} // namespace pbj
