// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: rpc_meta.proto

#include "rpc_meta.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG
namespace ecron {
constexpr RpcMeta::RpcMeta(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : request_(nullptr)
  , response_(nullptr)
  , correlation_id_(int64_t{0}){}
struct RpcMetaDefaultTypeInternal {
  constexpr RpcMetaDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~RpcMetaDefaultTypeInternal() {}
  union {
    RpcMeta _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT RpcMetaDefaultTypeInternal _RpcMeta_default_instance_;
constexpr RpcRequestMeta::RpcRequestMeta(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : service_name_(&::PROTOBUF_NAMESPACE_ID::internal::fixed_address_empty_string)
  , method_name_(&::PROTOBUF_NAMESPACE_ID::internal::fixed_address_empty_string){}
struct RpcRequestMetaDefaultTypeInternal {
  constexpr RpcRequestMetaDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~RpcRequestMetaDefaultTypeInternal() {}
  union {
    RpcRequestMeta _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT RpcRequestMetaDefaultTypeInternal _RpcRequestMeta_default_instance_;
constexpr RpcResponseMeta::RpcResponseMeta(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : error_text_(&::PROTOBUF_NAMESPACE_ID::internal::fixed_address_empty_string)
  , error_code_(0){}
struct RpcResponseMetaDefaultTypeInternal {
  constexpr RpcResponseMetaDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~RpcResponseMetaDefaultTypeInternal() {}
  union {
    RpcResponseMeta _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT RpcResponseMetaDefaultTypeInternal _RpcResponseMeta_default_instance_;
}  // namespace ecron
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_rpc_5fmeta_2eproto[3];
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_rpc_5fmeta_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_rpc_5fmeta_2eproto = nullptr;

const uint32_t TableStruct_rpc_5fmeta_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::ecron::RpcMeta, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::ecron::RpcMeta, request_),
  PROTOBUF_FIELD_OFFSET(::ecron::RpcMeta, response_),
  PROTOBUF_FIELD_OFFSET(::ecron::RpcMeta, correlation_id_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::ecron::RpcRequestMeta, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::ecron::RpcRequestMeta, service_name_),
  PROTOBUF_FIELD_OFFSET(::ecron::RpcRequestMeta, method_name_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::ecron::RpcResponseMeta, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::ecron::RpcResponseMeta, error_code_),
  PROTOBUF_FIELD_OFFSET(::ecron::RpcResponseMeta, error_text_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::ecron::RpcMeta)},
  { 9, -1, -1, sizeof(::ecron::RpcRequestMeta)},
  { 17, -1, -1, sizeof(::ecron::RpcResponseMeta)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::ecron::_RpcMeta_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::ecron::_RpcRequestMeta_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::ecron::_RpcResponseMeta_default_instance_),
};

const char descriptor_table_protodef_rpc_5fmeta_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\016rpc_meta.proto\022\005ecron\"s\n\007RpcMeta\022&\n\007re"
  "quest\030\001 \001(\0132\025.ecron.RpcRequestMeta\022(\n\010re"
  "sponse\030\002 \001(\0132\026.ecron.RpcResponseMeta\022\026\n\016"
  "correlation_id\030\004 \001(\003\";\n\016RpcRequestMeta\022\024"
  "\n\014service_name\030\001 \001(\t\022\023\n\013method_name\030\002 \001("
  "\t\"9\n\017RpcResponseMeta\022\022\n\nerror_code\030\001 \001(\005"
  "\022\022\n\nerror_text\030\002 \001(\t*i\n\tErrorCode\022\014\n\010ENO"
  "ERROR\020\000\022\017\n\nENOSERVICE\020\351\007\022\016\n\tENOMETHOD\020\352\007"
  "\022\r\n\010EREQUEST\020\353\007\022\016\n\tERESPONSE\020\354\007\022\016\n\tEINTE"
  "RNAL\020\355\007b\006proto3"
  ;
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_rpc_5fmeta_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_rpc_5fmeta_2eproto = {
  false, false, 375, descriptor_table_protodef_rpc_5fmeta_2eproto, "rpc_meta.proto", 
  &descriptor_table_rpc_5fmeta_2eproto_once, nullptr, 0, 3,
  schemas, file_default_instances, TableStruct_rpc_5fmeta_2eproto::offsets,
  file_level_metadata_rpc_5fmeta_2eproto, file_level_enum_descriptors_rpc_5fmeta_2eproto, file_level_service_descriptors_rpc_5fmeta_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_rpc_5fmeta_2eproto_getter() {
  return &descriptor_table_rpc_5fmeta_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_rpc_5fmeta_2eproto(&descriptor_table_rpc_5fmeta_2eproto);
namespace ecron {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* ErrorCode_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_rpc_5fmeta_2eproto);
  return file_level_enum_descriptors_rpc_5fmeta_2eproto[0];
}
bool ErrorCode_IsValid(int value) {
  switch (value) {
    case 0:
    case 1001:
    case 1002:
    case 1003:
    case 1004:
    case 1005:
      return true;
    default:
      return false;
  }
}


// ===================================================================

class RpcMeta::_Internal {
 public:
  static const ::ecron::RpcRequestMeta& request(const RpcMeta* msg);
  static const ::ecron::RpcResponseMeta& response(const RpcMeta* msg);
};

const ::ecron::RpcRequestMeta&
RpcMeta::_Internal::request(const RpcMeta* msg) {
  return *msg->request_;
}
const ::ecron::RpcResponseMeta&
RpcMeta::_Internal::response(const RpcMeta* msg) {
  return *msg->response_;
}
RpcMeta::RpcMeta(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:ecron.RpcMeta)
}
RpcMeta::RpcMeta(const RpcMeta& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  if (from._internal_has_request()) {
    request_ = new ::ecron::RpcRequestMeta(*from.request_);
  } else {
    request_ = nullptr;
  }
  if (from._internal_has_response()) {
    response_ = new ::ecron::RpcResponseMeta(*from.response_);
  } else {
    response_ = nullptr;
  }
  correlation_id_ = from.correlation_id_;
  // @@protoc_insertion_point(copy_constructor:ecron.RpcMeta)
}

inline void RpcMeta::SharedCtor() {
::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
    reinterpret_cast<char*>(&request_) - reinterpret_cast<char*>(this)),
    0, static_cast<size_t>(reinterpret_cast<char*>(&correlation_id_) -
    reinterpret_cast<char*>(&request_)) + sizeof(correlation_id_));
}

RpcMeta::~RpcMeta() {
  // @@protoc_insertion_point(destructor:ecron.RpcMeta)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

inline void RpcMeta::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  if (this != internal_default_instance()) delete request_;
  if (this != internal_default_instance()) delete response_;
}

void RpcMeta::ArenaDtor(void* object) {
  RpcMeta* _this = reinterpret_cast< RpcMeta* >(object);
  (void)_this;
}
void RpcMeta::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void RpcMeta::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void RpcMeta::Clear() {
// @@protoc_insertion_point(message_clear_start:ecron.RpcMeta)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (GetArenaForAllocation() == nullptr && request_ != nullptr) {
    delete request_;
  }
  request_ = nullptr;
  if (GetArenaForAllocation() == nullptr && response_ != nullptr) {
    delete response_;
  }
  response_ = nullptr;
  correlation_id_ = int64_t{0};
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* RpcMeta::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // .ecron.RpcRequestMeta request = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          ptr = ctx->ParseMessage(_internal_mutable_request(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .ecron.RpcResponseMeta response = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          ptr = ctx->ParseMessage(_internal_mutable_response(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // int64 correlation_id = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 32)) {
          correlation_id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* RpcMeta::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:ecron.RpcMeta)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // .ecron.RpcRequestMeta request = 1;
  if (this->_internal_has_request()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(
        1, _Internal::request(this), target, stream);
  }

  // .ecron.RpcResponseMeta response = 2;
  if (this->_internal_has_response()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(
        2, _Internal::response(this), target, stream);
  }

  // int64 correlation_id = 4;
  if (this->_internal_correlation_id() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt64ToArray(4, this->_internal_correlation_id(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:ecron.RpcMeta)
  return target;
}

size_t RpcMeta::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:ecron.RpcMeta)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // .ecron.RpcRequestMeta request = 1;
  if (this->_internal_has_request()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *request_);
  }

  // .ecron.RpcResponseMeta response = 2;
  if (this->_internal_has_response()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *response_);
  }

