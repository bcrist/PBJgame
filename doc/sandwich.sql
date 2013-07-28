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
