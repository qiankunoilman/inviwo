/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2012-2020 Inviwo Foundation
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

#include <inviwo/core/properties/compositeproperty.h>
#include <inviwo/core/properties/templateproperty.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/network/networklock.h>
#include <inviwo/core/util/stringconversion.h>
#include <inviwo/core/network/networkvisitor.h>

namespace inviwo {

const std::string CompositeProperty::classIdentifier = "org.inviwo.CompositeProperty";
std::string CompositeProperty::getClassIdentifier() const { return classIdentifier; }

CompositeProperty::CompositeProperty(const std::string& identifier, const std::string& displayName,
                                     InvalidationLevel invalidationLevel,
                                     PropertySemantics semantics)
    : Property(identifier, displayName, invalidationLevel, semantics)
    , PropertyOwner()
    , collapsed_("collapsed", false)
    , subPropertyInvalidationLevel_(InvalidationLevel::Valid) {}

CompositeProperty* CompositeProperty::clone() const { return new CompositeProperty(*this); }

const std::string& CompositeProperty::getIdentifier() const { return Property::getIdentifier(); }

std::string CompositeProperty::getClassIdentifierForWidget() const {
    return CompositeProperty::classIdentifier;
}

void CompositeProperty::setOwner(PropertyOwner* owner) {
    Property::setOwner(owner);
    for (Property* property : properties_) property->setOwner(this);
}

void CompositeProperty::set(const Property* srcProperty) {
    if (const auto compositeSrcProp = dynamic_cast<const CompositeProperty*>(srcProperty)) {
        set(compositeSrcProp);
    }
}

void CompositeProperty::set(const CompositeProperty* src) {
    NetworkLock lock(this);
    const auto& subProperties = src->getProperties();
    if (subProperties.size() == properties_.size()) {
        for (size_t i = 0; i < subProperties.size(); i++) {
            this->properties_[i]->set(subProperties[i]);
        }
        propertyModified();
    } else {
        LogWarn("Unable to link CompositeProperties: \n"
                << src->getPath() << "\n to \n"
                << getPath() << ".\nNumber of sub properties differ (" << subProperties.size()
                << " vs " << properties_.size() << ")");
    }
}

InvalidationLevel CompositeProperty::getInvalidationLevel() const {
    return std::min(subPropertyInvalidationLevel_, Property::getInvalidationLevel());
}

void CompositeProperty::invalidate(InvalidationLevel invalidationLevel,
                                   Property* modifiedProperty) {
    PropertyOwner::invalidate(invalidationLevel, modifiedProperty);
    subPropertyInvalidationLevel_ = invalidationLevel;
    Property::propertyModified();
}

void CompositeProperty::setValid() {
    Property::setValid();
    PropertyOwner::setValid();
    subPropertyInvalidationLevel_ = InvalidationLevel::Valid;
}

CompositeProperty& CompositeProperty::setCurrentStateAsDefault() {
    Property::setCurrentStateAsDefault();
    collapsed_.setAsDefault();
    for (auto elem : properties_) {
        elem->setCurrentStateAsDefault();
    }
    return *this;
}

CompositeProperty& CompositeProperty::resetToDefaultState() {
    NetworkLock lock(this);
    for (auto elem : properties_) {
        elem->resetToDefaultState();
    }
    return *this;
}

bool CompositeProperty::isDefaultState() const {
    return collapsed_.isDefault() &&
           std::all_of(properties_.begin(), properties_.end(),
                       [](const Property* p) { return p->isDefaultState(); });
}

bool CompositeProperty::needsSerialization() const {
    switch (serializationMode_) {
        case PropertySerializationMode::All:
            return true;
        case PropertySerializationMode::None:
            return false;
        case PropertySerializationMode::Default:
            [[fallthrough]];
        default:
            return std::any_of(properties_.begin(), properties_.end(),
                               [](const Property* p) { return p->needsSerialization(); });
    }
}

CompositeProperty& CompositeProperty::setReadOnly(bool value) {
    Property::setReadOnly(value);
    for (auto& elem : properties_) {
        elem->setReadOnly(value);
    }
    return *this;
}

void CompositeProperty::serialize(Serializer& s) const {
    Property::serialize(s);
    PropertyOwner::serialize(s);
    collapsed_.serialize(s, serializationMode_);
}

void CompositeProperty::deserialize(Deserializer& d) {
    Property::deserialize(d);
    PropertyOwner::deserialize(d);
    collapsed_.deserialize(d, serializationMode_);
}

const PropertyOwner* CompositeProperty::getOwner() const { return Property::getOwner(); }

PropertyOwner* CompositeProperty::getOwner() { return Property::getOwner(); }

void CompositeProperty::accept(NetworkVisitor& visitor) {
    if (visitor.visit(*this)) {
        for (auto* elem : properties_) {
            elem->accept(visitor);
        }
    }
}

Processor* CompositeProperty::getProcessor() {
    if (auto owner = getOwner()) {
        return owner->getProcessor();
    } else {
        return nullptr;
    }
}

const Processor* CompositeProperty::getProcessor() const {
    if (const auto owner = getOwner()) {
        return owner->getProcessor();
    } else {
        return nullptr;
    }
}

bool CompositeProperty::isCollapsed() const { return collapsed_; }
CompositeProperty& CompositeProperty::setCollapsed(bool value) {
    if (collapsed_ != value) {
        collapsed_ = value;
        notifyObserversOnSetCollapsed(collapsed_);
    }
    return *this;
}

}  // namespace inviwo
