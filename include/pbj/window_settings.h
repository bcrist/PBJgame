///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/window_settings.h
/// \author Benjamin Crist
///
/// \brief  pbj::WindowSettings class header.

#ifndef PBJ_WINDOW_SETTINGS_H_
#define PBJ_WINDOW_SETTINGS_H_

#include "pbj/sw/resource_id.h"
#include "pbj/_math.h"
#include "pbj/_pbj.h"

namespace pbj {
namespace sw {

class Sandwich;

} // namespace pbj::sw

///////////////////////////////////////////////////////////////////////////////
/// \struct WindowSettings   pbj/window_settings.h "pbj/window_settings.h"
///
/// \brief  A collection of settings which must be known before constructing a
///         window.
/// \details Settings can be loaded and saved from a sandwich, and are stored
///         there in a stack-like structure, allowing previous settings to be
///         reverted if, for instance, new settings don't work.
struct WindowSettings
{
   WindowSettings();

   
   /// \brief  Specifies where these settings are saved in persistent storage.
   sw::ResourceId id;

   
   /// \brief  Determines whether the window is fullscreen or not, and how much
   ///         control the user has over its position/size.
   /// \details The LSB controls whether the window is user-resizable or not.
   ///         The next bit controls whether the window has a border, close
   ///         button, etc.  This allows for "windowed fullscreen" modes as
   ///         well as splash screens, etc.  Note that most operating systems
   ///         still provide a means to move and close undecorated windows.
   ///         The third bit determines if the window should be sized such
   ///         that it takes up the entire monitor, i.e. fullscreen windowed.
   ///         Finally, if the fourth bit is set, any other bits are ignored
   ///         and the window will be created in fullscreen exclusive mode.
   enum Mode
   {
      MResizable = 0,            ///< The window should have a border and be user-resizable.
      MFixed = 1,                ///< The window should have a border but should not be user-resizable.
      MUndecoratedResizable = 2, ///< The window should not have a border but the operating system may still provide the user a way to resize it.
      MUndecorated = 3,          ///< The window should not have a border and should not be user-resizable.
      MFullscreenWindowed = 7,   ///< The window should not have a border or be resizable, and it should be created such that it covers the whole monitor.
      MFullscreenExclusive = 8   ///< The window should be opened with exclusive fullscreen access to its monitor.

   } mode;  ///< Specifies the type of window to create.
   
   /// \brief  If false, the window should be moved to
   ///         \ref WindowSettings::position "position" after it is created.
   /// \details Ignored in fullscreen exclusive mode.
   bool system_positioned;

   /// \brief  If true, \ref WindowSettings::position "position" and
   ///         \ref WindowSettings::size "size" will be updated and saved to
   ///         the bed specified by \ref WindowSettings::id "id" when the
   ///         window is destroyed.
   bool save_position_on_close;
   
   /// \brief  The position the window should be placed at in desktop
   ///         coordinates.
   ivec2 position;

   /// \brief  The size of the window when it is created, or the resolution
   ///         of the exclusive fullscreen mode to request.
   ivec2 size;
   
   /// \brief  In fullscreen exclusive or fullscreen windowed mode, specifies
   ///         the index of the monitor to use.
   /// \details If less than 0, the primary monitor will be used.
   int monitor_index;

   /// \brief  The desired refresh rate for a fullscreen exclusive window.
   /// \details Ignored if not in fullscreen exclusive mode.  Set to zero
   ///         to request the highest available refresh rate.
   unsigned int refresh_rate;
   
   /// \brief  Determines whether to enable "vertical sync" mode.
   /// \details Enabling VSync forces the main thread to wait until the
   ///         currently displayed framebuffer is finished being sent to the
   ///         display before swapping buffers.  Some adapters and drivers
   ///         support an enhanced VSync that allows frames which are
   ///         slightly late to be swapped, while still delaying early
   ///         frames.
   enum VSyncMode
   {
      VSMDisabled = 0,           ///< Always swap buffers immediately.
      VSMEnabled = 1,            ///< Always wait for the current frame to finish.
      VSMEnabledLateAllowed = -1 ///< Only waits for the current frame to finish if the framebuffer was swapped after the last frame.
   } v_sync;   ///< Determines the "vertical synchronization" behavior for this window.

   /// \brief  Specifies if multisample anti-aliasing should be used.
   /// \details Specifies the number of samples (fragments) that should be
   ///         used per pixel.  Set to zero to disable MSAA.
   unsigned int msaa_level;

   /// \brief  Specifies the number of bits to use for the red component of
   ///         colors in the framebuffer.
   unsigned int red_bits;

   /// \brief  Specifies the number of bits to use for the green component of
   ///         colors in the framebuffer.
   unsigned int green_bits;

   /// \brief  Specifies the number of bits to use for the blue component of
   ///         colors in the framebuffer.
   unsigned int blue_bits;

   /// \brief  Specifies the number of bits to use for the alpha component of
   ///         colors in the framebuffer.
   /// \details Set to zero to disable destination-alpha on the default
   ///         framebuffer object.
   /// \note   Blending effects do not always require the framebuffer to
   ///         contain an alpha channel.
   unsigned int alpha_bits;

   /// \brief  Specifies the number of bits to use for the depth buffer.
   /// \details Set to zero to disable the depth buffer.
   unsigned int depth_bits;

   /// \brief  Specifies the number of bits to use for the stencil buffer.
   /// \details Set to zero to disable the stencil buffer.
   unsigned int stencil_bits;

   /// \brief  If true, an sRGB-capable framebuffer will be requested.
   bool srgb_capable;

   /// \brief  Determines whether to use a custom gamma ramp.
   /// \details Ignored if not in fullscreen exclusive mode.
   bool use_custom_gamma;

   /// \brief  Specifies the exponent to use to generate the custom gamma ramp
   ///         if use_custom_gamma is set.
   /// \details Ignored if not in fullscreen exclusive mode.
   float custom_gamma;
};

WindowSettings loadWindowSettings(sw::Sandwich& sandwich, const Id& id);

bool saveWindowSettings(const WindowSettings& window_settings);
bool updateSavedPosition(const WindowSettings& window_settings);
WindowSettings revertWindowSettings(const sw::ResourceId& id);
void truncateWindowSettingsHistory(const sw::ResourceId& id, int max_history_entries);

} // namespace pbj

#endif
