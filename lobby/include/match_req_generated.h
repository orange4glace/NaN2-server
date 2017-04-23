// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_MATCHREQ_NAN2_LOBBY_H_
#define FLATBUFFERS_GENERATED_MATCHREQ_NAN2_LOBBY_H_

#include "flatbuffers/flatbuffers.h"

namespace nan2 {
namespace lobby {

struct MatchReq;

enum M_MODE {
  M_MODE_DEATH = 0,
  M_MODE_MIN = M_MODE_DEATH,
  M_MODE_MAX = M_MODE_DEATH
};

inline const char **EnumNamesM_MODE() {
  static const char *names[] = {
    "DEATH",
    nullptr
  };
  return names;
}

inline const char *EnumNameM_MODE(M_MODE e) {
  const size_t index = static_cast<int>(e);
  return EnumNamesM_MODE()[index];
}

struct MatchReq FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_MODE = 4
  };
  M_MODE mode() const {
    return static_cast<M_MODE>(GetField<int8_t>(VT_MODE, 0));
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int8_t>(verifier, VT_MODE) &&
           verifier.EndTable();
  }
};

struct MatchReqBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_mode(M_MODE mode) {
    fbb_.AddElement<int8_t>(MatchReq::VT_MODE, static_cast<int8_t>(mode), 0);
  }
  MatchReqBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  MatchReqBuilder &operator=(const MatchReqBuilder &);
  flatbuffers::Offset<MatchReq> Finish() {
    const auto end = fbb_.EndTable(start_, 1);
    auto o = flatbuffers::Offset<MatchReq>(end);
    return o;
  }
};

inline flatbuffers::Offset<MatchReq> CreateMatchReq(
    flatbuffers::FlatBufferBuilder &_fbb,
    M_MODE mode = M_MODE_DEATH) {
  MatchReqBuilder builder_(_fbb);
  builder_.add_mode(mode);
  return builder_.Finish();
}

inline const nan2::lobby::MatchReq *GetMatchReq(const void *buf) {
  return flatbuffers::GetRoot<nan2::lobby::MatchReq>(buf);
}

inline bool VerifyMatchReqBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<nan2::lobby::MatchReq>(nullptr);
}

inline void FinishMatchReqBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<nan2::lobby::MatchReq> root) {
  fbb.Finish(root);
}

}  // namespace lobby
}  // namespace nan2

#endif  // FLATBUFFERS_GENERATED_MATCHREQ_NAN2_LOBBY_H_