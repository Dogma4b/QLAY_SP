#pragma semicolon 1
#pragma newdecls required

#include <cstrike>
// ¯\_(ツ)_/¯
#include <qlay>

public Plugin myinfo = {
    name = "[SCP] Qlay Core",
    author = "Andrey::Dono, GeTtOo",
    description = "QLAY for CS:GO",
    version = "1.0",
    url = ""
};

//////////////////////////////////////////////////////////////////////////////
//
//                                Main
//
//////////////////////////////////////////////////////////////////////////////

Handle RegMetaForward;
Handle OnRoundStartForward;
Handle OnRoundEndForward;
Handle OnPlayerJoinForward;
Handle OnPlayerLeaveForward;
Handle PreClientSpawnForward;
Handle OnPlayerSpawnForward;
Handle PostClientSpawnForward;
Handle OnPlayerClearForward;
Handle OnPlayerEntUseForward;
Handle OnPlayerPickupWeaponForward;
Handle OnPlayerSwitchWeaponForward;
Handle OnTakeDamageForward;
Handle OnPlayerDeathForward;
Handle OnEmitSoundForward;
Handle OnButtonPressedForward;
Handle OnInputForward;

#include "qlay-base\functions"
#include "qlay-base\events"
#include "qlay-base\callbacks"
#include "qlay-base\natives"
#include "qlay-base\commands"

public void OnPluginStart()
{
    //LoadTranslations("qlay.phrases");

    qlay = new QLAY();
    meta = new Meta();
    ents = new EntitySingleton();
    player = new PlayerSingleton();
    worldtext = new WorldTextSingleton();
    timer = new Timers();

    HookEvent("round_start", OnRoundStart);
    HookEvent("round_prestart", OnRoundPreStart);
    HookEvent("player_team", OnPlayerSwitchTeam);
    HookEvent("player_death", OnPlayerDeath, EventHookMode_Pre);
    
    HookEntityOutput("func_button", "OnPressed", OnButtonPressed);

    RegAdminCmd("qlay", Command_Base, ADMFLAG_RCON);
    RegAdminCmd("ents", Command_Ents, ADMFLAG_RCON);
    RegAdminCmd("player", Command_Player, ADMFLAG_RCON);
    RegAdminCmd("getentsinbox", Command_GetEntsInBox, ADMFLAG_RCON);
}

public void OnPluginEnd()
{
    qlay.Dispose();
    meta.Dispose();
    ents.Dispose();
    player.Dispose();
    worldtext.Dispose();
    timer.Dispose();
}

public APLRes AskPluginLoad2(Handle self, bool late, char[] error, int err_max)
{
    CreateNative("Timers.list.get", NativeTimers_GetList);
    CreateNative("Timers.HideCreate", NativeTimers_HideCreate);

    CreateNative("EntitySingleton.list.get", NativeEntities_GetList);
    CreateNative("EntitySingleton.Create", NativeEntities_Create);
    CreateNative("EntitySingleton.Push", NativeEntities_Push);
    CreateNative("EntitySingleton.Remove", NativeEntities_Remove);
    CreateNative("EntitySingleton.RemoveByID", NativeEntities_RemoveByID);
    CreateNative("EntitySingleton.Dissolve", NativeEntities_Dissolve);
    CreateNative("EntitySingleton.IndexUpdate", NativeEntities_IndexUpdate);
    CreateNative("EntitySingleton.Clear", NativeEntities_Clear);

    CreateNative("PlayerSingleton.list.get", NativeEntities_GetList);
    CreateNative("PlayerSingleton.Add", NativeClients_Add);
    CreateNative("PlayerSingleton.Remove", NativeClients_Remove);

    CreateNative("WorldTextSingleton.list.get", NativeEntities_GetList);
    CreateNative("WorldTextSingleton.Create", NativeWT_Create);
    CreateNative("WorldTextSingleton.Remove", NativeWT_Remove);

    CreateNative("Player.Give", NativePlayer_GiveWeapon);
    CreateNative("Player.DropWeapons", NativePlayer_DropWeapons);
    CreateNative("Player.RestrictWeapons", NativePlayer_RestrictWeapons);

    RegMetaForward = CreateGlobalForward("QLAY_RegisterMetaData", ET_Event);
    OnRoundStartForward = CreateGlobalForward("QLAY_OnRoundStart", ET_Event);
    OnRoundEndForward = CreateGlobalForward("QLAY_OnRoundEnd", ET_Event);
    OnPlayerJoinForward = CreateGlobalForward("QLAY_OnPlayerJoin", ET_Event, Param_CellByRef);
    OnPlayerLeaveForward = CreateGlobalForward("QLAY_OnPlayerLeave", ET_Event, Param_CellByRef);
    PreClientSpawnForward = CreateGlobalForward("QLAY_PrePlayerSpawn", ET_Event, Param_CellByRef);
    OnPlayerSpawnForward = CreateGlobalForward("QLAY_OnPlayerSpawn", ET_Event, Param_CellByRef);
    PostClientSpawnForward = CreateGlobalForward("QLAY_PostPlayerSpawn", ET_Event, Param_CellByRef);
    OnPlayerClearForward = CreateGlobalForward("QLAY_OnPlayerClear", ET_Event, Param_CellByRef);
    OnPlayerEntUseForward = CreateGlobalForward("QLAY_OnPlayerEntUse", ET_Event, Param_CellByRef, Param_CellByRef);
    OnPlayerPickupWeaponForward = CreateGlobalForward("QLAY_OnPlayerPickupWeapon", ET_Event, Param_CellByRef, Param_CellByRef);
    OnPlayerSwitchWeaponForward = CreateGlobalForward("QLAY_OnPlayerSwitchWeapon", ET_Event, Param_CellByRef, Param_CellByRef);
    OnTakeDamageForward = CreateGlobalForward("QLAY_OnTakeDamage", ET_Event, Param_CellByRef, Param_CellByRef, Param_FloatByRef, Param_CellByRef, Param_CellByRef);
    OnPlayerDeathForward = CreateGlobalForward("QLAY_OnPlayerDeath", ET_Event, Param_CellByRef, Param_CellByRef);
    OnEmitSoundForward = CreateGlobalForward("QLAY_OnEmitSound", ET_Event, Param_Cell, Param_String, Param_FloatByRef, Param_CellByRef, Param_CellByRef, Param_CellByRef);
    OnButtonPressedForward = CreateGlobalForward("QLAY_OnButtonPressed", ET_Event, Param_CellByRef, Param_Cell);
    OnInputForward = CreateGlobalForward("QLAY_OnInput", ET_Event, Param_CellByRef, Param_Cell);

    RegPluginLibrary("qlay");
    return APLRes_Success;
}

