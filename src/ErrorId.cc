#include "ErrorId.h"

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::Local;

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

    constructor_template.Reset(tpl);
    exports->Set(Nan::New("ErrorId").ToLocalChecked(), tpl->GetFunction());
  }

  NAN_METHOD(ErrorId::New) {
    Nan::HandleScope scope;

    if (!info.IsConstructCall()) {
      return Nan::ThrowTypeError("Use the new operator to create new ErrorId objects");
    }

    ErrorId* errorId = new ErrorId();
    info.GetReturnValue().Set(info.This());
  }
}
