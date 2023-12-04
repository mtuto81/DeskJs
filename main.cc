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
#include "src/Deskjs.hpp"

#include <iostream>

// read script
static void die(const char* why) {
  std::cerr << "fatal error: " << why;
  exit(1);
}
void LogException(JSContext* cx) {
  JS::RootedValue exception(cx);
  if (!JS_GetPendingException(cx, &exception))
    die("Uncatchable exception thrown, out of memory or something");

  JS_ClearPendingException(cx);

  JS::RootedString exc_str(cx, JS::ToString(cx, exception));
  if (!exc_str) die("Exception thrown, could not be converted to string");

  std::cout << "Exception thrown: " << JS_EncodeStringToUTF8(cx, exc_str).get()
            << '\n';
}

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
const char *filename;
static bool HelloExample(JSContext* cx) {
  JS::RootedObject global(cx, deskjs::CreateGlobal(cx));
  if (!global) {
    return false;
  }

  JSAutoRealm ar(cx, global);
   LogException(cx);
 // Open the file
 int fd = open(filename, O_RDONLY);
 if (fd == -1) {
   perror("open");
   return false;
 }

 // Read the file
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

int main(int argc,  char* argv[]) {
 filename = argv[1];

  if (!deskjs::Run(HelloExample,true)) {
     
    return 1;
  }
  return 0;
}