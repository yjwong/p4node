#include <node.h>
#include "ClientApi.h"
#include "ClientUser.h"
#include "StrDict.h"
#include "Error.h"

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::Local;
using v8::Number;
using v8::String;

namespace p4node {
  Nan::Persistent<FunctionTemplate> ClientApi::constructor_template;

  ClientApi::ClientApi() {
    _obj = new ::ClientApi();
  }

  ClientApi::~ClientApi() {
    delete _obj;
  }

  void ClientApi::Init(Handle<Object> exports) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->Inherit(Nan::New(StrDict::constructor_template));
    tpl->SetClassName(Nan::New("ClientApi").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "SetProtocol", SetProtocol);
    Nan::SetPrototypeMethod(tpl, "SetProtocolV", SetProtocolV);

    Nan::SetPrototypeMethod(tpl, "Init", Init);
    Nan::SetPrototypeMethod(tpl, "Final", Final);
    Nan::SetPrototypeMethod(tpl, "Dropped", Dropped);
    Nan::SetPrototypeMethod(tpl, "GetErrors", GetErrors);
    Nan::SetPrototypeMethod(tpl, "GetTrans", GetTrans);
    Nan::SetPrototypeMethod(tpl, "IsUnicode", IsUnicode);

    Nan::SetPrototypeMethod(tpl, "RunTag", RunTag);
    Nan::SetPrototypeMethod(tpl, "WaitTag", WaitTag);

    Nan::SetPrototypeMethod(tpl, "SetCharset", SetCharset);
    Nan::SetPrototypeMethod(tpl, "SetClient", SetClient);
    Nan::SetPrototypeMethod(tpl, "SetCwd", SetCwd);
    Nan::SetPrototypeMethod(tpl, "SetCwdNoReload", SetCwdNoReload);
    Nan::SetPrototypeMethod(tpl, "SetHost", SetHost);
    Nan::SetPrototypeMethod(tpl, "SetIgnoreFile", SetIgnoreFile);
    Nan::SetPrototypeMethod(tpl, "SetLanguage", SetLanguage);
    Nan::SetPrototypeMethod(tpl, "SetPassword", SetPassword);
    Nan::SetPrototypeMethod(tpl, "SetPort", SetPort);
    Nan::SetPrototypeMethod(tpl, "SetUser", SetUser);
    Nan::SetPrototypeMethod(tpl, "SetProg", SetProg);
    Nan::SetPrototypeMethod(tpl, "SetVersion", SetVersion);
    Nan::SetPrototypeMethod(tpl, "SetTicketFile", SetTicketFile);
    Nan::SetPrototypeMethod(tpl, "SetEnviroFile", SetEnviroFile);

    Nan::SetPrototypeMethod(tpl, "DefineCharset", DefineCharset);
    Nan::SetPrototypeMethod(tpl, "DefineClient", DefineClient);
    Nan::SetPrototypeMethod(tpl, "DefineHost", DefineHost);
    Nan::SetPrototypeMethod(tpl, "DefineIgnoreFile", DefineIgnoreFile);
    Nan::SetPrototypeMethod(tpl, "DefineLanguage", DefineLanguage);
    Nan::SetPrototypeMethod(tpl, "DefinePassword", DefinePassword);
    Nan::SetPrototypeMethod(tpl, "DefinePort", DefinePort);
    Nan::SetPrototypeMethod(tpl, "DefineUser", DefineUser);

    Nan::SetPrototypeMethod(tpl, "GetCharset", GetCharset);
    Nan::SetPrototypeMethod(tpl, "GetClient", GetClient);
    Nan::SetPrototypeMethod(tpl, "GetClientNoHost", GetClientNoHost);
    Nan::SetPrototypeMethod(tpl, "GetCwd", GetCwd);
    Nan::SetPrototypeMethod(tpl, "GetExecutable", GetExecutable);
    Nan::SetPrototypeMethod(tpl, "GetHost", GetHost);
    Nan::SetPrototypeMethod(tpl, "GetIgnoreFile", GetIgnoreFile);
    Nan::SetPrototypeMethod(tpl, "GetLanguage", GetLanguage);
    Nan::SetPrototypeMethod(tpl, "GetOs", GetOs);
    Nan::SetPrototypeMethod(tpl, "GetPassword", GetPassword);
    Nan::SetPrototypeMethod(tpl, "GetPort", GetPort);
    Nan::SetPrototypeMethod(tpl, "GetUser", GetUser);
    Nan::SetPrototypeMethod(tpl, "GetConfig", GetConfig);
    Nan::SetPrototypeMethod(tpl, "GetBuild", GetBuild);

