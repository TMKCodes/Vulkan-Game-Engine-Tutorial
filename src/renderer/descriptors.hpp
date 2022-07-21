#pragma once

#include "device.hpp"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace tml {

class TmlDescriptorSetLayout {
 public:
  class Builder {
   public:
    Builder(TmlDevice &tmlDevice) : tmlDevice{tmlDevice} {}

    Builder &addBinding(
        uint32_t binding,
        VkDescriptorType descriptorType,
        VkShaderStageFlags stageFlags,
        uint32_t count = 1);
    std::unique_ptr<TmlDescriptorSetLayout> build() const;

   private:
    TmlDevice &tmlDevice;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
  };

  TmlDescriptorSetLayout(
      TmlDevice &tmlDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
  ~TmlDescriptorSetLayout();
  TmlDescriptorSetLayout(const TmlDescriptorSetLayout &) = delete;
  TmlDescriptorSetLayout &operator=(const TmlDescriptorSetLayout &) = delete;

  VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

 private:
  TmlDevice &tmlDevice;
  VkDescriptorSetLayout descriptorSetLayout;
  std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

  friend class TmlDescriptorWriter;
};

class TmlDescriptorPool {
 public:
  class Builder {
   public:
    Builder(TmlDevice &tmlDevice) : tmlDevice{tmlDevice} {}

    Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
    Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
    Builder &setMaxSets(uint32_t count);
    std::unique_ptr<TmlDescriptorPool> build() const;

   private:
    TmlDevice &tmlDevice;
    std::vector<VkDescriptorPoolSize> poolSizes{};
    uint32_t maxSets = 1000;
    VkDescriptorPoolCreateFlags poolFlags = 0;
  };

  TmlDescriptorPool(
      TmlDevice &tmlDevice,
      uint32_t maxSets,
      VkDescriptorPoolCreateFlags poolFlags,
      const std::vector<VkDescriptorPoolSize> &poolSizes);
  ~TmlDescriptorPool();
  TmlDescriptorPool(const TmlDescriptorPool &) = delete;
  TmlDescriptorPool &operator=(const TmlDescriptorPool &) = delete;

  bool allocateDescriptor(
      const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;

  void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

  void resetPool();

 private:
  TmlDevice &tmlDevice;
  VkDescriptorPool descriptorPool;

  friend class TmlDescriptorWriter;
};

class TmlDescriptorWriter {
 public:
  TmlDescriptorWriter(TmlDescriptorSetLayout &setLayout, TmlDescriptorPool &pool);

  TmlDescriptorWriter &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
  TmlDescriptorWriter &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);

  bool build(VkDescriptorSet &set);
  void overwrite(VkDescriptorSet &set);

 private:
  TmlDescriptorSetLayout &setLayout;
  TmlDescriptorPool &pool;
  std::vector<VkWriteDescriptorSet> writes;
};

}  // namespace tml
