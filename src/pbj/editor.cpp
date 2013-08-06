///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/editor.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::Editor functions.

#include "pbj/editor.h"

#include "pbj/gfx/texture_font.h"
#include "pbj/scene/ui_root.h"
#include "pbj/scene/ui_button.h"
#include "pbj/scene/ui_listbox.h"
#include "pbj/input_controller.h"

#include "pbj/sw/sandwich_open.h"

#include <iostream>
#include <thread>

namespace pbj {

///////////////////////////////////////////////////////////////////////////////
Editor::Editor()
    : engine_(getEngine()),
      batcher_(getEngine().getBatcher()),
      builtins_(getEngine().getBuiltIns()),
      window_(*getEngine().getWindow())
{
    initUI();

    setMode(MDatabases);

    window_.registerContextResizeListener([=](I32 width, I32 height) { onContextResized_(width, height); });

    InputController::registerKeyAllListener([&](I32 keycode, I32 scancode, I32 action, I32 modifiers)
    {
        if (keycode == GLFW_KEY_SPACE && action == GLFW_RELEASE && scene_.ui.getFocus() == &scene_.ui.panel)
        {
            if (menu_->isVisible())
                menu_->setVisible(false);
            else if (action == GLFW_RELEASE)
                menu_->setVisible(true);
        }
    });

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

///////////////////////////////////////////////////////////////////////////////
Editor::~Editor()
{
}

class DbListboxModel : public scene::UIListboxModel
{
public:
    DbListboxModel()
    {
        sandwiches = sw::getSandwichIds();
    }

    virtual ~DbListboxModel() {}

    virtual std::string operator[](size_t index)
    {
        return sandwiches[index].to_string();
    }

    virtual size_t size() const
    {
        return sandwiches.size();
    }

    virtual bool isDirty()
    {
        return false;
    }

private:
    std::vector<Id> sandwiches;
};

///////////////////////////////////////////////////////////////////////////////
void Editor::initUI()
{
    generateButtonStateConfigs_(color3(0.5f, 0.6f, 0.65f), bsc_a_, "a");
    generateButtonStateConfigs_(color3(0.6f, 0.5f, 0.45f), bsc_b_, "b");

    last_created_focusable_element_ = &scene_.ui.panel;

    scene::UIPanel* menu_parent = new scene::UIPanel();
    menu_ = new scene::UIPanel();
    scene_.ui.panel.addElement(std::unique_ptr<scene::UIElement>(menu_parent));
    menu_parent->addElement(std::unique_ptr<scene::UIElement>(menu_));
    ui_elements_[Id("menu")] = menu_;

    scene::UIButton
        *menu_b_databases = newButton_(Id("menu_b_databases"), "Databases", vec2(0, 0 * 30),  vec2(100, 22), [=]() { setMode(MDatabases); },    menu_),
        *menu_b_levels    = newButton_(Id("menu_b_levels"),    "Levels",    vec2(0, 1 * 30),  vec2(100, 22), [=]() { setMode(MLevels); },       menu_),
        *menu_b_world     = newButton_(Id("menu_b_world"),     "World",     vec2(0, 2 * 30),  vec2(100, 22), [=]() { setMode(MWorld); },        menu_),
        *menu_b_objects   = newButton_(Id("menu_b_objects"),   "Objects",   vec2(0, 3 * 30),  vec2(100, 22), [=]() { setMode(MObjects); },      menu_),
        *menu_b_textures  = newButton_(Id("menu_b_textures"),  "Textures",  vec2(0, 4 * 30),  vec2(100, 22), [=]() { setMode(MTextures); },     menu_),
        *menu_b_settings  = newButton_(Id("menu_b_settings"),  "Settings",  vec2(0, 11 * 30), vec2(100, 22), [=]() { setMode(MSettings); },     menu_),
        *menu_b_exit      = newButton_(Id("menu_b_exit"),      "Exit",      vec2(0, 12 * 30), vec2(100, 22), [&]() { window_.requestClose(); }, menu_);

    frame_time_label_ = new scene::UILabel();
    menu_->addElement(std::unique_ptr<scene::UIElement>(frame_time_label_));
    frame_time_label_->setAlign(scene::UILabel::AlignRight);
    frame_time_label_->setDimensions(vec2(200, 22));
    frame_time_label_->setPosition(vec2(550, 375));
    frame_time_label_->setFont(builtins_.getTextureFont(Id("TextureFont.default")).getHandle());
    frame_time_label_->setTextColor(color4(0.5f, 0.6f, 0.65f, 1.0f));

    //menu_b_levels->setDisabled(true);
    //menu_b_textures->setDisabled(true);
    menu_b_settings->setDisabled(true);
    menu_b_exit->setNextFocusElement(menu_b_databases);
    
    newRootPanel_(MDatabases, Id("p_databases"), color3(0.1f, 0.12f, 0.13f));
    newRootPanel_(MLevels,    Id("p_levels"),    color3(0.1f, 0.12f, 0.13f));
    newRootPanel_(MObjects,   Id("p_objects"),   color3(0.1f, 0.12f, 0.13f));
    newRootPanel_(MTextures,  Id("p_textures"),  color3(0.1f, 0.12f, 0.13f));
    newRootPanel_(MSettings,  Id("p_settings"),  color3(0.1f, 0.12f, 0.13f));


    scene::UIListbox
        *lb_databases = newListbox_(Id("lb_databases"), color3(0.5f, 0.6f, 0.65f), vec2(50, 50), vec2(300, 500), panels_[MDatabases]);

    lb_databases->model = std::unique_ptr<scene::UIListboxModel>(new DbListboxModel());

    scene::UIPanel* p_world = new scene::UIPanel();
    menu_->addElement(std::unique_ptr<scene::UIElement>(p_world));
    panels_[MWorld] = p_world;
    ui_elements_[Id("p_world")] = p_world;
    p_world->setVisible(false);

    ivec2 wnd_size(window_.getSize());
    onContextResized_(wnd_size.x, wnd_size.y);
}

///////////////////////////////////////////////////////////////////////////////
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

        if (menu_->isVisible())
        {
            frame_time_label_->setText(std::to_string(1000.0f * last_frame_time) + " ms");
        }

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

///////////////////////////////////////////////////////////////////////////////
void Editor::setMode(Mode mode)
{
    if (mode_ != mode)
    {
        switch (mode_)
        {
            case MDatabases:
                useButtonConfigs_(static_cast<scene::UIButton*>(ui_elements_[Id("menu_b_databases")]), "a");
                panels_[MDatabases]->setVisible(false);
                break;

            case MLevels:
                useButtonConfigs_(static_cast<scene::UIButton*>(ui_elements_[Id("menu_b_levels")]), "a");
                panels_[MLevels]->setVisible(false);
                break;

            case MWorld:
                useButtonConfigs_(static_cast<scene::UIButton*>(ui_elements_[Id("menu_b_world")]), "a");
                panels_[MWorld]->setVisible(false);
                break;

            case MObjects:
                useButtonConfigs_(static_cast<scene::UIButton*>(ui_elements_[Id("menu_b_objects")]), "a");
                panels_[MObjects]->setVisible(false);
                break;

            case MTextures:
                useButtonConfigs_(static_cast<scene::UIButton*>(ui_elements_[Id("menu_b_textures")]), "a");
                panels_[MTextures]->setVisible(false);
                break;

            case MSettings:
                useButtonConfigs_(static_cast<scene::UIButton*>(ui_elements_[Id("menu_b_settings")]), "a");
                panels_[MSettings]->setVisible(false);
                break;

            default:
                //assert(false);
                // we don't care if the old mode was invalid
                break;
        }

        mode_ = mode;

        switch (mode_)
        {
            case MDatabases:
                useButtonConfigs_(static_cast<scene::UIButton*>(ui_elements_[Id("menu_b_databases")]), "b");
                panels_[MDatabases]->setVisible(true);
                break;

            case MLevels:
                useButtonConfigs_(static_cast<scene::UIButton*>(ui_elements_[Id("menu_b_levels")]), "b");
                panels_[MLevels]->setVisible(true);
                break;

            case MWorld:
                useButtonConfigs_(static_cast<scene::UIButton*>(ui_elements_[Id("menu_b_world")]), "b");
                panels_[MWorld]->setVisible(true);
                break;

            case MObjects:
                useButtonConfigs_(static_cast<scene::UIButton*>(ui_elements_[Id("menu_b_objects")]), "b");
                panels_[MObjects]->setVisible(true);
                break;

            case MTextures:
                useButtonConfigs_(static_cast<scene::UIButton*>(ui_elements_[Id("menu_b_textures")]), "b");
                panels_[MTextures]->setVisible(true);
                break;

            case MSettings:
                useButtonConfigs_(static_cast<scene::UIButton*>(ui_elements_[Id("menu_b_settings")]), "b");
                panels_[MSettings]->setVisible(true);
                break;

            default:
                assert(false);
                // but we do care if the new mode is invalid
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
Editor::Mode Editor::getMode() const
{
    return mode_;
}



///////////////////////////////////////////////////////////////////////////////
void Editor::onContextResized_(I32 width, I32 height)
{
    scene_.ui.panel.setDimensions(vec2(width, height));

    for (size_t i = 0; i < 6; ++i)
        panels_[i]->setDimensions(vec2(width, height));


    // Place/scale menu panel correctly
    const ivec2 min_menu_size(760, 400);
    int menu_scale = std::max(1, std::min(width / min_menu_size.x, height / min_menu_size.y));
    vec2 menu_offset = vec2(I32(0.5 * (width - menu_scale * min_menu_size.x)),
                            I32(0.5 * (height - menu_scale * min_menu_size.y)));
    if (menu_offset.y > menu_offset.x)
        menu_offset.y = menu_offset.x;

    scene::UIPanel* menu_ = static_cast<scene::UIPanel*>(ui_elements_[Id("menu")]);

    menu_->setPosition(menu_offset);
    menu_->setScale(vec2(menu_scale, menu_scale));
}



///////////////////////////////////////////////////////////////////////////////
scene::UIListbox* Editor::newListbox_(const Id& id, const color3& color,
        const vec2& position, const vec2& dimensions, scene::UIPanel* parent)
{
    const gfx::TextureFont& font = builtins_.getTextureFont(Id("TextureFont.default"));

    scene::UIPanelAppearance pa;
    pa.solid = true;
    pa.background_color = color4(color * 0.6f, 0.4f);
    pa.border_color = color4(color, 1.0f);
    pa.border_width_left = 0.5f;
    pa.border_width_right = 0.5f;
    pa.border_width_top = 0.5f;
    pa.border_width_bottom = 0.5f;
    pa.margin_left = 0.5f;
    pa.margin_right = 0.5f;
    pa.margin_top = 0.5f;
    pa.margin_bottom = 0.5f;

    scene::UIListbox* listbox = new scene::UIListbox();
    parent->addElement(std::unique_ptr<scene::UIElement>(listbox));
    ui_elements_[id] = listbox;

    listbox->setPosition(position);
    listbox->setDimensions(dimensions);
    listbox->panel.setAppearance(pa);

    scene::UIButtonStateConfig bsc;
    bsc.button_state = Id("__normal__");
    bsc.font        = font.getHandle();
    bsc.text_scale  = vec2(1.0f, 1.0f);
    bsc.text_color       = color4(color * 1.3f, 1.0f);
    bsc.background_color = color4(color * 0.66f, 0.0f);
    bsc.border_color     = color4(color * 1.1f, 0.0f);
    bsc.border_width_left   = 0.5f;
    bsc.border_width_right  = 0.5f;
    bsc.border_width_top    = 0.5f;
    bsc.border_width_bottom = 0.5f;
    bsc.margin_left   = 0.5f;
    bsc.margin_right  = 1.5f;
    bsc.margin_top    = 0.5f;
    bsc.margin_bottom = 1.5f;
    listbox->scroll_up->setStateConfig(bsc);
    listbox->scroll_down->setStateConfig(bsc);
    

    return listbox;
}

///////////////////////////////////////////////////////////////////////////////
scene::UIPanel* Editor::newRootPanel_(U32 index, const Id& id, const color3& color)
{
    scene::UIPanelAppearance pa;
    pa.solid = true;
    pa.background_color = color4(color, 1.0f);
    pa.border_width_left = 0;
    pa.border_width_right = 0;
    pa.border_width_top = 0;
    pa.border_width_bottom = 0;
    pa.margin_left = 0;
    pa.margin_right = 0;
    pa.margin_top = 0;
    pa.margin_bottom = 0;

    scene::UIPanel* panel = new scene::UIPanel();
    scene_.ui.panel.addElement(std::unique_ptr<scene::UIElement>(panel));
    panels_[index] = panel;
    ui_elements_[id] = panel;
    panel->setAppearance(pa);
    panel->setVisible(false);

    return panel;
}

///////////////////////////////////////////////////////////////////////////////
void Editor::useButtonConfigs_(scene::UIButton* btn, const std::string& affix)
{
    assert(btn);

    btn->setNormalState(Id("__normal_" + affix + "__"));
    btn->setHoveredState(Id("__hovered_" + affix + "__"));
    btn->setActiveState(Id("__active_" + affix + "__"));
    btn->setDisabledState(Id("__disabled_" + affix + "__"));
    btn->setFocusedState(Id("__focused_" + affix + "__"));
    btn->setFocusedHoveredState(Id("__focused_hovered_" + affix + "__"));
    btn->setFocusedActiveState(Id("__focused_active_" + affix + "__"));
}

///////////////////////////////////////////////////////////////////////////////
void Editor::generateButtonStateConfigs_(const color3& color, scene::UIButtonStateConfig* configs, const std::string& affix)
{
    const gfx::TextureFont& font = builtins_.getTextureFont(Id("TextureFont.default"));
    
    configs[0].button_state = Id("__normal_" + affix + "__");

    configs[0].font        = font.getHandle();
    configs[0].text_scale  = vec2(1.0f, 1.0f);
    configs[0].text_color       = color4(color * 1.3f, 1.0f);
    configs[0].background_color = color4(color * 0.66f, 0.5f);
    configs[0].border_color     = color4(color * 1.1f, 0.75f);
    configs[0].border_width_left   = 0.5f;
    configs[0].border_width_right  = 0.5f;
    configs[0].border_width_top    = 0.5f;
    configs[0].border_width_bottom = 0.5f;
    configs[0].margin_left   = 0.5f;
    configs[0].margin_right  = 1.5f;
    configs[0].margin_top    = 0.5f;
    configs[0].margin_bottom = 1.5f;

    configs[1] = configs[0];
    configs[1].button_state = Id("__hovered_" + affix + "__");
    configs[1].background_color.a = 0.7f;
    configs[1].text_color = color4(color * 1.4f, 1.0f);
    configs[1].border_color = color4(color * 1.2f, 1.0f);

    configs[2] = configs[1];
    configs[2].button_state = Id("__active_" + affix + "__");
    configs[2].background_color.a = 0.6f;
    configs[2].text_color = color4(color * 1.3f, 1.0f);

    configs[2].border_width_left = 0.0f;
    configs[2].border_width_right = 0.0f;
    configs[2].border_width_top = 0.0f;
    configs[2].border_width_bottom = 0.0f;
    configs[2].margin_left = 1.0f;
    configs[2].margin_right = 0.0f;
    configs[2].margin_top = 1.0f;
    configs[2].margin_bottom = 0.0f;

    configs[3] = configs[0];
    configs[3].button_state = Id("__disabled_" + affix + "__");
    configs[3].background_color.a = 0.3f;
    configs[3].text_color *= 0.6f;
    configs[3].text_color.a = 1.0f;
    configs[3].border_width_left = 0.0f;
    configs[3].border_width_right = 0.0f;
    configs[3].border_width_top = 0.0f;
    configs[3].border_width_bottom = 0.0f;
    configs[3].margin_left = 0.0f;
    configs[3].margin_right = 0.0f;
    configs[3].margin_top = 0.0f;
    configs[3].margin_bottom = 0.0f;

    configs[4] = configs[0];
    configs[4].button_state = Id("__focused_" + affix + "__");
    configs[4].border_color = color4(color * 1.2f, 1.0f);

    configs[5] = configs[1];
    configs[5].button_state = Id("__focused_hovered_" + affix + "__");

    configs[6] = configs[2];
    configs[6].button_state = Id("__focused_active_" + affix + "__");
}

///////////////////////////////////////////////////////////////////////////////
scene::UIButton* Editor::newButton_(const Id& id,
                                    const std::string& text,
                                    const vec2& position,
                                    const vec2& dimensions,
                                    const std::function<void()>& callback,
                                    scene::UIPanel* parent)
{
    scene::UIButton* btn = new scene::UIButton();
    parent->addElement(std::unique_ptr<scene::UIElement>(btn));

    ui_elements_[id] = btn;
    if (last_created_focusable_element_)
        last_created_focusable_element_->setNextFocusElement(btn);

    last_created_focusable_element_ = btn;

    btn->setText(text);
    btn->setPosition(position);
    btn->setDimensions(dimensions);
    
    useButtonConfigs_(btn, "a");

    for (int i = 0; i < 7; ++i)
    {
        scene::UIButtonStateConfig bsc = bsc_a_[i];
        bsc.click_callback = callback;
        btn->setStateConfig(bsc);

        bsc = bsc_b_[i];
        bsc.click_callback = callback;
        btn->setStateConfig(bsc);
    }

    return btn;
}

} // namespace pbj
