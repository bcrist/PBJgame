CREATE TABLE sw_sandwich_properties
(
      property          TEXT PRIMARY KEY,
      value             NUMERIC
);

CREATE TABLE sw_editor_metadata
(
      -- Identifies the type of resource this metadata describes.
      resource_type     INTEGER NOT NULL,
      -- Identifies the specific resource of type `resource_type` that this
      -- metadata describes.
      resource_id       INTEGER NOT NULL,
      
      property          TEXT NOT NULL,
      value             NUMERIC,
      
      PRIMARY KEY (resource_type, resource_id, property)
);

CREATE TABLE sw_maps
(
      id                INTEGER PRIMARY KEY,
      name              TEXT NOT NULL,
);

CREATE TABLE sw_static_entities
(
      map_id            INTEGER NOT NULL,
      id                INTEGER PRIMARY KEY,
      entity_type       INTEGER NOT NULL,
      r_x               REAL NOT NULL,
      r_y               REAL NOT NULL,
      r_z               REAL NOT NULL,
      r_w               REAL NOT NULL,
      t_x               REAL NOT NULL,
      t_y               REAL NOT NULL,
      t_z               REAL NOT NULL,
      s_x               REAL NOT NULL,
      s_y               REAL NOT NULL,
      s_z               REAL NOT NULL,
      mesh_id           INTEGER,
      program_id        INTEGER,
      collider_id       INTEGER
);

CREATE TABLE sw_colliders
(
      id                INTEGER PRIMARY KEY,
      collider_type     INTEGER NOT NULL,
      point_count       INTEGER NOT NULL,
      point_data        NOT NULL
);

CREATE TABLE sw_textures
(
      -- Identifies this texture among all other textures in this sandwich
      id                INTEGER PRIMARY KEY,
      -- Hashed Id describing the type of data stored in `data`.
      --    Id("stbi")        Image file data decodable by the STB Image lib.
      --    Id("raw_rgba8")   Raw RGBA8 data (tightly packed).
      --    Id("raw_rgb8")    Raw RGB8 data (tightly packed).
      --    Id("raw_rg8")     Raw RG8 data (tightly packed).
      --    Id("raw_r8")      Raw R8 data (tightly packed).
      data_format       INTEGER NOT NULL,
      -- For raw data types, describes the width of the stored image.
      width             INTEGER,
      -- For raw data types, describes the height of the stored image.
      height            INTEGER,
      -- The image data for the texture.  Format depends on the value of `data_format`
      data              NOT NULL,
      -- Describes the OpenGL internal format to request for storing the texture on the GPU.
      internal_format   INTEGER NOT NULL,
      -- If non-zero, the texture data will be linearized from sRGB when sampling.
      srgb              INTEGER NOT NULL,
      -- The sampler filtering mode to use for magnification
      mag_filter        INTEGER NOT NULL,
      -- The sampler filtering mode to use for minification
      min_filter        INTEGER NOT NULL
);

CREATE TABLE sw_texture_fonts
(
      id                INTEGER PRIMARY KEY,
      texture_id        INTEGER NOT NULL,
      cap_height        INTEGER NOT NULL
);

CREATE TABLE sw_texture_font_chars
(
      font_id           INTEGER NOT NULL,
      codepoint         INTEGER NOT NULL,
      
      tc_x              INTEGER NOT NULL,
      tc_y              INTEGER NOT NULL,
      
      tc_width          INTEGER NOT NULL,
      tc_height         INTEGER NOT NULL,
      
      offset_x          INTEGER NOT NULL,
      offset_y          INTEGER NOT NULL,
      
      advance           INTEGER NOT NULL,
      
      PRIMARY KEY (font_id, codepoint)
);

