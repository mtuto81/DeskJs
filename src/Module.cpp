#include <jsapi.h>
#include <js/Module.h>
#include <js/RootingAPI.h>
#include <string>
#include <fstream>
#include <sstream>
#include "src/util.h"
class ModuleResolver : public JS::ModuleResolver {
public:
    JSObject* resolve(JSContext* cx, const char* specifier, JS::Handle<JSObject*> importer, JS::MutableHandle<JS::ModuleResolveResult> result) override {
        // Convertir le spécificateur en chaîne de caractères std::string
        std::string specifierStr(specifier);

        // Charger le contenu du module à partir du système de fichiers
        std::ifstream file(specifierStr);
        if (!file.is_open()) {
            // Le fichier n'a pas pu être ouvert, renvoyer une erreur
            result.set(JS::ModuleResolveResult::Error("Module not found"));
            return nullptr;
        }
        std::stringstream content;
        content << file.rdbuf();
        std::string moduleContent = content.str();

        // Créer un objet JSModule pour le module chargé
        JS::Rooted<JSObject*> module(cx, JS::NewPlainObject(cx));
        if (!module) {
            return nullptr;
        }
        JS::Rooted<JS::Value> exports(cx);
        if (!JS::DefineProperty(cx, module, "exports", exports, JSPROP_ENUMERATE | JSPROP_PERMANENT | JSPROP_READONLY)) {
            return nullptr;
        }

        // Compiler le code du module et l'ajouter à l'objet JSModule
        JS::Rooted<JS::Value> code(cx, JS::CompileScript(cx, moduleContent.c_str(), specifierStr.c_str(), 0, nullptr));
        if (!code) {
            return nullptr;
        }
        if (!JS::ExecuteScript(cx, module, code, nullptr)) {
            return nullptr;
        }

        // Renvoyer l'objet JSModule comme résultat
        result.set(JS::ModuleResolveResult::ModuleObject(module));
        return nullptr;
    }
};
