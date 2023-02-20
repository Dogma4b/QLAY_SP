#if defined _qlay_included
    #endinput
#endif

#define _qlay_included

//Define types
#define Ptr Address
#define nullptr Address_Null
#define string QString

//Macroses
#define ptr(%1) view_as<Ptr>(%1)
#define int(%1) view_as<int>(%1)
#define char(%1) view_as<char>(%1)
#define websocket(%1) view_as<WebSocket>(%1)

#pragma semicolon 1
#pragma newdecls required
#pragma dynamic 67108864

#include <sourcemod>
#include <sdktools>
#include <sdkhooks>

#include "qlay\structures\string"
#include "qlay\structures\base"
#include "qlay\structures\vector"
#include "qlay\structures\angle"
#include "qlay\structures\color"
#include "qlay\structures\json"

#include "qlay\utils"

#include "qlay\classes\qlay"

stock QLAY qlay;

#include "qlay\classes\websocket"
#include "qlay\classes\meta"
#include "qlay\classes\timers"

stock Meta meta;
stock Timers timer;

#include "qlay\entities\entity\entity"
#include "qlay\entities\player\player"
#include "qlay\entities\ambient"
#include "qlay\entities\worldtext"

#include "qlay\classes\singletons"

stock PlayerSingleton player;
stock EntitySingleton ents;
stock WorldTextSingleton worldtext;

forward void QLAY_RegisterMetaData();

forward void QLAY_OnRoundStart();

forward void QLAY_OnRoundEnd();

forward void QLAY_OnPlayerJoin(Player &ply);

forward void QLAY_OnPlayerLeave(Player &ply);

forward void QLAY_PrePlayerSpawn(Player &ply);

forward void QLAY_OnPlayerSpawn(Player &ply);

forward void QLAY_PostPlayerSpawn(Player &ply);

forward void QLAY_OnPlayerClear(Player &ply);

forward bool OnPlayerEntUse(Player &ply, Entity &ent);

forward void QLAY_OnPlayerPickupWeapon(Player &ply, Entity &weapon);

forward void QLAY_OnPlayerSwitchWeapon(Player &ply, Entity &weapon);

forward Action QLAY_OnTakeDamage(Player &vic, Player &atk, float &damage, int &damagetype, int &inflictor);

forward void QLAY_OnPlayerDeath(Player &vic, Player &atk);

forward Action QLAY_OnEmitSound(Entity &ent, char[] sound, int &volume, float &level, int &pitch, int &flags);

forward void QLAY_OnButtonPressed(Player &ply, int buttonid);

forward void QLAY_OnInput(Player &ply, int buttons);

public SharedPlugin __pl_qlay = {
    name = "qlay",
    file = "qlay.smx",
    #if defined REQUIRE_PLUGIN
        required = 1,
    #else
        required = 0,
    #endif
};

#if !defined REQUIRE_PLUGIN
public void __pl_qlay_SetNTVOptional()
{
    MarkNativeAsOptional("Timers.list.get");
    MarkNativeAsOptional("Timers.HideCreate");

    MarkNativeAsOptional("EntitySingleton.list.get");
    MarkNativeAsOptional("EntitySingleton.Create");
    MarkNativeAsOptional("EntitySingleton.Push");
    MarkNativeAsOptional("EntitySingleton.Remove");
    MarkNativeAsOptional("EntitySingleton.RemoveByID");
    MarkNativeAsOptional("EntitySingleton.Dissolve");
    MarkNativeAsOptional("EntitySingleton.IndexUpdate");
    MarkNativeAsOptional("EntitySingleton.Clear");

    MarkNativeAsOptional("PlayerSingleton.list.get");
    MarkNativeAsOptional("PlayerSingleton.Add");
    MarkNativeAsOptional("PlayerSingleton.Remove");

    MarkNativeAsOptional("WorldTextSingleton.list.get");
    MarkNativeAsOptional("WorldTextSingleton.Create");
    MarkNativeAsOptional("WorldTextSingleton.Remove");

    MarkNativeAsOptional("Player.Give");
    MarkNativeAsOptional("Player.DropWeapons");
    MarkNativeAsOptional("Player.RestrictWeapons");
}
#endif