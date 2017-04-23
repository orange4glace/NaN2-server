// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_MATCHNTF_NAN2_LOBBY_H_
#define FLATBUFFERS_GENERATED_MATCHNTF_NAN2_LOBBY_H_

#include "flatbuffers/flatbuffers.h"

namespace nan2 {
namespace lobby {

struct MatchNtf;

enum M_NTF {
  M_NTF_MATCHED = 0,
  M_NTF_FAIL = 1,
  M_NTF_MIN = M_NTF_MATCHED,
  M_NTF_MAX = M_NTF_FAIL
};

inline const char **EnumNamesM_NTF() {
  static const char *names[] = {
    "MATCHED",
    "FAIL",
    nullptr
  };
  return names;
}

inline const char *EnumNameM_NTF(M_NTF e) {
  const size_t index = static_cast<int>(e);
  return EnumNamesM_NTF()[index];
}

struct MatchNtf FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_NTF = 4
  };
  M_NTF ntf() const {
    return static_cast<M_NTF>(GetField<int8_t>(VT_NTF, 0));
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int8_t>(verifier, VT_NTF) &&
           verifier.EndTable();
  }
};

struct MatchNtfBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_ntf(M_NTF ntf) {
    fbb_.AddElement<int8_t>(MatchNtf::VT_NTF, static_cast<int8_t>(ntf), 0);
  }
  MatchNtfBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  MatchNtfBuilder &operator=(const MatchNtfBuilder &);
  flatbuffers::Offset<MatchNtf> Finish() {
    const auto end = fbb_.EndTable(start_, 1);
    auto o = flatbuffers::Offset<MatchNtf>(end);
    return o;
  }
};

inline flatbuffers::Offset<MatchNtf> CreateMatchNtf(
    flatbuffers::FlatBufferBuilder &_fbb,
    M_NTF ntf = M_NTF_MATCHED) {
  MatchNtfBuilder builder_(_fbb);
  builder_.add_ntf(ntf);
  return builder_.Finish();
}

inline const nan2::lobby::MatchNtf *GetMatchNtf(const void *buf) {
  return flatbuffers::GetRoot<nan2::lobby::MatchNtf>(buf);
}

inline bool VerifyMatchNtfBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<nan2::lobby::MatchNtf>(nullptr);
}

inline void FinishMatchNtfBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<nan2::lobby::MatchNtf> root) {
  fbb.Finish(root);
}

}  // namespace lobby
}  // namespace nan2

#endif  // FLATBUFFERS_GENERATED_MATCHNTF_NAN2_LOBBY_H_