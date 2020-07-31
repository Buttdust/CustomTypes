#include "types.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

type_info::type_info(Il2CppTypeEnum typeE, std::string_view ns, std::string_view n, Il2CppClass* b) {
    typeEnum = typeE;
    base = b;
    namespaze = ns;
    name = n;
}

field_info::field_info(std::string_view name, const Il2CppType* type, int32_t offset, uint16_t fieldAttrs) {
    // Create FieldInfo*
    info = FieldInfo{};
    info.name = name.data();
    // We want to make sure we set the correct attributes on this type
    // For that purpose, we actually want to COPY the type
    auto* tmp = new Il2CppType();
    tmp->attrs = type->attrs | fieldAttrs;
    tmp->byref = type->byref;
    tmp->data.dummy = type->data.dummy;
    tmp->num_mods = type->num_mods;
    tmp->pinned = type->pinned;
    tmp->type = type->type;
    info.type = tmp;
    info.offset = offset;
    info.token = -1;
}

field_info::~field_info() {
    delete info.type;
}

constexpr const FieldInfo field_info::get() const {
    return info;
}

method_info::method_info(std::string_view name, void* func, InvokerMethod invoker, const Il2CppType* returnType, std::vector<ParameterInfo>& parameters, uint16_t flags) {
    // Create MethodInfo*
    info = new MethodInfo();
    params = parameters;
    info->name = name.data();
    info->methodPointer = (Il2CppMethodPointer)func;
    // TODO: Need to figure out how to reliably set invoker functions
    // This is probably going to be really challenging...
    // For now, we will try to make our own and see if we crash
    info->invoker_method = invoker;
    info->return_type = returnType;
    info->parameters = params.data();
    info->parameters_count = params.size();
    info->flags = flags;
    info->slot = kInvalidIl2CppMethodSlot;
    // TODO: set more data on method, perhaps pass in less?
}

method_info::~method_info() {
    delete info;
}

constexpr const MethodInfo* method_info::get() const {
    return info;
}

constexpr void method_info::setClass(Il2CppClass* klass) const {
    info->klass = klass;
}
