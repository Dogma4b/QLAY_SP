//////////////////////////////////////////////////////////////////////////////
//
//                                Natives
//
//////////////////////////////////////////////////////////////////////////////

public any NativeTimers_GetList(Handle Plugin, int numArgs) { return timer.GetArrayList("timers"); }

public any NativeTimers_HideCreate(Handle Plugin, int numArgs) {
    char timername[64], funcname[64];
    GetNativeString(3, timername, sizeof(timername));
    GetNativeString(6, funcname, sizeof(funcname));

    Tmr tmr = new Tmr(GetNativeCell(2), timername, GetNativeCell(4), GetNativeCell(5), funcname, GetNativeCell(7));
    tmr.active = true;
    timer.GetArrayList("timers").Push(tmr);
    return tmr;
}

public any NativeEntities_GetList(Handle Plugin, int numArgs) { return ents.GetArrayList("entities"); }

public any NativeEntities_Create(Handle Plugin, int numArgs) {
    char EntName[32];
    GetNativeString(2, EntName, sizeof(EntName));

    EntityMeta entdata = meta.GetEntity(EntName);

    Entity entity;
    if (entdata)
    {
        entity = new Entity();
        entity.meta = entdata;
        entity.Create();
        
        entity.SetHook(SDKHook_Use, CB_EntUse);
        if (entity.meta.ontouch) entity.SetHook(SDKHook_TouchPost, CB_EntTouch);
    }
    else
    {
        entity = new Entity(CreateEntityByName(EntName));
    }

    entity.spawned = false;
    entity.SetClass(EntName);
    
    if (view_as<bool>(GetNativeCell(3)))
    {
        ents.Push(entity);
    }

    return entity;
}

public any NativeEntities_Push(Handle Plugin, int numArgs) {
    Entity ent = GetNativeCell(2);

    any data[2];
    data[0] = ent.id;
    data[1] = ent;

    ents.list.PushArray(data);

    return ent;
}

public any NativeEntities_Remove(Handle Plugin, int numArgs) {
    ArrayList entities = ents.list;
    Entity entin = GetNativeCell(2);
    int idx = entities.FindValue(entin, 1);
    if (idx != -1)
    {
        Entity ent = entities.Get(idx, 1);
        if (ent.meta)
        {
            ent.RemoveHook(SDKHook_Use, CB_EntUse);
            if (ent.meta.ontouch) ent.RemoveHook(SDKHook_TouchPost, CB_EntTouch);
        }
        ent.Remove();
        entities.Erase(idx);
    }
    else
    {
        char classname[32];
        entin.GetClass(classname, sizeof(classname));
        PrintToServer("Cant find entity in storage. id:%i, class:%s", entin.id, classname);
        entin.Remove();
    }

    return false;
}

public any NativeEntities_RemoveByID(Handle Plugin, int numArgs) {
    ArrayList entities = ents.list;
    int id = GetNativeCell(2);
    int idx = entities.FindValue(id, 0);
    if (idx != -1)
    {
        Entity ent = entities.Get(idx, 1);
        if (ent.meta)
        {
            ent.RemoveHook(SDKHook_Use, CB_EntUse);
            if (ent.meta.ontouch) ent.RemoveHook(SDKHook_TouchPost, CB_EntTouch);
        }
        ent.Remove();
        entities.Erase(idx);
    }
    else
    {
        PrintToServer("Cant find entity in storage. id:%i", id);
    }

    return false;
}

public any NativeEntities_Dissolve(Handle Plugin, int numArgs) {
    ArrayList entities = ents.list;
    Entity ent = GetNativeCell(2);
    int idx = entities.FindValue(ent, 1);

    char targetname[32];
    ent.GetTargetName(targetname, sizeof(targetname));

    Entity disolver = new Entity();
    disolver.Create("env_entity_dissolver");
    disolver.SetKV("dissolvetype", "0");
    disolver.SetKV("target", targetname);

    if (idx != -1)
    {
        ent.Dispose();
        ents.list.Erase(idx);
    }
    else
    {
        char classname[32];
        ent.GetClass(classname, sizeof(classname));
        PrintToServer("Cant find entity in storage. id:%i, class:%s", ent.id, classname);
        ent.Dispose();
    }

    disolver.Input("Dissolve");
    disolver.Remove();

    return false;
}

