///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/window_settings.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of be::wnd::WindowSettings functions

#include "pbj/window_settings.h"

#include "be/bed/transaction.h"
#include "pbj/sw/sandwich_open.h"
#include <iostream>

#pragma region SQL statements

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to load a set of window settings from a sandwich.
/// \param  1 The id of the winow settings stack to load.
#define PBJ_WINDOW_SETTINGS_SQL_LOAD \
      "SELECT window_mode, " \
      "system_positioned, save_pos_on_close, position_x, position_y, " \
      "size_x, size_y, monitor_index, refresh_rate, v_sync, " \
      "msaa_level, red_bits, green_bits, blue_bits, alpha_bits, " \
      "depth_bits, stencil_bits, " \
      "srgb_capable, use_custom_gamma, custom_gamma " \
      "FROM sw_window_settings WHERE id = ? " \
      "ORDER BY history_index DESC LIMIT 1"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to check if the sw_window_settings table exists
///         in a sandwich.
#define PBJ_WINDOW_SETTINGS_SQL_TABLE_EXISTS \
      "SELECT count(*) FROM sqlite_master " \
      "WHERE type='table' AND name='sw_window_settings'"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to get the active (highest) history_index for the
///         requested window settings.
/// \param  1 The id of the window settings stack to look at.
#define PBJ_WINDOW_SETTINGS_SQL_LATEST_INDEX \
      "SELECT max(history_index) " \
      "FROM sw_window_settings WHERE id = ?"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to create the sw_window_settings table.
#define PBJ_WINDOW_SETTINGS_SQL_CREATE_TABLE \
      "CREATE TABLE IF NOT EXISTS sw_window_settings (" \
      "id INTEGER NOT NULL, " \
      "history_index INTEGER NOT NULL, " \
      "window_mode INTEGER NOT NULL, " \
      "system_positioned INTEGER NOT NULL, " \
      "maximized INTEGER NOT NULL, " \
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
      "custom_gamma REAL NOT NULL " \
      "PRIMARY KEY (id, history_index) )"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to save a set of window settings using a specific id
///         and history index.
///
/// \details The history_index should be incremented from te result of
///         #PBJ_WINDOW_SETTINGS_SQL_LATEST_INDEX so that the settings can
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
#define PBJ_WINDOW_SETTINGS_SQL_SAVE \
      "INSERT INTO sw_window_settings (" \
      "id, history_index, window_mode, " \
      "system_positioned, save_pos_on_close, position_x, position_y, " \
      "size_x, size_y, monitor_index, refresh_rate, v_sync, " \
      "msaa_level, red_bits, green_bits, blue_bits, alpha_bits, " \
      "depth_bits, stencil_bits, " \
      "srgb_capable, use_custom_gamma, custom_gamma" \
      ") VALUES (" \
      "?,?,?," \
      "?,?,?,?," \
      "?,?,?,?,?," \
      "?,?,?,?,?," \
      "?,?," \
      "?,?,?)"

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
#define PBJ_WINDOW_SETTINGS_SQL_SAVE_POS \
      "UPDATE sw_window_settings " \
      "SET position_x = ?, position_y = ?, " \
      "size_x = ?, size_y = ? " \
      "WHERE id = ? AND history_index = ?"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to remove any history entries with the id provided 
///         and an history_index less than the provided minimum.
///
/// \param  1 The id of the window settings stack to truncate.
/// \param  2 The minimum history_index that will remain untouched.
#define PBJ_WINDOW_SETTINGS_SQL_TRUNCATE \
      "DELETE FROM sw_window_settings " \
      "WHERE id = ? AND history_index < ?"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to remove a specific history_index from a window
///         settings stack.
///
/// \details The history_index should almost always be retrieved using
///         #PBJ_WINDOW_SETTINGS_SQL_LATEST_INDEX.
///
/// \param  1 The id of the window_settings stack to modify.
/// \param  2 This history_index to delete.
#define PBJ_WINDOW_SETTINGS_SQL_REVERT \
      "DELETE FROM sw_window_settings " \
      "WHERE id = ? AND history_index = ?"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to compress history_index values by subtracting a
