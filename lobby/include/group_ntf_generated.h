// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_GROUPNTF_NAN2_LOBBY_H_
#define FLATBUFFERS_GENERATED_GROUPNTF_NAN2_LOBBY_H_

#include "flatbuffers/flatbuffers.h"

namespace nan2 {
namespace lobby {

struct GroupNtf;

enum G_NTF {
  G_NTF_JOIN = 0,
  G_NTF_JOIN_RQ = 1,
  G_NTF_JOIN_NEW = 2,
  G_NTF_JOIN_SC = 3,
  G_NTF_OUT = 4,
  G_NTF_MIN = G_NTF_JOIN,
  G_NTF_MAX = G_NTF_OUT
};

inline const char **EnumNamesG_NTF() {
  static const char *names[] = {
    "JOIN",
    "JOIN_RQ",
    "JOIN_NEW",
    "JOIN_SC",
    "OUT",
    nullptr
  };
  return names;
}

inline const char *EnumNameG_NTF(G_NTF e) {
  const size_t index = static_cast<int>(e);
  return EnumNamesG_NTF()[index];
}

struct GroupNtf FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_NTF = 4,
    VT_NTF_ID = 6,
    VT_USER_TAGS = 8
  };
  G_NTF ntf() const {
    return static_cast<G_NTF>(GetField<int8_t>(VT_NTF, 0));
  }
  int32_t ntf_id() const {
    return GetField<int32_t>(VT_NTF_ID, 0);
  }
  const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *user_tags() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *>(VT_USER_TAGS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int8_t>(verifier, VT_NTF) &&
           VerifyField<int32_t>(verifier, VT_NTF_ID) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_USER_TAGS) &&
           verifier.Verify(user_tags()) &&
           verifier.VerifyVectorOfStrings(user_tags()) &&
           verifier.EndTable();
  }
};

struct GroupNtfBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_ntf(G_NTF ntf) {
    fbb_.AddElement<int8_t>(GroupNtf::VT_NTF, static_cast<int8_t>(ntf), 0);
  }
  void add_ntf_id(int32_t ntf_id) {
    fbb_.AddElement<int32_t>(GroupNtf::VT_NTF_ID, ntf_id, 0);
  }
  void add_user_tags(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> user_tags) {
    fbb_.AddOffset(GroupNtf::VT_USER_TAGS, user_tags);
  }
  GroupNtfBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  GroupNtfBuilder &operator=(const GroupNtfBuilder &);
  flatbuffers::Offset<GroupNtf> Finish() {
    const auto end = fbb_.EndTable(start_, 3);
    auto o = flatbuffers::Offset<GroupNtf>(end);
    return o;
  }
};

inline flatbuffers::Offset<GroupNtf> CreateGroupNtf(
    flatbuffers::FlatBufferBuilder &_fbb,
    G_NTF ntf = G_NTF_JOIN,
    int32_t ntf_id = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> user_tags = 0) {
  GroupNtfBuilder builder_(_fbb);
  builder_.add_user_tags(user_tags);
  builder_.add_ntf_id(ntf_id);
  builder_.add_ntf(ntf);
  return builder_.Finish();
}

inline flatbuffers::Offset<GroupNtf> CreateGroupNtfDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    G_NTF ntf = G_NTF_JOIN,
    int32_t ntf_id = 0,
    const std::vector<flatbuffers::Offset<flatbuffers::String>> *user_tags = nullptr) {
  return nan2::lobby::CreateGroupNtf(
      _fbb,
      ntf,
      ntf_id,
      user_tags ? _fbb.CreateVector<flatbuffers::Offset<flatbuffers::String>>(*user_tags) : 0);
}

inline const nan2::lobby::GroupNtf *GetGroupNtf(const void *buf) {
  return flatbuffers::GetRoot<nan2::lobby::GroupNtf>(buf);
}

inline bool VerifyGroupNtfBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<nan2::lobby::GroupNtf>(nullptr);
}

inline void FinishGroupNtfBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<nan2::lobby::GroupNtf> root) {
  fbb.Finish(root);
}

}  // namespace lobby
}  // namespace nan2

#endif  // FLATBUFFERS_GENERATED_GROUPNTF_NAN2_LOBBY_H_
