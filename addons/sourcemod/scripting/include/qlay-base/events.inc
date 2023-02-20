//////////////////////////////////////////////////////////////////////////////
//
//                                Events
//
//////////////////////////////////////////////////////////////////////////////

public Action OnPlayerSpawn(int client)
{
    Player ply = player.GetByID(client);

    if (IsClientExist(client) && ply && !ply.GetHandle("rsptmr") && GetClientTeam(client) > 1)
    {
        Call_StartForward(PreClientSpawnForward);
        Call_PushCellRef(ply);
        Call_Finish();

        ply.SetHandle("rsptmr", ply.TimerSimple(100, "PlayerSpawn", ply));
        if (ply.FirstSpawn) ply.FirstSpawn = false;
    }

    return Plugin_Continue;
}

public Action OnPlayerSwitchTeam(Event event, const char[] name, bool dbroadcast)
{
    Player ply = player.GetByID(GetClientOfUserId(GetEventInt(event, "userid")));
    int team = GetEventInt(event, "team");

    if (team == 1)
    {
        ply.Kill();
        ply.ready = false;

        SetHudTextParams(0.025, 0.915, 9999.0, 102, 102, 255, 190);
        ShowHudText(ply.id, 1, "%t", "Spec msg");
    }

    return Plugin_Continue;
}

public void OnWeaponEquip(int client, int iWeapon)
{
    Player ply = player.GetByID(client);
    Entity ent = ents.Get(iWeapon);

    if (ply && ent)
    {
        Call_StartForward(OnPlayerPickupWeaponForward);
        Call_PushCellRef(ply);
        Call_PushCellRef(ent);
        Call_Finish();
    }
}

public Action OnWeaponSwitch(int client, int iWeapon)
{
    Player ply = player.GetByID(client);
    Entity ent = ents.Get(iWeapon);

    if (ply && ent)
    {
        Call_StartForward(OnPlayerSwitchWeaponForward);
        Call_PushCellRef(ply);
        Call_PushCellRef(ent);
        Call_Finish();
    }

    return Plugin_Continue;
}

public Action OnButtonPressed(const char[] output, int btnid, int activator, float delay)
{
    if(IsClientExist(activator) && IsValidEntity(btnid) && IsPlayerAlive(activator) && !IsClientInSpec(activator))
    {
        Call_StartForward(OnButtonPressedForward);
        Call_PushCell(player.GetByID(activator));
        Call_PushCell(btnid);
        Call_Finish();
    }

    return Plugin_Continue;
}

public Action OnTakeDamage(int victim, int &attacker, int &inflictor, float &damage, int &damagetype)
{   
    if(IsClientExist(victim))
    {
        Player atk, vic = player.GetByID(victim);
        Action result;

        //if (atk == null) return Plugin_Continue;
        if (!vic) return Plugin_Continue;
        
        Call_StartForward(OnTakeDamageForward);
        Call_PushCellRef(vic);
        Call_PushCellRef(atk);
        Call_PushFloatRef(damage);
        Call_PushCellRef(damagetype);
        Call_PushCellRef(inflictor);
        Call_Finish(result);

        return result;
    }

    return Plugin_Continue;
}

public Action OnPlayerDeath(Event event, const char[] name, bool dontBroadcast)
{
    Player vic = player.GetByID(GetClientOfUserId(GetEventInt(event, "userid"))), atk = player.GetByID(GetClientOfUserId(GetEventInt(event, "attacker")))
    if (!vic) return Plugin_Continue;

    Call_StartForward(OnPlayerDeathForward);
    Call_PushCellRef(vic);
    Call_PushCellRef(atk);
    Call_Finish();

    return Plugin_Continue;
}

public void OnRoundStart(Event event, const char[] name, bool dbroadcast)
{
    Call_StartForward(OnRoundStartForward);
    Call_Finish();
}

public void OnRoundPreStart(Event event, const char[] name, bool dbroadcast)
{
    Call_StartForward(OnRoundEndForward);
    Call_Finish();
}

public void OnPlayerRunCmdPost(int client, int buttons)
{
    Player ply = player.GetByID(client);
    
    if (ply)
    {
        Call_StartForward(OnInputForward);
        Call_PushCellRef(ply);
        Call_PushCell(buttons);
        Call_Finish();
    }
}