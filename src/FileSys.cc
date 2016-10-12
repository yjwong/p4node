#include <node.h>
#include <node_buffer.h>
#include "FileSys.h"
#include "Error.h"

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::Local;
using v8::Number;
using v8::External;
using v8::Int32;
using v8::Function;
using v8::Value;

namespace p4node {
  Nan::Persistent<FunctionTemplate> FileSys::constructor_template;

  FileSys::FileSys(::FileSys* obj) : _obj(obj) {}

  FileSys::~FileSys() {}

  void FileSys::Init(Handle<Object> exports) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("FileSys").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "Chmod", Chmod);
    Nan::SetPrototypeMethod(tpl, "Close", Close);
    Nan::SetPrototypeMethod(tpl, "Create", Create);
    Nan::SetPrototypeMethod(tpl, "Open", Open);
    Nan::SetPrototypeMethod(tpl, "Read", Read);
    Nan::SetPrototypeMethod(tpl, "Rename", Rename);
    Nan::SetPrototypeMethod(tpl, "Stat", Stat);
    Nan::SetPrototypeMethod(tpl, "StatModTime", StatModTime);
    Nan::SetPrototypeMethod(tpl, "Truncate", Truncate);
    Nan::SetPrototypeMethod(tpl, "Unlink", Unlink);
    Nan::SetPrototypeMethod(tpl, "Write", Write);

    constructor_template.Reset(tpl);
    exports->Set(Nan::New("FileSys").ToLocalChecked(), tpl->GetFunction());
  }

  NAN_METHOD(FileSys::New) {
    Nan::HandleScope scope;

    if (!info.IsConstructCall()) {
      return Nan::ThrowTypeError("Use the new operator to create new FileSys objects");
    }

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("FileSys object cannot be created directly");
    }

    if (!info[0]->IsExternal()) {
      return Nan::ThrowTypeError("First argument must be a FileSys object");
    }

    FileSys* fileSys = new FileSys(static_cast<::FileSys*>(
      External::Cast(*info[0])->Value()));
    fileSys->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  }

  NAN_METHOD(FileSys::Chmod) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
      return Nan::ThrowTypeError("Chmod requires at least 2 arguments");
    }

    if (!info[0]->IsNumber()) {
      return Nan::ThrowTypeError("First argument must be a number");
    }

    if (!info[1]->IsObject()) {
      return Nan::ThrowTypeError("Second argument must be an Error object");
    }

    Local<Int32> perms = Nan::To<Int32>(info[0]).ToLocalChecked();
    Local<Object> errorObj = info[1]->ToObject();
    Error* error = ObjectWrap::Unwrap<Error>(errorObj);

    FileSys* obj = ObjectWrap::Unwrap<FileSys>(info.This());
    obj->_obj->Chmod(static_cast<::FilePerm>(perms->Value()), error->Unwrap());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(FileSys::Close) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("Close requires at least 1 argument");
    }

    if (!info[0]->IsObject()) {
      return Nan::ThrowTypeError("First argument must be an Error object");
    }

    Local<Object> errorObj = info[0]->ToObject();
    Error* error = ObjectWrap::Unwrap<Error>(errorObj);

    FileSys* obj = ObjectWrap::Unwrap<FileSys>(info.This());
    obj->_obj->Close(error->Unwrap());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(FileSys::Create) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("Create requires at least 1 argument");
    }

    if (!info[0]->IsNumber()) {
      return Nan::ThrowTypeError("First argument must be a number");
    }

    Local<Int32> type = Nan::To<Int32>(info[0]).ToLocalChecked();

    // Construct the new FileSys object.
    FileSys* fileSys = ObjectWrap::Unwrap<FileSys>(info.This());
    ::FileSys* newFileSys = fileSys->_obj->Create(
      static_cast<::FileSysType>(type->Value())
    );

    Local<FunctionTemplate> tpl = Nan::New(FileSys::constructor_template);
    Local<Function> func = tpl->GetFunction();
    Handle<Value> newFuncArgs[] = { Nan::New<External>(newFileSys) };
    Local<Object> newObj = Nan::NewInstance(func, 1, newFuncArgs).ToLocalChecked();

    info.GetReturnValue().Set(newObj);
  }

  NAN_METHOD(FileSys::Open) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
      return Nan::ThrowTypeError("Open requires at least 2 arguments");
    }

    if (!info[0]->IsNumber()) {
      return Nan::ThrowTypeError("First argument must be a number");
    }

    if (!info[1]->IsObject()) {
      return Nan::ThrowTypeError("Second argument must be an Error object");
    }

    Local<Int32> mode = Nan::To<Int32>(info[0]).ToLocalChecked();
    Local<Object> errorObj = info[1]->ToObject();
    Error* error = ObjectWrap::Unwrap<Error>(errorObj);

    FileSys* obj = ObjectWrap::Unwrap<FileSys>(info.This());
    obj->_obj->Open(static_cast<::FileOpenMode>(mode->Value()), error->Unwrap());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(FileSys::Read) {
    Nan::HandleScope scope;

    if (info.Length() < 3) {
      return Nan::ThrowTypeError("Read requires at least 3 arguments");
    }

    if (!node::Buffer::HasInstance(info[0])) {
      return Nan::ThrowTypeError("First argument must be a buffer");
    }

    if (!info[1]->IsNumber()) {
      return Nan::ThrowTypeError("Second argument must be a number");
    }

    if (!info[2]->IsObject()) {
      return Nan::ThrowTypeError("Third argument must be an Error object");
    }

    char* buf = node::Buffer::Data(info[0]);
    Local<Int32> lenInt32 = Nan::To<Int32>(info[1]).ToLocalChecked();
    Local<Object> errorObj = info[2]->ToObject();
    Error* error = ObjectWrap::Unwrap<Error>(errorObj);

    FileSys* obj = ObjectWrap::Unwrap<FileSys>(info.This());
    int bytesRead = obj->_obj->Read(buf, lenInt32->Value(), error->Unwrap());

    info.GetReturnValue().Set(Nan::New<Number>(bytesRead));
  }

  NAN_METHOD(FileSys::Rename) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
      return Nan::ThrowTypeError("Rename requires at least 2 arguments");
    }

    if (!info[0]->IsObject()) {
      return Nan::ThrowTypeError("First argument must be a FileSys object");
    }

    if (!info[1]->IsObject()) {
      return Nan::ThrowTypeError("Second argument must be an Error object");
    }

    Local<Object> fileSysObj = info[0]->ToObject();
    FileSys* target = ObjectWrap::Unwrap<FileSys>(fileSysObj);
    Local<Object> errorObj = info[1]->ToObject();
    Error* error = ObjectWrap::Unwrap<Error>(errorObj);

    FileSys* obj = ObjectWrap::Unwrap<FileSys>(info.This());
    obj->_obj->Rename(target->_obj, error->Unwrap());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(FileSys::Stat) {
    Nan::HandleScope scope;
    FileSys* obj = ObjectWrap::Unwrap<FileSys>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(obj->_obj->Stat()));
  }

  NAN_METHOD(FileSys::StatModTime) {
    Nan::HandleScope scope;
    FileSys* obj = ObjectWrap::Unwrap<FileSys>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(obj->_obj->StatModTime()));
  }

  NAN_METHOD(FileSys::Truncate) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("Truncate requires at least 1 argument");
    }

    if (!info[0]->IsObject()) {
      return Nan::ThrowTypeError("First argument must be an Error object");
    }

    Local<Object> errorObj = info[0]->ToObject();
    Error* error = ObjectWrap::Unwrap<Error>(errorObj);

    FileSys* obj = ObjectWrap::Unwrap<FileSys>(info.This());
    obj->_obj->Truncate(error->Unwrap());
    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(FileSys::Unlink) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("Rename requires at least 1 argument");
    }

    if (!info[0]->IsObject()) {
      return Nan::ThrowTypeError("First argument must be an Error object");
    }

    Local<Object> errorObj = info[0]->ToObject();
    Error* error = ObjectWrap::Unwrap<Error>(errorObj);

    FileSys* obj = ObjectWrap::Unwrap<FileSys>(info.This());
    obj->_obj->Unlink(error->Unwrap());
    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(FileSys::Write) {
    Nan::HandleScope scope;

    if (info.Length() < 3) {
      return Nan::ThrowTypeError("Write requires at least 3 arguments");
    }

    if (!node::Buffer::HasInstance(info[0])) {
      return Nan::ThrowTypeError("First argument must be a buffer");
    }

    if (!info[1]->IsNumber()) {
      return Nan::ThrowTypeError("Second argument must be a number");
    }

    if (!info[2]->IsObject()) {
      return Nan::ThrowTypeError("Third argument must be an Error object");
    }

    char* buf = node::Buffer::Data(info[0]);
    Local<Int32> lenInt32 = Nan::To<Int32>(info[1]).ToLocalChecked();
    Local<Object> errorObj = info[2]->ToObject();
    Error* error = ObjectWrap::Unwrap<Error>(errorObj);

    FileSys* obj = ObjectWrap::Unwrap<FileSys>(info.This());
    obj->_obj->Write(buf, lenInt32->Value(), error->Unwrap());

    info.GetReturnValue().Set(Nan::Undefined());
  }
}
