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
/// \file   be/wnd/window_settings.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of be::wnd::WindowSettings functions

#include "be/wnd/window_settings.h"

#include "be/bed/stmt_cache.h"
#include "be/bed/transaction.h"
#include "be/bed/bed_open.h"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to load a set of window settings from a bed.
/// \param  1 The id of the winow settings stack to load.
#define BE_WND_WINDOW_SETTINGS_SQL_LOAD \
      "SELECT window_mode, " \
      "system_positioned, save_pos_on_close, position_x, position_y, " \
      "size_x, size_y, monitor_index, refresh_rate, v_sync, " \
      "msaa_level, red_bits, green_bits, blue_bits, alpha_bits, " \
      "depth_bits, stencil_bits, " \
      "srgb_capable, use_custom_gamma, custom_gamma, " \
      "context_ver_major, context_ver_minor, forward_compatible, " \
      "debug_context, profile_type " \
      "FROM be_wnd_window_settings WHERE id = ? " \
      "ORDER BY history_index DESC LIMIT 1"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to check if the be_wnd_window_settings table exists
///         in a bed.
#define BE_WND_WINDOW_SETTINGS_SQL_TABLE_EXISTS \
      "SELECT count(*) FROM sqlite_master " \
      "WHERE type='table' AND name='be_wnd_window_settings'"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to get the active (highest) history_index for the
///         requested window settings.
/// \param  1 The id of the window settings stack to look at.
#define BE_WND_WINDOW_SETTINGS_SQL_LATEST_INDEX \
      "SELECT max(history_index) " \
      "FROM be_wnd_window_settings WHERE id = ?"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to create the be_wnd_window_settings table.
#define BE_WND_WINDOW_SETTINGS_SQL_CREATE_TABLE \
      "CREATE TABLE IF NOT EXISTS be_wnd_window_settings (" \
      "id INTEGER NOT NULL, " \
      "history_index INTEGER NOT NULL, " \
      "window_mode INTEGER NOT NULL, " \
      "system_positioned INTEGER NOT NULL, " \
      "save_pos_on_close INTEGER NOT NULL, " \
      "position_x INTEGER NOT NULL, " \
      "position_y INTEGER NOT NULL, " \
      "size_x INTEGER NOT NULL, " \
      "size_y INTEGER NOT NULL, " \
      "monitor_index INTEGER NOT NULL, " \
      "refresh_rate INTEGER NOT NULL, " \
      "v_sync INTEGER NOT NULL, " \
      "msaa_level INTEGER NOT NULL, " \
      "red_bits INTEGER NOT NULL, " \
      "green_bits INTEGER NOT NULL, " \
      "blue_bits INTEGER NOT NULL, " \
      "alpha_bits INTEGER NOT NULL, " \
      "depth_bits INTEGER NOT NULL, " \
      "stencil_bits INTEGER NOT NULL, " \
      "srgb_capable INTEGER NOT NULL, " \
      "use_custom_gamma INTEGER NOT NULL, " \
      "custom_gamma REAL NOT NULL, " \
      "context_ver_major INTEGER NOT NULL, " \
      "context_ver_minor INTEGER NOT NULL, " \
      "forward_compatible INTEGER NOT NULL, " \
      "debug_context INTEGER NOT NULL, " \
      "profile_type INTEGER NOT NULL, " \
      "PRIMARY KEY (id, history_index) )"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to save a set of window settings using a specific id
