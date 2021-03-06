// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_JOINANS_NAN2_LOBBY_H_
#define FLATBUFFERS_GENERATED_JOINANS_NAN2_LOBBY_H_

#include "flatbuffers/flatbuffers.h"

namespace nan2 {
namespace lobby {

struct JoinAns;

enum J_ANS {
  J_ANS_SUCC = 0,
  J_ANS_FAIL = 1,
  J_ANS_FAIL_UNAUTH = 2,
  J_ANS_MIN = J_ANS_SUCC,
  J_ANS_MAX = J_ANS_FAIL_UNAUTH
};

inline const char **EnumNamesJ_ANS() {
  static const char *names[] = {
    "SUCC",
    "FAIL",
    "FAIL_UNAUTH",
    nullptr
  };
  return names;
}

inline const char *EnumNameJ_ANS(J_ANS e) {
  const size_t index = static_cast<int>(e);
  return EnumNamesJ_ANS()[index];
}

struct JoinAns FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_ANS = 4
  };
  J_ANS ans() const {
    return static_cast<J_ANS>(GetField<int8_t>(VT_ANS, 0));
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int8_t>(verifier, VT_ANS) &&
           verifier.EndTable();
  }
};

struct JoinAnsBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_ans(J_ANS ans) {
    fbb_.AddElement<int8_t>(JoinAns::VT_ANS, static_cast<int8_t>(ans), 0);
  }
  JoinAnsBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  JoinAnsBuilder &operator=(const JoinAnsBuilder &);
  flatbuffers::Offset<JoinAns> Finish() {
    const auto end = fbb_.EndTable(start_, 1);
    auto o = flatbuffers::Offset<JoinAns>(end);
    return o;
  }
};

inline flatbuffers::Offset<JoinAns> CreateJoinAns(
    flatbuffers::FlatBufferBuilder &_fbb,
    J_ANS ans = J_ANS_SUCC) {
  JoinAnsBuilder builder_(_fbb);
  builder_.add_ans(ans);
  return builder_.Finish();
}

inline const nan2::lobby::JoinAns *GetJoinAns(const void *buf) {
  return flatbuffers::GetRoot<nan2::lobby::JoinAns>(buf);
}

inline bool VerifyJoinAnsBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<nan2::lobby::JoinAns>(nullptr);
}

inline void FinishJoinAnsBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<nan2::lobby::JoinAns> root) {
  fbb.Finish(root);
}

}  // namespace lobby
}  // namespace nan2

#endif  // FLATBUFFERS_GENERATED_JOINANS_NAN2_LOBBY_H_
