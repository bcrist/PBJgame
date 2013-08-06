///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/editor.h
/// \author Benjamin Crist
///
/// \brief  pbj::Editor class header.

#ifndef PBJ_EDITOR_H_
#define PBJ_EDITOR_H_

#include "be/id.h"
#include "pbj/_pbj.h"
#include "pbj/window.h"
#include "pbj/engine.h"
#include "pbj/gfx/built_ins.h"
#include "pbj/gfx/batcher.h"

#include "pbj/scene/scene.h"
#include "pbj/scene/ui_button.h"

#include <memory>

namespace pbj {

class Editor
{
public:
    enum Mode
    {
        MDatabases = 0,
        MLevels = 1,
        MWorld = 2,
        MObjects = 3,
        MTextures = 4,
        MSettings = 5,
    };

    Editor();
    ~Editor();

    void initUI();

    void run();


    void setMode(Mode mode);
    Mode getMode() const;

private:
    void onContextResized_(I32 width, I32 height);

    Engine& engine_;
    gfx::Batcher& batcher_;
    const gfx::BuiltIns& builtins_;
    Window& window_;

    Mode mode_;

    scene::Scene scene_;

    scene::UIPanel* menu_;
    scene::UILabel* frame_time_label_;
    scene::UIPanel* panels_[6];

    std::unordered_map<Id, scene::UIElement*> ui_elements_;

    scene::UIElement* last_created_focusable_element_;
    scene::UIButtonStateConfig bsc_a_[7];
    scene::UIButtonStateConfig bsc_b_[7];
    
    scene::UIListbox* newListbox_(const Id& id, const color3& color,
        const vec2& position, const vec2& dimensions, scene::UIPanel* parent);

    scene::UIPanel* newRootPanel_(U32 index, const Id& id, const color3& color);

    void useButtonConfigs_(scene::UIButton* btn, const std::string& affix);

    void generateButtonStateConfigs_(const color3& color,
        scene::UIButtonStateConfig* configs,
        const std::string& affix);

    scene::UIButton* newButton_(const Id& id,
        const std::string& text,
        const vec2& position, const vec2& dimensions,
        const std::function<void()>& callback,
        scene::UIPanel* parent);

    Editor(const Editor&);
    void operator=(const Editor&);
};

} // namespace pbj

#endif
