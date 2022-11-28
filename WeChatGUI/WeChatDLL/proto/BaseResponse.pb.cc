// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: BaseResponse.proto

#include "BaseResponse.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

PROTOBUF_CONSTEXPR BaseResponse::BaseResponse(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.errmsg_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.code_)*/0
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct BaseResponseDefaultTypeInternal {
  PROTOBUF_CONSTEXPR BaseResponseDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~BaseResponseDefaultTypeInternal() {}
  union {
    BaseResponse _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 BaseResponseDefaultTypeInternal _BaseResponse_default_instance_;

// ===================================================================

class BaseResponse::_Internal {
 public:
};

BaseResponse::BaseResponse(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::MessageLite(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:BaseResponse)
}
BaseResponse::BaseResponse(const BaseResponse& from)
  : ::PROTOBUF_NAMESPACE_ID::MessageLite() {
  BaseResponse* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.errmsg_){}
    , decltype(_impl_.code_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<std::string>(from._internal_metadata_);
  _impl_.errmsg_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.errmsg_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_errmsg().empty()) {
    _this->_impl_.errmsg_.Set(from._internal_errmsg(), 
      _this->GetArenaForAllocation());
  }
  _this->_impl_.code_ = from._impl_.code_;
  // @@protoc_insertion_point(copy_constructor:BaseResponse)
}

inline void BaseResponse::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.errmsg_){}
    , decltype(_impl_.code_){0}
    , /*decltype(_impl_._cached_size_)*/{}
  };
  _impl_.errmsg_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.errmsg_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

BaseResponse::~BaseResponse() {
  // @@protoc_insertion_point(destructor:BaseResponse)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<std::string>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void BaseResponse::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.errmsg_.Destroy();
}

void BaseResponse::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void BaseResponse::Clear() {
// @@protoc_insertion_point(message_clear_start:BaseResponse)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.errmsg_.ClearToEmpty();
  _impl_.code_ = 0;
  _internal_metadata_.Clear<std::string>();
}

const char* BaseResponse::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // int32 code = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          _impl_.code_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // string errMsg = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          auto str = _internal_mutable_errmsg();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, nullptr));
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
        _internal_metadata_.mutable_unknown_fields<std::string>(),
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

uint8_t* BaseResponse::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:BaseResponse)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 code = 1;
  if (this->_internal_code() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(1, this->_internal_code(), target);
  }

  // string errMsg = 2;
  if (!this->_internal_errmsg().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_errmsg().data(), static_cast<int>(this->_internal_errmsg().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "BaseResponse.errMsg");
    target = stream->WriteStringMaybeAliased(
        2, this->_internal_errmsg(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = stream->WriteRaw(_internal_metadata_.unknown_fields<std::string>(::PROTOBUF_NAMESPACE_ID::internal::GetEmptyString).data(),
        static_cast<int>(_internal_metadata_.unknown_fields<std::string>(::PROTOBUF_NAMESPACE_ID::internal::GetEmptyString).size()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:BaseResponse)
  return target;
}

size_t BaseResponse::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:BaseResponse)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string errMsg = 2;
  if (!this->_internal_errmsg().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_errmsg());
  }

  // int32 code = 1;
  if (this->_internal_code() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_code());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    total_size += _internal_metadata_.unknown_fields<std::string>(::PROTOBUF_NAMESPACE_ID::internal::GetEmptyString).size();
  }
  int cached_size = ::_pbi::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void BaseResponse::CheckTypeAndMergeFrom(
    const ::PROTOBUF_NAMESPACE_ID::MessageLite& from) {
  MergeFrom(*::_pbi::DownCast<const BaseResponse*>(
      &from));
}

void BaseResponse::MergeFrom(const BaseResponse& from) {
  BaseResponse* const _this = this;
  // @@protoc_insertion_point(class_specific_merge_from_start:BaseResponse)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_errmsg().empty()) {
    _this->_internal_set_errmsg(from._internal_errmsg());
  }
  if (from._internal_code() != 0) {
    _this->_internal_set_code(from._internal_code());
  }
  _this->_internal_metadata_.MergeFrom<std::string>(from._internal_metadata_);
}

void BaseResponse::CopyFrom(const BaseResponse& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:BaseResponse)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool BaseResponse::IsInitialized() const {
  return true;
}

void BaseResponse::InternalSwap(BaseResponse* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.errmsg_, lhs_arena,
      &other->_impl_.errmsg_, rhs_arena
  );
  swap(_impl_.code_, other->_impl_.code_);
}

std::string BaseResponse::GetTypeName() const {
  return "BaseResponse";
}


// @@protoc_insertion_point(namespace_scope)
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::BaseResponse*
Arena::CreateMaybeMessage< ::BaseResponse >(Arena* arena) {
  return Arena::CreateMessageInternal< ::BaseResponse >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
