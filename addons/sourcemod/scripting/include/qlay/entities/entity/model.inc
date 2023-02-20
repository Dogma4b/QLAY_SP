/*----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------Model class------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------*/

methodmap Mdl < Base {

    public Mdl(int entid) {
        Mdl self = view_as<Mdl>(new Base());
        self.SetInt("entid", entid);

        return self;
    }

    property int entid {
        public set(int id) { this.SetInt("entid", id); }
        public get() { return this.GetInt("entid"); }
    }

    property float scale {
        public set(float val) { SetEntPropFloat(this.entid, Prop_Send, "m_flModelScale", val); }
        public get() { return GetEntPropFloat(this.entid, Prop_Send, "m_flModelScale"); }
    }

    public Mdl SetPath(char[] modelName) { SetEntityModel(this.entid, modelName); return this; }

    public Mdl GetPath(char[] modelName, int max_size) { GetEntPropString(this.entid, Prop_Data, "m_ModelName", modelName, max_size); return this; }

    public void SetSkin(int skin) { SetEntProp(this.entid, Prop_Send, "m_nSkin", skin); }

    public int GetSkin() { return GetEntProp(this.entid, Prop_Send, "m_nSkin"); }

    public Mdl SetRenderMode(RenderMode mode) { SetEntityRenderMode(this.entid, mode); return this; }

    public Mdl SetRenderColor(Colour clr) { SetEntityRenderColor(this.entid, clr.r, clr.g, clr.b, clr.a); delete clr; return this; }

    public void Dispose() {
        delete this;
    }
}