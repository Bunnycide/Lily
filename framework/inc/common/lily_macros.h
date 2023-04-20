//
// Created by jae on 20/04/23.
//

#ifndef VK_FRAMEWORK_IMPL_LILY_MACROS_H
#define VK_FRAMEWORK_IMPL_LILY_MACROS_H

#include <vk_loader/vulkan/vk_enum_string_helper.h>

#define VK_CHECK_RESULT(f)																				\
{																										\
	VkResult res = (f);																					\
	if (res != VK_SUCCESS)																				\
	{																									\
		Log::error("Fatal : VkResult is \" %s \" in %s at line %d", string_VkResult(res) , __FILE__, __LINE__); \
		std::abort();																		\
	}																									\
}

#endif //VK_FRAMEWORK_IMPL_LILY_MACROS_H
