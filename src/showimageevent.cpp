/*
 * A Photo Tool (Libre)
 *
 * Copyright © 2021-2022 Jari Ahola
 * GNU General Public License (GPLv3)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "showimageevent.h"
#include <iostream>

static const QEvent::Type showImageEventType = (QEvent::Type)QEvent::registerEventType();
ShowImageEvent *ShowImageEvent::self = nullptr;
QEvent::Type ShowImageEvent::eventType = QEvent::None;

ShowImageEvent::ShowImageEvent() : QEvent(showImageEventType)
{
    eventType = type();
}

ShowImageEvent::~ShowImageEvent()
{
    self = nullptr;
}

ShowImageEvent* ShowImageEvent::getEvent() {
    /* We asume event is not posted or post is handled
       before calling getEvent() after first call. */
    delete self;
    self = new ShowImageEvent();
    return self;
}
