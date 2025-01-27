/* Copyright 2013-2019 Bas van den Berg
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef BUILDER_LIBRARY_LOADER_H
#define BUILDER_LIBRARY_LOADER_H

#include <string>
#include <deque>
#include <map>

#include "AST/Component.h"
#include "CGenerator/HeaderNamer.h"
#include "Utils/StringList.h"

namespace C2 {

class Recipe;
class Module;

struct LibInfo {
    LibInfo(const std::string& h, const std::string& f, Component* c, Module* m)
        : headerLibInfo(h), c2file(f), component(c), module(m)
    {}
    std::string headerLibInfo;
    std::string c2file;
    Component* component;
    Module* module;
};

class LibraryLoader : public HeaderNamer {
public:
    LibraryLoader(Components& components_,
                  Modules& modules_,
                  const Recipe& recipe_)
        : mainComponent(0)
        , components(components_)
        , modules(modules_)
        , recipe(recipe_)
    {}
    virtual ~LibraryLoader();

    void addDir(const std::string& libdir);

    bool createComponents();
    Component* getMainComponent() const { return mainComponent; }

    // for mainComponent
    const LibInfo* addModule(Component* C, Module* M, const std::string& header, const std::string& file);

    void showLibs(bool useColors, bool showModules) const;

    virtual const std::string& getIncludeName(const std::string& modName) const;

    const LibInfo* findModuleLib(const std::string& moduleName) const;
private:
    void createMainComponent();
    bool loadExternals();
    void addDep(Component* src, const std::string& dest, Component::Type type);

    Component* findComponent(const std::string& name) const;
    Component* createComponent(const std::string& name,
                               const std::string& path,
                               bool isCLib,
                               Component::Type type);
    Component* findModuleComponent(const std::string& moduleName) const;
    bool findComponentDir(const std::string& name, char* dirname) const;

    void showLib(StringBuilder& out, const std::string& libdir, bool useColors, bool showModules) const;

    bool checkComponent(Component* C);
    void checkLibDirs();

    StringList libraryDirs;

    Component* mainComponent;
    Components& components;
    Modules& modules;

    // TODO BB: merge this into Modules?
    typedef std::map<std::string, LibInfo*> Libraries; // moduleName -> LibInfo
    typedef Libraries::const_iterator LibrariesConstIter;
    typedef Libraries::iterator LibrariesIter;
    Libraries libs;

    const Recipe& recipe;
};

}

#endif

