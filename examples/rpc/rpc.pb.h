// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: rpc.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_rpc_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_rpc_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3019000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3019001 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_rpc_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_rpc_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_rpc_2eproto;
class RpcMessage;
struct RpcMessageDefaultTypeInternal;
extern RpcMessageDefaultTypeInternal _RpcMessage_default_instance_;
PROTOBUF_NAMESPACE_OPEN
template<> ::RpcMessage* Arena::CreateMaybeMessage<::RpcMessage>(Arena*);
PROTOBUF_NAMESPACE_CLOSE

enum MessageType : int {
  REQUEST = 0,
  RESPONSE = 1,
  ERROR = 2,
  MessageType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  MessageType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool MessageType_IsValid(int value);
constexpr MessageType MessageType_MIN = REQUEST;
constexpr MessageType MessageType_MAX = ERROR;
constexpr int MessageType_ARRAYSIZE = MessageType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* MessageType_descriptor();
template<typename T>
inline const std::string& MessageType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, MessageType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function MessageType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    MessageType_descriptor(), enum_t_value);
}
inline bool MessageType_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, MessageType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<MessageType>(
    MessageType_descriptor(), name, value);
}
enum ErrorCode : int {
  NO_ERROR = 0,
  NO_SERVICE = 1,
  NO_METHOD = 2,
  INVALID_REQUEST = 3,
  INVALID_RESPONSE = 4,
  TIMEOUT = 5,
  ErrorCode_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  ErrorCode_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool ErrorCode_IsValid(int value);
constexpr ErrorCode ErrorCode_MIN = NO_ERROR;
constexpr ErrorCode ErrorCode_MAX = TIMEOUT;
constexpr int ErrorCode_ARRAYSIZE = ErrorCode_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* ErrorCode_descriptor();
template<typename T>
inline const std::string& ErrorCode_Name(T enum_t_value) {
  static_assert(::std::is_same<T, ErrorCode>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function ErrorCode_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    ErrorCode_descriptor(), enum_t_value);
}
inline bool ErrorCode_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, ErrorCode* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<ErrorCode>(
    ErrorCode_descriptor(), name, value);
}
// ===================================================================

