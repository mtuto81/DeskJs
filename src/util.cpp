#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <gtk/gtk.h>


#include <jsapi.h>
#include <js/CompilationAndEvaluation.h>
#include <js/SourceText.h>
#include <js/ErrorReport.h>
#include <js/Exception.h>
#include <jsfriendapi.h>
#include <js/String.h>
#include <js/Value.h>
#include <js/CallArgs.h>

#include <js/Conversions.h>
#include <js/Object.h>
#include "Deskjs.hpp"
#include "util.h"

  #ifdef __linux__
#define OS "linux"
#elif __APPLE__
#define OS "darwin"
#elif __unix__ // all unices not caught above
#define OS "unix"
#elif defined(_WIN32)
#define OS "win32"
#endif

   
using namespace JS;

#include <iostream>
#include <jsapi.h>

bool console::Console_log(JSContext* cx, unsigned argc, JS::Value* vp) {
 JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
 JS::RootedString message(cx, JS::ToString(cx, args.get(0)));
 if (!message){
  return false;
}
 printf("%s\n", JS_EncodeStringToASCII(cx, message).get());
 args.rval().setUndefined();
 return true;
}

bool platform ::os(JSContext* cx,unsigned argc, JS::Value* vp) {
  JS::CallArgs args = JS::CallArgsFromVp(argc, vp);

   

  JS::RootedString OS_name(cx, JS_NewStringCopyZ(cx,OS));
   args.rval().setString(OS_name);
 return true;
}


static GtkWidget *window = NULL;


static void destroy(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}

bool Window::Window_alert(JSContext* cx, unsigned argc, JS::Value* vp) {
   JS::CallArgs arg = JS::CallArgsFromVp(argc, vp);
    JS::RootedValue rval(cx);
 // Convert the first argument to a string
   JS::RootedString alert(cx, JS::ToString(cx, arg.get(0)));
 
    GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
   JS_EncodeStringToASCII(cx,alert).get());
    gtk_widget_show(dialog);

    gtk_main();


    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    
    return true;
}
