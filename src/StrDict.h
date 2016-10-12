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
    static void Init(v8::Handle<v8::Object> exports);

  private:
    static NAN_METHOD(New);
    static NAN_METHOD(SetArgv);

    explicit StrDict(::StrDict* obj);
    ~StrDict();

    ::StrDict* _obj;
  };
}

#endif /* P4NODE_STR_DICT_H */
