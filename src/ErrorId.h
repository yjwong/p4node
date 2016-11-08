#ifndef P4NODE_ERROR_ID_H
#define P4NODE_ERROR_ID_H

#include <node.h>
#include <node_object_wrap.h>
#include "nan.h"
#include "p4/error.h"

namespace p4node {
  class ErrorId : public node::ObjectWrap {
  public:
    static Nan::Persistent<v8::FunctionTemplate> constructor_template;
    static void Init(v8::Handle<v8::Object> exports);
    ::ErrorId* Unwrap();

  private:
    static NAN_METHOD(New);

    static NAN_METHOD(SubCode);
    static NAN_METHOD(Subsystem);
    static NAN_METHOD(Generic);
    static NAN_METHOD(ArgCount);
    static NAN_METHOD(Severity);
    static NAN_METHOD(UniqueCode);

    static NAN_GETTER(Code);
    static NAN_GETTER(Fmt);

    explicit ErrorId();
    ~ErrorId();

    ::ErrorId* _obj;
  };
}

#endif /* P4NODE_ERROR_ID_H */
