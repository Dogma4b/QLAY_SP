#include <socket>

enum WS_Event
{
    On_Open = 0,
    On_Close,
    On_Error,
    On_Message
}

enum WebSocketFrameType
{
    WebSocketFrame_Continue = 0x0,
    WebSocketFrame_Text,
    WebSocketFrame_Binary,
    WebSocketFrame_Close = 0x8,
    WebSocketFrame_Ping,
    WebSocketFrame_Pong,
    WebSocketFrame_Invalid
}

methodmap WebSocketFrame < Base
{
    public WebSocketFrame(Handle socket)
    {
        WebSocketFrame frame = view_as<WebSocketFrame>(new Base());

        frame.SetInt("_size", 0);
        frame.SetQString("_payload", new QString());
        frame.SetHandle("_socket", socket);

        return frame;
    }

    property Handle _socket
    {
        public get() { return this.GetHandle("_socket"); }
    }

    property WebSocketFrameType type
    {
        public set(WebSocketFrameType val) { this.SetValue("_type", val); }
        public get() { WebSocketFrameType val; return this.GetValue("_type", val) ? val : WebSocketFrame_Invalid; }
    }

    property int size
    {
        public set(int val) { this.SetInt("_size", val); }
        public get() { return this.GetInt("_size"); }
    }

    property int cursor
    {
        public set(int val) { this.SetInt("_cursor", val); }
        public get() { return this.GetInt("_cursor"); }
    }

    property bool fin
    {
        public set(bool val) { this.SetBool("_fin", val); }
        public get() { return this.GetBool("_fin"); }
    }

    property bool ismask
    {
        public set(bool val) { this.SetBool("_ismask", val); }
        public get() { return this.GetBool("_ismask"); }
    }

    property string payload
    {
        public get() { return this.GetQString("_payload"); }
    }

    public void SerializeAndSend(char[] data)
    {
        int size = strlen(data);
        this.size = size;

        int[] mask = new int[4];
        mask[0] = GetRandomInt(1,255);
        mask[1] = GetRandomInt(1,255);
        mask[2] = GetRandomInt(1,255);
        mask[3] = GetRandomInt(1,255);

        for (int i=0; i < size; i++)
            data[i] = data[i] ^ mask[i % 4];

        char[] final = new char[size + 12];
        if (this.fin)
            final[0] |= (1 << 0);
        final[0] |= (view_as<char>(this.type) << 7);

        //Message size block
        int offset = 1;
        if (size < 126)
            final[offset++] = size;
        else if (size < 65534)
        {
            final[offset++] = 126;
            final[offset++] = (size >> 8) & 255;
            final[offset++] = size & 255;
        }
        else if (size < 4294967295)
        {
            final[offset++] = 127;
            final[offset++] = (size >> 56) & 255;
            final[offset++] = (size >> 48) & 255;
            final[offset++] = (size >> 40) & 255;
            final[offset++] = (size >> 32) & 255;
            final[offset++] = (size >> 24) & 255;
            final[offset++] = (size >> 16) & 255;
            final[offset++] = (size >> 8) & 255;
            final[offset++] = size & 255;
        }
        else
            ThrowError("WebSocket packet size limit reached... > 4.2 GB. How it possible? ¯\\_(ツ)_/¯");

        final[1] |= (1 << 7); //Setup mask bit to 1

        final[offset++] = mask[0];
        final[offset++] = mask[1];
        final[offset++] = mask[2];
        final[offset++] = mask[3];

        //Adding payload and save packet in _data variable
        for (int i=0; i < size; i++)
            final[i + offset] = data[i];

        this.size += offset;

        SocketSend(this._socket, final, this.size);

        this.size = 0;
    }

    public bool Deserialise(const char[] data, int size = 4096)
    {
        int offset = 0;

        if (this.size == 0)
        {
            this.type = view_as<WebSocketFrameType>(data[0] & 0x0f);
            this.fin = view_as<bool>(data[0] & 0x80);
            this.ismask = view_as<bool>(data[1] & 0x80);

            int pcksize = data[++offset] & 0x7f;

            if (pcksize < 126)
                this.size = pcksize;
            else if (pcksize == 126)
                this.size = (data[++offset] << 8) | data[++offset];
            else if (pcksize == 127)
                this.size = (data[++offset] << 56) | (data[++offset] << 48) | (data[++offset] << 40) | (data[++offset] << 32) | (data[++offset] << 24) | (data[++offset] << 16) | (data[++offset] << 8) | data[++offset];

            int mask[4];

            if (this.ismask)
            {
                mask[0] = data[++offset];
                mask[1] = data[++offset];
                mask[2] = data[++offset];
                mask[3] = data[++offset];
            }
        }

        int psize = size - offset;
        char[] payload = new char[psize];

        for (int i=0; i < psize; i++)
            payload[i] = data[++offset];

        this.cursor += psize;
        this.payload.append(payload);

        if (this.size < this.cursor)
        {
            //PrintToServer("Frame size: %i", this.size);

            this.size = 0;
            this.cursor = 0;

            return true;
        }

        return false;
    }
}