class RpcMessage final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:RpcMessage) */ {
 public:
  inline RpcMessage() : RpcMessage(nullptr) {}
  ~RpcMessage() override;
  explicit constexpr RpcMessage(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  RpcMessage(const RpcMessage& from);
  RpcMessage(RpcMessage&& from) noexcept
    : RpcMessage() {
    *this = ::std::move(from);
  }

  inline RpcMessage& operator=(const RpcMessage& from) {
    CopyFrom(from);
    return *this;
  }
  inline RpcMessage& operator=(RpcMessage&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const RpcMessage& default_instance() {
    return *internal_default_instance();
  }
  static inline const RpcMessage* internal_default_instance() {
    return reinterpret_cast<const RpcMessage*>(
               &_RpcMessage_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(RpcMessage& a, RpcMessage& b) {
    a.Swap(&b);
  }
  inline void Swap(RpcMessage* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(RpcMessage* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  RpcMessage* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<RpcMessage>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const RpcMessage& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const RpcMessage& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(RpcMessage* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "RpcMessage";
  }
  protected:
  explicit RpcMessage(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kServiceFieldNumber = 3,
    kMethodFieldNumber = 4,
    kRequestFieldNumber = 6,
    kResponseFieldNumber = 7,
    kIdFieldNumber = 2,
    kTypeFieldNumber = 1,
    kErrorFieldNumber = 5,
  };
  // string service = 3;
  void clear_service();
  const std::string& service() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_service(ArgT0&& arg0, ArgT... args);
  std::string* mutable_service();
  PROTOBUF_NODISCARD std::string* release_service();
  void set_allocated_service(std::string* service);
  private:
  const std::string& _internal_service() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_service(const std::string& value);
  std::string* _internal_mutable_service();
  public:

  // string method = 4;
  void clear_method();
  const std::string& method() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_method(ArgT0&& arg0, ArgT... args);
  std::string* mutable_method();
  PROTOBUF_NODISCARD std::string* release_method();
  void set_allocated_method(std::string* method);
  private:
  const std::string& _internal_method() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_method(const std::string& value);
  std::string* _internal_mutable_method();
  public:

  // bytes request = 6;
  void clear_request();
  const std::string& request() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_request(ArgT0&& arg0, ArgT... args);
  std::string* mutable_request();
  PROTOBUF_NODISCARD std::string* release_request();
  void set_allocated_request(std::string* request);
  private:
  const std::string& _internal_request() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_request(const std::string& value);
  std::string* _internal_mutable_request();
  public:

  // bytes response = 7;
  void clear_response();
  const std::string& response() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_response(ArgT0&& arg0, ArgT... args);
  std::string* mutable_response();
  PROTOBUF_NODISCARD std::string* release_response();
  void set_allocated_response(std::string* response);
  private:
  const std::string& _internal_response() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_response(const std::string& value);
  std::string* _internal_mutable_response();
  public:

  // uint64 id = 2;
  void clear_id();
  uint64_t id() const;
  void set_id(uint64_t value);
  private:
  uint64_t _internal_id() const;
  void _internal_set_id(uint64_t value);
  public:

  // .MessageType type = 1;
  void clear_type();
  ::MessageType type() const;
  void set_type(::MessageType value);
  private:
  ::MessageType _internal_type() const;
  void _internal_set_type(::MessageType value);
  public:

  // .ErrorCode error = 5;
  void clear_error();
  ::ErrorCode error() const;
  void set_error(::ErrorCode value);
  private:
  ::ErrorCode _internal_error() const;
  void _internal_set_error(::ErrorCode value);
  public:

  // @@protoc_insertion_point(class_scope:RpcMessage)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr service_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr method_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr request_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr response_;
  uint64_t id_;
  int type_;
  int error_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_rpc_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// RpcMessage

// .MessageType type = 1;
inline void RpcMessage::clear_type() {
  type_ = 0;
}
inline ::MessageType RpcMessage::_internal_type() const {
  return static_cast< ::MessageType >(type_);
}
inline ::MessageType RpcMessage::type() const {
  // @@protoc_insertion_point(field_get:RpcMessage.type)
  return _internal_type();
}
inline void RpcMessage::_internal_set_type(::MessageType value) {
  
  type_ = value;
}
inline void RpcMessage::set_type(::MessageType value) {
  _internal_set_type(value);
  // @@protoc_insertion_point(field_set:RpcMessage.type)
}

// uint64 id = 2;
inline void RpcMessage::clear_id() {
  id_ = uint64_t{0u};
}
inline uint64_t RpcMessage::_internal_id() const {
  return id_;
}
inline uint64_t RpcMessage::id() const {
  // @@protoc_insertion_point(field_get:RpcMessage.id)
  return _internal_id();
}
inline void RpcMessage::_internal_set_id(uint64_t value) {
  
  id_ = value;
}
inline void RpcMessage::set_id(uint64_t value) {
  _internal_set_id(value);
  // @@protoc_insertion_point(field_set:RpcMessage.id)
}

// string service = 3;
inline void RpcMessage::clear_service() {
  service_.ClearToEmpty();
}
inline const std::string& RpcMessage::service() const {
  // @@protoc_insertion_point(field_get:RpcMessage.service)
  return _internal_service();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void RpcMessage::set_service(ArgT0&& arg0, ArgT... args) {
 
 service_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:RpcMessage.service)
}
inline std::string* RpcMessage::mutable_service() {
  std::string* _s = _internal_mutable_service();
  // @@protoc_insertion_point(field_mutable:RpcMessage.service)
  return _s;
}
inline const std::string& RpcMessage::_internal_service() const {
  return service_.Get();
}
inline void RpcMessage::_internal_set_service(const std::string& value) {
  
  service_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* RpcMessage::_internal_mutable_service() {
  
  return service_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* RpcMessage::release_service() {
  // @@protoc_insertion_point(field_release:RpcMessage.service)
  return service_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void RpcMessage::set_allocated_service(std::string* service) {
  if (service != nullptr) {
    
  } else {
    
  }
  service_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), service,
      GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (service_.IsDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited())) {
    service_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:RpcMessage.service)
}

// string method = 4;
inline void RpcMessage::clear_method() {
  method_.ClearToEmpty();
}
inline const std::string& RpcMessage::method() const {
  // @@protoc_insertion_point(field_get:RpcMessage.method)
  return _internal_method();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void RpcMessage::set_method(ArgT0&& arg0, ArgT... args) {
 
 method_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:RpcMessage.method)
}
inline std::string* RpcMessage::mutable_method() {
  std::string* _s = _internal_mutable_method();
  // @@protoc_insertion_point(field_mutable:RpcMessage.method)
  return _s;
}
inline const std::string& RpcMessage::_internal_method() const {
  return method_.Get();
}
inline void RpcMessage::_internal_set_method(const std::string& value) {
  
  method_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* RpcMessage::_internal_mutable_method() {
  
  return method_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* RpcMessage::release_method() {
  // @@protoc_insertion_point(field_release:RpcMessage.method)
  return method_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void RpcMessage::set_allocated_method(std::string* method) {
  if (method != nullptr) {
    
  } else {
    
  }
  method_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), method,
      GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (method_.IsDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited())) {
    method_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:RpcMessage.method)
}

// .ErrorCode error = 5;
inline void RpcMessage::clear_error() {
  error_ = 0;
}
inline ::ErrorCode RpcMessage::_internal_error() const {
  return static_cast< ::ErrorCode >(error_);
}
inline ::ErrorCode RpcMessage::error() const {
  // @@protoc_insertion_point(field_get:RpcMessage.error)
  return _internal_error();
}
inline void RpcMessage::_internal_set_error(::ErrorCode value) {
  
  error_ = value;
}
inline void RpcMessage::set_error(::ErrorCode value) {
  _internal_set_error(value);
  // @@protoc_insertion_point(field_set:RpcMessage.error)
}

// bytes request = 6;
inline void RpcMessage::clear_request() {
  request_.ClearToEmpty();
}
inline const std::string& RpcMessage::request() const {
  // @@protoc_insertion_point(field_get:RpcMessage.request)
  return _internal_request();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void RpcMessage::set_request(ArgT0&& arg0, ArgT... args) {
 
 request_.SetBytes(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:RpcMessage.request)
}
inline std::string* RpcMessage::mutable_request() {
  std::string* _s = _internal_mutable_request();
  // @@protoc_insertion_point(field_mutable:RpcMessage.request)
  return _s;
}
inline const std::string& RpcMessage::_internal_request() const {
  return request_.Get();
}
inline void RpcMessage::_internal_set_request(const std::string& value) {
  
  request_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* RpcMessage::_internal_mutable_request() {
  
  return request_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* RpcMessage::release_request() {
  // @@protoc_insertion_point(field_release:RpcMessage.request)
  return request_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void RpcMessage::set_allocated_request(std::string* request) {
  if (request != nullptr) {
    
  } else {
    
  }
  request_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), request,
      GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (request_.IsDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited())) {
    request_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:RpcMessage.request)
}

// bytes response = 7;
inline void RpcMessage::clear_response() {
  response_.ClearToEmpty();
}
inline const std::string& RpcMessage::response() const {
  // @@protoc_insertion_point(field_get:RpcMessage.response)
  return _internal_response();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void RpcMessage::set_response(ArgT0&& arg0, ArgT... args) {
 
 response_.SetBytes(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:RpcMessage.response)
}
inline std::string* RpcMessage::mutable_response() {
  std::string* _s = _internal_mutable_response();
  // @@protoc_insertion_point(field_mutable:RpcMessage.response)
  return _s;
}
inline const std::string& RpcMessage::_internal_response() const {
  return response_.Get();
}
inline void RpcMessage::_internal_set_response(const std::string& value) {
  
  response_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* RpcMessage::_internal_mutable_response() {
  
  return response_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* RpcMessage::release_response() {
  // @@protoc_insertion_point(field_release:RpcMessage.response)
  return response_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void RpcMessage::set_allocated_response(std::string* response) {
  if (response != nullptr) {
    
  } else {
    
  }
  response_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), response,
      GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (response_.IsDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited())) {
    response_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:RpcMessage.response)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)


PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::MessageType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::MessageType>() {
  return ::MessageType_descriptor();
}
template <> struct is_proto_enum< ::ErrorCode> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::ErrorCode>() {
  return ::ErrorCode_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_rpc_2eproto
