/*----------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------Ambient class-----------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------*/

methodmap Ambient < Entity {
    
    public Ambient(Entity ent = null, int volume = 10, float radius = 800.0, char[] spath = "") {
        Ambient self = view_as<Ambient>(new Entity());
        self.SetBool("spawned", false);

        if (ent)
        {
            char target[32];
            ent.GetString("targetname", target, sizeof(target));

            self.Create("ambient_generic");

            self.SetKV("SourceEntityName", target);
            self.SetKV("message", spath);
            self.SetKVInt("health", volume);
            self.SetKVFloat("radius", radius);

            self.SetInt("volume", volume);
            self.SetFloat("radius", radius);
        }

        return self;
    }
    
    property int volume {
        public set(int val) { this.SetInt("volume", val); this.SetKVInt("health", val); }
        public get() { return this.GetInt("volume", 10); }
    }

    property float radius {
        public set(float val) { this.SetFloat("radius", val); this.SetKVFloat("radius", val); }
        public get() { return this.GetFloat("radius", 800.0); }
    }

    public Ambient SetSource(char[] target) { this.SetKV("SourceEntityName", target); }

    public Ambient SetSourceEnt(Entity ent)
    {
        char target[32];
        ent.GetString("targetname", target, sizeof(target));
        this.SetKV("SourceEntityName", target);
    }

    public Ambient SetSound(char[] path)
    {
        this.SetString("sound", path);
        this.SetKV("message", path);
    }

    public Ambient Play(char[] path = "")
    {
        if (strlen(path) > 0)
        {
            this.Input("StopSound");
            this.SetSound(path);
        }

        if (!this.spawned)
        {
            this.Spawn();
            this.Activate();
            this.SetBool("spawned", true);
        }

        this.Input("PlaySound");
    }

    public Ambient Stop() { this.Input("StopSound"); }
}