///         constant value from all history_index values in a window settings
///         stack.
///
/// \details This should generally only be done after
///         #PBJ_WINDOW_SETTINGS_SQL_TRUNCATE.
///
/// \param  1 The value of the history_index which should become zero after
///         this statement.
/// \param  2 The id of the window_settings stack to modify.
#define PBJ_WINDOW_SETTINGS_SQL_COMPRESS \
      "UPDATE sw_window_settings " \
      "SET history_index = history_index - ? " \
      "WHERE id = ?"

#ifdef BE_ID_NAMES_ENABLED
#define PBJ_WINDOW_SETTINGS_SQLID_LOAD         PBJ_WINDOW_SETTINGS_SQL_LOAD
#define PBJ_WINDOW_SETTINGS_SQLID_TABLE_EXISTS PBJ_WINDOW_SETTINGS_SQL_TABLE_EXISTS
#define PBJ_WINDOW_SETTINGS_SQLID_CREATE_TABLE PBJ_WINDOW_SETTINGS_SQL_CREATE_TABLE
#define PBJ_WINDOW_SETTINGS_SQLID_LATEST_INDEX PBJ_WINDOW_SETTINGS_SQL_LATEST_INDEX
#define PBJ_WINDOW_SETTINGS_SQLID_SAVE         PBJ_WINDOW_SETTINGS_SQL_SAVE
#define PBJ_WINDOW_SETTINGS_SQLID_SAVE_POS     PBJ_WINDOW_SETTINGS_SQL_SAVE_POS
#define PBJ_WINDOW_SETTINGS_SQLID_TRUNCATE     PBJ_WINDOW_SETTINGS_SQL_TRUNCATE
#define PBJ_WINDOW_SETTINGS_SQLID_REVERT       PBJ_WINDOW_SETTINGS_SQL_REVERT
#define PBJ_WINDOW_SETTINGS_SQLID_COMPRESS     PBJ_WINDOW_SETTINGS_SQL_COMPRESS
#else
// TODO: precalculate ids using idgen.exe
#define PBJ_WINDOW_SETTINGS_SQLID_LOAD         PBJ_WINDOW_SETTINGS_SQL_LOAD
#define PBJ_WINDOW_SETTINGS_SQLID_TABLE_EXISTS PBJ_WINDOW_SETTINGS_SQL_TABLE_EXISTS
#define PBJ_WINDOW_SETTINGS_SQLID_CREATE_TABLE PBJ_WINDOW_SETTINGS_SQL_CREATE_TABLE
#define PBJ_WINDOW_SETTINGS_SQLID_LATEST_INDEX PBJ_WINDOW_SETTINGS_SQL_LATEST_INDEX
#define PBJ_WINDOW_SETTINGS_SQLID_SAVE         PBJ_WINDOW_SETTINGS_SQL_SAVE
#define PBJ_WINDOW_SETTINGS_SQLID_SAVE_POS     PBJ_WINDOW_SETTINGS_SQL_SAVE_POS
#define PBJ_WINDOW_SETTINGS_SQLID_TRUNCATE     PBJ_WINDOW_SETTINGS_SQL_TRUNCATE
#define PBJ_WINDOW_SETTINGS_SQLID_REVERT       PBJ_WINDOW_SETTINGS_SQL_REVERT
#define PBJ_WINDOW_SETTINGS_SQLID_COMPRESS     PBJ_WINDOW_SETTINGS_SQL_COMPRESS
/* These are outdated:
#define PBJ_WINDOW_SETTINGS_SQLID_LOAD         0xf87b0cbda9871ec2
#define PBJ_WINDOW_SETTINGS_SQLID_TABLE_EXISTS 0x1f291c4330f2e631
#define PBJ_WINDOW_SETTINGS_SQLID_CREATE_TABLE 0x5be45399e0383389
#define PBJ_WINDOW_SETTINGS_SQLID_LATEST_INDEX 0xd76d8693b84a643c
#define PBJ_WINDOW_SETTINGS_SQLID_SAVE         0x4a8f712d4305761c
#define PBJ_WINDOW_SETTINGS_SQLID_SAVE_POS     0xf9e73cb74a980b0c
#define PBJ_WINDOW_SETTINGS_SQLID_TRUNCATE     0xdacc0b7fdef69265
#define PBJ_WINDOW_SETTINGS_SQLID_REVERT       0xe29c347fe32614fa
#define PBJ_WINDOW_SETTINGS_SQLID_COMPRESS     0x7c719307914c91fb
*/
#endif

