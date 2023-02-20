
methodmap QLAY < Base
{
    public QLAY()
    {
        QLAY qlay = view_as<QLAY>(new Base());

        return qlay;
    }

    property bool IsWarmup {
        public get() { return (GameRules_GetProp("m_bWarmupPeriod")) ? true : false; }
    }
}