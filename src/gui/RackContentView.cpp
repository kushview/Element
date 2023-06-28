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

#include "gui/RackContentView.h"
#include <element/ui/style.hpp>

namespace element {

class RackView::Impl
{
public:
    Impl() {}
    ~Impl() {}
};

RackView::RackView()
{
    impl = std::make_unique<Impl>();
}

RackView::~RackView()
{
    impl = nullptr;
}

void RackView::paint (Graphics& g)
{
    g.fillAll (Colors::backgroundColor);

    g.setColour (Colors::elemental);
    g.setFont (14.0f);
    g.drawText ("No Selection...", getLocalBounds(), Justification::centred, true);
}

void RackView::resized()
{
    if (! main)
        return;

    main->setBounds (getLocalBounds().reduced (2));
}

void RackView::setMainComponent (Component* comp)
{
    if (comp != nullptr && comp == main.get())
    {
        main = nullptr;
    }
    else
    {
        main.reset (comp);
        if (main)
            addAndMakeVisible (comp);
    }

    resized();
}

} // namespace element
