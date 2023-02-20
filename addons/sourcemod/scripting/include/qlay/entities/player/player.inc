/*----------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------Player classes----------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------*/

#include "qlay/entities/player/cookies"
#include "qlay/entities/player/progress"

methodmap Player < Entity {

    public Player(int id) {
        Player ply = view_as<Player>(new Entity(id));
        ply.SetBase("cookies", new PlayerCookies(ply));
        ply.SetBase("progress", new ProgressBar(ply));
        ply.SetBool("spawned", false);

        char targetname[32];
        FormatEx(targetname, sizeof(targetname), "player-%i", ply.id);
        ply.SetTargetName(targetname);

        return ply;
    }

    property int lang {
        public get() { return GetClientLanguage(this.id); }
    }

    property int health {
        public set(int val) { SetEntityHealth(this.id, val); }
        public get() { return GetClientHealth(this.id); }
    }

    property int armor {
        public set(int val) { this.SetProp("m_ArmorValue", val); }
        public get() { return this.GetProp("m_ArmorValue"); }
    }

    property float speed {
        public set(float val) { SetEntPropFloat(this.id, Prop_Send, "m_flMaxspeed", val); }
        public get() { return GetEntPropFloat(this.id, Prop_Send, "m_flMaxspeed"); }
    }

    property float multipler {
        public set(float val) { SetEntPropFloat(this.id, Prop_Send, "m_flLaggedMovementValue", val); }
        public get() { return GetEntPropFloat(this.id, Prop_Send, "m_flLaggedMovementValue"); }
    }

    property bool ready {
        public set(bool val) { this.SetBool("ready", val); }
        public get() { return this.GetBool("ready", true); }
    }

    property bool FirstSpawn {
        public set(bool val) { this.SetBool("firstspawn", val); }
        public get() { return this.GetBool("firstspawn", true); }
    }

    property PlayerCookies cookies {
        public set(PlayerCookies cookies) { this.SetHandle("cookies", cookies); }
        public get() { return view_as<PlayerCookies>(this.GetHandle("cookies")); }
    }

    property ProgressBar progress {
        public set(ProgressBar pb) { this.SetHandle("progress", pb); }
        public get() { return view_as<ProgressBar>(this.GetHandle("progress")); }
    }

    public void GetLangInfo(char[] name, int max_size) { GetLanguageInfo(this.lang, name, max_size); }

    public bool GetName(char[] buffer, int max_size = 0) { return GetClientName(this.id, buffer, max_size); }

    public bool GetAuth(char[] buffer, int max_size = 0, AuthIdType type = AuthId_Steam2) { return GetClientAuthId(this.id, type, buffer, max_size, true); }

    public bool IsAlive() { return IsPlayerAlive(this.id); }

    public bool IsAdmin(AdminFlag flag = Admin_Generic) { return GetAdminFlag(GetUserAdmin(this.id), flag); }

    public bool InGame() { return IsClientInGame(this.id); }

    public Vector EyePos() { float vecarr[3]; GetClientEyePosition(this.id, vecarr); return new Vector(vecarr[0], vecarr[1], vecarr[2]); }

    public Angle GetAng() { float angarr[3]; GetClientEyeAngles(this.id, angarr); return new Angle(angarr[0], angarr[1], angarr[2]); }

    public void SetHandsModel(char[] model) { SetEntPropString(this.id, Prop_Send, "m_szArmsModel", model); }

    public void Team(char[] buffer, int max_size = 0) { (max_size == 0) ? this.SetString("team", buffer) : this.GetString("team", buffer, max_size); }

    public void ExecCommand(char[] format, any ...) {
        int len = strlen(format) + 255;
        char[] data = new char[len];
        VFormat(data, len, format, 3);
        ClientCommand(this.id, data);
    }

    public void PrintNotify(const char[] format, any ...) {
        int len = strlen(format) + 8192;
        char[] formattedText = new char[len];
        VFormat(formattedText, len, format, 3);
        PrintHintText(this.id, "%s", formattedText);
    }

    public void PrintWarning(const char[] format, any ...) {
        int len = strlen(format) + 8192;
        char[] formattedText = new char[len];
        VFormat(formattedText, len, format, 3);
        PrintCenterText(this.id, "%s", formattedText);
    }

    public void SetListen(Player ply, bool islisten)
    {
        SetListenOverride(this.id, ply.id, islisten ? Listen_Yes : Listen_No);
    }

    public bool GetListen(Player ply)
    {
        return (GetListenOverride(this.id, ply.id) == Listen_Yes) ? true : false;
    }

    public void PlaySound(char[] path, int channel = 1, int level = 100, int entity = 0) {
        if (!this.ready) return;

        int clients[1];
        clients[0] = this.id;

        EmitSound(clients, sizeof(clients), path, (entity == 0) ? this.id : entity, channel, level);
    }

    public void PlayNonCheckSound(char[] path) {
        if (!this.ready) return;

        char soundcommand[256];
        FormatEx(soundcommand, sizeof(soundcommand), "playgamesound %s", path);
        this.ExecCommand(soundcommand);
    }

    public void PlayAmbient(char[] path)
    {
        float vecarr[3];
        Vector vec = this.EyePos();
        vec.GetArr(vecarr);
        delete vec;
        EmitAmbientSound(path, vecarr, this.id);
    }

    public void StopSound(char[] path, int channel = 1) {
        StopSound(this.id, channel, path);
    }

    public void GetCurWeapon(char[] name, int max_size = 0) { GetClientWeapon(this.id, name, max_size); }

    public native Entity Give(char[] item);

    public native void DropWeapons();

    public native void RestrictWeapons();

    public void TakeDamage(Player ply = null, float damage = 25.0, any dmgtype = DMG_GENERIC) {
        SDKHooks_TakeDamage(this.id, (ply) ? ply.id : 0, (ply) ? ply.id : 0, damage, dmgtype);
    }

    public void Kill() { ForcePlayerSuicide(this.id); }

    public void Kick(char[] reason) { KickClient(this.id, reason); }

    public void Dispose() {
        this.progress.Dispose();
        this.model.Dispose();
        delete this;
    }
}