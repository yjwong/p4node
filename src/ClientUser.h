#ifndef P4NODE_CLIENT_USER_H
#define P4NODE_CLIENT_USER_H

#include <node.h>
#include <node_object_wrap.h>
#include "nan.h"
#include "p4/clientapi.h"

namespace p4node {
  class ClientUser : public node::ObjectWrap {
  public:
    static Nan::Persistent<v8::FunctionTemplate> constructor_template;
    static void Init(v8::Handle<v8::Object> exports);
    ::ClientUser* Unwrap();

  private:
    static NAN_METHOD(New);

    explicit ClientUser();
    ~ClientUser();

    ::ClientUser* _obj;
  };
}

#endif /* P4NODE_CLIENT_USER_H */
