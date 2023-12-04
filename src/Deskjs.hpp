#include<jsapi.h>


namespace deskjs {

    extern const JSClassOps DefaultGlobalClassOps;

     JSObject* CreateGlobal(JSContext* cx);

     bool Run(bool (*task)(JSContext*), bool initSelfHosting = true);
     void ReportAndClearException(JSContext* cx);
     

}