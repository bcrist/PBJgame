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
/// \defgroup ids Identifiers
///
/// \brief  Value-objects for uniquely identifying objects.

///////////////////////////////////////////////////////////////////////////////
/// \defgroup handles Handles
///
/// \brief  Pointer-like objects which remain usable when their target is
///         destroyed.
/// \details Handles can be used to uniquely identify an object in memory.
///         Unlike plain pointers or references, if the handle's target is
///         destroyed or invalidated, the handle remains usable (if the object
///         is no longer accessible, it will resolve to null, rather than
///         causing illegal access errors).  Note that this means handles
///         should not be used as keys in map or set data structures which
///         assume key objects remain constant as long as they exist in the
///         container.  If a handle were used as a key (using a comparator
///         based on the value returned by get()), and the handled object were
///         destroyed, it would likely corrupt the rest of the container.
///
///         A ConstHandle behaves similarly to a pointer to const data.  A
///         Handle behaves similarly to a pointer to non-const data.  Handle
///         objects can be converted into ConstHandle objects implicitly (but
///         the reverse is not possible).  Neither Handle nor ConstHandle
///         provides any facility for setting or changing the target of a
///         handle.  For that, a SourceHandle is required.  SourceHandles
///         define the objects from which handles can be created.
///
///         A ConstComponentHandle or ComponentHandle is a form of
///         Handle<Component> that can be used as if the handle were of a type
///         derived from Component, for instance bsc::Transform.  This can be
///         a convenience since it reduces the amount of explicit downcasting
///         of components that would be required.

///////////////////////////////////////////////////////////////////////////////
/// \defgroup entities Component/Entity Object Model
///
/// \brief  Hierarchical game object structures.

///////////////////////////////////////////////////////////////////////////////
/// \defgroup sc Standard Components
///
/// \brief  A set of commonly used components.
/// \ingroup entities

///////////////////////////////////////////////////////////////////////////////
/// \defgroup loading AssetCaches and Resource Loading
///
/// \brief  Functions & Classes for loading assets, entities, and components
///         from beds.

///////////////////////////////////////////////////////////////////////////////
/// \defgroup db SQLite BED Databases
/// \ingroup loading
///
/// \brief  Low-level access to beds and other SQLite databases files.

///////////////////////////////////////////////////////////////////////////////
/// \defgroup window Window Management
///
/// \brief  Manages one or more windows which can be drawn on using OpenGL.

///////////////////////////////////////////////////////////////////////////////
/// \page building Building Bengine
/// \tableofcontents
///
/// \section switches Preprocessor Switches
/// There are a variety of preprocessor switches which can be defined on a
/// project-wide level to change the way bengine is compiled.
///
/// Switch                    |  Description
/// --------------------------|-----------------------
/// #DEBUG                    | Specifies that this is a debug build.
/// #NDEBUG                   | Specifies that this is \b not a debug build.
/// #BE_BEDITOR               | Specifies that the app is an editor rather than a game.
/// #BE_TEST                  | Specifies that the unit tests and testing app should be built rather than regular app.
/// #BE_ID_NAMES_ENABLED      | Enables storing of Id names.
/// #BE_ID_NAMES_DISABLED     | Disables storing of Id names.
///

///////////////////////////////////////////////////////////////////////////////
/// \page handles Handles
/// \tableofcontents
///
/// \section types Handle-able Types
///    - be::Entity
///    - be::Component
///    - be::wnd::Window
///    - be::AssetCache
///    - be::strgen::Generator
