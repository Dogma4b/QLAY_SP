enum InventoryEvents {
    ON_TOUCH = 0,
    ON_USE,
    ON_DROP
}

methodmap OnUseMeta < Base {

    public OnUseMeta() {
        return view_as<OnUseMeta>(new Base());
    }
    
    property Handle hndl {
        public set(Handle val) { this.SetValue("hndl", val); }
        public get() { Handle val; return this.GetValue("hndl", val) ? val : null; }
    }

    public void name(char[] buffer, int max_size = 0) { (max_size == 0) ? this.SetString("name", buffer) : this.GetString("name", buffer, max_size); }
}

methodmap OnTouchMeta < Base {

    public OnTouchMeta() {
        return view_as<OnTouchMeta>(new Base());
    }
    
    property Handle hndl {
        public set(Handle val) { this.SetValue("hndl", val); }
        public get() { Handle val; return this.GetValue("hndl", val) ? val : null; }
    }

    public void name(char[] buffer, int max_size = 0) { (max_size == 0) ? this.SetString("name", buffer) : this.GetString("name", buffer, max_size); }
}

methodmap OnDropMeta < Base {

    public OnDropMeta() {
        return view_as<OnDropMeta>(new Base());
    }
    
    property Handle hndl {
        public set(Handle val) { this.SetValue("hndl", val); }
        public get() { Handle val; return this.GetValue("hndl", val) ? val : null; }
    }

    public void name(char[] buffer, int max_size = 0) { (max_size == 0) ? this.SetString("name", buffer) : this.GetString("name", buffer, max_size); }
}

methodmap ItemSoundMeta < Base {

    public ItemSoundMeta() {
        return view_as<ItemSoundMeta>(new Base());
    }

    public bool touch(char[] buffer, int max_size = 0) { return (max_size == 0) ? this.SetString("touch", buffer) : this.GetString("touch", buffer, max_size); }

    public bool pickup(char[] buffer, int max_size = 0) { return (max_size == 0) ? this.SetString("pickup", buffer) : this.GetString("pickup", buffer, max_size); }

    public bool use(char[] buffer, int max_size = 0) { return (max_size == 0) ? this.SetString("use", buffer) : this.GetString("use", buffer, max_size); }

    public bool drop(char[] buffer, int max_size = 0) { return (max_size == 0) ? this.SetString("drop", buffer) : this.GetString("drop", buffer, max_size); }
}

methodmap ItemMenuMeta < Base {

    public ItemMenuMeta() {
        return view_as<ItemMenuMeta>(new Base());
    }

    public bool use(char[] buffer, int max_size = 0) { return (max_size == 0) ? this.SetString("use", buffer) : this.GetString("use", buffer, max_size); }

    public bool cd(char[] buffer, int max_size = 0) { return (max_size == 0) ? this.SetString("cd", buffer) : this.GetString("cd", buffer, max_size); }

    public bool info(char[] buffer, int max_size = 0) { return (max_size == 0) ? this.SetString("info", buffer) : this.GetString("info", buffer, max_size); }

    public bool drop(char[] buffer, int max_size = 0) { return (max_size == 0) ? this.SetString("drop", buffer) : this.GetString("drop", buffer, max_size); }
}

