#include <node.h>
#include "Error.h"
#include "StrDict.h"
#include "ClientUser.h"

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::Local;
using v8::Int32;

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
    Nan::SetPrototypeMethod(tpl, "HandleError", HandleError);
    Nan::SetPrototypeMethod(tpl, "Message", Message);
    Nan::SetPrototypeMethod(tpl, "OutputError", OutputError);
    Nan::SetPrototypeMethod(tpl, "OutputInfo", OutputInfo);
    Nan::SetPrototypeMethod(tpl, "OutputBinary", OutputBinary);
    Nan::SetPrototypeMethod(tpl, "OutputText", OutputText);
    Nan::SetPrototypeMethod(tpl, "OutputStat", OutputStat);

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

  NAN_METHOD(ClientUser::HandleError) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("HandleError requires at least 1 argument");
    }

    if (!info[0]->IsObject()) {
      return Nan::ThrowTypeError("First argument must be an Error object");
    }

    Local<Object> errorObj = Nan::To<Object>(info[0]).ToLocalChecked();
    Error* error = ObjectWrap::Unwrap<Error>(errorObj);

    ClientUser* ui = ObjectWrap::Unwrap<ClientUser>(info.This());
    ui->_obj->HandleError(error->Unwrap());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientUser::Message) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("Message requires at least 1 argument");
    }

    if (!info[0]->IsObject()) {
      return Nan::ThrowTypeError("First argument must be an Error object");
    }

    Local<Object> errorObj = Nan::To<Object>(info[0]).ToLocalChecked();
    Error* error = ObjectWrap::Unwrap<Error>(errorObj);

    ClientUser* ui = ObjectWrap::Unwrap<ClientUser>(info.This());
    ui->_obj->Message(error->Unwrap());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientUser::OutputError) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("OutputError requires at least 1 argument");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    Nan::Utf8String* charErrBuf = new Nan::Utf8String(info[0]->ToString());
    ClientUser* ui = ObjectWrap::Unwrap<ClientUser>(info.This());
    ui->_obj->OutputError(charErrBuf->operator*());
    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientUser::OutputInfo) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
      return Nan::ThrowTypeError("OutputInfo requires at least 2 arguments");
    }

    if (!info[0]->IsNumber()) {
      return Nan::ThrowTypeError("First argument must be a number");
    }

    if (!info[1]->IsString()) {
      return Nan::ThrowTypeError("Second argument must be a string");
    }

    Local<Int32> level = Nan::To<Int32>(info[0]).ToLocalChecked();
    Nan::Utf8String* charData = new Nan::Utf8String(info[1]->ToString());

    ClientUser* ui = ObjectWrap::Unwrap<ClientUser>(info.This());
    ui->_obj->OutputInfo(level->Value(), charData->operator*());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientUser::OutputBinary) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
      return Nan::ThrowTypeError("OutputBinary requires at least 2 arguments");
    }

    if (!node::Buffer::HasInstance(info[0])) {
      return Nan::ThrowTypeError("First argument must be a buffer");
    }

    if (!info[1]->IsNumber()) {
      return Nan::ThrowTypeError("Second argument must be a number");
    }

    char* charData = node::Buffer::Data(info[0]);
    Local<Int32> lenInt32 = Nan::To<Int32>(info[1]).ToLocalChecked();

    ClientUser* ui = ObjectWrap::Unwrap<ClientUser>(info.This());
    ui->_obj->OutputBinary(charData, lenInt32->Value());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientUser::OutputText) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
      return Nan::ThrowTypeError("OutputText requires at least 2 arguments");
    }

    if (!node::Buffer::HasInstance(info[0])) {
      return Nan::ThrowTypeError("First argument must be a buffer");
    }

    if (!info[1]->IsNumber()) {
      return Nan::ThrowTypeError("Second argument must be a number");
    }

    char* charData = node::Buffer::Data(info[0]);
    Local<Int32> lenInt32 = Nan::To<Int32>(info[1]).ToLocalChecked();

    ClientUser* ui = ObjectWrap::Unwrap<ClientUser>(info.This());
    ui->_obj->OutputText(charData, lenInt32->Value());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientUser::OutputStat) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("OutputStat requires at least 1 argument");
    }

    if (!info[0]->IsObject()) {
      return Nan::ThrowTypeError("First argument must be an object");
    }

    Local<Object> varListObj = info[0]->ToObject();
    StrDict* strDict = ObjectWrap::Unwrap<StrDict>(varListObj);

    ClientUser* ui = ObjectWrap::Unwrap<ClientUser>(info.This());
    ui->_obj->OutputStat(strDict->Unwrap());

    info.GetReturnValue().Set(Nan::Undefined());
  }
}
