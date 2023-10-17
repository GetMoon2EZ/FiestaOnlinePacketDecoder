#include "fopd/fopd_translation.h"

#include <stdint.h>
#include <map>

#include "fopd/fopd_utils.h"


const std::map<const char *, const char *, cmp_str> tr_map_names_fr {
    /* Added from game files (resmenu/minimap) */
    { "Adl", "Adelia" },
    { "Adl_1", "Vallee d'Adelia" },
    { "AdlF", "Adelia enflamme" },
    { "AdlFH", "Adelia assiege" },
    { "AdlThorn01", "Grotte epineuse" },
    { "AdlVal01", "Vallee d'Adelia" },
    { "AlDn01", "Origine de l'arbre de vie" },
    { "Apoline", "Hedonisme des Epreuves" },
    { "ArkDn01", "Arkroute" },
    { "ArkDn02", "Arkroute des Abysses" },
    { "B_Albi", "Antre d'Albireo" },
    { "B_Cracker", "Repaire de Malephar" },
    { "Battlefield", "Champ de bataille de Pagel" },
    { "BD_Eld", "Elderine" },
    { "BD_Rou", "Roumen" },
    { "BD_Urg", "Uruga" },
    { "Beach", "Crique de baignade" },
    { "Bera", "Bera" },
    { "BerFrz01", "Champs de neige de Kahal" },
    { "BerKal01", "Plaines Kahal" },
    { "BerVale01", "Canyon de Kahal" },
    { "BFGate01", "Perimetre forestier (Elderine)" },
    { "BFGate02", "Perimetre forestier (Uruga)" },
    { "BFGate03", "Perimetre forestier (Les Ruines d'Alberstol)" },
    { "BFGate04", "Perimetre forestier (Adelia/Bera)" },
    { "BH_Albi", "Antre d'Albireo (mode infernal)" },
    { "BH_Cracker", "Repaire de Malephar (mode infernal)" },
    { "BH_Helga", "Tombeau d'Helga (mode infernal)" },
    { "BH_Karen", "Repaire de la Reine demoniaque" },
    { "Bla", "Citadelle du chaos" },
    { "CemDn01", "CemDn01" },
    { "CemDn02", "CemDn02" },
    { "Croad01", "Passage sombre II" },
    { "Croad01D", "Antre de Crepuscule" },
    { "CypDn01", "Coeur de la Foret Putrefiee" },
    { "Cypian", "Foret de la vie" },
    { "CypVal01", "Bosquet des Bois aux Murmures" },
    { "DarkTown", "Sanctuaire Arbor Vitae" },
    { "E_Hwin2", "Halloween" },
    { "E_XmasRide", "Ski de Noel" },
    { "EchoCave", "Caverne des echos" },
    { "Egma", "Elga" },
    { "Eld", "Elderine" },
    { "EldCem01", "Tombeau du claire de lune" },
    { "EldCem02", "Tombeau des vignes" },
    { "EldFor01", "Ancienne foret elfique" },
    { "EldGbl01", "Camps des Goblins" },
    { "EldGbl02", "Colline sableuse" },
    { "EldPri01", "Prison en ruine" },
    { "EldPri02", "Zone d'echafaud" },
    { "EldPriDn01", "Prison cachee 1er etage" },
    { "EldPriDn02", "Prison cachee 2eme etage" },
    { "EldSleep01", "Foret du sommeil" },
    // { "ElfDn01", "Adelia" },
    // { "ElfDn02", "Adelia" },
    { "EventF01", "Colline cyclone (Glaire Daniel)" },
    { "Fbattle01", "PvP 1 (Roumen)" },
    { "Fbattle02", "PvP 2 (Elderine)" },
    { "Fbattle03", "PvP 3 (Uruga)" },
    { "Fbattle04", "PvP 4 (Bera)" },
    { "FireDn01", "Sanctuaire sacre des gardiens" },
    { "FireDn02", "Sanctuaire sacre des gardiens" }, // TODO Abyss
    { "ForDn01", "Vestiges d'atout" },
    { "ForDn02", "Vestiges d'atout" }, // TODO Abyss
    { "Gate", "Champ des portails" },
    { "GBHouse", "Maison de la chance (casino)" },
    { "GblDn01", "Pic caché" },
    { "GblDn02", "Pic caché" }, // TODO Abyss
    { "GoldCave", "Caverne doree" },
    { "GuildT01", "Tournois de guilde" },
    { "GuildT02", "Tournois de guilde" },
    { "GuildT03", "Tournois de guilde" },
    { "GuildT04", "Tournois de guilde" },
    { "H_Adl", "Adelia" },
    { "H_Eld", "Elderine" },
    { "H_Rou", "Roumen" },
    { "H_Teva", "Jardins Celestes" },
    { "H_Urg", "Uruga" },
    { "H_UrgAlruin", "Les Ruines d'Alberstol" },
    { "Job2_Dn02", "Ancienne foret elfique (test lvl 60)" },
    { "KDAntiHenis", "[KQ] La guerre de rebellion de Henneath" },
    { "KDArena", "[KQ] Arene" },
    { "KDBirthday", "[KQ] Anniversaire" },
    { "KDBRP", "[KQ] Battle royale" },
    { "KDCake", "[KQ] Bataille de Gateau" },
    { "KDCollisio", "[KQ] Sanctuaire de Collisio" },
    { "KDEddyHill", "[KQ] Roi des glaires" },
    { "KDEgg", "[KQ] Bataille d'oeufs" },
    { "KDEnMaze", "[KQ] Diabolique Maitre Gordon" },
    { "KDFargels", "[KQ] Sanctuaire de Bijou" },
    { "KDGoldHill", "[KQ] La colline doree" },
    { "KDGreenHill", "[KQ] La rage de l'Empereur Glaire" },
    { "KDHBat1", "[KQ] WAWA" },
    { "KDHero", "[KQ] Hero" },
    { "KDHoneying", "[KQ] Bandit geant mechant" },
    { "KDKingkong", "[KQ] Chaos de King Kong Phino" },
    { "KDMDragon", "[KQ] Mini-dragon perdu (hard)" },
    { "KDMine", "[KQ] Mine cachée" },
    { "KDMiniDragon", "[KQ] Mini-dragon perdu" },
    { "KDPrtShip", "[KQ] Fureur de pirate Mara" },
    { "KDRockCan", "[KQ] ???" },
    { "KDSpider", "[KQ] Assaut d'araignee" },
    { "KDSpring", "[KQ] Oeuf de paque" },
    { "KDTravelerDn", "[KQ] ???" },
    { "KDUnHall", "[KQ] ???" },
    { "KDVictor", "[KQ] Les heritiers de Bijou" },
    { "KDWater", "[KQ] Bataille de ballon d'eau" },
    { "Leviathan", "Nid du Leviathan" },
    { "LinkField01", "Passage Sombre I" },
    { "LinkField02", "Passage Sombre II" },
    { "OX_field", "Zone Quiz OX" },
    { "PriDn01", "Liaison des tenebres" },
    { "PriDn02", "Liaison des tenebres" }, // TODO Abyss ?
    { "PsyIn", "Manoir de Psiken" },
    { "PsyOut", "Jardin de Psiken" },
    { "R_Helga01", "Tombeau d'Helga" },
    { "Rou", "Roumen" },
    { "RouCos01", "Plage" },
    { "RouCos02", "Foret des flots" },
    { "RouCos03", "Mer de l'avidite" },
    { "RouN", "New Roumen" },
    { "RouT", "New Roumen" },
    { "RouTemDn01", "Pierre lumineuse 1" },
    { "RouTemDn02", "Pierre lumineuse 2" },
    { "RouVal01", "Foret des brumes" },
    { "RouVal02", "Colline enflammee" },
    { "RShip", "Furie du Dragon des oceans" },
    { "RShip0", "Furie du Dragon des oceans" },
    { "SD_Vale00", "Valee de l'Augure" },
    { "SD_Vale01", "Canyon de l'Augure" },
    { "Ser", "Refuge de Khazul" },
    { "Siren", "Chateau de la Sirene" },
    { "SwaDn01", "Maraicage des pleurs" },
    { "SwaDn02", "Maraicage des pleurs" }, // TODO Abyss ?
    { "TCave", "Cave de cristal" },
    { "TempleG", "Temple des dieux" },
    { "Teva", "Jardins Celestes" },
    { "TevaBoss", "Temple d'Amatheon" },
    { "TevaDn01", "Temple du desespoir" },
    { "TevaDn02", "Temple d'Amatheon" },
    { "TevaL", "L'ile d'Eya" },
    { "TevaVal01", "Ilots Celestes" },
    { "TVoid", "Crepuscule du neant" },
    { "Urg", "Uruga" },
    { "Urg_Alruin", "Les ruines d'Alberstol" },
    { "Urg_AlruinT", "Tutoriel" },
    { "UrgDark01", "Sombreterre" },
    { "UrgDragon", "Tombeau du dragon" },
    { "UrgFire01", "Pierre brulante" },
    { "UrgFireDn01", "Mine enflammee" },
    { "UrgSwa01", "Marais de l'aube" },
    { "UrgSwaDn01", "Temple Spirituel" },
    { "ValDn01", "???" },
    { "ValDn02", "???" },
    { "VoidAbyss", "Refuge en ruine des abysses" },
    { "VoidDn", "Crepuscule du neant" },
    { "WarBL", "Mine de Devildom (40-50)" },
    { "WarH", "Forteresse de Devildom (105-120)" },
    { "WarL", "Baraquements de Devildom (50-60)" },
    { "WarN", "Territoire Devildom (70-80)" },
    { "Wedding", "Zone de mariage" },
    { "WindyCave", "Caverne des vents" },
    { "X_Adl", "Adelia" },
    { "X_Eld", "Elderine" },
    { "X_Rou", "Roumen" },
    { "X_RouN", "New Roumen" },
    { "X_Urg", "Uruga" },
    { "X_UrgAlruin", "Les ruines d'Alberstol" },
    /* Added manually */
    { "S_Tower03", "Tour spirale du sacrifice (20-85)" },
    { "S_Tower030", "Tour spirale de la vangence (86-125)" },
    { "S_Tower0300", "Tour spirale de la corruption (126-140)" }
};