  // int64 correlation_id = 4;
  if (this->_internal_correlation_id() != 0) {
    total_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int64SizePlusOne(this->_internal_correlation_id());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData RpcMeta::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    RpcMeta::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*RpcMeta::GetClassData() const { return &_class_data_; }

void RpcMeta::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to,
                      const ::PROTOBUF_NAMESPACE_ID::Message& from) {
  static_cast<RpcMeta *>(to)->MergeFrom(
      static_cast<const RpcMeta &>(from));
}


void RpcMeta::MergeFrom(const RpcMeta& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:ecron.RpcMeta)
  GOOGLE_DCHECK_NE(&from, this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_has_request()) {
    _internal_mutable_request()->::ecron::RpcRequestMeta::MergeFrom(from._internal_request());
  }
  if (from._internal_has_response()) {
    _internal_mutable_response()->::ecron::RpcResponseMeta::MergeFrom(from._internal_response());
  }
  if (from._internal_correlation_id() != 0) {
    _internal_set_correlation_id(from._internal_correlation_id());
  }
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void RpcMeta::CopyFrom(const RpcMeta& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:ecron.RpcMeta)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool RpcMeta::IsInitialized() const {
  return true;
}

void RpcMeta::InternalSwap(RpcMeta* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(RpcMeta, correlation_id_)
      + sizeof(RpcMeta::correlation_id_)
      - PROTOBUF_FIELD_OFFSET(RpcMeta, request_)>(
          reinterpret_cast<char*>(&request_),
          reinterpret_cast<char*>(&other->request_));
}

::PROTOBUF_NAMESPACE_ID::Metadata RpcMeta::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_rpc_5fmeta_2eproto_getter, &descriptor_table_rpc_5fmeta_2eproto_once,
      file_level_metadata_rpc_5fmeta_2eproto[0]);
}

