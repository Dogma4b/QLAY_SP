#include <json>

enum JSON_TYPE {
    Invalid = -1,
    String = 0,
    Int,
    Int64,
    Float,
    Bool,
    Object
}

methodmap JSON_ARRAY < JSON_Array {

    public JSON_ARRAY() { return view_as<JSON_ARRAY>(new JSON_Array()); }

    public JSON_TYPE GetType(int index) {
        return view_as<JSON_TYPE>(this.GetKeyType(index));
    }

    public Vector GetVector(int index) {
        JSON_ARRAY pos = view_as<JSON_ARRAY>(this.Get(index));
        if (pos == null || pos.Length == 0) return null;
        return new Vector(float(pos.GetInt(0)),float(pos.GetInt(1)),float(pos.GetInt(2)));
    }

    public Angle GetAngle(int index) {
        JSON_ARRAY ang = view_as<JSON_ARRAY>(this.Get(index));
        if (ang == null || ang.Length == 0) return null;
        return new Angle(float(ang.GetInt(0)),float(ang.GetInt(1)),float(ang.GetInt(2)));
    }

    public Colour GetColor(int index) {
        JSON_ARRAY col = view_as<JSON_ARRAY>(this.Get(index));
        if (col == null || col.Length == 0) return null;
        return new Colour(col.GetInt(0),col.GetInt(1),col.GetInt(2));
    }

    public JSON_Object Get(int index) { return view_as<JSON_Object>(this.GetObject(index)); } 
    public JSON_ARRAY GetArr(int index) { return view_as<JSON_ARRAY>(this.GetObject(index)); }
    
    public void Dispose() {
        this.Cleanup();
    }
}

methodmap JSON_OBJECT < JSON_Object {

    public JSON_OBJECT() { return view_as<JSON_OBJECT>(new JSON_Object()); }

    public static JSON_OBJECT FromText(char[] data) { return view_as<JSON_OBJECT>(json_decode(data)); }

    public JSON_TYPE GetType(char[] name) {
        return view_as<JSON_TYPE>(this.GetKeyType(name));
    }

    public Vector GetVector(char[] name) {
        JSON_ARRAY pos = view_as<JSON_ARRAY>(this.Get(name));
        if (pos == null || pos.Length == 0) return null;
        return new Vector(float(pos.GetInt(0)),float(pos.GetInt(1)),float(pos.GetInt(2)));
    }

    public Angle GetAngle(char[] name) {
        JSON_ARRAY ang = view_as<JSON_ARRAY>(this.Get(name));
        if (ang == null || ang.Length == 0) return null;
        return new Angle(float(ang.GetInt(0)),float(ang.GetInt(1)),float(ang.GetInt(2)));
    }

    public Colour GetColor(char[] name) {
        JSON_ARRAY col = view_as<JSON_ARRAY>(this.Get(name));
        if (col == null || col.Length == 0) return null;
        return new Colour(col.GetInt(0),col.GetInt(1),col.GetInt(2));
    }

    public JSON_OBJECT Get(char[] name) { return view_as<JSON_OBJECT>(this.GetObject(name)); }
    public JSON_ARRAY GetArr(char[] name) { return view_as<JSON_ARRAY>(this.GetObject(name)); }

    public void Dispose() {
        this.Cleanup();
    }
}