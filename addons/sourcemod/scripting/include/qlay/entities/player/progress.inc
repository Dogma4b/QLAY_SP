/*----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------Progress bar-----------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------*/

methodmap ProgressBar < Base {

    public ProgressBar(Base ply) {
        ProgressBar self = view_as<ProgressBar>(new Base());
        self.SetValue("ply", ply);
        self.SetInt("st", FindSendPropInfo("CBaseEntity", "m_flSimulationTime"));
        self.SetInt("pbst", FindSendPropInfo("CCSPlayer", "m_flProgressBarStartTime"));
        self.SetInt("pbd", FindSendPropInfo("CCSPlayer", "m_iProgressBarDuration"));
        self.SetInt("buaip", FindSendPropInfo("CCSPlayer", "m_iBlockingUseActionInProgress"));
        return self;
    }

    property Base ply {
        public get() { Base val; return this.GetValue("ply", val) ? val : null; }
    }

    property int clientID {
        public get() { return this.ply.GetInt("id"); } 
    }

    property bool active {
        public set(bool val) { this.SetBool("active", val); }
        public get() { return this.GetBool("active"); }
    }

    public void timername(char[] buffer, int max_size = 0) { (max_size == 0) ? this.SetString("timername", buffer) : this.GetString("timername", buffer, max_size); }

    public Tmr Start(int time, char[] funcName = "") {
        
        int it = time / 1000;
        float ft = float(it);
        float gm = GetGameTime();

        SetEntDataFloat(this.clientID, this.GetInt("st"), gm + ft);
        SetEntData(this.clientID, this.GetInt("pbd"), it, 4, true);
        SetEntDataFloat(this.clientID, this.GetInt("pbst"), gm - (it - ft), true);
        SetEntData(this.clientID, this.GetInt("buaip"), 0, 4, true);

        char timername[64];
        Format(timername, sizeof(timername), "ply-%i|progress-bar", this.clientID);

        this.timername(timername);

        this.active = true;

        if (!StrEqual(funcName, ""))
            return timer.Create(timername, time, 1, funcName, this.ply);

        return null;
    }

    public void Stop(bool tmrdest = true) {
        char timername[64];
        this.timername(timername, sizeof(timername));

        SetEntDataFloat(this.clientID, this.GetInt("pbst"), 0.0, true);
        SetEntData(this.clientID, this.GetInt("pbd"), 0, 1, true);
        
        if (tmrdest)
            timer.RemoveByName(timername);
        
        this.active = false;
    }

    public void Dispose() {
        delete this;
    }
}