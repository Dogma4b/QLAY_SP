
methodmap QString < StringMap
{
    public QString(char[] data = "")
    {
        QString self = view_as<QString>(new Base());
        self.SetString("_data", data);
        self.SetValue("_size", strlen(data));

        return self;
    }

    property int len
    {
        public get() { int size; return this.GetValue("_size", size) ? size : 0; }
    }

    property int size
    {
        public get() { int size; return this.GetValue("_size", size) ? size : 0; }
    }

    property int Length
    {
        public get() { int size; return this.GetValue("_size", size) ? size : 0; }
    }

    public QString append(char[] data)
    {
        int bufsize = this.len + strlen(data) + 1;
        char[] newstr = new char[bufsize];
        char[] curstr = new char[this.len];
        this.GetString("_data", curstr, this.len + 1);
        FormatEx(newstr, bufsize, "%s%s", curstr, data);
        this.SetString("_data", newstr);
        this.SetValue("_size", bufsize - 1);
        return this;
    }

    public QString copy(int offset, int size)
    {
        char[] source = new char[this.len];
        this.GetString("_data", source, this.len + 1);

        int realsize = (this.len >= offset + size) ? size : size - ((offset + size) - this.len);
        char[] dest = new char[realsize];

        for (int i = 0; i < realsize; i++)
            dest[i] = source[i + offset];

        return new QString(dest);
    }

    public QString copyd(int offset, int size)
    {
        QString str = this.copy(offset, size);
        delete this;
        return str;
    }

    public void GetCString(char[] str, int size) { this.GetString("_data", str, size + 1); }

    public char[] c_str()
    {
        char chars[4096];
        this.GetString("_data", chars, sizeof(chars));
        return chars;
    }

    public char[] c_strd()
    {
        char chars[4096];
        this.GetString("_data", chars, sizeof(chars));
        delete this;
        return chars;
    }

    public void Dispose()
    {
        delete this;
    }
}

stock QString operator+(QString str1, QString str2)
{
    int bufsize = str1.len + str2.len;
    char[] newstr = new char[bufsize];
    FormatEx(newstr, bufsize, "%s%s", str1.c_str(), str2.c_str());
    delete str1;
    delete str2;
    return new QString(newstr);
}

stock QString operator+(QString str1, int val)
{
    int bufsize = str1.len + 4;
    char[] newstr = new char[bufsize];
    FormatEx(newstr, bufsize, "%s%i", str1.c_str(), val);
    delete str1;
    return new QString(newstr);
}

stock QString operator+(QString str1, float val)
{
    int bufsize = str1.len + 4;
    char[] newstr = new char[bufsize];
    FormatEx(newstr, bufsize, "%s%f", str1.c_str(), val);
    delete str1;
    return new QString(newstr);
}

stock bool operator==(QString str1, QString str2)
{
    char[] cstr1 = new char[str1.len];
    char[] cstr2 = new char[str2.len];
    str1.GetCString(cstr1, str1.len);
    str2.GetCString(cstr2, str2.len);
    return (strcmp(cstr1, cstr2) == 0) ? true : false;
}

stock bool operator!=(QString str1, QString str2)
{
    return !(str1 == str2);
}