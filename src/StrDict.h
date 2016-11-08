#ifndef P4NODE_STR_DICT_H
#define P4NODE_STR_DICT_H

#include <node.h>
#include <node_object_wrap.h>
#include "nan.h"
#include "p4/clientapi.h"

namespace p4node {
  class StrDict : public node::ObjectWrap {
  friend class ClientApi;
  public:
    static Nan::Persistent<v8::FunctionTemplate> constructor_template;

    explicit StrDict(::StrDict* obj);
    ~StrDict();

    static void Init(v8::Handle<v8::Object> exports);
    ::StrDict* Unwrap();

  private:
    static NAN_METHOD(New);

    static NAN_METHOD(SetVar);
    static NAN_METHOD(SetArgv);

    static NAN_METHOD(GetVar);

    ::StrDict* _obj;
  };
}

#endif /* P4NODE_STR_DICT_H */