// ===================================================================

class RpcRequestMeta::_Internal {
 public:
};

RpcRequestMeta::RpcRequestMeta(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:ecron.RpcRequestMeta)
}
RpcRequestMeta::RpcRequestMeta(const RpcRequestMeta& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  service_name_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    service_name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_service_name().empty()) {
    service_name_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_service_name(), 
      GetArenaForAllocation());
  }
  method_name_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    method_name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_method_name().empty()) {
    method_name_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_method_name(), 
      GetArenaForAllocation());
  }
  // @@protoc_insertion_point(copy_constructor:ecron.RpcRequestMeta)
}

inline void RpcRequestMeta::SharedCtor() {
service_name_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  service_name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
method_name_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  method_name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

RpcRequestMeta::~RpcRequestMeta() {
  // @@protoc_insertion_point(destructor:ecron.RpcRequestMeta)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

inline void RpcRequestMeta::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  service_name_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  method_name_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void RpcRequestMeta::ArenaDtor(void* object) {
  RpcRequestMeta* _this = reinterpret_cast< RpcRequestMeta* >(object);
  (void)_this;
}
void RpcRequestMeta::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void RpcRequestMeta::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void RpcRequestMeta::Clear() {
// @@protoc_insertion_point(message_clear_start:ecron.RpcRequestMeta)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  service_name_.ClearToEmpty();
  method_name_.ClearToEmpty();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* RpcRequestMeta::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // string service_name = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          auto str = _internal_mutable_service_name();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "ecron.RpcRequestMeta.service_name"));
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // string method_name = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          auto str = _internal_mutable_method_name();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "ecron.RpcRequestMeta.method_name"));
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* RpcRequestMeta::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:ecron.RpcRequestMeta)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // string service_name = 1;
  if (!this->_internal_service_name().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_service_name().data(), static_cast<int>(this->_internal_service_name().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "ecron.RpcRequestMeta.service_name");
    target = stream->WriteStringMaybeAliased(
        1, this->_internal_service_name(), target);
  }

  // string method_name = 2;
  if (!this->_internal_method_name().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_method_name().data(), static_cast<int>(this->_internal_method_name().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "ecron.RpcRequestMeta.method_name");
    target = stream->WriteStringMaybeAliased(
        2, this->_internal_method_name(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:ecron.RpcRequestMeta)
  return target;
}

size_t RpcRequestMeta::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:ecron.RpcRequestMeta)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string service_name = 1;
  if (!this->_internal_service_name().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_service_name());
  }

  // string method_name = 2;
  if (!this->_internal_method_name().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_method_name());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData RpcRequestMeta::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    RpcRequestMeta::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*RpcRequestMeta::GetClassData() const { return &_class_data_; }

void RpcRequestMeta::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to,
                      const ::PROTOBUF_NAMESPACE_ID::Message& from) {
  static_cast<RpcRequestMeta *>(to)->MergeFrom(
      static_cast<const RpcRequestMeta &>(from));
}


void RpcRequestMeta::MergeFrom(const RpcRequestMeta& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:ecron.RpcRequestMeta)
  GOOGLE_DCHECK_NE(&from, this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_service_name().empty()) {
    _internal_set_service_name(from._internal_service_name());
  }
  if (!from._internal_method_name().empty()) {
    _internal_set_method_name(from._internal_method_name());
  }
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void RpcRequestMeta::CopyFrom(const RpcRequestMeta& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:ecron.RpcRequestMeta)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool RpcRequestMeta::IsInitialized() const {
  return true;
}

void RpcRequestMeta::InternalSwap(RpcRequestMeta* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      &service_name_, lhs_arena,
      &other->service_name_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      &method_name_, lhs_arena,
      &other->method_name_, rhs_arena
  );
}

