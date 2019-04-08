class Tasks
{
    public:
        Tasks();
        static void motorDriver(void * parameter);
        static void cameraInput(void * parameter);
        static void readDistanceSensor(void * parameter);
        static void remoteDebugger(void * parameter);
        static void IRdecoder(void * parameter);
};