///         and history index.
///
/// \details The history_index should be incremented from te result of
///         #BE_WND_WINDOW_SETTINGS_SQL_LATEST_INDEX so that the settings can
///         be reverted if desired.
/// \param  1 The id of the window_settings stack to save to.
/// \param  2 The history_index of the entry to save to.
/// \param  3 The value of be::wnd::WindowSettings::mode.
/// \param  4 The value of be::wnd::WindowSettings::system_positioned.
/// \param  5 The value of be::wnd::WindowSettings::save_position_on_close.
/// \param  6 The value of be::wnd::WindowSettings::position.x.
/// \param  7 The value of be::wnd::WindowSettings::position.y.
/// \param  8 The value of be::wnd::WindowSettings::size.x.
/// \param  9 The value of be::wnd::WindowSettings::size.y.
/// \param  10 The value of be::wnd::WindowSettings::monitor_index.
/// \param  11 The value of be::wnd::WindowSettings::refresh_rate.
/// \param  12 The value of be::wnd::WindowSettings::v_sync.
/// \param  13 The value of be::wnd::WindowSettings::msaa_level.
/// \param  14 The value of be::wnd::WindowSettings::red_bits.
/// \param  15 The value of be::wnd::WindowSettings::green_bits.
/// \param  16 The value of be::wnd::WindowSettings::blue_bits.
/// \param  17 The value of be::wnd::WindowSettings::alpha_bits.
/// \param  18 The value of be::wnd::WindowSettings::depth_bits.
/// \param  19 The value of be::wnd::WindowSettings::stencil_bits.
/// \param  20 The value of be::wnd::WindowSettings::srgb_capable.
/// \param  21 The value of be::wnd::WindowSettings::use_custom_gamma.
/// \param  22 The value of be::wnd::WindowSettings::custom_gamma.
/// \param  23 The value of be::wnd::WindowSettings::context_version_major.
/// \param  24 The value of be::wnd::WindowSettings::context_version_minor.
/// \param  25 The value of be::wnd::WindowSettings::forward_compatible_context.
/// \param  26 The value of be::wnd::WindowSettings::debug_context.
/// \param  27 The value of be::wnd::WindowSettings::context_profile_type.
#define BE_WND_WINDOW_SETTINGS_SQL_SAVE \
      "INSERT INTO be_wnd_window_settings (" \
      "id, history_index, window_mode, " \
      "system_positioned, save_pos_on_close, position_x, position_y, " \
      "size_x, size_y, monitor_index, refresh_rate, v_sync, " \
      "msaa_level, red_bits, green_bits, blue_bits, alpha_bits, " \
      "depth_bits, stencil_bits, " \
      "srgb_capable, use_custom_gamma, custom_gamma, " \
      "context_ver_major, context_ver_minor, forward_compatible, " \
      "debug_context, profile_type" \
      ") VALUES (" \
      "?,?,?,?,?,?,?,?,?,?," \
      "?,?,?,?,?,?,?,?,?,?," \
      "?,?,?,?,?,?)"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to update the position and size settings from a
///         specific id and history_index.
///
/// \param  1 The new x position.
/// \param  2 The new y position.
/// \param  3 The new width.
/// \param  4 The new height.
/// \param  5 The id of the window settings stack to update.
/// \param  6 The history_index to update.
#define BE_WND_WINDOW_SETTINGS_SQL_SAVE_POS \
      "UPDATE be_wnd_window_settings " \
      "SET position_x = ?, position_y = ?, " \
      "size_x = ?, size_y = ? " \
      "WHERE id = ? AND history_index = ?"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to remove any history entries with the id provided 
///         and an history_index less than the provided minimum.
///
/// \param  1 The id of the window settings stack to truncate.
/// \param  2 The minimum history_index that will remain untouched.
#define BE_WND_WINDOW_SETTINGS_SQL_TRUNCATE \
      "DELETE FROM be_wnd_window_settings " \
      "WHERE id = ? AND history_index < ?"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to remove a specific history_index from a window
///         settings stack.
///
/// \details The history_index should almost always be retrieved using
///         #BE_WND_WINDOW_SETTINGS_SQL_LATEST_INDEX.
///
/// \param  1 The id of the window_settings stack to modify.
/// \param  2 This history_index to delete.
#define BE_WND_WINDOW_SETTINGS_SQL_REVERT \
      "DELETE FROM be_wnd_window_settings " \
      "WHERE id = ? AND history_index = ?"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to compress history_index values by subtracting a
///         constant value from all history_index values in a window settings
///         stack.
///
/// \details This should generally only be done after
///         #BE_WND_WINDOW_SETTINGS_SQL_TRUNCATE.
///
/// \param  1 The value of the history_index which should become zero after
///         this statement.
/// \param  2 The id of the window_settings stack to modify.
#define BE_WND_WINDOW_SETTINGS_SQL_COMPRESS \
      "UPDATE be_wnd_window_settings " \
      "SET history_index = history_index - ? " \
      "WHERE id = ?"

