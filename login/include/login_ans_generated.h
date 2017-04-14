// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_LOGINANS_NAN2_LOGIN_H_
#define FLATBUFFERS_GENERATED_LOGINANS_NAN2_LOGIN_H_

#include "flatbuffers/flatbuffers.h"

namespace nan2 {
namespace login {

struct LoginAns;

enum L_ANS {
  L_ANS_SUCC = 0,
  L_ANS_FAIL = 1,
  L_ANS_FAIL_UNREG = 2,
  L_ANS_FAIL_LOGINED = 3,
  L_ANS_MIN = L_ANS_SUCC,
  L_ANS_MAX = L_ANS_FAIL_LOGINED
};

inline const char **EnumNamesL_ANS() {
  static const char *names[] = {
    "SUCC",
    "FAIL",
    "FAIL_UNREG",
    "FAIL_LOGINED",
    nullptr
  };
  return names;
}

inline const char *EnumNameL_ANS(L_ANS e) {
  const size_t index = static_cast<int>(e);
  return EnumNamesL_ANS()[index];
}

struct LoginAns FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_ANS = 4,
    VT_USER_TAG = 6,
    VT_TOKEN = 8
  };
  L_ANS ans() const {
    return static_cast<L_ANS>(GetField<int8_t>(VT_ANS, 0));
  }
  const flatbuffers::String *user_tag() const {
    return GetPointer<const flatbuffers::String *>(VT_USER_TAG);
  }
  const flatbuffers::String *token() const {
    return GetPointer<const flatbuffers::String *>(VT_TOKEN);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int8_t>(verifier, VT_ANS) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_USER_TAG) &&
           verifier.Verify(user_tag()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_TOKEN) &&
           verifier.Verify(token()) &&
           verifier.EndTable();
  }
};

struct LoginAnsBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_ans(L_ANS ans) {
    fbb_.AddElement<int8_t>(LoginAns::VT_ANS, static_cast<int8_t>(ans), 0);
  }
  void add_user_tag(flatbuffers::Offset<flatbuffers::String> user_tag) {
    fbb_.AddOffset(LoginAns::VT_USER_TAG, user_tag);
  }
  void add_token(flatbuffers::Offset<flatbuffers::String> token) {
    fbb_.AddOffset(LoginAns::VT_TOKEN, token);
  }
  LoginAnsBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  LoginAnsBuilder &operator=(const LoginAnsBuilder &);
  flatbuffers::Offset<LoginAns> Finish() {
    const auto end = fbb_.EndTable(start_, 3);
    auto o = flatbuffers::Offset<LoginAns>(end);
    return o;
  }
};

inline flatbuffers::Offset<LoginAns> CreateLoginAns(
    flatbuffers::FlatBufferBuilder &_fbb,
    L_ANS ans = L_ANS_SUCC,
    flatbuffers::Offset<flatbuffers::String> user_tag = 0,
    flatbuffers::Offset<flatbuffers::String> token = 0) {
  LoginAnsBuilder builder_(_fbb);
  builder_.add_token(token);
  builder_.add_user_tag(user_tag);
  builder_.add_ans(ans);
  return builder_.Finish();
}

inline flatbuffers::Offset<LoginAns> CreateLoginAnsDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    L_ANS ans = L_ANS_SUCC,
    const char *user_tag = nullptr,
    const char *token = nullptr) {
  return nan2::login::CreateLoginAns(
      _fbb,
      ans,
      user_tag ? _fbb.CreateString(user_tag) : 0,
      token ? _fbb.CreateString(token) : 0);
}

inline const nan2::login::LoginAns *GetLoginAns(const void *buf) {
  return flatbuffers::GetRoot<nan2::login::LoginAns>(buf);
}

inline bool VerifyLoginAnsBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<nan2::login::LoginAns>(nullptr);
}

inline void FinishLoginAnsBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<nan2::login::LoginAns> root) {
  fbb.Finish(root);
}

}  // namespace login
}  // namespace nan2

#endif  // FLATBUFFERS_GENERATED_LOGINANS_NAN2_LOGIN_H_