public any NativeEntities_IndexUpdate(Handle Plugin, int numArgs) {
    Entity ent = GetNativeCell(2);
    ArrayList entities = ents.list;
    int idx = entities.FindValue(ent, 1);
    if (idx != -1)
    {
        entities.Set(idx, ent.id, 0);
        return true;
    }
    else
        return false;
}

public any NativeEntities_Clear(Handle Plugin, int numArgs) {

    ArrayList entities = ents.list;

    for(int i=0; i < entities.Length; i++)
    {
        int id = entities.Get(i, 0);

        if (id > MaxClients)
        {
            Entity ent = entities.Get(i, 1);
            ent.Dispose();
            entities.Erase(i);
            i--;
        }
    }

    return false;
}

public any NativeClients_Add(Handle Plugin, int numArgs) {
    return ents.Push(new Player(GetNativeCell(2)));
}

public any NativeClients_Remove(Handle Plugin, int numArgs) {
    ArrayList entities = ents.list;
    int idx = entities.FindValue(GetNativeCell(2), 0);
    view_as<Player>(entities.Get(idx, 1)).Dispose();
    entities.Erase(idx);
    return false;
}

public any NativeWT_Create(Handle Plugin, int numArgs) {
    WorldText wt = view_as<WorldText>(ents.Create("point_worldtext"));

    wt.type = GetNativeCell(4);
    wt.SetPos(GetNativeCell(2), GetNativeCell(3));

    return wt;
}

public any NativeWT_Remove(Handle Plugin, int numArgs) {
    ArrayList entities = ents.list;
    int idx = entities.FindValue(GetNativeCell(2), 1);
    if (idx != -1)
    {
        Entity ent = entities.Get(idx, 0);
        ent.Remove();
        ents.list.Erase(idx);
    }

    return false;
}

public any NativePlayer_GiveWeapon(Handle Plugin, int numArgs) {
    Player ply = GetNativeCell(1);
    
    char itemname[32];
    GetNativeString(2, itemname, sizeof(itemname));

    Entity item = new Entity(GivePlayerItem(ply.id, itemname));
    item.SetClass(itemname);

    ents.Push(item);

    return item;
}

public int NativePlayer_DropWeapons(Handle Plugin, int numArgs) {
    Player ply = GetNativeCell(1);

    int itemid, weparrsize = GetEntPropArraySize(ply.id, Prop_Send, "m_hMyWeapons");

    for(int weparridx = 0; weparridx < weparrsize; weparridx++)
    { 
        itemid = GetEntPropEnt(ply.id, Prop_Send, "m_hMyWeapons", weparridx);

        if(itemid != -1)
        {
            char wepclass[64];
            GetEntityClassname(itemid, wepclass, sizeof(wepclass));
            
            if (!StrEqual(wepclass, "weapon_fists"))
                CS_DropWeapon(ply.id, itemid, false, false);
        }
    }

    return 0;
}

public int NativePlayer_RestrictWeapons(Handle Plugin, int numArgs) {
    Player ply = GetNativeCell(1);

    int itemid, weparrsize = GetEntPropArraySize(ply.id, Prop_Send, "m_hMyWeapons");
    for(int weparridx = 0; weparridx < weparrsize; weparridx++)
    {
        itemid = GetEntPropEnt(ply.id, Prop_Send, "m_hMyWeapons", weparridx);

        if(itemid != -1)
        {
            char wepclass[64];
            GetEntityClassname(itemid, wepclass, sizeof(wepclass));
            
            if (StrEqual(wepclass, "weapon_fists")) continue;

            ents.RemoveByID(itemid);
            RemovePlayerItem(ply.id, itemid);
            AcceptEntityInput(itemid, "Kill");
        }
    }

    return 0;
}