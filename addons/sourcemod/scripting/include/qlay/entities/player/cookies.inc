/*----------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------Player storage system-------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------*/

#include <clientprefs>

methodmap PlayerCookies < Base {

    public PlayerCookies(Base ply) {
        PlayerCookies cookies = view_as<PlayerCookies>(new Base());

        cookies.SetHandle("player", ply);
        
        return cookies;
    }

    public bool SetString(char[] index, char[] val)
    {
        char plname[64];
        GetPluginInfo(GetMyHandle(), PlInfo_Name, plname, sizeof(plname));
        Cookie cookie = new Cookie(index, plname, CookieAccess_Protected);
        cookie.Set(this.GetBase("player").GetInt("id"), val);
        delete cookie;
    }
    public bool SetInt(char[] index, int val) { char buf[32]; IntToString(val, buf, sizeof(buf)); this.SetString(index, buf); }
    public bool SetFloat(char[] index, float val) { char buf[32]; FloatToString(val, buf, sizeof(buf)); this.SetString(index, buf); }
    public bool SetBool(char[] index, bool val) { char buf[32]; IntToString(val, buf, sizeof(buf)); this.SetString(index, buf); }

    public bool GetString(char[] index, char[] val, int max_size)
    {
        char plname[64];
        GetPluginInfo(GetMyHandle(), PlInfo_Name, plname, sizeof(plname));
        Cookie cookie = new Cookie(index, plname, CookieAccess_Protected);
        cookie.Get(this.GetBase("player").GetInt("id"), val, max_size);
        delete cookie;
    }
    public int GetInt(char[] index, int defval = 0) { char buf[32]; this.GetString(index, buf, sizeof(buf)); return StringToInt(buf); }
    public float GetFloat(char[] index, float defval = 0.0) { char buf[32]; this.GetString(index, buf, sizeof(buf)); return StringToFloat(buf); }
    public bool GetBool(char[] index, bool defval = false) { char buf[32]; this.GetString(index, buf, sizeof(buf)); return view_as<bool>(StringToInt(buf)); }
}