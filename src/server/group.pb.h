// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: group.proto

#ifndef PROTOBUF_group_2eproto__INCLUDED
#define PROTOBUF_group_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace group {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_group_2eproto();
void protobuf_AssignDesc_group_2eproto();
void protobuf_ShutdownFile_group_2eproto();

class HandleGroup;
class GroupMessage;

enum HandleGroup_OperateType {
  HandleGroup_OperateType_CREATE = 1,
  HandleGroup_OperateType_DELETE = 2,
  HandleGroup_OperateType_JOIN = 3,
  HandleGroup_OperateType_QUIT = 4
};
bool HandleGroup_OperateType_IsValid(int value);
const HandleGroup_OperateType HandleGroup_OperateType_OperateType_MIN = HandleGroup_OperateType_CREATE;
const HandleGroup_OperateType HandleGroup_OperateType_OperateType_MAX = HandleGroup_OperateType_QUIT;
const int HandleGroup_OperateType_OperateType_ARRAYSIZE = HandleGroup_OperateType_OperateType_MAX + 1;

const ::google::protobuf::EnumDescriptor* HandleGroup_OperateType_descriptor();
inline const ::std::string& HandleGroup_OperateType_Name(HandleGroup_OperateType value) {
  return ::google::protobuf::internal::NameOfEnum(
    HandleGroup_OperateType_descriptor(), value);
}
inline bool HandleGroup_OperateType_Parse(
    const ::std::string& name, HandleGroup_OperateType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<HandleGroup_OperateType>(
    HandleGroup_OperateType_descriptor(), name, value);
}
// ===================================================================

class HandleGroup : public ::google::protobuf::Message {
 public:
  HandleGroup();
  virtual ~HandleGroup();

  HandleGroup(const HandleGroup& from);

