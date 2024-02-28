#include <jsapi.h>
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
#include <js/Conversions.h>
#include<js/Object.h>
#include "Deskjs.hpp"




namespace console
{
    bool Console_log(JSContext* cx, unsigned argc, JS::Value* vp);
} // namespace name


namespace Window
{

    bool Window_alert(JSContext* cx, unsigned argc, JS::Value* vp);


};
namespace platform
{
    bool os(JSContext* cx,unsigned argc, JS::Value* vp);
} // namespace platform