#ifdef BE_ID_NAMES_ENABLED
#define BE_WND_WINDOW_SETTINGS_SQLID_LOAD         BE_WND_WINDOW_SETTINGS_SQL_LOAD
#define BE_WND_WINDOW_SETTINGS_SQLID_TABLE_EXISTS BE_WND_WINDOW_SETTINGS_SQL_TABLE_EXISTS
#define BE_WND_WINDOW_SETTINGS_SQLID_CREATE_TABLE BE_WND_WINDOW_SETTINGS_SQL_CREATE_TABLE
#define BE_WND_WINDOW_SETTINGS_SQLID_LATEST_INDEX BE_WND_WINDOW_SETTINGS_SQL_LATEST_INDEX
#define BE_WND_WINDOW_SETTINGS_SQLID_SAVE         BE_WND_WINDOW_SETTINGS_SQL_SAVE
#define BE_WND_WINDOW_SETTINGS_SQLID_SAVE_POS     BE_WND_WINDOW_SETTINGS_SQL_SAVE_POS
#define BE_WND_WINDOW_SETTINGS_SQLID_TRUNCATE     BE_WND_WINDOW_SETTINGS_SQL_TRUNCATE
#define BE_WND_WINDOW_SETTINGS_SQLID_REVERT       BE_WND_WINDOW_SETTINGS_SQL_REVERT
#define BE_WND_WINDOW_SETTINGS_SQLID_COMPRESS     BE_WND_WINDOW_SETTINGS_SQL_COMPRESS
#else
#define BE_WND_WINDOW_SETTINGS_SQLID_LOAD         0xf87b0cbda9871ec2
#define BE_WND_WINDOW_SETTINGS_SQLID_TABLE_EXISTS 0x1f291c4330f2e631
#define BE_WND_WINDOW_SETTINGS_SQLID_CREATE_TABLE 0x5be45399e0383389
#define BE_WND_WINDOW_SETTINGS_SQLID_LATEST_INDEX 0xd76d8693b84a643c
#define BE_WND_WINDOW_SETTINGS_SQLID_SAVE         0x4a8f712d4305761c
#define BE_WND_WINDOW_SETTINGS_SQLID_SAVE_POS     0xf9e73cb74a980b0c
#define BE_WND_WINDOW_SETTINGS_SQLID_TRUNCATE     0xdacc0b7fdef69265
#define BE_WND_WINDOW_SETTINGS_SQLID_REVERT       0xe29c347fe32614fa
#define BE_WND_WINDOW_SETTINGS_SQLID_COMPRESS     0x7c719307914c91fb
#endif

