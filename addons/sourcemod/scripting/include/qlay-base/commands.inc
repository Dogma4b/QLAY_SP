//////////////////////////////////////////////////////////////////////////////
//
//                                Commands
//
//////////////////////////////////////////////////////////////////////////////

//-----------------------------Server-----------------------------//

//-----------------------------Player-----------------------------//

public Action Command_Base(int client, int argc)
{
    Player ply = player.GetByID(client);

    char arg1[32], arg2[32], arg3[32];

    GetCmdArg(1, arg1, sizeof(arg1));
    GetCmdArg(2, arg2, sizeof(arg2));
    GetCmdArg(3, arg3, sizeof(arg3));

    if (StrEqual(arg1, "timers", false))
    {
        PrintToConsole(ply.id, "-----------------------------------------Timers----------------------------------------");
        PrintToConsole(ply.id, "|                    Name                    |   delay   |  repeations  |  time left  |");
        PrintToConsole(ply.id, "---------------------------------------------------------------------------------------");

        for (int i=0; i < timer.list.Length; i++)
        {
            char tmrname[45], tmrdelay[12], tmrrepeat[15], tmrtl[14];
            Tmr tmr = timer.list.Get(i);
            tmr.name(tmrname, sizeof(tmrname));
            FormatEx(tmrdelay, sizeof(tmrdelay), "%.3f", tmr.delay);
            FormatEx(tmrrepeat, sizeof(tmrrepeat), (tmr.repeations == 0) ? "loop" : "%i", tmr.repeations);
            IntToString(tmr.GetTimeLeft(), tmrtl, sizeof(tmrtl));
            while (strlen(tmrname) <= 43) Format(tmrname, sizeof(tmrname), "%s ", tmrname);
            while (strlen(tmrdelay) <= 10) Format(tmrdelay, sizeof(tmrdelay), "%s ", tmrdelay);
            while (strlen(tmrrepeat) <= 13) Format(tmrrepeat, sizeof(tmrrepeat), "%s ", tmrrepeat);
            while (strlen(tmrtl) <= 12) Format(tmrtl, sizeof(tmrtl), "%s ", tmrtl);
            PrintToConsole(ply.id, "|%s|%s|%s|%s|", tmrname, tmrdelay, tmrrepeat, tmrtl);
        }

        PrintToConsole(ply.id, "---------------------------------------------------------------------------------------");
    }
    if (StrEqual(arg1, "changelevel", false))
        ServerCommand("changelevel %s", arg2);
    if (StrEqual(arg1, "entscount", false))
    {
        int entscount = 0;
        for (int i=0; i < 2048; i++) if (IsValidEntity(i)) entscount++;
        PrintToConsole(ply.id, "%i/2048", entscount);
    }

    return Plugin_Stop;
}

public Action Command_Ents(int client, int argc)
{
    Player ply = player.GetByID(client);

    char arg[32];

    GetCmdArg(1, arg, sizeof(arg));

    if (StrEqual(arg, "getall", false))
    {
        ArrayList entities = ents.GetAll();
        int idx = 0;

        if (entities.Length >= 150) idx = entities.Length - 50;

        while (idx < entities.Length)
        {
            Entity ent = entities.Get(idx);
            char name[32];

            ent.GetClass(name, sizeof(name));

            if (ent.id != 5000)
                PrintToConsole(ply.id, "%s id: %i", name, ent.id);
            else
                PrintToConsole(ply.id, "%s (picked)", name);

            idx++;
        }

        PrintToConsole(ply.id, "------------------------");

        PrintToConsole(ply.id, "Count: %i", entities.Length);

        delete entities;
    }
    
    return Plugin_Stop;
}

public Action Command_Player(int client, int argc)
{
    Player ply = player.GetByID(client);

    char arg1[32],arg2[32],arg3[32],arg4[32];

    GetCmdArg(1, arg1, sizeof(arg1));
    GetCmdArg(2, arg2, sizeof(arg2));
    GetCmdArg(3, arg3, sizeof(arg3));
    GetCmdArg(4, arg4, sizeof(arg4));

    if (StrEqual(arg1, "getall", false))
    {
        ArrayList players = player.GetAll();

        for (int i=0; i < players.Length; i++) 
        {
            Player user = players.Get(i);
            char name[32];
            user.GetName(name, sizeof(name));
            
            PrintToConsole(ply.id, "Id: %i | Name: %s", user.id, name);
        }

        delete players;
    }
    
    return Plugin_Stop;
}

public Action Command_GetEntsInBox(int client, int argc)
{
    Player ply = player.GetByID(client);

    char filter[4][32] = { "prop_physics", "weapon_", "func_door", "prop_dynamic" };

    ArrayList entArr = ents.FindInBox(ply.GetPos() - new Vector(200.0, 200.0, 200.0), ply.GetPos() + new Vector(200.0, 200.0, 200.0), filter, sizeof(filter));

    for(int i=0; i < entArr.Length; i++) 
    {
        Entity ent = entArr.Get(i, 0);

        char entclass[32];
        ent.GetClass(entclass, sizeof(entclass));

        Vector entPos = ent.GetPos();
        Angle entAng = ent.GetAng();
        
        PrintToChat(ply.id, "class: %s, id: %i, pos: {\"vec\":[%i,%i,%i],\"ang\":[%i,%i,%i]}", entclass, ent.id, RoundFloat(entPos.x), RoundFloat(entPos.y), RoundFloat(entPos.z), RoundFloat(entAng.x), RoundFloat(entAng.y), RoundFloat(entAng.z));

        delete entPos;
        delete entAng;
    }

    delete entArr;

    return Plugin_Stop;
}