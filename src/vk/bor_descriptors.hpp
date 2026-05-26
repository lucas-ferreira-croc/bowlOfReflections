#ifndef BOR_DESCRIPTORS_HPP
#define BOR_DESCRIPTORS_HPP

#include "bor_device.hpp"

#include <memory>
#include <unordered_map>
#include <vector>


namespace bor
{
    class BoRDescriptorSetLayout
    {
    public:
        class Builder 
        {
        public:
            Builder(BoRDevice &BoRDevice) : borDevice{BoRDevice} {}
        
            Builder &addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);
            std::unique_ptr<BoRDescriptorSetLayout> build() const;
        
        private:
            BoRDevice &borDevice;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };
        
        BoRDescriptorSetLayout(
            BoRDevice &BoRDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~BoRDescriptorSetLayout();
        BoRDescriptorSetLayout(const BoRDescriptorSetLayout &) = delete;
        BoRDescriptorSetLayout &operator=(const BoRDescriptorSetLayout &) = delete;
        
        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }
        
    private:
        BoRDevice &borDevice;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;
        
        friend class BoRDescriptorWriter;
    };

    class BoRDescriptorPool 
    {
    public:
        class Builder 
        {
        public:
            Builder(BoRDevice &BoRDevice) : borDevice{BoRDevice} {}
        
            Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder &setMaxSets(uint32_t count);
            std::unique_ptr<BoRDescriptorPool> build() const;
        
        private:
            BoRDevice &borDevice;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };
    
        BoRDescriptorPool(
            BoRDevice &BoRDevice,
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize> &poolSizes);
        ~BoRDescriptorPool();
        BoRDescriptorPool(const BoRDescriptorPool &) = delete;
        BoRDescriptorPool &operator=(const BoRDescriptorPool &) = delete;
        
        bool allocateDescriptor(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;
        
        void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;
        
        void resetPool();
            
        VkDescriptorPool getDescriptorPool() { return descriptorPool; }
    private:

        BoRDevice &borDevice;
        VkDescriptorPool descriptorPool;
        
        friend class BoRDescriptorWriter;
    };
 

    class BoRDescriptorWriter {
    public:
        BoRDescriptorWriter(BoRDescriptorSetLayout &setLayout, BoRDescriptorPool &pool);
        
        BoRDescriptorWriter &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
        BoRDescriptorWriter &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);
        
        bool build(VkDescriptorSet &set);
        void overwrite(VkDescriptorSet &set);
    
    private:
        BoRDescriptorSetLayout &setLayout;
        BoRDescriptorPool &pool;
        std::vector<VkWriteDescriptorSet> writes;
    };
}

#endif