methodmap EntityMeta < Base {

    public EntityMeta() {
        EntityMeta meta = view_as<EntityMeta>(new Base());
        return meta;
    }
    
    property float mass {
        public set(float val) { this.SetFloat("mass", val); }
        public get() { return this.GetFloat("mass"); }
    }

    property int bodygroup {
        public set(int val) { this.SetInt("bodygroup", val); }
        public get() { return this.GetInt("bodygroup"); }
    }

    property int skin {
        public set(int val) { this.SetInt("skin", val); }
        public get() { return this.GetInt("skin"); }
    }

    property int spawnflags {
        public set(int val) { this.SetInt("spawnflags", val); }
        public get() { return this.GetInt("spawnflags", 4356); }
    }

    property bool usable {
        public set(bool val) { this.SetBool("usable", val); }
        public get() { return this.GetBool("usable"); }
    }

    property bool SCPCanUse {
        public set(bool val) { this.SetBool("scpcanuse", val); }
        public get() { return this.GetBool("scpcanuse"); }
    }

    property int cd {
        public set(int val) { this.SetInt("cooldown", val); }
        public get() { return this.GetInt("cooldown"); }
    }

    property OnTouchMeta ontouch {
        public set(OnTouchMeta val) { this.SetValue("ontouch", val); }
        public get() { OnTouchMeta val; return this.GetValue("ontouch", val) ? val : null; }
    }

    property OnUseMeta onuse {
        public set(OnUseMeta val) { this.SetValue("onuse", val); }
        public get() { OnUseMeta val; return this.GetValue("onuse", val) ? val : null; }
    }

    property OnDropMeta ondrop {
        public set(OnDropMeta val) { this.SetValue("ondrop", val); }
        public get() { OnDropMeta val; return this.GetValue("ondrop", val) ? val : null; }
    }

    property ItemSoundMeta sound {
        public set(ItemSoundMeta val) { this.SetValue("sound", val); }
        public get() { ItemSoundMeta val; return this.GetValue("sound", val) ? val : null; }
    }

    property ItemMenuMeta menu {
        public set(ItemMenuMeta val) { this.SetValue("menu", val); }
        public get() { ItemMenuMeta val; return this.GetValue("menu", val) ? val : null; }
    }

    public void name(char[] buffer, int max_size = 0) { (max_size == 0) ? this.SetString("name", buffer) : this.GetString("name", buffer, max_size); }

    public void model(char[] buffer, int max_size = 0) { (max_size == 0) ? this.SetString("model", buffer) : this.GetString("model", buffer, max_size); }
    
    public void Dispose() {
        if (this.ontouch) delete this.ontouch.hndl;
        delete this.ontouch;
        if (this.onuse) delete this.onuse.hndl;
        delete this.onuse;
        if (this.ondrop) delete this.ondrop.hndl;
        delete this.ondrop;
        delete this.sound;
        delete this.menu;
        delete this;
    }
}

methodmap Meta < Base {

    public Meta() {
        Meta meta = view_as<Meta>(new Base());
        meta.CreateBase("entities");
        return meta;
    }

    public void RegisterEntity(char[] entclass, EntityMeta entmeta) { this.GetBase("entities").SetHandle(entclass, entmeta); }

    public EntityMeta GetEntity(char[] entclass) { return view_as<EntityMeta>(this.GetBase("entities").GetHandle(entclass)); }

    public void RegEntEvent(InventoryEvents eid, char[] entclass, char[] funcname, char[] sound = "") {
        EntityMeta entdata = this.GetEntity(entclass);

        if (strlen(sound) > 0 && !entdata.GetHandle("sound")) entdata.sound = new ItemSoundMeta();
        
        switch (eid) {
            case ON_TOUCH: {
                entdata.ontouch = new OnTouchMeta();

                entdata.ontouch.hndl = GetMyHandle();
                entdata.ontouch.name(funcname);

                if (strlen(sound) > 0) entdata.sound.touch(sound);
            }
            case ON_USE: {
                entdata.onuse = new OnUseMeta();

                entdata.onuse.hndl = GetMyHandle();
                entdata.onuse.name(funcname);

                if (strlen(sound) > 0) entdata.sound.use(sound);
            }
            case ON_DROP: {
                entdata.ondrop = new OnDropMeta();

                entdata.ondrop.hndl = GetMyHandle();
                entdata.ondrop.name(funcname);

                if (strlen(sound) > 0) entdata.sound.drop(sound);
            }
        }
    }

    public void Dispose()
    {
        ArrayList entskeys = this.GetBase("entities").GetKeys();

        char keyname[64];
        for (int i=0; i < entskeys.Length; i++)
        {
            entskeys.GetString(i, keyname, sizeof(keyname));
            view_as<EntityMeta>(this.GetBase("entities").GetHandle(keyname)).Dispose();
        }

        delete entskeys;

        this.GetBase("entities").Dispose();

        delete this;
    }
}