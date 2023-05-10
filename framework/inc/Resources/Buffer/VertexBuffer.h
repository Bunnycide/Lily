//
// Created by jae on 20/04/23.
//

#ifndef VK_FRAMEWORK_IMPL_VERTEXBUFFER_H
#define VK_FRAMEWORK_IMPL_VERTEXBUFFER_H

#include <Resources/Buffer.h>

class VertexBuffer : public Buffer{
public:
    void init(Device, VkDeviceSize, void* );
};


#endif //VK_FRAMEWORK_IMPL_VERTEXBUFFER_H
