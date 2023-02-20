//////////////////////////////////////////////////////////////////////////////
//
//                                Functions
//
//////////////////////////////////////////////////////////////////////////////

public void WarmupGiveWeapon(Player ply)
{
    char melees[4][32] = {"weapon_knife", "weapon_axe", "weapon_spanner", "weapon_hammer"};

    if (ply.IsAlive()) ply.Give(melees[GetRandomInt(0, 3)]);
}

stock void FakePrecacheSound(const char[] szPath)
{
    AddToStringTable(FindStringTable( "soundprecache" ), szPath);
}

stock bool IsClientExist(int client)
{
    if((0 < client < MaxClients) && IsClientInGame(client) && !IsClientSourceTV(client))
    {
        return true;
    }

    return false;
}

stock bool IsClientInSpec(int client)
{
    if(GetClientTeam(client) != 1)
    {
        return false;
    }

    return true;
}