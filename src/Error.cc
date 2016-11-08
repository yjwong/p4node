#include <node.h>
#include "ErrorId.h"
#include "StrDict.h"
#include "Error.h"

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::Local;
using v8::String;
using v8::Number;
using v8::Int32;
using v8::Function;
using v8::Value;
using v8::External;

namespace p4node {
  Nan::Persistent<FunctionTemplate> Error::constructor_template;

  Error::Error() {
    _obj = new ::Error();
  }

  Error::~Error() {
    delete _obj;
  }

  ::Error* Error::Unwrap() {
    return _obj;
  }

  void Error::Init(Handle<Object> exports) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("Error").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(2);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "Clear", Clear);
    Nan::SetPrototypeMethod(tpl, "Dump", Dump);
    Nan::SetPrototypeMethod(tpl, "FmtSeverity", FmtSeverity);
    Nan::SetPrototypeMethod(tpl, "GetGeneric", GetGeneric);
    Nan::SetPrototypeMethod(tpl, "GetSeverity", GetSeverity);
    Nan::SetPrototypeMethod(tpl, "IsError", IsError);
    Nan::SetPrototypeMethod(tpl, "IsFatal", IsFatal);
    Nan::SetPrototypeMethod(tpl, "IsWarning", IsWarning);
    Nan::SetPrototypeMethod(tpl, "IsInfo", IsInfo);
    Nan::SetPrototypeMethod(tpl, "Net", Net);
    Nan::SetPrototypeMethod(tpl, "Test", Test);

    Nan::SetPrototypeMethod(tpl, "GetErrorCount", GetErrorCount);
    Nan::SetPrototypeMethod(tpl, "LimitErrorCount", LimitErrorCount);

    Nan::SetPrototypeMethod(tpl, "GetId", GetId);
    Nan::SetPrototypeMethod(tpl, "CheckId", CheckId);
    Nan::SetPrototypeMethod(tpl, "GetDict", GetDict);

    constructor_template.Reset(tpl);
    exports->Set(Nan::New("Error").ToLocalChecked(), tpl->GetFunction());
  }

  NAN_METHOD(Error::New) {
    Nan::HandleScope scope;

    if (!info.IsConstructCall()) {
      return Nan::ThrowTypeError("Use the new operator to create new Error objects");
    }

    Error* client = new Error();
    client->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  }

  NAN_METHOD(Error::Clear) {
    Nan::HandleScope scope;
    Error* obj = ObjectWrap::Unwrap<Error>(info.This());
    obj->_obj->Clear();
    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(Error::Dump) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("Dump requires at least 1 argument");
    }

    Error* obj = ObjectWrap::Unwrap<Error>(info.This());
    Local<String> trace = info[0]->ToString();

    Nan::Utf8String* charTrace = new Nan::Utf8String(trace);
    obj->_obj->Dump(charTrace->operator*());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(Error::FmtSeverity) {
    Nan::HandleScope scope;
    Error* obj = ObjectWrap::Unwrap<Error>(info.This());
    info.GetReturnValue().Set(Nan::New(obj->_obj->FmtSeverity()).ToLocalChecked());
  }

  NAN_METHOD(Error::GetGeneric) {
    Nan::HandleScope scope;
    Error* obj = ObjectWrap::Unwrap<Error>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(obj->_obj->GetGeneric()));
  }

  NAN_METHOD(Error::GetSeverity) {
    Nan::HandleScope scope;
    Error* obj = ObjectWrap::Unwrap<Error>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(obj->_obj->GetSeverity()));
  }

  NAN_METHOD(Error::IsError) {
    Nan::HandleScope scope;
    Error* obj = ObjectWrap::Unwrap<Error>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(obj->_obj->IsError()));
  }

  NAN_METHOD(Error::IsFatal) {
    Nan::HandleScope scope;
    Error* obj = ObjectWrap::Unwrap<Error>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(obj->_obj->IsFatal()));
  }

  NAN_METHOD(Error::IsWarning) {
    Nan::HandleScope scope;
    Error* obj = ObjectWrap::Unwrap<Error>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(obj->_obj->IsWarning()));
  }

  NAN_METHOD(Error::IsInfo) {
    Nan::HandleScope scope;
    Error* obj = ObjectWrap::Unwrap<Error>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(obj->_obj->IsInfo()));
  }

  NAN_METHOD(Error::Net) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
      return Nan::ThrowTypeError("Net requires at least 2 arguments");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    if (!info[1]->IsString()) {
      return Nan::ThrowTypeError("Second argument must be a string");
    }

    Nan::Utf8String* charOperation = new Nan::Utf8String(info[0]->ToString());
    Nan::Utf8String* charArg = new Nan::Utf8String(info[1]->ToString());

    Error* obj = ObjectWrap::Unwrap<Error>(info.This());
    obj->_obj->Net(charOperation->operator*(), charArg->operator*());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(Error::Set) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
      return Nan::ThrowTypeError("Set requires at least 2 arguments");
    }

    if (!info[0]->IsNumber()) {
      return Nan::ThrowTypeError("First argument must be a number");
    }

    if (!info[1]->IsString()) {
      return Nan::ThrowTypeError("Second argument must be a string");
    }

    Local<Int32> errorSeverity = Nan::To<Int32>(info[0]).ToLocalChecked();
    Nan::Utf8String* charFmt = new Nan::Utf8String(info[1]->ToString());

    Error* obj = ObjectWrap::Unwrap<Error>(info.This());
    obj->_obj->Set(static_cast<::ErrorSeverity>(errorSeverity->Value()), charFmt->operator*());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(Error::Test) {
    Nan::HandleScope scope;
    Error* obj = ObjectWrap::Unwrap<Error>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(obj->_obj->Test()));
  }

  NAN_METHOD(Error::GetErrorCount) {
    Nan::HandleScope scope;
    Error* obj = ObjectWrap::Unwrap<Error>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(obj->_obj->GetErrorCount()));
  }

  NAN_METHOD(Error::LimitErrorCount) {
    Nan::HandleScope scope;
    Error* obj = ObjectWrap::Unwrap<Error>(info.This());
    obj->_obj->LimitErrorCount();
    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(Error::GetId) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("GetId requires at least 1 argument");
    }

    if (!info[0]->IsInt32()) {
      return Nan::ThrowTypeError("First argument must be an integer");
    }

    Local<Int32> id = Nan::To<Int32>(info[0]).ToLocalChecked();

    Error* obj = ObjectWrap::Unwrap<Error>(info.This());
    ::ErrorId* errorId = obj->_obj->GetId(id->Value());

    // Convert ErrorId object to managed ErrorId.
    Local<FunctionTemplate> tpl = Nan::New(ErrorId::constructor_template);
    Local<Function> func = tpl->GetFunction();
    Handle<Value> funcArgs[] = { Nan::New<External>(errorId) };
    Local<Object> newObj = Nan::NewInstance(func, 1, funcArgs).ToLocalChecked();

    info.GetReturnValue().Set(newObj);
  }

  NAN_METHOD(Error::CheckId) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("CheckId requires at least 1 argument");
    }

    if (!info[0]->IsObject()) {
      return Nan::ThrowTypeError("First argumennt must be an ErrorId object");
    }

    Local<Object> idObj = info[0]->ToObject();
    ErrorId* id = ObjectWrap::Unwrap<ErrorId>(idObj);
    
    Error* obj = ObjectWrap::Unwrap<Error>(info.This());
    int retval = obj->_obj->CheckId(*id->Unwrap());
    info.GetReturnValue().Set(Nan::New<Number>(retval));
  }

  NAN_METHOD(Error::GetDict) {
    Nan::HandleScope scope;

    Error* obj = ObjectWrap::Unwrap<Error>(info.This());
    ::StrDict* strDict = obj->_obj->GetDict();

    // Convert StrDict object to managed StrDict.
    Local<FunctionTemplate> tpl = Nan::New(StrDict::constructor_template);
    Local<Function> func = tpl->GetFunction();
    Handle<Value> funcArgs[] = { Nan::New<External>(strDict) };
    Local<Object> newObj = Nan::NewInstance(func, 1, funcArgs).ToLocalChecked();

    info.GetReturnValue().Set(newObj);
  }
}
