/*
 * Copyright (C) 2006-2011 ScriptDev2 <http://www.scriptdev2.com/>
 * Copyright (C) 2010-2011 ScriptDev0 <http://github.com/scriptdev/scriptdevzero>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "precompiled.h"
#include "system.h"
#include "ProgressBar.h"
#include "ObjectMgr.h"
#include "Database/DatabaseEnv.h"

DatabaseType SD0Database;
std::string  strSD0Version;

SystemMgr::SystemMgr()
{
}

SystemMgr& SystemMgr::Instance()
{
    static SystemMgr pSysMgr;
    return pSysMgr;
}

void SystemMgr::LoadVersion()
{
    //Get Version information
    QueryResult* pResult = SD0Database.PQuery("SELECT version FROM sd0_db_version LIMIT 1");

    if (pResult)
    {
        Field* pFields = pResult->Fetch();

        strSD0Version = pFields[0].GetCppString();

        outstring_log("Loading %s", strSD0Version.c_str());
        outstring_log("");

        delete pResult;
    }
    else
    {
        error_log("SD0: Missing `sd0_db_version` information.");
        outstring_log("");
    }
}

void SystemMgr::LoadScriptTexts()
{
    outstring_log("SD0: Loading Script Texts...");
    LoadMangosStrings(SD0Database, "script_texts", TEXT_SOURCE_TEXT_START, TEXT_SOURCE_TEXT_END);

    QueryResult* pResult = SD0Database.PQuery("SELECT entry, sound, type, language, emote FROM script_texts WHERE entry BETWEEN %i AND %i", TEXT_SOURCE_GOSSIP_END, TEXT_SOURCE_TEXT_START);

    outstring_log("SD0: Loading Script Texts additional data...");

    if (pResult)
    {
        barGoLink bar(pResult->GetRowCount());
        uint32 uiCount = 0;

        do
        {
            bar.step();
            Field* pFields = pResult->Fetch();
            StringTextData pTemp;

            int32 iId           = pFields[0].GetInt32();
            pTemp.uiSoundId     = pFields[1].GetUInt32();
            pTemp.uiType        = pFields[2].GetUInt32();
            pTemp.uiLanguage    = pFields[3].GetUInt32();
            pTemp.uiEmote       = pFields[4].GetUInt32();

            if (iId >= 0)
            {
                error_db_log("SD0: Entry %i in table `script_texts` is not a negative value.", iId);
                continue;
            }

            if (pTemp.uiSoundId)
            {
                if (!GetSoundEntriesStore()->LookupEntry(pTemp.uiSoundId))
                    error_db_log("SD0: Entry %i in table `script_texts` has soundId %u but sound does not exist.", iId, pTemp.uiSoundId);
            }

            if (!GetLanguageDescByID(pTemp.uiLanguage))
                error_db_log("SD0: Entry %i in table `script_texts` using Language %u but Language does not exist.", iId, pTemp.uiLanguage);

            if (pTemp.uiType > CHAT_TYPE_ZONE_YELL)
                error_db_log("SD0: Entry %i in table `script_texts` has Type %u but this Chat Type does not exist.", iId, pTemp.uiType);

            m_mTextDataMap[iId] = pTemp;
            ++uiCount;
        } while (pResult->NextRow());

        delete pResult;

        outstring_log("");
        outstring_log(">> Loaded %u additional Script Texts data.", uiCount);
    }
    else
    {
        barGoLink bar(1);
        bar.step();
        outstring_log("");
        outstring_log(">> Loaded 0 additional Script Texts data. DB table `script_texts` is empty.");
    }
}

void SystemMgr::LoadScriptTextsCustom()
{
    outstring_log("SD0: Loading Custom Texts...");
    LoadMangosStrings(SD0Database, "custom_texts", TEXT_SOURCE_CUSTOM_START, TEXT_SOURCE_CUSTOM_END);

    QueryResult* pResult = SD0Database.PQuery("SELECT entry, sound, type, language, emote FROM custom_texts WHERE entry BETWEEN %i AND %i", TEXT_SOURCE_CUSTOM_END, TEXT_SOURCE_CUSTOM_START);

    outstring_log("SD0: Loading Custom Texts additional data...");

    if (pResult)
    {
        barGoLink bar(pResult->GetRowCount());
        uint32 uiCount = 0;

        do
        {
            bar.step();
            Field* pFields = pResult->Fetch();
            StringTextData pTemp;

            int32 iId              = pFields[0].GetInt32();
            pTemp.uiSoundId        = pFields[1].GetUInt32();
            pTemp.uiType           = pFields[2].GetUInt32();
            pTemp.uiLanguage       = pFields[3].GetUInt32();
            pTemp.uiEmote          = pFields[4].GetUInt32();

            if (iId >= 0)
            {
                error_db_log("SD0: Entry %i in table `custom_texts` is not a negative value.", iId);
                continue;
            }

            if (pTemp.uiSoundId)
            {
                if (!GetSoundEntriesStore()->LookupEntry(pTemp.uiSoundId))
                    error_db_log("SD0: Entry %i in table `custom_texts` has soundId %u but sound does not exist.", iId, pTemp.uiSoundId);
            }

            if (!GetLanguageDescByID(pTemp.uiLanguage))
                error_db_log("SD0: Entry %i in table `custom_texts` using Language %u but Language does not exist.", iId, pTemp.uiLanguage);

            if (pTemp.uiType > CHAT_TYPE_ZONE_YELL)
                error_db_log("SD0: Entry %i in table `custom_texts` has Type %u but this Chat Type does not exist.", iId, pTemp.uiType);

            m_mTextDataMap[iId] = pTemp;
            ++uiCount;
        } while (pResult->NextRow());

        delete pResult;

        outstring_log("");
        outstring_log(">> Loaded %u additional Custom Texts data.", uiCount);
    }
    else
    {
        barGoLink bar(1);
        bar.step();
        outstring_log("");
        outstring_log(">> Loaded 0 additional Custom Texts data. DB table `custom_texts` is empty.");
    }
}

void SystemMgr::LoadScriptGossipTexts()
{
    outstring_log("SD0: Loading Gossip Texts...");
    LoadMangosStrings(SD0Database, "gossip_texts", TEXT_SOURCE_GOSSIP_START, TEXT_SOURCE_GOSSIP_END);
}

void SystemMgr::LoadScriptWaypoints()
{
    // Drop Existing Waypoint list
    m_mPointMoveMap.clear();

    uint64 uiCreatureCount = 0;

    // Load Waypoints
    QueryResult* pResult = SD0Database.PQuery("SELECT COUNT(entry) FROM script_waypoint GROUP BY entry");
    if (pResult)
    {
        uiCreatureCount = pResult->GetRowCount();
        delete pResult;
    }

    outstring_log("SD0: Loading Script Waypoints for %u creature(s)...", uiCreatureCount);

    pResult = SD0Database.PQuery("SELECT entry, pointid, location_x, location_y, location_z, waittime FROM script_waypoint ORDER BY pointid");

    if (pResult)
    {
        barGoLink bar(pResult->GetRowCount());
        uint32 uiNodeCount = 0;

        do
        {
            bar.step();
            Field* pFields = pResult->Fetch();
            ScriptPointMove pTemp;

            pTemp.uiCreatureEntry   = pFields[0].GetUInt32();
            uint32 uiEntry          = pTemp.uiCreatureEntry;
            pTemp.uiPointId         = pFields[1].GetUInt32();
            pTemp.fX                = pFields[2].GetFloat();
            pTemp.fY                = pFields[3].GetFloat();
            pTemp.fZ                = pFields[4].GetFloat();
            pTemp.uiWaitTime        = pFields[5].GetUInt32();

            CreatureInfo const* pCInfo = GetCreatureTemplateStore(pTemp.uiCreatureEntry);

            if (!pCInfo)
            {
                error_db_log("SD0: DB table script_waypoint has waypoint for nonexistent creature entry %u", pTemp.uiCreatureEntry);
                continue;
            }

            if (!pCInfo->ScriptID)
                error_db_log("SD0: DB table script_waypoint has waypoint for creature entry %u, but creature does not have ScriptName defined and then useless.", pTemp.uiCreatureEntry);

            m_mPointMoveMap[uiEntry].push_back(pTemp);
            ++uiNodeCount;
        } while (pResult->NextRow());

        delete pResult;

        outstring_log("");
        outstring_log(">> Loaded %u Script Waypoint nodes.", uiNodeCount);
    }
    else
    {
        barGoLink bar(1);
        bar.step();
        outstring_log("");
        outstring_log(">> Loaded 0 Script Waypoints. DB table `script_waypoint` is empty.");
    }
}
