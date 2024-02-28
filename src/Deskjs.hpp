#include<jsapi.h>


namespace deskjs {

    extern const JSClassOps DefaultGlobalClassOps;

     JSObject* CreateGlobal(JSContext* cx);
    JSObject* LoadModule(JSContext* cx, const char* filename);
    static bool Require(JSContext* cx, unsigned argc, JS::Value* vp);
     bool Run(bool (*task)(JSContext*), bool initSelfHosting );
     void ReportAndClearException(JSContext* cx);
     

}