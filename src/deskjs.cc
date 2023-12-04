#include <jsapi.h>
#include <js/Initialization.h>
#include "Deskjs.hpp"


// Root object
JSObject* deskjs::CreateGlobal(JSContext* cx) {

  JS::RealmOptions options;
// The class of the global object.
static JSClass global_class = {
"global", JSCLASS_GLOBAL_FLAGS,&JS::DefaultGlobalClassOps};

return JS_NewGlobalObject(cx, &global_class,nullptr,JS::FireOnNewGlobalHook,options);
}

// Helper to read current exception and dump to stderr.
//
// NOTE: This must be called with a JSAutoRealm (or equivalent) on the stack.
void deskjs::ReportAndClearException(JSContext* cx) {
  JS::ExceptionStack stack(cx);
  if (!JS::StealPendingExceptionStack(cx, &stack)) {
    fprintf(stderr, "Uncatchable exception thrown, out of memory or something");
    exit(1);
  }

  JS::ErrorReportBuilder report(cx);
  if (!report.init(cx, stack, JS::ErrorReportBuilder::WithSideEffects)) {
    fprintf(stderr, "Couldn't build error report");
    exit(1);
  }

  JS::PrintError(stderr, report, false);
}



bool deskjs::Run(bool (*task)(JSContext*),bool initSelfHosting) {
    // Initialize the JS engine.
    if (!JS_Init()) {
      return false;
    }

    // Create a JS context.
   JSContext* cx = JS_NewContext(JS::DefaultHeapMaxBytes);
    if (!cx) {
      return false;
    }

if (initSelfHosting && !JS::InitSelfHostedCode(cx)){
  return false;
}

if(!task(cx)){
    return false;

}

    // Shut everything down.
    JS_DestroyContext(cx);
    JS_ShutDown();

    return true;
}
