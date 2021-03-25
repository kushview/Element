/*
    This file is part of Element
    Copyright (C) 2019  Kushview, LLC.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "engine/nodes/BaseProcessor.h"
#include "engine/nodes/AudioRouterNode.h"
#include "engine/nodes/LuaNode.h"
#include "engine/nodes/MidiChannelSplitterNode.h"
#include "engine/nodes/MidiMonitorNode.h"
#include "engine/nodes/MidiProgramMapNode.h"
#include "engine/nodes/MidiRouterNode.h"
// #include "engine/nodes/MidiSequencerNode.h"
#include "engine/nodes/OSCReceiverNode.h"
#include "engine/nodes/OSCSenderNode.h"
#include "engine/nodes/ScriptNode.h"
#include "NodeManager.h"

namespace Element {

NodeManager::NodeManager()
{
    add<AudioRouterNode> (EL_INTERNAL_ID_AUDIO_ROUTER);
    add<LuaNode> (EL_INTERNAL_ID_LUA);
    add<MidiChannelSplitterNode> (EL_INTERNAL_ID_MIDI_CHANNEL_SPLITTER);
    add<MidiMonitorNode> (EL_INTERNAL_ID_MIDI_MONITOR);
    add<MidiProgramMapNode> (EL_INTERNAL_ID_MIDI_PROGRAM_MAP);
    add<MidiRouterNode> (EL_INTERNAL_ID_MIDI_ROUTER);
    // add<AudioRouterNode> (EL_INTERNAL_ID_MIDI_SEQUENCER);
    add<OSCSenderNode> (EL_INTERNAL_ID_OSC_RECEIVER);
    add<OSCReceiverNode> (EL_INTERNAL_ID_OSC_SENDER);
    add<ScriptNode> (EL_INTERNAL_ID_SCRIPT);
}

NodeManager::~NodeManager() 
{
    knownIDs.clearQuick();
    providers.clearQuick (true);
}

//==============================================================================
void NodeManager::getPluginDescriptions (OwnedArray<PluginDescription>& out, const String& identifier)
{
    for (auto* f : providers)
    {
        if (NodeObjectPtr ptr = f->create (identifier))
        {
            auto* desc = out.add (new PluginDescription());
            ptr->getPluginDescription (*desc);
            break;
        }
    }
}

//==============================================================================
NodeManager& NodeManager::add (NodeProvider* f)
{
    providers.add (f);
    knownIDs.addArray (f->findTypes());
    knownIDs.removeDuplicates (true);
    knownIDs.removeEmptyStrings();
    return *this;
}

//==============================================================================
NodeObjectPtr NodeManager::instantiate (const PluginDescription& desc)
{
    return instantiate (desc.fileOrIdentifier);
}

NodeObjectPtr NodeManager::instantiate (const String& identifier)
{
    NodeObjectPtr node = nullptr;
    for (const auto& f : providers)
        if (auto* const n = f->create (identifier))
            { node = n; break; }

    if (node)
    {
        // node->init();
    }

    return node;
}

}