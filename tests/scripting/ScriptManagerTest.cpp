/*
    This file is part of Element
    Copyright (C) 2020  Kushview, LLC.  All rights reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Tests.h"
#include "scripting/ScriptManager.h"

using namespace Element;

static File scriptsDir()
{
    return File::getSpecialLocation (File::invokedExecutableFile)
                        .getParentDirectory().getParentDirectory().getParentDirectory()
                        .getChildFile ("scripts")
                        .getFullPathName();
}

//=============================================================================
class ScriptManagerTest : public UnitTestBase
{
public:
    ScriptManagerTest ()
        : UnitTestBase ("Script Manager", "Scripting", "manager") { }

    void runTest() override
    {
        beginTest ("scanning");
        ScriptManager scripts;
        scripts.scanDirectory (scriptsDir());
    }
};

static ScriptManagerTest sScriptManagerTest;