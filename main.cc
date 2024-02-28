#include <cstdio>
#include <fcntl.h>
#include <unistd.h>

#include <jsapi.h>
#include <js/CompilationAndEvaluation.h>
#include <js/SourceText.h>
#include <js/ErrorReport.h>
#include <js/Exception.h>
#include <jsfriendapi.h>
#include <js/Conversions.h>
#include<js/Object.h>
#include "src/Deskjs.hpp"
#include "src/util.h"
#include <iostream>
using namespace JS;
using namespace console;
using namespace platform;
 int *p  = nullptr;
 const char *filename = nullptr;
JSClass consoleClass = {"Console",0};
JSClass platformCl = {"Platform",0};
JSClass window = {"Window",0};


// read script
static bool ExecuteCodePrintResult(JSContext* cx, const char* code) {
  JS::CompileOptions options(cx);


  JS::SourceText<mozilla::Utf8Unit> source;
  if (!source.init(cx, code, strlen(code), JS::SourceOwnership::Borrowed)) {
    return false;
  }

  JS::RootedValue rval(cx);
  if (!JS::Evaluate(cx, options, source, &rval)) return false;

  // There are many ways to display an arbitrary value as a result. In this
  // case, we know that the value is an ASCII string because of the expression
  // that we executed, so we can just print the string directly.
  printf("%s\n", JS_EncodeStringToASCII(cx, rval.toString()).get());
  return true;
}

static bool Repl(JSContext* cx) {
  JS::RootedObject global(cx, deskjs::CreateGlobal(cx));
  if (!global) {
    return false;
  }

  JSAutoRealm ar(cx, global);
    static JSFunctionSpec console_methods[] = {
      JS_FN("log", console::Console_log, 1, 0),
      JS_FS_END
  };
static JSFunctionSpec platform_methods[] = {
      JS_FN("OS",platform::os, 1, 0),
      JS_FS_END
  };
  static JSFunctionSpec window_methods[] = {
    JS_FN("alert", Window::Window_alert, 1, 0),
    JS_FS_END
  };

  JS::RootedObject window_obj(cx, JS_DefineObject(cx, global, "window", &window,  JSPROP_READONLY | JSPROP_ENUMERATE));
if (!window_obj) {
  // Handle error
  return false;
}

  
  if (!JS_DefineFunctions(cx, window_obj, window_methods)) {
    // Handle error
 return false;
  }

    JS::RootedObject console_obj(cx, JS_DefineObject(cx, global, "console", &consoleClass,  JSPROP_READONLY | JSPROP_ENUMERATE));
  if (!console_obj) {
      // Handle error
      return false;
  }

  if (!JS_DefineFunctions(cx, console_obj, console_methods)) {
      // Handle error
      return false;
  }

       JS::RootedObject plat_obj(cx, JS_DefineObject(cx, global, "platform", &platformCl,  JSPROP_READONLY | JSPROP_ENUMERATE));
  if (!plat_obj) {
      // Handle error
      return false;
  }

  if (!JS_DefineFunctions(cx, plat_obj, platform_methods)) {
      // Handle error
      return false;
  }

 // Offload a blocking task to a separate thread




 // Open the file
 int fd = open(filename, O_RDONLY);
 if (fd == -1) {
   perror("open\n");
   return false;
 }

 // buffer size
 char buffer[1024];
 ssize_t bytesRead = read(fd, buffer, sizeof(buffer));
 if (bytesRead == -1) {
   perror("read");
   
   return false;
 }

 // Close the file
 if (close(fd) == -1) {
   perror("close");
   return false;
 }
  std::string code(buffer, bytesRead);
  return ExecuteCodePrintResult(cx, code.c_str());
}


void getEvent(){

}










int main(int argc,  char* argv[]) {
 filename = argv[1];
 gtk_init(&argc, &argv);
while (true) {
    
    // Execute the event's callback
 if (!deskjs::Run(Repl,true)) {
      printf("error\n");
      
    return 1;
  }
  return 0;
}

  
}