namespace be {
namespace wnd {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a WindowSettings object with basic default settings
///         which should work on most hardware.
WindowSettings::WindowSettings()
   : mode(MResizable),
     system_positioned(true),
     save_position_on_close(false),
     size(640, 480),
     monitor_index(-1),
     refresh_rate(0),
     v_sync(VSMDisabled),
     msaa_level(0),
     red_bits(8),
     green_bits(8),
     blue_bits(8),
     alpha_bits(8),
     depth_bits(24),
     stencil_bits(0),
     srgb_capable(true),
     use_custom_gamma(false),
     custom_gamma(2.2f),
     context_version_major(1),
     context_version_minor(0),
     forward_compatible_context(false),
     debug_context(false),
     context_profile_type(CPTAnyProfile)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Load a set of WindowSettings from a bed.
///
/// \details Each WindowSettings Id represents a stack of current and previous
///         values.  This allows the user to revert to previous settings if
///         new settings are tried and not supported.
///
/// \param  bed The bed to load from.
/// \param  id The Id of the WindowSettings stack to load.
///
/// \ingroup loading
WindowSettings loadWindowSettings(bed::Bed& bed, const Id& id)
{
   try
   {
      bed::StmtCache& cache = bed.getStmtCache();
      bed::CachedStmt& stmt = cache.hold(Id(BE_WND_WINDOW_SETTINGS_SQLID_LOAD), BE_WND_WINDOW_SETTINGS_SQL_LOAD);

      stmt.bind(1, id.value());
      if (stmt.step())
      {
         WindowSettings ws;
         ws.id.bed = bed.getId();
         ws.id.asset = id;

         ws.mode = static_cast<WindowSettings::Mode>(stmt.getInt(0));
         ws.system_positioned = stmt.getBool(1);
         ws.save_position_on_close = stmt.getBool(2);
         ws.position.x = stmt.getInt(3);
         ws.position.y = stmt.getInt(4);
         ws.size.x = stmt.getInt(5);
         ws.size.y = stmt.getInt(6);
         ws.monitor_index = stmt.getInt(7);
         ws.refresh_rate = stmt.getUInt(8);
         ws.v_sync = static_cast<WindowSettings::VSyncMode>(stmt.getInt(9));
         ws.msaa_level = stmt.getUInt(10);
         ws.red_bits = stmt.getUInt(11);
         ws.green_bits = stmt.getUInt(12);
         ws.blue_bits = stmt.getUInt(13);
         ws.alpha_bits = stmt.getUInt(14);
         ws.depth_bits = stmt.getUInt(15);
         ws.stencil_bits = stmt.getUInt(16);
         ws.srgb_capable = stmt.getBool(17);
         ws.use_custom_gamma = stmt.getBool(18);
         ws.custom_gamma = float(stmt.getDouble(19));
         ws.context_version_major = stmt.getUInt(20);
         ws.context_version_minor = stmt.getUInt(21);
         ws.forward_compatible_context = stmt.getBool(22);
         ws.debug_context = stmt.getBool(23);
         ws.context_profile_type = static_cast<WindowSettings::ContextProfileType>(stmt.getInt(24));

         return ws;
      }
      else
         throw std::runtime_error("WindowSettings record not found!");
   }
   catch (const bed::Db::error& err)
   {
      BE_LOG(VWarning) << "Database error while loading window settings!" << BE_LOG_NL
                       << "              Bed: " << bed.getId() << BE_LOG_NL
                       << "WindowSettings ID: " << id << BE_LOG_NL
                       << "        Exception: " << err.what() << BE_LOG_NL
                       << "              SQL: " << err.sql() << BE_LOG_END;
   }
   catch (const std::runtime_error& err)
   {
      BE_LOG(VWarning) << "Exception while loading window settings!" << BE_LOG_NL
                       << "              Bed: " << bed.getId() << BE_LOG_NL
                       << "WindowSettings ID: " << id << BE_LOG_NL
                       << "        Exception: " << err.what() << BE_LOG_END;
   }

   return WindowSettings();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Saves a set of WindowSettings to a bed.
///
/// \details The bed and Id that are saved to are determined by the id field
///         of the WindowSettings object passed in.  If the WindowSettings 
///         already exist in the bed, a new history index will be created.
///
///         If there is a problem saving the WindowSettings, a warning will
///         be emitted, and false will be returned.
///
/// \param  window_settings The WindowSettings to save.
/// \return \c true if the WindowSettings were saved successfully.
///
/// \ingroup loading
bool saveWindowSettings(const WindowSettings& window_settings)
{
   try
   {
      std::shared_ptr<bed::Bed> bed = bed::openWritable(window_settings.id.bed);
      if (!bed)
         throw std::runtime_error("Could not open bed for writing!");

      bed::Db& db = bed->getDb();

      bed::Transaction transaction(db, bed::Transaction::Immediate);

      int history_index = 0;
   
      if (db.getInt(BE_WND_WINDOW_SETTINGS_SQL_TABLE_EXISTS, 0) == 0)
      {
         db.exec(BE_WND_WINDOW_SETTINGS_SQL_CREATE_TABLE);
      }
      else
      {
         bed::Stmt latest(db, Id(BE_WND_WINDOW_SETTINGS_SQLID_LATEST_INDEX), BE_WND_WINDOW_SETTINGS_SQL_LATEST_INDEX);
         latest.bind(1, window_settings.id.asset.value());
         if (latest.step())
            history_index = latest.getInt(0);
      }

      bed::Stmt save(db, Id(BE_WND_WINDOW_SETTINGS_SQLID_SAVE), BE_WND_WINDOW_SETTINGS_SQL_SAVE);
      save.bind(1, window_settings.id.asset.value());
      save.bind(2, history_index + 1);
      save.bind(3, static_cast<int>(window_settings.mode));
      save.bind(4, window_settings.system_positioned);
      save.bind(5, window_settings.save_position_on_close);
      save.bind(6, window_settings.position.x);
      save.bind(7, window_settings.position.y);
      save.bind(8, window_settings.size.x);
      save.bind(9, window_settings.size.y);
      save.bind(10, window_settings.monitor_index);
      save.bind(11, window_settings.refresh_rate);
      save.bind(12, static_cast<int>(window_settings.v_sync));
      save.bind(13, window_settings.msaa_level);
      save.bind(14, window_settings.red_bits);
      save.bind(15, window_settings.green_bits);
      save.bind(16, window_settings.blue_bits);
      save.bind(17, window_settings.alpha_bits);
      save.bind(18, window_settings.depth_bits);
      save.bind(19, window_settings.stencil_bits);
      save.bind(20, window_settings.srgb_capable);
      save.bind(21, window_settings.use_custom_gamma);
      save.bind(22, window_settings.custom_gamma);
      save.bind(23, window_settings.context_version_major);
      save.bind(24, window_settings.context_version_minor);
      save.bind(25, window_settings.forward_compatible_context ? 1 : 0);
      save.bind(26, window_settings.debug_context ? 1 : 0);
      save.bind(27, static_cast<int>(window_settings.context_profile_type));
      
      save.step();
      transaction.commit();
      return true;
   }
   catch (const bed::Db::error& err)
   {
      BE_LOG(VWarning) << "Database error while saving window settings!" << BE_LOG_NL
                       << "              Bed: " << window_settings.id.bed << BE_LOG_NL
                       << "WindowSettings ID: " << window_settings.id.asset << BE_LOG_NL
                       << "        Exception: " << err.what() << BE_LOG_NL
                       << "              SQL: " << err.sql() << BE_LOG_END;
   }
   catch (const std::runtime_error& err)
   {
      BE_LOG(VWarning) << "Exception while saving window settings!" << BE_LOG_NL
                       << "              Bed: " << window_settings.id.bed << BE_LOG_NL
                       << "WindowSettings ID: " << window_settings.id.asset << BE_LOG_NL
                       << "        Exception: " << err.what() << BE_LOG_END;
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Updates the position and size properties of an existing saved
///         WindowSettings object.
///
/// \details The WindowSettings object will be found by looking at the AssetId
///         found in window_settings.id.  The history index updated will be
///         the most recent one.  Only the size and position properties will
///         be updated, and only if window_settings.save_position_on_close
///         is true.
///
/// \param  window_settings Determines where the WindowSettings are saved and
///         the new position/size values to update.
/// \return \c true if the size and position were updated successfully.
///
/// \ingroup loading
bool updateSavedPosition(const WindowSettings& window_settings)
{
   if (!window_settings.save_position_on_close)
      return false;

   try {
      std::shared_ptr<bed::Bed> bed = bed::openWritable(window_settings.id.bed);
      if (!bed)
         throw std::runtime_error("Could not open bed for writing!");

      bed::Db& db = bed->getDb();
  
      if (db.getInt(BE_WND_WINDOW_SETTINGS_SQL_TABLE_EXISTS, 0) == 0)
         throw std::runtime_error("WindowSettings table does not exist!");

      bed::Transaction transaction(db, bed::Transaction::Immediate);

      int history_index = 0;
      bed::Stmt latest(db, Id(BE_WND_WINDOW_SETTINGS_SQLID_LATEST_INDEX), BE_WND_WINDOW_SETTINGS_SQL_LATEST_INDEX);
      latest.bind(1, window_settings.id.asset.value());
      if (latest.step())
         history_index = latest.getInt(0);

      bed::Stmt save(db, Id(BE_WND_WINDOW_SETTINGS_SQLID_SAVE_POS), BE_WND_WINDOW_SETTINGS_SQL_SAVE_POS);
      save.bind(1, window_settings.position.x);
      save.bind(2, window_settings.position.y);
      save.bind(3, window_settings.size.x);
      save.bind(4, window_settings.size.y);
      save.bind(5, window_settings.id.asset.value());
      save.bind(6, history_index);

      save.step();
      transaction.commit();
      return true;
   }
   catch (const bed::Db::error& err)
   {
      BE_LOG(VWarning) << "Database error while saving window position!" << BE_LOG_NL
                       << "              Bed: " << window_settings.id.bed << BE_LOG_NL
                       << "WindowSettings ID: " << window_settings.id.asset << BE_LOG_NL
                       << "        Exception: " << err.what() << BE_LOG_NL
                       << "              SQL: " << err.sql() << BE_LOG_END;
   }
   catch (const std::runtime_error& err)
   {
      BE_LOG(VWarning) << "Exception while saving window position!" << BE_LOG_NL
                       << "              Bed: " << window_settings.id.bed << BE_LOG_NL
                       << "WindowSettings ID: " << window_settings.id.asset << BE_LOG_NL
                       << "        Exception: " << err.what() << BE_LOG_END;
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Deletes the most recent history index of the specified
///         WindowSettings object.
/// 
/// \param  id The AssetId defining the WindowSettings stack to revert.
/// \return The next most recent history index for the WindowSettings object.
///         If there is a problem deleting the most recent window settings or
///         if there is no next most recent history index, a
///         default-constructed WindowSettings object will be returned.
///
/// \ingroup loading
WindowSettings revertWindowSettings(const AssetId& id)
{
   try
   {
      std::shared_ptr<bed::Bed> bed = bed::openWritable(id.bed);
      if (!bed)
         throw std::runtime_error("Could not open bed for writing!");

      bed::Db& db = bed->getDb();
  
      if (db.getInt(BE_WND_WINDOW_SETTINGS_SQL_TABLE_EXISTS, 0) == 0)
         throw std::runtime_error("WindowSettings table does not exist!");

      bed::Transaction transaction(db, bed::Transaction::Immediate);

      int history_index = 0;
      bed::Stmt latest(db, BE_WND_WINDOW_SETTINGS_SQLID_LATEST_INDEX);
      latest.bind(1, id.asset.value());
      if (latest.step())
         history_index = latest.getInt(0);

      bed::Stmt remove(db, BE_WND_WINDOW_SETTINGS_SQLID_REVERT);
      remove.bind(1, id.asset.value());
      remove.bind(2, history_index);
      remove.step();

      transaction.commit();
      return loadWindowSettings(*bed, id.asset);
   }
   catch (const bed::Db::error& err)
   {
      BE_LOG(VWarning) << "Database error while reverting window settings!" << BE_LOG_NL
                       << "              Bed: " << id.bed << BE_LOG_NL
                       << "WindowSettings ID: " << id.asset << BE_LOG_NL
                       << "        Exception: " << err.what() << BE_LOG_NL
                       << "              SQL: " << err.sql() << BE_LOG_END;
   }
   catch (const std::runtime_error& err)
   {
      BE_LOG(VWarning) << "Exception while reverting window settings!" << BE_LOG_NL
                       << "              Bed: " << id.bed << BE_LOG_NL
                       << "WindowSettings ID: " << id.asset << BE_LOG_NL
                       << "        Exception: " << err.what() << BE_LOG_END;
   }

   return WindowSettings();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  If there are lots of history entries, removes all old entries,
///         leaving only the most recently created ones.
///
/// \details If history entires are removed, the remaining entries are
///         compressed so that their indices start at 0 for the oldest entry.
///
/// \param  id The AssetId of the WindowSettings stack to truncate.
/// \param  max_history_entries The number of entries to leave in the database.
///
/// \ingroup loading
void truncateWindowSettingsHistory(const AssetId& id, int max_history_entries)
{
   try
   {
      std::shared_ptr<bed::Bed> bed = bed::openWritable(id.bed);
      if (!bed)
         throw std::runtime_error("Could not open bed for writing!");

      bed::Db& db = bed->getDb();
  
      if (db.getInt(BE_WND_WINDOW_SETTINGS_SQL_TABLE_EXISTS, 0) == 0)
         throw std::runtime_error("WindowSettings table does not exist!");

      bed::Transaction transaction(db, bed::Transaction::Immediate);

      int history_index = 0;
      bed::Stmt latest(db, Id(BE_WND_WINDOW_SETTINGS_SQLID_LATEST_INDEX), BE_WND_WINDOW_SETTINGS_SQL_LATEST_INDEX);
      latest.bind(1, id.asset.value());
      if (latest.step())
         history_index = latest.getInt(0);

      history_index -= max_history_entries - 1;

      // remove old history entries
      bed::Stmt remove(db, Id(BE_WND_WINDOW_SETTINGS_SQLID_TRUNCATE), BE_WND_WINDOW_SETTINGS_SQL_TRUNCATE);
      remove.bind(1, id.asset.value());
      remove.bind(2, history_index);
      remove.step();

      // compress remaining history indices closer to 0
      if (history_index > 1)
      {
         bed::Stmt compress(db, Id(BE_WND_WINDOW_SETTINGS_SQLID_COMPRESS), BE_WND_WINDOW_SETTINGS_SQL_COMPRESS);
         compress.bind(2, id.asset.value());
         compress.bind(1, history_index - 1);
         compress.step();
      }

      transaction.commit();
   }
   catch (const bed::Db::error& err)
   {
      BE_LOG(VWarning) << "Database error while truncating window settings history!" << BE_LOG_NL
                       << "              Bed: " << id.bed << BE_LOG_NL
                       << "WindowSettings ID: " << id.asset << BE_LOG_NL
                       << "        Exception: " << err.what() << BE_LOG_NL
                       << "              SQL: " << err.sql() << BE_LOG_END;
   }
   catch (const std::runtime_error& err)
   {
      BE_LOG(VWarning) << "Exception while truncating window settings history!" << BE_LOG_NL
                       << "              Bed: " << id.bed << BE_LOG_NL
                       << "WindowSettings ID: " << id.asset << BE_LOG_NL
                       << "        Exception: " << err.what() << BE_LOG_END;
   }
}

} // namespace be::wnd
} // namespace be