CREATE TABLE sw_meshes
(
      -- Identifies this texture among all other meshes in this sandwich.
      id                INTEGER PRIMARY KEY,
      -- Determines which VAO attribute index the vec4s in the corresponding slots in `data` apply to.
      -- The first null or negative index determines the size of a single vertex.
      -- For example s0ai = 4, s1ai = 0, s2ai = -1 means that the there are two
      -- vertex attributes, data[0], data[2], data[4], etc. apply to VAO attribute index 4,
      -- and data[1], data[3], etc are bound to VAO attribute index 0.
      s0ai,             INTEGER,
      s1ai,             INTEGER,
      s2ai,             INTEGER,
      s3ai,             INTEGER,
      s4ai,             INTEGER,
      s5ai,             INTEGER,
      s6ai,             INTEGER,
      s7ai,             INTEGER,
      s8ai,             INTEGER,
      s9ai,             INTEGER,
      s10ai,            INTEGER,
      s11ai,            INTEGER,
      s12ai,            INTEGER,
      s13ai,            INTEGER,
      s14ai,            INTEGER,
      s15ai,            INTEGER,
      -- Specifies the number of vertices in `vertex_data`.
      vertex_count      INTEGER NOT NULL,
      -- The vertex/normal/color/etc. data.  The length of `vertex_data`
      -- should be sizeof(vec4) * slot_count * vertex_count
      -- where slot_count is the number of `s*ai` fields before the first null
      -- or negative value.
      vertex_data       NOT NULL,
      -- The data type of the indices in the index buffer.
      --    1  GL_UNSIGNED_BYTE (1 byte per index; up to 256 verts)
      --    2  GL_UNSIGNED_SHORT (2 bytes per index; up to 65536 verts)
      --    3  GL_UNSIGNED_INT (4 bytes per index; up to 4M verts)
      index_type        INTEGER NOT NULL,
      -- size of the element index buffer.
      index_count       INTEGER NOT NULL,
      -- The element index buffer data.  The length of `index_data` should be
      -- sizeof(index_data_type) * index_count.
      index_data        NOT NULL
);

CREATE TABLE sw_skeleton_bones
(
      id                INTEGER PRIMARY KEY,
      parent_id         INTEGER NOT NULL
);

CREATE TABLE sw_skeleton_bone_poses
(
      pose_id           INTEGER NOT NULL,
      bone_id           INTEGER NOT NULL,
      r_x               REAL NOT NULL,
      r_y               REAL NOT NULL,
      r_z               REAL NOT NULL,
      r_w               REAL NOT NULL,
      t_x               REAL NOT NULL,
      t_y               REAL NOT NULL,
      t_z               REAL NOT NULL,
      s_x               REAL NOT NULL,
      s_y               REAL NOT NULL,
      s_z               REAL NOT NULL,
);

CREATE TABLE sw_shaders
(
      -- Identifies this shader among other shaders of any type in this sandwich
      id                INTEGER PRIMARY KEY,
      -- Whether the shader is a vertex or fragment shader
      --    0  Vertex shader
      --    1  Fragment shader
      shader_type       INTEGER NOT NULL,
      source            TEXT
);

CREATE TABLE sw_program_shaders
(
      program_id        INTEGER NOT NULL,
      shader_index      INTEGER NOT NULL,
      shader_id         INTEGER NOT NULL,
      
      PRIMARY KEY (program_id, shader_index)
);

CREATE TABLE sw_window_settings
(
      -- Identifies this set of window settings, allowing multiple windows to
      -- store their settings independently.
      id                INTEGER NOT NULL,
      -- When window settings are changed (except when saving new window
      -- position/size on window close), a new history index is created so
      -- that the old settings can be restored if the new ones are bad.
      history_index     INTEGER NOT NULL,
      
      -- Determines whether the window is fullscreen or not, and how much
      -- control the user has over its position/size.
      --    0  Windowed, resizable
      --    1  Windowed, not resizable
      --    2  Windowed, undecorated, resizable
      --    3  Windowed, undecorated, not resizable
      --    7  Windowed Fullscreen
      --    8  Exclusive Fullscreen
      window_mode       INTEGER NOT NULL,
      -- If 0, the window should be moved to `position_x`,`position_y` after it
      -- is created.  Ignored in fullscreen exclusive mode.
      system_positioned INTEGER NOT NULL,
      maximized         INTEGER NOT NULL,
      save_pos_on_close INTEGER NOT NULL,
      position_x        INTEGER NOT NULL,
      position_y        INTEGER NOT NULL,
      size_x            INTEGER NOT NULL,
      size_y            INTEGER NOT NULL,
      monitor_index     INTEGER NOT NULL,
      
      refresh_rate      INTEGER NOT NULL,
      v_sync            INTEGER NOT NULL,
      msaa_level        INTEGER NOT NULL,
      
      red_bits          INTEGER NOT NULL,
      green_bits        INTEGER NOT NULL,
      blue_bits         INTEGER NOT NULL,
      alpha_bits        INTEGER NOT NULL,
      depth_bits        INTEGER NOT NULL,
      stencil_bits      INTEGER NOT NULL,
      
      srgb_capable      INTEGER NOT NULL,
      use_custom_gamma  INTEGER NOT NULL,
      custom_gamma      REAL NOT NULL,
      
      PRIMARY KEY (id, history_index)
);