::PROTOBUF_NAMESPACE_ID::Metadata RpcRequestMeta::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_rpc_5fmeta_2eproto_getter, &descriptor_table_rpc_5fmeta_2eproto_once,
      file_level_metadata_rpc_5fmeta_2eproto[1]);
}

// ===================================================================

class RpcResponseMeta::_Internal {
 public:
};

RpcResponseMeta::RpcResponseMeta(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:ecron.RpcResponseMeta)
}
RpcResponseMeta::RpcResponseMeta(const RpcResponseMeta& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  error_text_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    error_text_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_error_text().empty()) {
    error_text_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_error_text(), 
      GetArenaForAllocation());
  }
  error_code_ = from.error_code_;
  // @@protoc_insertion_point(copy_constructor:ecron.RpcResponseMeta)
}

inline void RpcResponseMeta::SharedCtor() {
error_text_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  error_text_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
error_code_ = 0;
}

RpcResponseMeta::~RpcResponseMeta() {
  // @@protoc_insertion_point(destructor:ecron.RpcResponseMeta)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

inline void RpcResponseMeta::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  error_text_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void RpcResponseMeta::ArenaDtor(void* object) {
  RpcResponseMeta* _this = reinterpret_cast< RpcResponseMeta* >(object);
  (void)_this;
}
void RpcResponseMeta::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void RpcResponseMeta::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void RpcResponseMeta::Clear() {
// @@protoc_insertion_point(message_clear_start:ecron.RpcResponseMeta)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  error_text_.ClearToEmpty();
  error_code_ = 0;
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* RpcResponseMeta::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // int32 error_code = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          error_code_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // string error_text = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          auto str = _internal_mutable_error_text();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "ecron.RpcResponseMeta.error_text"));
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* RpcResponseMeta::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:ecron.RpcResponseMeta)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 error_code = 1;
  if (this->_internal_error_code() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(1, this->_internal_error_code(), target);
  }

  // string error_text = 2;
  if (!this->_internal_error_text().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_error_text().data(), static_cast<int>(this->_internal_error_text().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "ecron.RpcResponseMeta.error_text");
    target = stream->WriteStringMaybeAliased(
        2, this->_internal_error_text(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:ecron.RpcResponseMeta)
  return target;
}

size_t RpcResponseMeta::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:ecron.RpcResponseMeta)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string error_text = 2;
  if (!this->_internal_error_text().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_error_text());
  }

  // int32 error_code = 1;
  if (this->_internal_error_code() != 0) {
    total_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32SizePlusOne(this->_internal_error_code());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData RpcResponseMeta::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    RpcResponseMeta::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*RpcResponseMeta::GetClassData() const { return &_class_data_; }

void RpcResponseMeta::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to,
                      const ::PROTOBUF_NAMESPACE_ID::Message& from) {
  static_cast<RpcResponseMeta *>(to)->MergeFrom(
      static_cast<const RpcResponseMeta &>(from));
}


void RpcResponseMeta::MergeFrom(const RpcResponseMeta& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:ecron.RpcResponseMeta)
  GOOGLE_DCHECK_NE(&from, this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_error_text().empty()) {
    _internal_set_error_text(from._internal_error_text());
  }
  if (from._internal_error_code() != 0) {
    _internal_set_error_code(from._internal_error_code());
  }
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void RpcResponseMeta::CopyFrom(const RpcResponseMeta& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:ecron.RpcResponseMeta)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool RpcResponseMeta::IsInitialized() const {
  return true;
}

void RpcResponseMeta::InternalSwap(RpcResponseMeta* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      &error_text_, lhs_arena,
      &other->error_text_, rhs_arena
  );
  swap(error_code_, other->error_code_);
}

::PROTOBUF_NAMESPACE_ID::Metadata RpcResponseMeta::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_rpc_5fmeta_2eproto_getter, &descriptor_table_rpc_5fmeta_2eproto_once,
      file_level_metadata_rpc_5fmeta_2eproto[2]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace ecron
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::ecron::RpcMeta* Arena::CreateMaybeMessage< ::ecron::RpcMeta >(Arena* arena) {
  return Arena::CreateMessageInternal< ::ecron::RpcMeta >(arena);
}
template<> PROTOBUF_NOINLINE ::ecron::RpcRequestMeta* Arena::CreateMaybeMessage< ::ecron::RpcRequestMeta >(Arena* arena) {
  return Arena::CreateMessageInternal< ::ecron::RpcRequestMeta >(arena);
}
template<> PROTOBUF_NOINLINE ::ecron::RpcResponseMeta* Arena::CreateMaybeMessage< ::ecron::RpcResponseMeta >(Arena* arena) {
  return Arena::CreateMessageInternal< ::ecron::RpcResponseMeta >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>