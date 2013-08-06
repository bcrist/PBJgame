///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene/ui_panel.cpp
/// \author Ben Crist

#include "pbj/scene/ui_panel.h"

namespace pbj {
namespace scene {

UIPanelAppearance::UIPanelAppearance()
    : solid(false),
      background_color(0.5f, 0.5f, 0.5f, 0.5f),
      border_color(1.0f, 1.0f, 1.0f, 1.0f),
      margin_color(0.0f, 0.0f, 0.0f, 0.0f),
      margin_left(0.5f),
      margin_right(0.5f),
      margin_top(0.5f),
      margin_bottom(0.5f),
      border_width_left(0.5f),
      border_width_right(0.5f),
      border_width_top(0.5f),
      border_width_bottom(0.5f)
{
}

UIPanel::UIPanel()
    : scale_(1.0f, 1.0f),
      panel_mesh_(getEngine().getBuiltIns().getMesh(Id("Mesh.std_quad")))
{
    const gfx::BuiltIns& builtins = getEngine().getBuiltIns();

    btask_.program_id = builtins.getProgram(Id("ShaderProgram.UIBox")).getGlId();

    btask_.vao_id = panel_mesh_.getVaoId();
    btask_.n_indices = panel_mesh_.getIndexCount();
    btask_.index_data_type = panel_mesh_.getIndexType();

    btask_.samplers = nullptr;
    btask_.n_samplers = 0;
    btask_.uniforms = uniforms_;
    btask_.n_uniforms = 5;

    uniforms_[u_transform_].location = glGetUniformLocation(btask_.program_id, "transform");
    uniforms_[u_transform_].type = gfx::UniformConfig::UM4f;
    uniforms_[u_transform_].array_size = 1;
    uniforms_[u_transform_].data = glm::value_ptr(panel_transform_);

    uniforms_[u_border_bounds_].location = glGetUniformLocation(btask_.program_id, "border_bounds");
    uniforms_[u_border_bounds_].type = gfx::UniformConfig::U2f;
    uniforms_[u_border_bounds_].array_size = 4;
    uniforms_[u_border_bounds_].data = glm::value_ptr(border_bounds_[0]);

    uniforms_[u_border_color_].location = glGetUniformLocation(btask_.program_id, "border_color");
    uniforms_[u_border_color_].type = gfx::UniformConfig::U4f;
    uniforms_[u_border_color_].array_size = 1;
    uniforms_[u_border_color_].data = glm::value_ptr(appearance_.border_color);

    uniforms_[u_background_color_].location = glGetUniformLocation(btask_.program_id, "background_color");
    uniforms_[u_background_color_].type = gfx::UniformConfig::U4f;
    uniforms_[u_background_color_].array_size = 1;
    uniforms_[u_background_color_].data = glm::value_ptr(appearance_.background_color);

    uniforms_[u_outside_color_].location = glGetUniformLocation(btask_.program_id, "outside_color");
    uniforms_[u_outside_color_].type = gfx::UniformConfig::U4f;
    uniforms_[u_outside_color_].array_size = 1;
    uniforms_[u_outside_color_].data = glm::value_ptr(appearance_.margin_color);

    btask_.depth_tested = false;
}

UIPanel::~UIPanel()
{
}

void UIPanel::setAppearance(const UIPanelAppearance& appearance)
{
    appearance_ = appearance;
    panel_transform_valid_ = false;
}

const UIPanelAppearance& UIPanel::getAppearance() const
{
    return appearance_;
}

void UIPanel::setScale(const vec2& scale)
{
    if (scale != scale_)
    {
        scale_ = scale;
        onBoundsChange_();
    }
}

const vec2& UIPanel::getScale() const
{
    return scale_;
}

void UIPanel::addElement(std::unique_ptr<UIElement>&& element)
{
    element->order_index_offset_ = &combined_order_index_offset_;
    element->scissor_ = scissor_;
    element->projection_ = projection_;
    element->view_ = &view_matrix_;
    element->inv_view_ = &inv_view_matrix_;
    element->focused_element_ = focused_element_;
    element->onBoundsChange_();

    elements_.push_back(std::move(element));
}

UIElement* UIPanel::getElementAt(const ivec2& screen_position)
{
    if (!inv_view_ || !isVisible())
        return nullptr;

    for (std::unique_ptr<UIElement>& ptr : elements_)
    {
        UIElement* element = ptr->getElementAt(screen_position);
        if (element)
            return element;
    }

    if (appearance_.solid)
    {
        vec2 pos = vec2(*inv_view_ * vec4(screen_position, 0, 1));

        if (pos.x >= position_.x && pos.x < position_.x + dimensions_.x &&
            pos.y >= position_.y && pos.y < position_.y + dimensions_.y)
        {
            return this;
        }
    }
    return nullptr;
}

void UIPanel::draw()
{
    if (!isVisible())
        return;

    if (appearance_.solid && projection_ && view_)
    {
        if (!panel_transform_valid_)
            calculateTransform_();

        getEngine().getBatcher().submit(btask_);
    }
    
    for (std::unique_ptr<UIElement>& ptr : elements_)
        ptr->draw();
}

void UIPanel::onBoundsChange_()
{
    view_matrix_ = glm::scale(glm::translate(*view_, vec3(getPosition(), 0)), vec3(scale_, 1)); 
    inv_view_matrix_ = glm::inverse(view_matrix_);
    combined_order_index_offset_ = *order_index_offset_ + 100;
    btask_.order_index = *order_index_offset_;
    btask_.scissor = scissor_;
    panel_transform_valid_ = false;

    for (std::unique_ptr<UIElement>& ptr : elements_)
    {
        ptr->order_index_offset_ = &combined_order_index_offset_;
        ptr->scissor_ = scissor_;
        ptr->projection_ = projection_;
        ptr->view_ = &view_matrix_;
        ptr->inv_view_ = &inv_view_matrix_;
        ptr->focused_element_ = focused_element_;
        ptr->onBoundsChange_();
    }
}

void UIPanel::calculateTransform_()
{
    if (!(projection_ && view_))
        return;
    
    vec3 scale(getDimensions(), 1);
    vec3 translate(getPosition(), 0);
    panel_transform_ = glm::scale(glm::translate(*projection_ * *view_, translate), scale);
        
    vec2 inv_scale(1.0f / scale.x, 1.0f / scale.y);

    border_bounds_[0] = inv_scale * vec2(appearance_.margin_left + appearance_.border_width_left,
                                         appearance_.margin_top + appearance_.border_width_top);
    border_bounds_[1] = vec2(1, 1) - inv_scale * vec2(appearance_.margin_right + appearance_.border_width_right,
                                                      appearance_.margin_bottom + appearance_.border_width_bottom);

    border_bounds_[2] = inv_scale * vec2(appearance_.margin_left - appearance_.border_width_left,
                                         appearance_.margin_top - appearance_.border_width_top);
    border_bounds_[3] = vec2(1, 1) - inv_scale * vec2(appearance_.margin_right - appearance_.border_width_right,
                                                      appearance_.margin_bottom - appearance_.border_width_bottom);
}

} // namespace pbj::scene
} // namespace pbj
