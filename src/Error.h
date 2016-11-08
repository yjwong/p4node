#ifndef P4NODE_ERROR_H
#define P4NODE_ERROR_H

#include <node.h>
#include <node_object_wrap.h>
#include "nan.h"
#include "p4/error.h"

namespace p4node {
  class Error : public node::ObjectWrap {
  public:
    static Nan::Persistent<v8::FunctionTemplate> constructor_template;
    static void Init(v8::Handle<v8::Object> exports);
    ::Error* Unwrap();

  private:
    static NAN_METHOD(New);
    static NAN_METHOD(Clear);
    static NAN_METHOD(Dump);
    //static NAN_METHOD(Fmt);
    static NAN_METHOD(FmtSeverity);
    static NAN_METHOD(GetGeneric);
    static NAN_METHOD(GetSeverity);
    static NAN_METHOD(IsError);
    static NAN_METHOD(IsFatal);
    static NAN_METHOD(IsWarning);
    static NAN_METHOD(IsInfo);
    static NAN_METHOD(Net);
    static NAN_METHOD(Set);
    //static NAN_METHOD(Sys);
    static NAN_METHOD(Test);

    static NAN_METHOD(GetErrorCount);
    static NAN_METHOD(LimitErrorCount);

    static NAN_METHOD(GetId);
    static NAN_METHOD(CheckId);
    static NAN_METHOD(GetDict);

    explicit Error();
    ~Error();

    ::Error* _obj;
  };
}

#endif /* P4NODE_ERROR_H */
