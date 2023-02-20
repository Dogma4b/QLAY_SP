//////////////////////////////////////////////////////////////////////////////
//
//                                Callbacks
//
//////////////////////////////////////////////////////////////////////////////

public Action CB_EntUse(int entity, int client)
{
    Player ply = player.GetByID(client);
    Entity ent = ents.Get(entity);

    if (ent.meta)
    {
        bool canentuse = true;

        Call_StartForward(OnPlayerEntUseForward);
        Call_PushCellRef(ply);
        Call_PushCellRef(ent);
        Call_Finish(canentuse);

        if (!canentuse) return Plugin_Handled;

        if (ent.meta.onuse)
        {
            char funcname[32];
            ent.meta.onuse.name(funcname, sizeof(funcname));

            Call_StartFunction(ent.meta.onuse.hndl, GetFunctionByName(ent.meta.onuse.hndl, funcname));
            Call_PushCellRef(ply);
            Call_PushCellRef(ent);
            Call_Finish(canentuse);
        }
    }

    return Plugin_Continue;
}

public void CB_EntTouch(int firstentity, int secondentity)
{
    Entity ent1 = ents.Get(firstentity), ent2 = ents.Get(secondentity);

    if (ent1.meta && ent1.meta.ontouch && ent2)
    {
        char funcname[32];
        ent1.meta.ontouch.name(funcname, sizeof(funcname));

        Call_StartFunction(ent1.meta.ontouch.hndl, GetFunctionByName(ent1.meta.ontouch.hndl, funcname));
        Call_PushCellRef(ent2);
        Call_PushCellRef(ent1);
        Call_Finish();
    }
}

public Action SoundHandler(int clients[MAXPLAYERS], int &numClients, char sample[PLATFORM_MAX_PATH], int &entity, int &channel, float &volume, int &level, int &pitch, int &flags, char soundEntry[PLATFORM_MAX_PATH], int& seed)
{
    Action state = Plugin_Continue;

    if (ents.IsExistID(entity))
    {
        Call_StartForward(OnEmitSoundForward);
        Call_PushCell(ents.Get(entity));
        Call_PushString(sample);
        Call_PushFloatRef(volume);
        Call_PushCellRef(level);
        Call_PushCellRef(pitch);
        Call_PushCellRef(flags);
        Call_Finish(state);
    }

    return state;
}