enum foclass {
    FOCLASS_FG = 0x01,  /* Fighter */
    FOCLASS_CF = 0x02,  /* Clever fighter */
    FOCLASS_WA = 0x03,  /* Warrior */
    FOCLASS_GL = 0x04,  /* Gladiator */
    FOCLASS_KN = 0x05,  /* Knight */
    FOCLASS_CL = 0x06,  /* Cleric */
    FOCLASS_HC = 0x07,  /* High cleric */
    FOCLASS_PA = 0x08,  /* Paladin */
    FOCLASS_HK = 0x09,  /* Holy knight */
    FOCLASS_GN = 0x0A,  /* Guardian */
    FOCLASS_AR = 0x0B,  /* Archer */
    FOCLASS_HA = 0x0C,  /* HawkArcher */
    FOCLASS_SC = 0x0D,  /* Scout */
    FOCLASS_SS = 0x0E,  /* Sharpshooter */
    FOCLASS_RG = 0x0F,  /* Ranger */
    FOCLASS_MG = 0x10,  /* Mage */
    FOCLASS_WM = 0x11,  /* WizMage */
    FOCLASS_EN = 0x12,  /* Enchanter */
    FOCLASS_WL = 0x13,  /* Warlock */
    FOCLASS_WZ = 0x14,  /* Wizard */
    FOCLASS_TR = 0x15,  /* Trickster */
    FOCLASS_GA = 0x16,  /* Gambit */
    FOCLASS_RE = 0x17,  /* Renegade */
    FOCLASS_SP = 0x18,  /* Specter */
    FOCLASS_RP = 0x19,  /* Reaper */
    FOCLASS_CR = 0x1A, /* Crusader */
    FOCLASS_TM = 0x1B,  /* Templar */
};