  inline HandleGroup& operator=(const HandleGroup& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const HandleGroup& default_instance();

  void Swap(HandleGroup* other);

  // implements Message ----------------------------------------------

  HandleGroup* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const HandleGroup& from);
  void MergeFrom(const HandleGroup& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  typedef HandleGroup_OperateType OperateType;
  static const OperateType CREATE = HandleGroup_OperateType_CREATE;
  static const OperateType DELETE = HandleGroup_OperateType_DELETE;
  static const OperateType JOIN = HandleGroup_OperateType_JOIN;
  static const OperateType QUIT = HandleGroup_OperateType_QUIT;
  static inline bool OperateType_IsValid(int value) {
    return HandleGroup_OperateType_IsValid(value);
  }
  static const OperateType OperateType_MIN =
    HandleGroup_OperateType_OperateType_MIN;
  static const OperateType OperateType_MAX =
    HandleGroup_OperateType_OperateType_MAX;
  static const int OperateType_ARRAYSIZE =
    HandleGroup_OperateType_OperateType_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  OperateType_descriptor() {
    return HandleGroup_OperateType_descriptor();
  }
  static inline const ::std::string& OperateType_Name(OperateType value) {
    return HandleGroup_OperateType_Name(value);
  }
  static inline bool OperateType_Parse(const ::std::string& name,
      OperateType* value) {
    return HandleGroup_OperateType_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // required string name = 1;
  inline bool has_name() const;
  inline void clear_name();
  static const int kNameFieldNumber = 1;
  inline const ::std::string& name() const;
  inline void set_name(const ::std::string& value);
  inline void set_name(const char* value);
  inline void set_name(const char* value, size_t size);
  inline ::std::string* mutable_name();
  inline ::std::string* release_name();
  inline void set_allocated_name(::std::string* name);

  // required int32 id = 2;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 2;
  inline ::google::protobuf::int32 id() const;
  inline void set_id(::google::protobuf::int32 value);

  // optional .group.HandleGroup.OperateType type = 3 [default = CREATE];
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 3;
  inline ::group::HandleGroup_OperateType type() const;
  inline void set_type(::group::HandleGroup_OperateType value);

  // @@protoc_insertion_point(class_scope:group.HandleGroup)
 private:
  inline void set_has_name();
  inline void clear_has_name();
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_type();
  inline void clear_has_type();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* name_;
  ::google::protobuf::int32 id_;
  int type_;
  friend void  protobuf_AddDesc_group_2eproto();
  friend void protobuf_AssignDesc_group_2eproto();
  friend void protobuf_ShutdownFile_group_2eproto();

  void InitAsDefaultInstance();
  static HandleGroup* default_instance_;
};
// -------------------------------------------------------------------

class GroupMessage : public ::google::protobuf::Message {
 public:
  GroupMessage();
  virtual ~GroupMessage();

  GroupMessage(const GroupMessage& from);

  inline GroupMessage& operator=(const GroupMessage& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const GroupMessage& default_instance();

  void Swap(GroupMessage* other);

  // implements Message ----------------------------------------------

  GroupMessage* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const GroupMessage& from);
  void MergeFrom(const GroupMessage& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string name = 1;
  inline bool has_name() const;
  inline void clear_name();
  static const int kNameFieldNumber = 1;
  inline const ::std::string& name() const;
  inline void set_name(const ::std::string& value);
  inline void set_name(const char* value);
  inline void set_name(const char* value, size_t size);
  inline ::std::string* mutable_name();
  inline ::std::string* release_name();
  inline void set_allocated_name(::std::string* name);

  // required int32 id = 2;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 2;
  inline ::google::protobuf::int32 id() const;
  inline void set_id(::google::protobuf::int32 value);

  // required string content = 3;
  inline bool has_content() const;
  inline void clear_content();
  static const int kContentFieldNumber = 3;
  inline const ::std::string& content() const;
  inline void set_content(const ::std::string& value);
  inline void set_content(const char* value);
  inline void set_content(const char* value, size_t size);
  inline ::std::string* mutable_content();
  inline ::std::string* release_content();
  inline void set_allocated_content(::std::string* content);

  // @@protoc_insertion_point(class_scope:group.GroupMessage)
 private:
  inline void set_has_name();
  inline void clear_has_name();
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_content();
  inline void clear_has_content();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* name_;
  ::std::string* content_;
  ::google::protobuf::int32 id_;
  friend void  protobuf_AddDesc_group_2eproto();
  friend void protobuf_AssignDesc_group_2eproto();
  friend void protobuf_ShutdownFile_group_2eproto();

  void InitAsDefaultInstance();
  static GroupMessage* default_instance_;
};
// ===================================================================


// ===================================================================

// HandleGroup

// required string name = 1;
inline bool HandleGroup::has_name() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void HandleGroup::set_has_name() {
  _has_bits_[0] |= 0x00000001u;
}
inline void HandleGroup::clear_has_name() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void HandleGroup::clear_name() {
  if (name_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    name_->clear();
  }
  clear_has_name();
}
inline const ::std::string& HandleGroup::name() const {
  // @@protoc_insertion_point(field_get:group.HandleGroup.name)
  return *name_;
}
inline void HandleGroup::set_name(const ::std::string& value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    name_ = new ::std::string;
  }
  name_->assign(value);
  // @@protoc_insertion_point(field_set:group.HandleGroup.name)
}
inline void HandleGroup::set_name(const char* value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    name_ = new ::std::string;
  }
  name_->assign(value);
  // @@protoc_insertion_point(field_set_char:group.HandleGroup.name)
}
inline void HandleGroup::set_name(const char* value, size_t size) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    name_ = new ::std::string;
  }
  name_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:group.HandleGroup.name)
}
inline ::std::string* HandleGroup::mutable_name() {
  set_has_name();
  if (name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    name_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:group.HandleGroup.name)
  return name_;
}
inline ::std::string* HandleGroup::release_name() {
  clear_has_name();
  if (name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = name_;
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void HandleGroup::set_allocated_name(::std::string* name) {
  if (name_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete name_;
  }
  if (name) {
    set_has_name();
    name_ = name;
  } else {
    clear_has_name();
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:group.HandleGroup.name)
}

// required int32 id = 2;
inline bool HandleGroup::has_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void HandleGroup::set_has_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void HandleGroup::clear_has_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void HandleGroup::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 HandleGroup::id() const {
  // @@protoc_insertion_point(field_get:group.HandleGroup.id)
  return id_;
}
inline void HandleGroup::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
  // @@protoc_insertion_point(field_set:group.HandleGroup.id)
}

// optional .group.HandleGroup.OperateType type = 3 [default = CREATE];
inline bool HandleGroup::has_type() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void HandleGroup::set_has_type() {
  _has_bits_[0] |= 0x00000004u;
}
inline void HandleGroup::clear_has_type() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void HandleGroup::clear_type() {
  type_ = 1;
  clear_has_type();
}
inline ::group::HandleGroup_OperateType HandleGroup::type() const {
  // @@protoc_insertion_point(field_get:group.HandleGroup.type)
  return static_cast< ::group::HandleGroup_OperateType >(type_);
}
inline void HandleGroup::set_type(::group::HandleGroup_OperateType value) {
  assert(::group::HandleGroup_OperateType_IsValid(value));
  set_has_type();
  type_ = value;
  // @@protoc_insertion_point(field_set:group.HandleGroup.type)
}

// -------------------------------------------------------------------

// GroupMessage

// required string name = 1;
inline bool GroupMessage::has_name() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void GroupMessage::set_has_name() {
  _has_bits_[0] |= 0x00000001u;
}
inline void GroupMessage::clear_has_name() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void GroupMessage::clear_name() {
  if (name_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    name_->clear();
  }
  clear_has_name();
}
inline const ::std::string& GroupMessage::name() const {
  // @@protoc_insertion_point(field_get:group.GroupMessage.name)
  return *name_;
}
inline void GroupMessage::set_name(const ::std::string& value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    name_ = new ::std::string;
  }
  name_->assign(value);
  // @@protoc_insertion_point(field_set:group.GroupMessage.name)
}
inline void GroupMessage::set_name(const char* value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    name_ = new ::std::string;
  }
  name_->assign(value);
  // @@protoc_insertion_point(field_set_char:group.GroupMessage.name)
}
inline void GroupMessage::set_name(const char* value, size_t size) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    name_ = new ::std::string;
  }
  name_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:group.GroupMessage.name)
}
inline ::std::string* GroupMessage::mutable_name() {
  set_has_name();
  if (name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    name_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:group.GroupMessage.name)
  return name_;
}
inline ::std::string* GroupMessage::release_name() {
  clear_has_name();
  if (name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = name_;
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void GroupMessage::set_allocated_name(::std::string* name) {
  if (name_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete name_;
  }
  if (name) {
    set_has_name();
    name_ = name;
  } else {
    clear_has_name();
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:group.GroupMessage.name)
}

// required int32 id = 2;
inline bool GroupMessage::has_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void GroupMessage::set_has_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void GroupMessage::clear_has_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void GroupMessage::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 GroupMessage::id() const {
  // @@protoc_insertion_point(field_get:group.GroupMessage.id)
  return id_;
}
inline void GroupMessage::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
  // @@protoc_insertion_point(field_set:group.GroupMessage.id)
}

// required string content = 3;
inline bool GroupMessage::has_content() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void GroupMessage::set_has_content() {
  _has_bits_[0] |= 0x00000004u;
}
inline void GroupMessage::clear_has_content() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void GroupMessage::clear_content() {
  if (content_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    content_->clear();
  }
  clear_has_content();
}
inline const ::std::string& GroupMessage::content() const {
  // @@protoc_insertion_point(field_get:group.GroupMessage.content)
  return *content_;
}
inline void GroupMessage::set_content(const ::std::string& value) {
  set_has_content();
  if (content_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    content_ = new ::std::string;
  }
  content_->assign(value);
  // @@protoc_insertion_point(field_set:group.GroupMessage.content)
}
inline void GroupMessage::set_content(const char* value) {
  set_has_content();
  if (content_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    content_ = new ::std::string;
  }
  content_->assign(value);
  // @@protoc_insertion_point(field_set_char:group.GroupMessage.content)
}
inline void GroupMessage::set_content(const char* value, size_t size) {
  set_has_content();
  if (content_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    content_ = new ::std::string;
  }
  content_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:group.GroupMessage.content)
}
inline ::std::string* GroupMessage::mutable_content() {
  set_has_content();
  if (content_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    content_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:group.GroupMessage.content)
  return content_;
}
inline ::std::string* GroupMessage::release_content() {
  clear_has_content();
  if (content_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = content_;
    content_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void GroupMessage::set_allocated_content(::std::string* content) {
  if (content_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete content_;
  }
  if (content) {
    set_has_content();
    content_ = content;
  } else {
    clear_has_content();
    content_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:group.GroupMessage.content)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace group

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::group::HandleGroup_OperateType> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::group::HandleGroup_OperateType>() {
  return ::group::HandleGroup_OperateType_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_group_2eproto__INCLUDED
