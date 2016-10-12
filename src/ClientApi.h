#ifndef P4NODE_CLIENT_API_H
#define P4NODE_CLIENT_API_H

#include <node.h>
#include <node_object_wrap.h>
#include "nan.h"
#include "p4/clientapi.h"

namespace p4node {
  class ClientApi : public node::ObjectWrap {
  public:
    static Nan::Persistent<v8::FunctionTemplate> constructor_template;
    static void Init(v8::Handle<v8::Object> exports);

  private:
    static NAN_METHOD(New);
    static NAN_METHOD(DefineClient);
    static NAN_METHOD(DefineHost);
    static NAN_METHOD(DefineIgnoreFile);
    static NAN_METHOD(DefinePassword);
    static NAN_METHOD(DefinePort);
    static NAN_METHOD(DefineUser);
    static NAN_METHOD(Dropped);
    static NAN_METHOD(Init);
    static NAN_METHOD(Run);
    static NAN_METHOD(Final);
    static NAN_METHOD(SetClient);
    static NAN_METHOD(SetCwd);
    static NAN_METHOD(SetHost);
    static NAN_METHOD(SetIgnoreFile);
    static NAN_METHOD(SetPassword);
    static NAN_METHOD(SetPort);
    static NAN_METHOD(SetProg);
    static NAN_METHOD(SetProtocol);
    static NAN_METHOD(SetProtocolV);
    static NAN_METHOD(SetUi);
    static NAN_METHOD(SetUser);
    static NAN_METHOD(SetVersion);

    static NAN_METHOD(GetCharset);
    static NAN_METHOD(GetClient);
    static NAN_METHOD(GetClientNoHost);
    static NAN_METHOD(GetCwd);
    static NAN_METHOD(GetExecutable);
    static NAN_METHOD(GetHost);
    static NAN_METHOD(GetIgnoreFile);
    static NAN_METHOD(GetLanguage);
    static NAN_METHOD(GetOs);
    static NAN_METHOD(GetPassword);
    static NAN_METHOD(GetPort);
    static NAN_METHOD(GetUser);
    static NAN_METHOD(GetConfig);
    static NAN_METHOD(GetBuild);

    explicit ClientApi();
    ~ClientApi();

    ::ClientApi* _obj;
  };
}

#endif /* P4NODE_CLIENT_API_H */