const std::map<uint8_t, const char*> tr_pclass_na {
    { FOCLASS_FG, "Fighter" },
    { FOCLASS_CF, "Clever Fighter" },
    { FOCLASS_WA, "Warrior" },
    { FOCLASS_GL, "Gladiator" },
    { FOCLASS_KN, "Knight" },
    { FOCLASS_CL, "Cleric" },
    { FOCLASS_HC, "High Cleric" },
    { FOCLASS_PA, "Paladin" },
    { FOCLASS_HK, "Holy Knight" },
    { FOCLASS_GN, "Guardian" },
    { FOCLASS_AR, "Archer" },
    { FOCLASS_HA, "HawkArcher" },
    { FOCLASS_SC, "Scout" },
    { FOCLASS_SS, "Sharpshooter" },
    { FOCLASS_RG, "Ranger" },
    { FOCLASS_MG, "Mage" },
    { FOCLASS_WM, "WizMage" },
    { FOCLASS_EN, "Enchanter" },
    { FOCLASS_WL, "Warlock" },
    { FOCLASS_WZ, "Wizard" },
    { FOCLASS_TR, "Trickster" },
    { FOCLASS_GA, "Gambit" },
    { FOCLASS_RE, "Renegade" },
    { FOCLASS_SP, "Specter" },
    { FOCLASS_RP, "Reaper" },
    { FOCLASS_CR, "Crusader" },
    { FOCLASS_TM, "Templar" }
};

const char *
translate_map_name(const char *raw_map)
{
    auto translation = tr_map_names_fr.find(raw_map);
    if (translation == tr_map_names_fr.end()) {
        return raw_map;
    }
    return translation->second;
}

const char *
translate_pclass(uint8_t pclass)
{
    auto translation = tr_pclass_na.find(pclass);
    if (translation == tr_pclass_na.end()) {
        return "Unknown";
    }
    return translation->second;
}
