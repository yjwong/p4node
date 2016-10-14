#include <iostream>
#include <cmath>
#include <node.h>
#include "StrDict.h"

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::Local;
using v8::Number;
using v8::Array;
using v8::Int32;
using v8::Value;
using v8::String;

namespace p4node {
  Nan::Persistent<FunctionTemplate> StrDict::constructor_template;

  StrDict::StrDict(::StrDict* obj) : _obj(obj) {}

  StrDict::~StrDict() {}

  ::StrDict* StrDict::Unwrap() {
    return _obj;
  }

  void StrDict::Init(Handle<Object> exports) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("StrDict").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "SetArgv", SetArgv);

    constructor_template.Reset(tpl);
    exports->Set(Nan::New("StrDict").ToLocalChecked(), tpl->GetFunction());
  }

  NAN_METHOD(StrDict::New) {
    Nan::HandleScope scope;
    info.GetReturnValue().Set(info.This());
  }

  NAN_METHOD(StrDict::SetArgv) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
      return Nan::ThrowTypeError("SetArgv requires at least 2 arguments");
    }

    if (!info[0]->IsNumber()) {
      return Nan::ThrowTypeError("First argument must be a number");
    }

    if (!info[1]->IsArray()) {
      return Nan::ThrowTypeError("Second argument must be an array");
    }

    Local<Int32> argcNumber = Nan::To<Int32>(info[0]).ToLocalChecked();
    Local<Array> argvArray = Local<Array>::Cast(info[1]);
    char** charArgvArray = new char*[argvArray->Length()];

    // For every string in the array, convert it to a C-style string.
    for (uint32_t i = 0; i < argvArray->Length(); i++) {
      Local<Value> arrayItem = Nan::Get(argvArray, i).ToLocalChecked();
      if (!arrayItem->IsString()) {
        return Nan::ThrowTypeError("Elements of second argument must be strings");
      }

      Nan::Utf8String* charArgvItem = new Nan::Utf8String(arrayItem->ToString());
      charArgvArray[i] = charArgvItem->operator*();
    }

    StrDict* dict = ObjectWrap::Unwrap<StrDict>(info.This());
    dict->_obj->SetArgv(argcNumber->Value(), charArgvArray);

    info.GetReturnValue().Set(Nan::Undefined());
  }
}
