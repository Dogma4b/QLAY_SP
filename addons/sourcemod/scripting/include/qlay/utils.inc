enum TimeCharTypeRU {
    Sec = 0,
    Min,
    Hour
}

methodmap Utils < StringMap {
    
    public static JSON_OBJECT JSONRead(char[] path)
    {
        BuildPath(Path_SM, path, PLATFORM_MAX_PATH, path);
        
        if (FileExists(path))
        {
            char buffer[131072];
            Handle file = OpenFile(path,"r+");
            ReadFileString(file, buffer, sizeof(buffer));
            delete file;

            return view_as<JSON_OBJECT>(json_decode(buffer));
        }
        else
            return new JSON_OBJECT();
    }
    
    public static void JSONWrite(char[] path, JSON_OBJECT json)
    {
        BuildPath(Path_SM, path, PLATFORM_MAX_PATH, path);

        char buffer[131072];
        Handle file = OpenFile(path,"w+");
        json_encode(json, buffer, sizeof(buffer));
        WriteFileString(file, buffer, true);
        delete file;
    }

    public static JSON_OBJECT ReadMapConfig(char[] folder = "default", char[] mapname, char[] filename)
    {
        char path[256];
        Format(path, sizeof(path),"configs/%s/%s/%s.json", folder, mapname, filename);
        return Utils.JSONRead(path);
    }

    public static JSON_OBJECT ReadCurMapConfig(char[] folder = "default", char[] filename)
    {
        char mapname[128];
        GetCurrentMap(mapname, sizeof(mapname));
        return Utils.ReadMapConfig(folder, mapname, filename);
    }

    public static void TransformPath(char[] metapath, char[] outpath, char[] lang = "en", char[] name = "")
    {   
        strcopy(outpath, 256, metapath);
        ReplaceString(outpath, 256, "{lang}", lang);
        ReplaceString(outpath, 256, "{name}", name);
    }

    public static void GetTimeString(char[] str, int max_size, int time)
    {
        Format(str, max_size, (time / 60 < 10 ) ? ((time % 60 < 10 ) ? "%s (0%i:0%i)" : "%s (0%i:%i)") : ((time % 60 < 10 ) ? "%s (%i:0%i)" : "%s (%i:%i)"), str, time / 60, time % 60);
    }

    public static void GetRuTimeChar(char[] charout, int sec, TimeCharTypeRU type = Sec)
    {
        switch (type)
        {
            case Sec: FormatEx(charout, 2, "%s%s", (sec % 10 == 1 && sec % 100 != 11) ? "а": (sec % 10 >= 2 && sec % 10 <= 4 && (sec % 100 < 10 || sec % 100 >= 20)) ? "ы" : "");
            case Min: FormatEx(charout, 2, "%s%s", (sec % 10 == 1 && sec % 100 != 11) ? "а": (sec % 10 >= 2 && sec % 10 <= 4 && (sec % 100 < 10 || sec % 100 >= 20)) ? "ы" : "");
            case Hour: FormatEx(charout, 2, "%s%s", (sec % 10 == 1 && sec % 100 != 11) ? "": (sec % 10 >= 2 && sec % 10 <= 4 && (sec % 100 < 10 || sec % 100 >= 20)) ? "а" : "ов");
        }
    }

    public static void AddRuTimeChar(char[] charout, int max_size, int sec, TimeCharTypeRU type = Sec)
    {
        switch (type)
        {
            case Sec: Format(charout, max_size, "%s%s", charout, (sec % 10 == 1 && sec % 100 != 11) ? "а": (sec % 10 >= 2 && sec % 10 <= 4 && (sec % 100 < 10 || sec % 100 >= 20)) ? "ы" : "");
            case Min: Format(charout, max_size, "%s%s", charout, (sec % 10 == 1 && sec % 100 != 11) ? "а": (sec % 10 >= 2 && sec % 10 <= 4 && (sec % 100 < 10 || sec % 100 >= 20)) ? "ы" : "");
            case Hour: Format(charout, max_size, "%s%s", charout, (sec % 10 == 1 && sec % 100 != 11) ? "": (sec % 10 >= 2 && sec % 10 <= 4 && (sec % 100 < 10 || sec % 100 >= 20)) ? "а" : "ов");
        }
    }
}