public void OnMapStart()
{
    char mapName[128];
    GetCurrentMap(mapName, sizeof(mapName));

    AddNormalSoundHook(SoundHandler);

    //LoadMetaData();

    Call_StartForward(RegMetaForward);
    Call_Finish();

    //LoadAndPrecacheFileTable();
}

public void OnMapEnd()
{
    //qlay.Dispose();
    //meta.Dispose();
    //ents.Dispose();
    //player.Dispose();
    //worldtext.Dispose();
    //timer.Dispose();

    RemoveNormalSoundHook(SoundHandler);
}

public void OnGameFrame() { timer.Update(); }

public void OnClientPostAdminCheck(int id)
{
    Player ply = player.Add(id);

    ply.SetHook(SDKHook_WeaponSwitch, OnWeaponSwitch);
    ply.SetHook(SDKHook_WeaponEquipPost, OnWeaponEquip);
    ply.SetHook(SDKHook_Spawn, OnPlayerSpawn);
    ply.SetHook(SDKHook_OnTakeDamage, OnTakeDamage);

    Call_StartForward(OnPlayerJoinForward);
    Call_PushCellRef(ply);
    Call_Finish();
}

public void OnClientDisconnect(int id)
{
    Player ply = player.GetByID(id);
    
    if (ply)
    {
        char timername[32];
        FormatEx(timername, sizeof(timername), "ent-%i", ply.id);
        timer.RemoveIsContains(timername);

        ply.RemoveHook(SDKHook_WeaponSwitch, OnWeaponSwitch);
        ply.RemoveHook(SDKHook_WeaponEquipPost, OnWeaponEquip);
        ply.RemoveHook(SDKHook_Spawn, OnPlayerSpawn);
        ply.RemoveHook(SDKHook_OnTakeDamage, OnTakeDamage);

        Call_StartForward(OnPlayerLeaveForward);
        Call_StartForward(OnPlayerClearForward);
        Call_PushCellRef(ply);
        Call_Finish();

        player.Remove(id);
    }
}

public void PlayerSpawn(Player ply)
{
    if(ply)
    {
        Call_StartForward(OnPlayerSpawnForward);
        Call_PushCellRef(ply);
        Call_Finish();
    }

    ply.RemoveValue("rsptmr");
}

public void PostPlayerSpawn(Player ply)
{
    Call_StartForward(PostClientSpawnForward);
    Call_PushCellRef(ply);
    Call_Finish();
}

public void OnEntityCreated(int entid)
{
    if (ents.list && entid > 64 && !ents.IsExistID(entid))
        ents.Push(new Entity(entid));
}

public void OnEntityDestroyed(int entid)
{
    if (ents.list && ents.IsExistID(entid))
        ents.RemoveByID(entid);
}