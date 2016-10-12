#include <node.h>
#include "ClientUser.h"

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::Local;

namespace p4node {
  Nan::Persistent<FunctionTemplate> ClientUser::constructor_template;

  ClientUser::ClientUser() {
    _obj = new ::ClientUser();
  }

  ClientUser::~ClientUser() {
    delete _obj;
  }

  ::ClientUser* ClientUser::Unwrap() {
    return _obj;
  }

  void ClientUser::Init(Handle<Object> exports) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("ClientUser").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    //Nan::SetPrototypeMethod(tpl, "Init", Init);

    constructor_template.Reset(tpl);
    exports->Set(Nan::New("ClientUser").ToLocalChecked(), tpl->GetFunction());
  }

  NAN_METHOD(ClientUser::New) {
    Nan::HandleScope scope;

    if (!info.IsConstructCall()) {
      return Nan::ThrowTypeError("Use the new operator to create new ClientUser objects");
    }

    ClientUser* client = new ClientUser();
    client->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  }
}
