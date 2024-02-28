#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <jsapi.h>
#include <js/CallArgs.h>
#include <js/Value.h>
#include "src/util.h"
// Define the functions that will be exposed by the fs module
static bool js_fs_readFileSync(JSContext *cx, unsigned argc, JS::Value *vp) {
  if (argc < 1) {
    JS_ReportError(cx, "fs.readFileSync requires at least one argument");
    return false;
  }

  // Get the file path from the first argument
  JS::Rooted<JS::Value> path(cx, vp[0]);
  if (!path.isString()) {
    JS_ReportError(cx, "fs.readFileSync first argument must be a string");
    return false;
  }
  std::string filePath = JS::ToString(cx, path);

  // Read the file contents into a string
  FILE *file = fopen(filePath.c_str(), "rb");
  if (!file) {
    JS_ReportError(cx, "fs.readFileSync failed to open file: %s", filePath.c_str());
    return false;
  }
  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  fseek(file, 0, SEEK_SET);
  char *fileContents = new char[fileSize + 1];
  fread(fileContents, 1, fileSize, file);
  fileContents[fileSize] = 0;
  fclose(file);

  // Return the file contents as a string
  JS::Rooted<JS::Value> result(cx, JS::StringValue(cx, fileContents));
  delete[] fileContents;
  vp->set(result);
  return true;
}

static bool js_fs_writeFileSync(JSContext *cx, unsigned argc, JS::Value *vp) {
  if (argc < 2) {
    JS_ReportError(cx, "fs.writeFileSync requires at least two arguments");
    return false;
  }

  // Get the file path and contents from the first and second arguments
  JS::Rooted<JS::Value> path(cx, vp[0]);
  if (!path.isString()) {
    JS_ReportError(cx, "fs.writeFileSync first argument must be a string");
    return false;
  }
  std::string filePath = JS::ToString(cx, path);
  JS::Rooted<JS::Value> data(cx, vp[1]);
  if (!data.isString()) {
    JS_ReportError(cx, "fs.writeFileSync second argument must be a string");
    return false;
  }
  std::string fileContents = JS::ToString(cx, data);

  // Write the file contents to the file
  FILE *file = fopen(filePath.c_str(), "wb");
  if (!file) {
    JS_ReportError(cx, "fs.writeFileSync failed to open file: %s", filePath.c_str());
    return false;
  }
  fwrite(fileContents.c_str(), 1, fileContents.length(), file);
  fclose(file);

  // Return undefined
  vp->setUndefined();
  return true;
}

// Define the functions that will be exposed by the fs module
static JSFunctionSpec fs_functions[] = {
  JS_FN("readFileSync", js_fs_readFileSync, 1, 0),
  JS_FN("writeFileSync", js_fs_writeFileSync, 2, 0),
  JS_FS_END
};

// Initialize the fs module
extern "C" JS_PUBLIC_API JSModule *init_fs_module(JSContext *cx, JS::HandleObject global) {
  JS::RootedObject fs(cx, JS_NewPlainObject(cx));
  if (!JS_DefineFunctions(cx, fs, fs_functions)) {
    return nullptr;
  }
  if (!JS_DefineProperty(cx, global, "fs", fs, JSPROP_PERMANENT | JSPROP_READONLY)) {
    return nullptr;
  }
  return JS_NewModule(cx, &global.toObject(), nullptr, nullptr);
}