    constructor_template.Reset(tpl);
    exports->Set(Nan::New("ClientApi").ToLocalChecked(), tpl->GetFunction());
  }

  NAN_METHOD(ClientApi::New) {
    Nan::HandleScope scope;

    if (!info.IsConstructCall()) {
      return Nan::ThrowTypeError("Use the new operator to create new ClientApi objects");
    }

    ClientApi* client = new ClientApi();
    StrDict* dict = new StrDict(client->_obj);
    client->Wrap(info.This());
    dict->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  }

  NAN_METHOD(ClientApi::SetProtocol) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
      return Nan::ThrowTypeError("SetProtocol requires at least 2 arguments");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    if (!info[1]->IsString()) {
      return Nan::ThrowTypeError("Second argument must be a string");
    }

    Nan::Utf8String* charVarName = new Nan::Utf8String(info[0]->ToString());
    Nan::Utf8String* charVarValue = new Nan::Utf8String(info[1]->ToString());
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    obj->_obj->SetProtocol(charVarName->operator*(), charVarValue->operator*());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientApi::SetProtocolV) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("SetProtocolV requires at least 1 argument");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    Nan::Utf8String* charVars = new Nan::Utf8String(info[0]->ToString());
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    obj->_obj->SetProtocolV(charVars->operator*());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientApi::Init) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("Init requires at least 1 argument");
    }

    if (info[0]->IsObject()) {
      Local<Object> errorObj = info[0]->ToObject();
      Error* error = ObjectWrap::Unwrap<Error>(errorObj);
      ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
      obj->_obj->Init(error->Unwrap());
    } else {
      return Nan::ThrowTypeError("First argument must be an Error object");
    }

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientApi::Final) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("Final requires at least 1 argument");
    }

    if (!info[0]->IsObject()) {
      return Nan::ThrowTypeError("First argument must be an Error object");
    }

    Local<Object> errorObj = Nan::To<Object>(info[0]).ToLocalChecked();
    Error* error = ObjectWrap::Unwrap<Error>(errorObj);

    ClientApi* client = ObjectWrap::Unwrap<ClientApi>(info.This());

    info.GetReturnValue().Set(Nan::New<Number>(client->_obj->Final(error->Unwrap())));
  }

  NAN_METHOD(ClientApi::Dropped) {
    Nan::HandleScope scope;
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(obj->_obj->Dropped()));
  }

  NAN_METHOD(ClientApi::GetErrors) {
    Nan::HandleScope scope;
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(obj->_obj->GetErrors()));
  }

  NAN_METHOD(ClientApi::GetTrans) {
    Nan::HandleScope scope;
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(obj->_obj->GetTrans()));
  }

  NAN_METHOD(ClientApi::IsUnicode) {
    Nan::HandleScope scope;
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(obj->_obj->IsUnicode()));
  }

  NAN_METHOD(ClientApi::RunTag) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
      return Nan::ThrowTypeError("RunTag requires at least 2 arguments");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    if (!info[1]->IsObject()) {
      return Nan::ThrowTypeError("Second argument must be a ClientUser object");
    }

    Nan::Utf8String* charFunc = new Nan::Utf8String(info[0]->ToString());
    Local<Object> uiObj = Nan::To<Object>(info[1]).ToLocalChecked();

    ClientUser* ui = ObjectWrap::Unwrap<ClientUser>(uiObj);
    ClientApi* client = ObjectWrap::Unwrap<ClientApi>(info.This());
    client->_obj->RunTag(charFunc->operator*(), ui->Unwrap());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientApi::WaitTag) {
    Nan::HandleScope scope;
    ClientApi* client = ObjectWrap::Unwrap<ClientApi>(info.This());

    if (info.Length() >= 1) {
      if (!info[0]->IsObject()) {
          return Nan::ThrowTypeError("First argument must be a ClientUser object");
      }
      Local<Object> uiObj = Nan::To<Object>(info[0]).ToLocalChecked();
      ClientUser* ui = ObjectWrap::Unwrap<ClientUser>(uiObj);
      client->_obj->WaitTag(ui->Unwrap());
    } else {
      client->_obj->WaitTag();
    }

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientApi::SetCharset) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("SetCharset requires at least 1 arguments");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    Nan::Utf8String* charCharset = new Nan::Utf8String(info[0]->ToString());
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    obj->_obj->SetCharset(charCharset->operator*());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientApi::SetClient) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("SetClient requires at least 1 arguments");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    Nan::Utf8String* charClient = new Nan::Utf8String(info[0]->ToString());
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    obj->_obj->SetClient(charClient->operator*());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientApi::SetCwd) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("SetCwd requires at least 1 arguments");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    Nan::Utf8String* charCwd = new Nan::Utf8String(info[0]->ToString());
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    obj->_obj->SetCwd(charCwd->operator*());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientApi::SetCwdNoReload) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("SetCwdNoReload requires at least 1 arguments");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    Nan::Utf8String* charCwd = new Nan::Utf8String(info[0]->ToString());
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    obj->_obj->SetCwdNoReload(charCwd->operator*());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientApi::SetHost) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("SetHost requires at least 1 arguments");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    Nan::Utf8String* charHost = new Nan::Utf8String(info[0]->ToString());
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    obj->_obj->SetHost(charHost->operator*());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientApi::SetIgnoreFile) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("SetIgnoreFile requires at least 1 arguments");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    Nan::Utf8String* charIgnoreFile = new Nan::Utf8String(info[0]->ToString());
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    obj->_obj->SetIgnoreFile(charIgnoreFile->operator*());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientApi::SetLanguage) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("SetLanguage requires at least 1 arguments");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    Nan::Utf8String* charLanguage = new Nan::Utf8String(info[0]->ToString());
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    obj->_obj->SetLanguage(charLanguage->operator*());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientApi::SetPassword) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("SetPassword requires at least 1 arguments");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    Nan::Utf8String* charPassword = new Nan::Utf8String(info[0]->ToString());
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    obj->_obj->SetPassword(charPassword->operator*());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  // TODO: This is currently broken - the symbol is not found in the p4api library.
  NAN_METHOD(ClientApi::SetPort) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("SetPort requires at least 1 arguments");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    Nan::Utf8String* charPort = new Nan::Utf8String(info[0]->ToString());
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    //obj->_obj->SetPort(charPort->operator*());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientApi::SetUser) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("SetUser requires at least 1 argument");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    Nan::Utf8String* charUser = new Nan::Utf8String(info[0]->ToString());
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    obj->_obj->SetUser(charUser->operator*());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientApi::SetProg) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("SetProg requires at least 1 arguments");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    Nan::Utf8String* charProg = new Nan::Utf8String(info[0]->ToString());
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    obj->_obj->SetProg(charProg->operator*());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientApi::SetVersion) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("SetVersion requires at least 1 argument");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    Nan::Utf8String* charVersion = new Nan::Utf8String(info[0]->ToString());
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    obj->_obj->SetVersion(charVersion->operator*());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientApi::SetTicketFile) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("SetTicketFile requires at least 1 argument");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    Nan::Utf8String* charTicketFile = new Nan::Utf8String(info[0]->ToString());
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    obj->_obj->SetTicketFile(charTicketFile->operator*());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientApi::SetEnviroFile) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("SetEnviroFile requires at least 1 argument");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    Nan::Utf8String* charEnviroFile = new Nan::Utf8String(info[0]->ToString());
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    obj->_obj->SetEnviroFile(charEnviroFile->operator*());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientApi::DefineCharset) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
      return Nan::ThrowTypeError("DefineCharset requires at least 2 arguments");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    if (!info[1]->IsObject()) {
      return Nan::ThrowTypeError("Second argument must be an Error object");
    }

    Nan::Utf8String* charCharset = new Nan::Utf8String(info[0]->ToString());
    Local<Object> errorObj = info[1]->ToObject();
    Error* error = ObjectWrap::Unwrap<Error>(errorObj);

    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    obj->_obj->DefineCharset(charCharset->operator*(), error->Unwrap());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientApi::DefineClient) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
      return Nan::ThrowTypeError("DefineClient requires at least 2 arguments");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    if (!info[1]->IsObject()) {
      return Nan::ThrowTypeError("Second argument must be an Error object");
    }

    Nan::Utf8String* charClient = new Nan::Utf8String(info[0]->ToString());
    Local<Object> errorObj = info[1]->ToObject();
    Error* error = ObjectWrap::Unwrap<Error>(errorObj);

    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    obj->_obj->DefineClient(charClient->operator*(), error->Unwrap());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientApi::DefineHost) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
      return Nan::ThrowTypeError("DefineHost requires at least 2 arguments");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    if (!info[1]->IsObject()) {
      return Nan::ThrowTypeError("Second argument must be an Error object");
    }

    Nan::Utf8String* charHost = new Nan::Utf8String(info[0]->ToString());
    Local<Object> errorObj = info[1]->ToObject();
    Error* error = ObjectWrap::Unwrap<Error>(errorObj);

    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    obj->_obj->DefineHost(charHost->operator*(), error->Unwrap());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  // TODO: This is currently broken - the symbol is not found in the p4api library.
  NAN_METHOD(ClientApi::DefineIgnoreFile) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
      return Nan::ThrowTypeError("DefineIgnoreFile requires at least 2 arguments");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    if (!info[1]->IsObject()) {
      return Nan::ThrowTypeError("Second argument must be an Error object");
    }

    Nan::Utf8String* charIgnoreFile = new Nan::Utf8String(info[0]->ToString());
    Local<Object> errorObj = info[1]->ToObject();
    Error* error = ObjectWrap::Unwrap<Error>(errorObj);

    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    //obj->_obj->DefineIgnoreFile(charIgnoreFile->operator*(), error->Unwrap());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientApi::DefineLanguage) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
      return Nan::ThrowTypeError("DefineLanguage requires at least 2 arguments");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    if (!info[1]->IsObject()) {
      return Nan::ThrowTypeError("Second argument must be an Error object");
    }

    Nan::Utf8String* charLanguage = new Nan::Utf8String(info[0]->ToString());
    Local<Object> errorObj = info[1]->ToObject();
    Error* error = ObjectWrap::Unwrap<Error>(errorObj);

    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    obj->_obj->DefineLanguage(charLanguage->operator*(), error->Unwrap());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientApi::DefinePassword) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
      return Nan::ThrowTypeError("DefinePassword requires at least 2 arguments");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    if (!info[1]->IsObject()) {
      return Nan::ThrowTypeError("Second argument must be an Error object");
    }

    Nan::Utf8String* charPassword = new Nan::Utf8String(info[0]->ToString());
    Local<Object> errorObj = info[1]->ToObject();
    Error* error = ObjectWrap::Unwrap<Error>(errorObj);

    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    obj->_obj->DefinePassword(charPassword->operator*(), error->Unwrap());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientApi::DefinePort) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
      return Nan::ThrowTypeError("DefinePort requires at least 2 arguments");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    if (!info[1]->IsObject()) {
      return Nan::ThrowTypeError("Second argument must be an Error object");
    }

    Nan::Utf8String* charPort = new Nan::Utf8String(info[0]->ToString());
    Local<Object> errorObj = info[1]->ToObject();
    Error* error = ObjectWrap::Unwrap<Error>(errorObj);

    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    obj->_obj->DefinePort(charPort->operator*(), error->Unwrap());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientApi::DefineUser) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
      return Nan::ThrowTypeError("DefineUser requires at least 2 arguments");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    if (!info[1]->IsObject()) {
      return Nan::ThrowTypeError("Second argument must be an Error object");
    }

    Nan::Utf8String* charUser = new Nan::Utf8String(info[0]->ToString());
    Local<Object> errorObj = info[1]->ToObject();
    Error* error = ObjectWrap::Unwrap<Error>(errorObj);

    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    obj->_obj->DefineUser(charUser->operator*(), error->Unwrap());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientApi::GetCharset) {
    Nan::HandleScope scope;
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    info.GetReturnValue().Set(Nan::New<String>(obj->_obj->GetCharset().Text()).ToLocalChecked());
  }

  NAN_METHOD(ClientApi::GetClient) {
    Nan::HandleScope scope;
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    info.GetReturnValue().Set(Nan::New<String>(obj->_obj->GetClient().Text()).ToLocalChecked());
  }

  NAN_METHOD(ClientApi::GetClientNoHost) {
    Nan::HandleScope scope;
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    info.GetReturnValue().Set(Nan::New<String>(obj->_obj->GetClientNoHost().Text()).ToLocalChecked());
  }

  NAN_METHOD(ClientApi::GetCwd) {
    Nan::HandleScope scope;
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    info.GetReturnValue().Set(Nan::New<String>(obj->_obj->GetCwd().Text()).ToLocalChecked());
  }

  NAN_METHOD(ClientApi::GetExecutable) {
    Nan::HandleScope scope;
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    info.GetReturnValue().Set(Nan::New<String>(obj->_obj->GetExecutable().Text()).ToLocalChecked());
  }

  NAN_METHOD(ClientApi::GetHost) {
    Nan::HandleScope scope;
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    info.GetReturnValue().Set(Nan::New<String>(obj->_obj->GetHost().Text()).ToLocalChecked());
  }

  NAN_METHOD(ClientApi::GetIgnoreFile) {
    Nan::HandleScope scope;
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    info.GetReturnValue().Set(Nan::New<String>(obj->_obj->GetIgnoreFile().Text()).ToLocalChecked());
  }

  NAN_METHOD(ClientApi::GetLanguage) {
    Nan::HandleScope scope;
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    info.GetReturnValue().Set(Nan::New<String>(obj->_obj->GetLanguage().Text()).ToLocalChecked());
  }

  NAN_METHOD(ClientApi::GetOs) {
    Nan::HandleScope scope;
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    info.GetReturnValue().Set(Nan::New<String>(obj->_obj->GetOs().Text()).ToLocalChecked());
  }

  NAN_METHOD(ClientApi::GetPassword) {
    Nan::HandleScope scope;
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    info.GetReturnValue().Set(Nan::New<String>(obj->_obj->GetPassword().Text()).ToLocalChecked());
  }

  NAN_METHOD(ClientApi::GetPort) {
    Nan::HandleScope scope;
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    info.GetReturnValue().Set(Nan::New<String>(obj->_obj->GetPort().Text()).ToLocalChecked());
  }

  NAN_METHOD(ClientApi::GetUser) {
    Nan::HandleScope scope;
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    info.GetReturnValue().Set(Nan::New<String>(obj->_obj->GetUser().Text()).ToLocalChecked());
  }

  NAN_METHOD(ClientApi::GetConfig) {
    Nan::HandleScope scope;
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    info.GetReturnValue().Set(Nan::New<String>(obj->_obj->GetConfig().Text()).ToLocalChecked());
  }

  NAN_METHOD(ClientApi::GetBuild) {
    Nan::HandleScope scope;
    ClientApi* obj = ObjectWrap::Unwrap<ClientApi>(info.This());
    info.GetReturnValue().Set(Nan::New<String>(obj->_obj->GetBuild().Text()).ToLocalChecked());
  }
}
