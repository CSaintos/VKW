// vkw.hpp (convenience header)
#pragma once

// TODO disable certain functions from being available to user
// TODO block certain classes from being available to user
#include <vkw\Context.hpp>
#include <vkw\Validation.hpp>
#include <vkw\Instance.hpp>
//#include <vkw\QueueFamilyIndices.hpp> // class should be blocked
#include <vkw\PhysicalDevice.hpp>
#include <vkw\LogicalDevice.hpp>
#include <vkw\Swapchain.hpp>
#include <vkw\RenderPass.hpp>
#include <vkw\GraphicsPipeline.hpp>
#include <vkw\Framebuffer.hpp>
#include <vkw\Command.hpp>
#include <vkw\Synchronization.hpp>