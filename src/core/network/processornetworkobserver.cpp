/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2014-2015 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/

#include <inviwo/core/network/processornetworkobserver.h>

namespace inviwo {

void ProcessorNetworkObservable::notifyObserversProcessorNetworkChanged() const {
    for (auto it = observers_.rbegin(); it != observers_.rend(); ++it) {
        (*it)->onProcessorNetworkChange();
    }
}

void ProcessorNetworkObservable::notifyObserversProcessorNetworkEvaluateRequest() const {
    for (auto it = observers_.rbegin(); it != observers_.rend(); ++it) {
        (*it)->onProcessorNetworkEvaluateRequest();
    }
}

void ProcessorNetworkObservable::notifyObserversProcessorNetworkUnlocked() const {
    for (auto it = observers_.rbegin(); it != observers_.rend(); ++it) {
        (*it)->onProcessorNetworkUnlocked();
    }
}

void ProcessorNetworkObservable::notifyObserversProcessorNetworkWillAddProcessor(
    Processor* processor) const {
    for (auto it = observers_.rbegin(); it != observers_.rend(); ++it) {
        (*it)->onProcessorNetworkWillAddProcessor(processor);
    }
}

void ProcessorNetworkObservable::notifyObserversProcessorNetworkDidAddProcessor(
    Processor* processor) const {
    for (auto it = observers_.rbegin(); it != observers_.rend(); ++it) {
        (*it)->onProcessorNetworkDidAddProcessor(processor);
    }
}

void ProcessorNetworkObservable::notifyObserversProcessorNetworkWillRemoveProcessor(
    Processor* processor) const {
    for (auto it = observers_.rbegin(); it != observers_.rend(); ++it) {
        (*it)->onProcessorNetworkWillRemoveProcessor(processor);
    }
}

void ProcessorNetworkObservable::notifyObserversProcessorNetworkDidRemoveProcessor(
    Processor* processor) const {
    for (auto it = observers_.rbegin(); it != observers_.rend(); ++it) {
        (*it)->onProcessorNetworkDidRemoveProcessor(processor);
    }
}

void ProcessorNetworkObservable::notifyObserversProcessorNetworkWillAddConnection(
    PortConnection* portConnection) const {
    for (auto it = observers_.rbegin(); it != observers_.rend(); ++it) {
        (*it)->onProcessorNetworkWillAddConnection(portConnection);
    }
}

void ProcessorNetworkObservable::notifyObserversProcessorNetworkDidAddConnection(
    PortConnection* portConnection) const {
    for (auto it = observers_.rbegin(); it != observers_.rend(); ++it) {
        (*it)->onProcessorNetworkDidAddConnection(portConnection);
    }
}

void ProcessorNetworkObservable::notifyObserversProcessorNetworkWillRemoveConnection(
    PortConnection* portConnection) const {
    for (auto it = observers_.rbegin(); it != observers_.rend(); ++it) {
        (*it)->onProcessorNetworkWillRemoveConnection(portConnection);
    }
}

void ProcessorNetworkObservable::notifyObserversProcessorNetworkDidRemoveConnection(
    PortConnection* portConnection) const {
    for (auto it = observers_.rbegin(); it != observers_.rend(); ++it) {
        (*it)->onProcessorNetworkDidRemoveConnection(portConnection);
    }
}

void ProcessorNetworkObservable::notifyObserversProcessorNetworkWillAddLink(
    PropertyLink* propertyLink) const {
    for (auto it = observers_.rbegin(); it != observers_.rend(); ++it) {
        (*it)->onProcessorNetworkWillAddLink(propertyLink);
    }
}

void ProcessorNetworkObservable::notifyObserversProcessorNetworkDidAddLink(
    PropertyLink* propertyLink) const {
    for (auto it = observers_.rbegin(); it != observers_.rend(); ++it) {
        (*it)->onProcessorNetworkDidAddLink(propertyLink);
    }
}

void ProcessorNetworkObservable::notifyObserversProcessorNetworkWillRemoveLink(
    PropertyLink* propertyLink) const {
    for (auto it = observers_.rbegin(); it != observers_.rend(); ++it) {
        (*it)->onProcessorNetworkWillRemoveLink(propertyLink);
    }
}

void ProcessorNetworkObservable::notifyObserversProcessorNetworkDidRemoveLink(
    PropertyLink* propertyLink) const {
    for (auto it = observers_.rbegin(); it != observers_.rend(); ++it) {
        (*it)->onProcessorNetworkDidRemoveLink(propertyLink);
    }
}

}  // namespace
