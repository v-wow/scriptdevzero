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

#ifndef DEF_DM_H
#define DEF_DM_H

enum
{
    MAX_ENCOUNTER               = 9,
    MAX_GENERATORS              = 5,

    // East
    TYPE_ALZZIN                 = 0,                        // Do not change - Handled with Acid

    // West
    TYPE_IMMOLTHAR              = 1,
    TYPE_PRINCE                 = 2,
    TYPE_PYLON_1                = 3,
    TYPE_PYLON_2                = TYPE_PYLON_1 + 1,
    TYPE_PYLON_3                = TYPE_PYLON_1 + 2,
    TYPE_PYLON_4                = TYPE_PYLON_1 + 3,
    TYPE_PYLON_5                = TYPE_PYLON_1 + 4,

    // North
    TYPE_KING_GORDOK            = 8,

    // East
    GO_CRUMBLE_WALL             = 177220,
    GO_CORRUPT_VINE             = 179502,
    GO_FELVINE_SHARD            = 179559,
    GO_CONSERVATORY_DOOR        = 176907,

    NPC_OLD_IRONBARK            = 11491,
    NPC_IRONBARK_REDEEMED       = 14241,

    // West
    NPC_PRINCE_TORTHELDRIN      = 11486,
    NPC_IMMOLTHAR               = 11496,
    NPC_ARCANE_ABERRATION       = 11480,
    NPC_MANA_REMNANT            = 11483,
    NPC_HIGHBORNE_SUMMONER      = 11466,

    GO_PRINCES_CHEST            = 179545,
    GO_PRINCES_CHEST_AURA       = 179563,
    GO_CRYSTAL_GENERATOR_1      = 177259,
    GO_CRYSTAL_GENERATOR_2      = 177257,
    GO_CRYSTAL_GENERATOR_3      = 177258,
    GO_CRYSTAL_GENERATOR_4      = 179504,
    GO_CRYSTAL_GENERATOR_5      = 179505,
    GO_FORCEFIELD               = 179503,
    GO_WARPWOOD_DOOR            = 177221,

    // North
    NPC_GUARD_MOLDAR            = 14326,
    NPC_STOMPER_KREEG           = 14322,
    NPC_GUARD_FENGUS            = 14321,
    NPC_GUARD_SLIPKIK           = 14323,
    NPC_CAPTAIN_KROMCRUSH       = 14325,
    NPC_CHORUSH                 = 14324,
    NPC_KING_GORDOK             = 11501,
    NPC_MIZZLE_THE_CRAFTY       = 14353,

    GO_KNOTS_CACHE              = 179501,
    GO_KNOTS_BALL_AND_CHAIN     = 179511,
    GO_GORDOK_TRIBUTE           = 179564,

    SAY_FREE_IMMOLTHAR          = -1429000,
    SAY_KILL_IMMOLTHAR          = -1429001,

    FACTION_HOSTILE             = 14,
    SPELL_KING_OF_GORDOK        = 22799,
};

class MANGOS_DLL_DECL instance_dire_maul : public ScriptedInstance
{
    public:
        instance_dire_maul(Map* pMap);
        ~instance_dire_maul() {}

        void Initialize();

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);
        uint64 GetData64(uint32 uiData);

        void OnCreatureEnterCombat(Creature* pCreature);
        void OnCreatureDeath(Creature* pCreature);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

    protected:
        bool CheckAllGeneratorsDestroyed();
        void ProcessForceFieldOpening();

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        // East
        bool m_bWallDestroyed;

        uint64 m_uiCrumbleWallGUID;
        uint64 m_uiCorruptVineGUID;
        uint64 m_uiConservatoryDoorGUID;
        uint64 m_uiOldIronbarkGUID;

        std::list<uint64> m_lFelvineShardGUIDs;

        // West
        uint64 m_auiCrystalGeneratorGUID[MAX_GENERATORS];

        uint64 m_uiPrinceTortheldrinGUID;
        uint64 m_uiImmolTharGUID;
        uint64 m_uiForcefieldGUID;
        uint64 m_uiPrincesChestAuraGUID;
        uint64 m_uiTendrisWarpwoodDoorGUID;

        std::list<uint64> m_luiHighborneSummonerGUIDs;
        std::list<uint64> m_lGeneratorGuardGUIDs;
        std::set<uint32> m_sSortedGeneratorGuards[MAX_GENERATORS];

        // North
        uint64 m_uiGordokGUID;
        uint64 m_uiChoRushGUID;
        uint64 m_uiMizzleGUID;
};

#endif
