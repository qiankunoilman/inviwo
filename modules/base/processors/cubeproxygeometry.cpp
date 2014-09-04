/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Main file authors: Peter Steneteg, Erik Sund�n
 *
 *********************************************************************************/

#include "cubeproxygeometry.h"
#include <inviwo/core/datastructures/geometry/simplemeshcreator.h>

namespace inviwo {

ProcessorClassIdentifier(CubeProxyGeometry, "org.inviwo.CubeProxyGeometry");
ProcessorDisplayName(CubeProxyGeometry,  "Cube Proxy Geometry");
ProcessorTags(CubeProxyGeometry, Tags::None);
ProcessorCategory(CubeProxyGeometry, "Geometry Creation");
ProcessorCodeState(CubeProxyGeometry, CODE_STATE_STABLE);

CubeProxyGeometry::CubeProxyGeometry()
    : Processor(),
      inport_("volume.inport"),
      outport_("geometry.outport"),
      clippingEnabled_("clippingEnabled", "Enable Clipping", true),
      clipX_("clipX", "Clip X Slices", 0, 256, 0, 256, 1, 1),
      clipY_("clipY", "Clip Y Slices", 0, 256, 0, 256, 1, 1),
      clipZ_("clipZ", "Clip Z Slices", 0, 256, 0, 256, 1, 1)
{
    addPort(inport_);
    addPort(outport_);
    addProperty(clippingEnabled_);
    addProperty(clipX_);
    addProperty(clipY_);
    addProperty(clipZ_);
    dims_ = uvec3(1,1,1);

    inport_.onChange(this, &CubeProxyGeometry::onVolumeChange);
}

CubeProxyGeometry::~CubeProxyGeometry() {}

void CubeProxyGeometry::initialize() {
    Processor::initialize();
}

void CubeProxyGeometry::deinitialize() {
    Processor::deinitialize();
}

void CubeProxyGeometry::process() {
    vec3 startDataTexCoord = vec3(0.0);
    vec3 endDataTexCoord = vec3(1.f);
    glm::vec3 pos(0.0f);
    glm::vec3 p1(1.0f,0.0f,0.0f);
    glm::vec3 p2(0.0f,1.0f,0.0f);
    glm::vec3 p3(0.0f,0.0f,1.0f);
    glm::vec3 tex(startDataTexCoord);
    glm::vec3 t1(endDataTexCoord.x,startDataTexCoord.y,startDataTexCoord.z);
    glm::vec3 t2(startDataTexCoord.x,endDataTexCoord.y,startDataTexCoord.z);
    glm::vec3 t3(startDataTexCoord.x,startDataTexCoord.y,endDataTexCoord.z);
    glm::vec4 col(startDataTexCoord,1.0f);
    glm::vec4 c1(t1,0.0f);
    glm::vec4 c2(t2,0.0f);
    glm::vec4 c3(t3,0.0f);

    if (clippingEnabled_.get()) {
        pos = pos + p1*static_cast<float>(clipX_.get().x)/static_cast<float>(dims_.x)
                  + p2*static_cast<float>(clipY_.get().x)/static_cast<float>(dims_.y)
                  + p3*static_cast<float>(clipZ_.get().x)/static_cast<float>(dims_.z);
        p1 = p1*(static_cast<float>(clipX_.get().y)-static_cast<float>(clipX_.get().x))/static_cast<float>(dims_.x);
        p2 = p2*(static_cast<float>(clipY_.get().y)-static_cast<float>(clipY_.get().x))/static_cast<float>(dims_.y);
        p3 = p3*(static_cast<float>(clipZ_.get().y)-static_cast<float>(clipZ_.get().x))/static_cast<float>(dims_.z);
        tex = tex + t1*static_cast<float>(clipX_.get().x)/static_cast<float>(dims_.x)
                  + t2*static_cast<float>(clipY_.get().x)/static_cast<float>(dims_.y)
                  + t3*static_cast<float>(clipZ_.get().x)/static_cast<float>(dims_.z);
        t1 = t1*(static_cast<float>(clipX_.get().y)-static_cast<float>(clipX_.get().x))/static_cast<float>(dims_.x);
        t2 = t2*(static_cast<float>(clipY_.get().y)-static_cast<float>(clipY_.get().x))/static_cast<float>(dims_.y);
        t3 = t3*(static_cast<float>(clipZ_.get().y)-static_cast<float>(clipZ_.get().x))/static_cast<float>(dims_.z);
        col = col + c1*static_cast<float>(clipX_.get().x)/static_cast<float>(dims_.x)
                  + c2*static_cast<float>(clipY_.get().x)/static_cast<float>(dims_.y)
                  + c3*static_cast<float>(clipZ_.get().x)/static_cast<float>(dims_.z);
        c1 = c1*(static_cast<float>(clipX_.get().y)-static_cast<float>(clipX_.get().x))/static_cast<float>(dims_.x);
        c2 = c2*(static_cast<float>(clipY_.get().y)-static_cast<float>(clipY_.get().x))/static_cast<float>(dims_.y);
        c3 = c3*(static_cast<float>(clipZ_.get().y)-static_cast<float>(clipZ_.get().x))/static_cast<float>(dims_.z);
    }

    // Create parallelepiped and set it to the outport
    Geometry* geom = SimpleMeshCreator::parallelepiped(pos, p1, p2, p3,
                                                       tex, t1, t2, t3,
                                                       col, c1, c2, c3);
    geom->setBasisAndOffset(inport_.getData()->getBasisAndOffset());
    geom->setWorldTransform(inport_.getData()->getWorldTransform());
    outport_.setData(geom);
}

void CubeProxyGeometry::onVolumeChange() {
    vec2 x,y,z;

    x = clipX_.get();
    y = clipY_.get();
    z = clipZ_.get();

    x /= static_cast<float>(clipX_.getRangeMax());
    y /= static_cast<float>(clipY_.getRangeMax());
    z /= static_cast<float>(clipZ_.getRangeMax());

    dims_ = inport_.getData()->getDimension();
    
    invalidationDisabled();

    clipX_.setRangeMax(dims_.x);
    clipY_.setRangeMax(dims_.y);
    clipZ_.setRangeMax(dims_.z);

    clipX_.set(x * static_cast<float>(dims_.x));
    clipY_.set(y * static_cast<float>(dims_.y));
    clipZ_.set(z * static_cast<float>(dims_.z));

    invalidationEnabled();
}

} // namespace