methodmap WebSocket < Base
{
    public WebSocket(char[] addr)
    {
        WebSocket ws = view_as<WebSocket>(new Base());

        if (strncmp(addr, "ws://", 5) != 0)
        {
            PrintToServer("Error: Invalid websocket address");
            return view_as<WebSocket>(null);
        }

        int idx = 5;
        char ipbuf[15];
        
        while (idx < strlen(addr))
        {
            if (addr[idx] == ':' || addr[idx] == '/') break;
            
            ipbuf[idx - 5] = addr[idx];
            idx++;
        }

        ws.SetQString("ip", new QString(ipbuf));

        char portbuf[15];
        int i = 0;
        idx++;
        while (idx < strlen(addr))
        {
            if (addr[idx] == '/') break;

            portbuf[i] = addr[idx];
            idx++; i++;
        }

        ws.SetInt("port", StringToInt(portbuf));

        ws.SetHandle("_socket", SocketCreate(SOCKET_TCP, Socket_OnError));
        ws.SetHandle("_framecontroller", new WebSocketFrame(ws.GetHandle("_socket")));
        ws.SetQString("_data", new QString());

        SocketSetArg(ws.GetHandle("_socket"), ws);

        return ws;
    }

    property Handle _socket
    {
        public get() { return this.GetHandle("_socket"); }
    }

    property bool IsRunning
    {
        public set(bool val) { this.SetBool("_isrunning", val); }
        public get() { return this.GetBool("_isrunning"); }
    }

    property WebSocketFrame frame
    {
        public get() { return view_as<WebSocketFrame>(this.GetHandle("_framecontroller")); }
    }

    property QString data
    {
        public get() { return view_as<string>(this.GetHandle("_data")); }
    }

    public void RegisterCallback(WS_Event state, char[] funcname)
    {
        switch (state)
        {
            case On_Open:
            {
                this.SetString("_connectcb", funcname);
            }
            case On_Message:
            {
                this.SetString("_receivecb", funcname);
            }
            case On_Close:
            {
                this.SetString("_disconnectcb", funcname);
            }
            case On_Error:
            {
                this.SetString("_errorcb", funcname);
            }
        }
    }

    public void Connect()
    {
        SocketConnect(this._socket, Socket_OnConnect, Socket_OnReceive, Socket_OnDisconnect, this.GetQString("ip").c_str(), this.GetInt("port"));
        this.GetQString("ip").Dispose();
    }

    public bool Disconnect()
    {
        //Добавить пакет закрытия WS
        Socket_OnDisconnect(this._socket, false);
        return SocketDisconnect(this._socket);
    }

    public void WSInit()
    {
        SocketSend(this._socket, "GET / HTTP/1.1\r\nHost: example.com\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\nSec-WebSocket-Version: 13\r\nsat-auth-key: vCEVvLs,.5_tWd.fgs)tiwSwUKaYgo4wTzU+9S%NsM5y9(GlUa!kzIc1Ur(i^C6(#,$kl)$,1ET$oISDo1)S1Vgg9jNGFzbv38O0ixVPYS#2O6*phx,IGAObVGNK$J&wu,iRz+rC7@VCFk+yuKqK.ZSMjqt.d#Ne4twtl=H=vrAG=1Fam.w(=7#^E%?DJ4^27m6WWS*fYY)tKuTSKrVV4UevhrqMc*?R@#b_nLketkDy=1CULanT.)GYDWMmnoq_\r\n\r\n");
    }

    public void Send(char[] data)
    {
        int size = strlen(data), framesize = 32768/*32 Mb*/, framescount = RoundToCeil(float(size) / float(framesize));

        for (int i=0; i < framescount; i++)
        {
            this.frame.type = (i == 0) ? WebSocketFrame_Text : WebSocketFrame_Continue;

            if (i == framescount - 1)
                this.frame.fin = true;

            this.frame.SerializeAndSend((new QString(data)).copyd(i * framesize, framesize).c_str());
        }

        this.frame.fin = false;
    }

    public void SendJSON(any json)
    {
        char data[8192];
        view_as<JSON_OBJECT>(json).Encode(data, sizeof(data));
        this.Send(data);
    }

    public void Dispose()
    {
        if (this.IsRunning)
            this.Disconnect();
    }
}