#pragma endregion

namespace pbj {

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
      custom_gamma(2.2f)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Load a set of WindowSettings from a sandwich.
///
/// \details Each WindowSettings Id represents a stack of current and previous
///         values.  This allows the user to revert to previous settings if
///         new settings are tried and not supported.
///
/// \param  sandwich The Sandwich to load from.
/// \param  id The Id of the WindowSettings stack to load.
///
/// \ingroup loading
WindowSettings loadWindowSettings(sw::Sandwich& sandwich, const Id& id)
{
    try
    {
        db::StmtCache& cache = sandwich.getStmtCache();
        db::CachedStmt& stmt = cache.hold(Id(PBJ_WINDOW_SETTINGS_SQLID_LOAD), PBJ_WINDOW_SETTINGS_SQL_LOAD);

        stmt.bind(1, id.value());
        if (stmt.step())
        {
            WindowSettings ws;
            ws.id.sandwich = sandwich.getId();
            ws.id.resource = id;

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

            return ws;
        }
        else
            throw std::runtime_error("WindowSettings record not found!");
    }
    catch (const db::Db::error& err)
    {
        PBJ_LOG(VWarning) << "Database error while loading window settings!" << PBJ_LOG_NL
                          << "      Sandwich ID: " << sandwich.getId() << PBJ_LOG_NL
                          << "WindowSettings ID: " << id << PBJ_LOG_NL
                          << "        Exception: " << err.what() << PBJ_LOG_NL
                          << "              SQL: " << err.sql() << PBJ_LOG_END;
    }
    catch (const std::runtime_error& err)
    {
        PBJ_LOG(VWarning) << "Exception while loading window settings!" << PBJ_LOG_NL
                          << "      Sandwich ID: " << sandwich.getId() << PBJ_LOG_NL
                          << "WindowSettings ID: " << id << PBJ_LOG_NL
                          << "        Exception: " << err.what() << PBJ_LOG_END;
    }

    return WindowSettings();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Saves a set of WindowSettings to a sandwich.
///
/// \details The sandwich and Id that are saved to are determined by the id field
///         of the WindowSettings object passed in.  If the WindowSettings 
///         already exist in the sandwich, a new history index will be created.
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
        std::shared_ptr<sw::Sandwich> sandwich = sw::openWritable(window_settings.id.sandwich);
        if (!sandwich)
            throw std::runtime_error("Could not open sandwich for writing!");

        db::Db& db = sandwich->getDb();

        db::Transaction transaction(db, db::Transaction::Immediate);

        int history_index = 0;
   
        if (db.getInt(PBJ_WINDOW_SETTINGS_SQL_TABLE_EXISTS, 0) == 0)
        {
            db.exec(PBJ_WINDOW_SETTINGS_SQL_CREATE_TABLE);
        }
        else
        {
            db::Stmt latest(db, Id(PBJ_WINDOW_SETTINGS_SQLID_LATEST_INDEX), PBJ_WINDOW_SETTINGS_SQL_LATEST_INDEX);
            latest.bind(1, window_settings.id.resource.value());
            if (latest.step())
                history_index = latest.getInt(0);
        }

        db::Stmt save(db, Id(PBJ_WINDOW_SETTINGS_SQLID_SAVE), PBJ_WINDOW_SETTINGS_SQL_SAVE);
        save.bind(1, window_settings.id.resource.value());
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
      
        save.step();
        transaction.commit();
        return true;
    }
    catch (const db::Db::error& err)
    {
        PBJ_LOG(VWarning) << "Database error while saving window settings!" << PBJ_LOG_NL
                          << "      Sandwich ID: " << window_settings.id.sandwich << PBJ_LOG_NL
                          << "WindowSettings ID: " << window_settings.id.resource << PBJ_LOG_NL
                          << "        Exception: " << err.what() << PBJ_LOG_NL
                          << "              SQL: " << err.sql() << PBJ_LOG_END;
    }
    catch (const std::runtime_error& err)
    {
        PBJ_LOG(VWarning) << "Exception while saving window settings!" << PBJ_LOG_NL
                          << "      Sandwich ID: " << window_settings.id.sandwich << PBJ_LOG_NL
                          << "WindowSettings ID: " << window_settings.id.resource << PBJ_LOG_NL
                          << "        Exception: " << err.what() << PBJ_LOG_END;
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Updates the position and size properties of an existing saved
///         WindowSettings object.
///
/// \details The WindowSettings object will be found by looking at the ResourceId
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

    try
    {
        std::shared_ptr<sw::Sandwich> sandwich = sw::openWritable(window_settings.id.sandwich);
        if (!sandwich)
            throw std::runtime_error("Could not open sandwich for writing!");

        db::Db& db = sandwich->getDb();
  
        if (db.getInt(PBJ_WINDOW_SETTINGS_SQL_TABLE_EXISTS, 0) == 0)
            throw std::runtime_error("WindowSettings table does not exist!");

        db::Transaction transaction(db, db::Transaction::Immediate);

        int history_index = 0;
        db::Stmt latest(db, Id(PBJ_WINDOW_SETTINGS_SQLID_LATEST_INDEX), PBJ_WINDOW_SETTINGS_SQL_LATEST_INDEX);
        latest.bind(1, window_settings.id.resource.value());
        if (latest.step())
            history_index = latest.getInt(0);

        db::Stmt save(db, Id(PBJ_WINDOW_SETTINGS_SQLID_SAVE_POS), PBJ_WINDOW_SETTINGS_SQL_SAVE_POS);
        save.bind(1, window_settings.position.x);
        save.bind(2, window_settings.position.y);
        save.bind(3, window_settings.size.x);
        save.bind(4, window_settings.size.y);
        save.bind(5, window_settings.id.resource.value());
        save.bind(6, history_index);

        save.step();
        transaction.commit();
        return true;
    }
    catch (const db::Db::error& err)
    {
        PBJ_LOG(VWarning) << "Database error while saving window position!" << PBJ_LOG_NL
                          << "      Sandwich ID: " << window_settings.id.sandwich << PBJ_LOG_NL
                          << "WindowSettings ID: " << window_settings.id.resource << PBJ_LOG_NL
                          << "        Exception: " << err.what() << PBJ_LOG_NL
                          << "              SQL: " << err.sql() << PBJ_LOG_END;
    }
    catch (const std::runtime_error& err)
    {
        PBJ_LOG(VWarning) << "Exception while saving window position!" << PBJ_LOG_NL
                          << "      Sandwich ID: " << window_settings.id.sandwich << PBJ_LOG_NL
                          << "WindowSettings ID: " << window_settings.id.resource << PBJ_LOG_NL
                          << "        Exception: " << err.what() << PBJ_LOG_END;
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Deletes the most recent history index of the specified
///         WindowSettings object.
/// 
/// \param  id The ResourceId defining the WindowSettings stack to revert.
/// \return The next most recent history index for the WindowSettings object.
///         If there is a problem deleting the most recent window settings or
///         if there is no next most recent history index, a
///         default-constructed WindowSettings object will be returned.
///
/// \ingroup loading
WindowSettings revertWindowSettings(const sw::ResourceId& id)
{
    try
    {
        std::shared_ptr<sw::Sandwich> sandwich = sw::openWritable(id.sandwich);
        if (!sandwich)
            throw std::runtime_error("Could not open sandwich for writing!");

        db::Db& db = sandwich->getDb();
  
        if (db.getInt(PBJ_WINDOW_SETTINGS_SQL_TABLE_EXISTS, 0) == 0)
            throw std::runtime_error("WindowSettings table does not exist!");

        db::Transaction transaction(db, db::Transaction::Immediate);

        int history_index = 0;
        db::Stmt latest(db, PBJ_WINDOW_SETTINGS_SQLID_LATEST_INDEX);
        latest.bind(1, id.resource.value());
        if (latest.step())
            history_index = latest.getInt(0);

        db::Stmt remove(db, PBJ_WINDOW_SETTINGS_SQLID_REVERT);
        remove.bind(1, id.resource.value());
        remove.bind(2, history_index);
        remove.step();

        transaction.commit();
        return loadWindowSettings(*sandwich, id.resource);
    }
    catch (const db::Db::error& err)
    {
        PBJ_LOG(VWarning) << "Database error while reverting window settings!" << PBJ_LOG_NL
                          << "      Sandwich ID: " << id.sandwich << PBJ_LOG_NL
                          << "WindowSettings ID: " << id.resource << PBJ_LOG_NL
                          << "        Exception: " << err.what() << PBJ_LOG_NL
                          << "              SQL: " << err.sql() << PBJ_LOG_END;
    }
    catch (const std::runtime_error& err)
    {
        PBJ_LOG(VWarning) << "Exception while reverting window settings!" << PBJ_LOG_NL
                          << "      Sandwich ID: " << id.sandwich << PBJ_LOG_NL
                          << "WindowSettings ID: " << id.resource << PBJ_LOG_NL
                          << "        Exception: " << err.what() << PBJ_LOG_END;
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
/// \param  id The ResourceId of the WindowSettings stack to truncate.
/// \param  max_history_entries The number of entries to leave in the database.
///
/// \ingroup loading
void truncateWindowSettingsHistory(const sw::ResourceId& id, int max_history_entries)
{
    try
    {
        std::shared_ptr<sw::Sandwich> sandwich = sw::openWritable(id.sandwich);
        if (!sandwich)
            throw std::runtime_error("Could not open sandwich for writing!");

        db::Db& db = sandwich->getDb();
  
        if (db.getInt(PBJ_WINDOW_SETTINGS_SQL_TABLE_EXISTS, 0) == 0)
            throw std::runtime_error("WindowSettings table does not exist!");

        db::Transaction transaction(db, db::Transaction::Immediate);

        int history_index = 0;
        db::Stmt latest(db, Id(PBJ_WINDOW_SETTINGS_SQLID_LATEST_INDEX), PBJ_WINDOW_SETTINGS_SQL_LATEST_INDEX);
        latest.bind(1, id.resource.value());
        if (latest.step())
            history_index = latest.getInt(0);

        history_index -= max_history_entries - 1;

        // remove old history entries
        db::Stmt remove(db, Id(PBJ_WINDOW_SETTINGS_SQLID_TRUNCATE), PBJ_WINDOW_SETTINGS_SQL_TRUNCATE);
        remove.bind(1, id.resource.value());
        remove.bind(2, history_index);
        remove.step();

        // compress remaining history indices closer to 0
        if (history_index > 1)
        {
            db::Stmt compress(db, Id(PBJ_WINDOW_SETTINGS_SQLID_COMPRESS), PBJ_WINDOW_SETTINGS_SQL_COMPRESS);
            compress.bind(2, id.resource.value());
            compress.bind(1, history_index - 1);
            compress.step();
        }

        transaction.commit();
    }
    catch (const db::Db::error& err)
    {
        PBJ_LOG(VWarning) << "Database error while truncating window settings history!" << PBJ_LOG_NL
                          << "      Sandwich ID: " << id.sandwich << PBJ_LOG_NL
                          << "WindowSettings ID: " << id.resource << PBJ_LOG_NL
                          << "        Exception: " << err.what() << PBJ_LOG_NL
                          << "              SQL: " << err.sql() << PBJ_LOG_END;
    }
    catch (const std::runtime_error& err)
    {
        PBJ_LOG(VWarning) << "Exception while truncating window settings history!" << PBJ_LOG_NL
                          << "      Sandwich ID: " << id.sandwich << PBJ_LOG_NL
                          << "WindowSettings ID: " << id.resource << PBJ_LOG_NL
                          << "        Exception: " << err.what() << PBJ_LOG_END;
    }
}

} // namespace pbj
