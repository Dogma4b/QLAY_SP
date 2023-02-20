/*----------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------World Text controller------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------*/

enum TypeWorldText {
    All = 0,
    Text,
    NukeMonitor
}

methodmap WorldText < Entity {
    
    public WorldText(int id) {
        WorldText self = view_as<WorldText>(new Entity(id));
        return self;
    }

    property TypeWorldText type {
        public set(TypeWorldText val) { this.SetInt("type", view_as<int>(val)); }
        public get() { return view_as<TypeWorldText>(this.GetInt("type", 0)); }
    }
    
    public WorldText SetText(char[] text) {
        this.SetPropString("m_szText", text);
        return this;
    }
    
    public WorldText SetSize(any size) {
        this.SetPropFloat("m_flTextSize", size);
        return this;
    }

    public WorldText SetColor(Colour col) {
        this.SetProp("m_textColor", (col.b << 16) + (col.g << 8) + col.r);
        delete col;
        return this;
    }
}