public void Socket_OnConnect(Handle socket, any arg)
{
    WebSocket ws = websocket(arg);
    if (ws && ws.HasKey("_connectcb"))
    {
        ws.WSInit();
    }
}

public void Socket_OnReceive(Handle socket, const char[] data, const int size, any arg)
{
    WebSocket ws = websocket(arg);
    if (ws && ws.HasKey("_receivecb"))
    {
        if (!ws.IsRunning && size == 129)
        {
            ws.IsRunning = true;

            char funcname[128];
            ws.GetString("_connectcb", funcname, sizeof(funcname));

            Call_StartFunction(null, GetFunctionByName(null, funcname));
            Call_PushCellRef(ws);
            Call_Finish();

            return;
        }

        char funcname[128];
        ws.GetString("_receivecb", funcname, sizeof(funcname));

        if (ws.frame.Deserialise(data, size))
        {
            //char[] chunk = new char[ws.frame.payload.size + 1];
            //ws.frame.payload.GetCString(chunk, ws.frame.payload.size + 1);

            //PrintToServer("Frame string buffer size: %i", ws.frame.payload.size);
            //PrintToServer("Received data size: %i", strlen(chunk));

            //ws.data.append(chunk);

            //char[] message = new char[ws.data.size];
            //ws.data.GetCString(message, ws.data.size);

            Call_StartFunction(null, GetFunctionByName(null, funcname));
            Call_PushCell(ws.frame.payload);
            Call_PushCell(ws.frame.payload.size);
            Call_Finish();

            ws.frame.payload.Dispose();
        }
    }
}

public void Socket_OnDisconnect(Handle socket, any arg)
{
    WebSocket ws = websocket(arg);
    if (ws)
    {
        ws.IsRunning = false;

        if (ws.HasKey("_disconnectcb"))
        {
            char funcname[128];
            ws.GetString("_disconnectcb", funcname, sizeof(funcname));

            Call_StartFunction(null, GetFunctionByName(null, funcname));
            Call_Finish();

            ws.Dispose();
        }
    }
}

public void Socket_OnError(Handle socket, const int error, const int num, any arg)
{
    WebSocket ws = websocket(arg);
    if (ws && ws.HasKey("_errorcb"))
    {
        char funcname[128];
        ws.GetString("_errorcb", funcname, sizeof(funcname));

        Call_StartFunction(null, GetFunctionByName(null, funcname));
        Call_PushCell(error);
        Call_PushCell(num);
        Call_Finish();
    }
}