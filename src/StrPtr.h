#ifndef P4NODE_STR_PTR_H
#define P4NODE_STR_PTR_H

#include <node.h>
#include <node_object_wrap.h>
#include "nan.h"
#include "p4/clientapi.h"

namespace p4node {
  class StrPtr : public node::ObjectWrap {
  public:
    static Nan::Persistent<v8::FunctionTemplate> constructor_template;
    static void Init(v8::Handle<v8::Object> exports);

  private:
    static NAN_METHOD(New);
    static NAN_METHOD(Text);
    static NAN_METHOD(Length);
    static NAN_METHOD(Atoi);
    static NAN_METHOD(IsNumeric);
    static NAN_METHOD(EndsWith);
    static NAN_METHOD(SetLength);

    explicit StrPtr();
    explicit StrPtr(::StrPtr* obj);
    ~StrPtr();

    ::StrPtr* _obj;
  };
}

#endif /* P4NODE_STR_PTR_H */
