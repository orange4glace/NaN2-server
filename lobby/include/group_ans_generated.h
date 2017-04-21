// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_GROUPANS_NAN2_LOBBY_H_
#define FLATBUFFERS_GENERATED_GROUPANS_NAN2_LOBBY_H_

#include "flatbuffers/flatbuffers.h"

namespace nan2 {
namespace lobby {

struct GroupAns;

enum G_ANS {
  G_ANS_SUCC = 0,
  G_ANS_FAIL = 1,
  G_ANS_MIN = G_ANS_SUCC,
  G_ANS_MAX = G_ANS_FAIL
};

inline const char **EnumNamesG_ANS() {
  static const char *names[] = {
    "SUCC",
    "FAIL",
    nullptr
  };
  return names;
}

inline const char *EnumNameG_ANS(G_ANS e) {
  const size_t index = static_cast<int>(e);
  return EnumNamesG_ANS()[index];
}

struct GroupAns FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_ANS = 4
  };
  G_ANS ans() const {
    return static_cast<G_ANS>(GetField<int8_t>(VT_ANS, 0));
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int8_t>(verifier, VT_ANS) &&
           verifier.EndTable();
  }
};

struct GroupAnsBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_ans(G_ANS ans) {
    fbb_.AddElement<int8_t>(GroupAns::VT_ANS, static_cast<int8_t>(ans), 0);
  }
  GroupAnsBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  GroupAnsBuilder &operator=(const GroupAnsBuilder &);
  flatbuffers::Offset<GroupAns> Finish() {
    const auto end = fbb_.EndTable(start_, 1);
    auto o = flatbuffers::Offset<GroupAns>(end);
    return o;
  }
};

inline flatbuffers::Offset<GroupAns> CreateGroupAns(
    flatbuffers::FlatBufferBuilder &_fbb,
    G_ANS ans = G_ANS_SUCC) {
  GroupAnsBuilder builder_(_fbb);
  builder_.add_ans(ans);
  return builder_.Finish();
}

inline const nan2::lobby::GroupAns *GetGroupAns(const void *buf) {
  return flatbuffers::GetRoot<nan2::lobby::GroupAns>(buf);
}

inline bool VerifyGroupAnsBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<nan2::lobby::GroupAns>(nullptr);
}

inline void FinishGroupAnsBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<nan2::lobby::GroupAns> root) {
  fbb.Finish(root);
}

}  // namespace lobby
}  // namespace nan2

#endif  // FLATBUFFERS_GENERATED_GROUPANS_NAN2_LOBBY_H_
