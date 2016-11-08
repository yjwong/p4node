#include "ErrorId.h"

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::Local;
using v8::Number;

namespace p4node {
  Nan::Persistent<FunctionTemplate> ErrorId::constructor_template;

  ErrorId::ErrorId() {
    _obj = new ::ErrorId();
  }

  ErrorId::~ErrorId() {
    delete _obj;
  }

  void ErrorId::Init(Handle<Object> exports) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->Inherit(Nan::New(ErrorId::constructor_template));
    tpl->SetClassName(Nan::New("ErrorId").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "SubCode", SubCode);
    Nan::SetPrototypeMethod(tpl, "Subsystem", Subsystem);
    Nan::SetPrototypeMethod(tpl, "Generic", Generic);
    Nan::SetPrototypeMethod(tpl, "ArgCount", ArgCount);
    Nan::SetPrototypeMethod(tpl, "Severity", Severity);
    Nan::SetPrototypeMethod(tpl, "UniqueCode", UniqueCode);

    constructor_template.Reset(tpl);
    exports->Set(Nan::New("ErrorId").ToLocalChecked(), tpl->GetFunction());
  }

  NAN_METHOD(ErrorId::New) {
    Nan::HandleScope scope;

    if (!info.IsConstructCall()) {
      return Nan::ThrowTypeError("Use the new operator to create new ErrorId objects");
    }

    ErrorId* errorId = new ErrorId();
    errorId->Wrap(info.This());

    Nan::SetAccessor(info.This(), Nan::New("code").ToLocalChecked(), Code);
    Nan::SetAccessor(info.This(), Nan::New("fmt").ToLocalChecked(), Fmt);

    info.GetReturnValue().Set(info.This());
  }

  NAN_METHOD(ErrorId::SubCode) {
    Nan::HandleScope scope;
    ErrorId* errorId = ObjectWrap::Unwrap<ErrorId>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(errorId->_obj->SubCode()));
  }

  NAN_METHOD(ErrorId::Subsystem) {
    Nan::HandleScope scope;
    ErrorId* errorId = ObjectWrap::Unwrap<ErrorId>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(errorId->_obj->Subsystem()));
  }

  NAN_METHOD(ErrorId::Generic) {
    Nan::HandleScope scope;
    ErrorId* errorId = ObjectWrap::Unwrap<ErrorId>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(errorId->_obj->Generic()));
  }

  NAN_METHOD(ErrorId::ArgCount) {
    Nan::HandleScope scope;
    ErrorId* errorId = ObjectWrap::Unwrap<ErrorId>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(errorId->_obj->ArgCount()));
  }

  NAN_METHOD(ErrorId::Severity) {
    Nan::HandleScope scope;
    ErrorId* errorId = ObjectWrap::Unwrap<ErrorId>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(errorId->_obj->Severity()));
  }

  NAN_METHOD(ErrorId::UniqueCode) {
    Nan::HandleScope scope;
    ErrorId* errorId = ObjectWrap::Unwrap<ErrorId>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(errorId->_obj->UniqueCode()));
  }

  NAN_GETTER(ErrorId::Code) {
    Nan::HandleScope scope;
    ErrorId* errorId = ObjectWrap::Unwrap<ErrorId>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(errorId->_obj->code));
  }

  NAN_GETTER(ErrorId::Fmt) {
    Nan::HandleScope scope;
    ErrorId* errorId = ObjectWrap::Unwrap<ErrorId>(info.This());
    info.GetReturnValue().Set(Nan::New(errorId->_obj->fmt).ToLocalChecked());
  }
}
