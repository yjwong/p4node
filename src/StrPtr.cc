#include <node.h>
#include "StrPtr.h"

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::Local;
using v8::External;
using v8::Number;
using v8::Int32;

namespace p4node {
  Nan::Persistent<FunctionTemplate> StrPtr::constructor_template;

  StrPtr::StrPtr() {
    _obj = new ::StrPtr();
  }

  StrPtr::StrPtr(::StrPtr* obj) : _obj(obj) {}

  StrPtr::~StrPtr() {
    delete _obj;
  }

  void StrPtr::Init(Handle<Object> exports) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("StrPtr").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "Text", Text);
    Nan::SetPrototypeMethod(tpl, "Value", Text);
    Nan::SetPrototypeMethod(tpl, "Length", Length);
    Nan::SetPrototypeMethod(tpl, "Atoi", Atoi);
    Nan::SetPrototypeMethod(tpl, "IsNumeric", IsNumeric);
    Nan::SetPrototypeMethod(tpl, "EndsWith", EndsWith);
    Nan::SetPrototypeMethod(tpl, "SetLength", SetLength);

    constructor_template.Reset(tpl);
    exports->Set(Nan::New("StrPtr").ToLocalChecked(), tpl->GetFunction());
  }

  NAN_METHOD(StrPtr::New) {
    Nan::HandleScope scope;

    if (!info.IsConstructCall()) {
      return Nan::ThrowTypeError("Use the new operator to create new StrPtr objects");
    }

    StrPtr* strPtr;
    if (info.Length() == 1) {
      if (!info[0]->IsExternal()) {
        return Nan::ThrowTypeError("First argument must be a StrPtr object");
      }

      strPtr = new StrPtr(static_cast<::StrPtr*>(
        External::Cast(*info[0])->Value()));
    } else {
      strPtr = new StrPtr();
    }

    strPtr->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  }

  NAN_METHOD(StrPtr::Text) {
    Nan::HandleScope scope;
    StrPtr* obj = ObjectWrap::Unwrap<StrPtr>(info.This());
    info.GetReturnValue().Set(
      Nan::NewBuffer(obj->_obj->Text(), obj->_obj->Length()).ToLocalChecked()
    );
  }

  NAN_METHOD(StrPtr::Length) {
    Nan::HandleScope scope;
    StrPtr* obj = ObjectWrap::Unwrap<StrPtr>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(obj->_obj->Length()));
  }

  NAN_METHOD(StrPtr::Atoi) {
    Nan::HandleScope scope;
    StrPtr* obj = ObjectWrap::Unwrap<StrPtr>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(obj->_obj->Atoi()));
  }

  NAN_METHOD(StrPtr::IsNumeric) {
    Nan::HandleScope scope;
    StrPtr* obj = ObjectWrap::Unwrap<StrPtr>(info.This());
    info.GetReturnValue().Set(obj->_obj->IsNumeric() ? Nan::True() : Nan::False());
  }

  NAN_METHOD(StrPtr::EndsWith) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
      return Nan::ThrowTypeError("EndsWith requires at least 2 arguments");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    if (!info[1]->IsNumber()) {
      return Nan::ThrowTypeError("Second argument must be a number");
    }

    Nan::Utf8String* str = new Nan::Utf8String(info[0]->ToString());
    Local<Int32> lenInt32 = Nan::To<Int32>(info[1]).ToLocalChecked();

    StrPtr* obj = ObjectWrap::Unwrap<StrPtr>(info.This());
    int ret = obj->_obj->EndsWith(str->operator*(), lenInt32->Value());

    info.GetReturnValue().Set(Nan::New<Number>(ret));
  }

  NAN_METHOD(StrPtr::SetLength) {
    Nan::HandleScope scope;
    StrPtr* obj = ObjectWrap::Unwrap<StrPtr>(info.This());

    if (info.Length() == 1) {
      if (!info[0]->IsNumber()) {
        return Nan::ThrowTypeError("First argument must be a number");
      }

      Local<Int32> lenInt32 = Nan::To<Int32>(info[0]).ToLocalChecked();
      obj->_obj->SetLength(lenInt32->Value());
    } else {
      obj->_obj->SetLength();
    }

    info.GetReturnValue().Set(Nan::Undefined());
  }
}
