#include <node.h>
#include "Error.h"
#include "StrDict.h"
#include "FileSys.h"
#include "ClientUser.h"

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::Local;
using v8::Int32;
using v8::Array;
using v8::Value;
using v8::Function;
using v8::External;
using v8::Number;

namespace p4node {
  Nan::Persistent<FunctionTemplate> ClientUser::constructor_template;

  ClientUser::ClientUser() {
    _obj = new ClientUserWrapper();
  }

  ClientUser::~ClientUser() {
    delete _obj;
  }

  ClientUserWrapper* ClientUser::Unwrap() {
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

    //Nan::SetPrototypeMethod(tpl, "Prompt", Prompt);
    Nan::SetPrototypeMethod(tpl, "ErrorPause", ErrorPause);

    Nan::SetPrototypeMethod(tpl, "Edit", Edit);
    Nan::SetPrototypeMethod(tpl, "Diff", Diff);
    Nan::SetPrototypeMethod(tpl, "Merge", Merge);
    //Nan::SetPrototypeMethod(tpl, "Resolve", Resolve);
    Nan::SetPrototypeMethod(tpl, "Help", Help);

    Nan::SetPrototypeMethod(tpl, "File", File);
    //Nan::SetPrototypeMethod(tpl, "CreateProgress", CreateProgress);
    Nan::SetPrototypeMethod(tpl, "ProgressIndicator", ProgressIndicator);

    Nan::SetPrototypeMethod(tpl, "Finished", Finished);

    Nan::SetPrototypeMethod(tpl, "SetOutputCharset", SetOutputCharset);
    Nan::SetPrototypeMethod(tpl, "DisableTmpCleanup", DisableTmpCleanup);
    Nan::SetPrototypeMethod(tpl, "SetQuiet", SetQuiet);
    Nan::SetPrototypeMethod(tpl, "CanAutoLoginPrompt", CanAutoLoginPrompt);

    Nan::SetPrototypeMethod(tpl, "SetCallbacks", SetCallbacks);

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

    Nan::SetAccessor(info.This(), Nan::New("varList").ToLocalChecked(), GetVarList, SetVarList);
    //Nan::SetAccessor(info.This(), Nan::New("enviro").ToLocalChecked(), GetEnviro, SetEnviro);

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

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    if (!info[1]->IsObject()) {
      return Nan::ThrowTypeError("Second argument must be an Error object");
    }

    Local<Object> varListObj = info[0]->ToObject();
    StrDict* strDict = ObjectWrap::Unwrap<StrDict>(varListObj);

    ClientUser* ui = ObjectWrap::Unwrap<ClientUser>(info.This());
    ui->_obj->OutputStat(strDict->Unwrap());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientUser::ErrorPause) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
      return Nan::ThrowTypeError("ErrorPause requires at least 2 arguments");
    }

    if (!info[0]->IsString()) {
      return Nan::ThrowTypeError("First argument must be a string");
    }

    if (!info[1]->IsObject()) {
      return Nan::ThrowTypeError("Second argument must be an Error object");
    }

    Nan::Utf8String* errBuf = new Nan::Utf8String(info[0]->ToString());
    Local<Object> errObj = info[1]->ToObject();
    Error* err = ObjectWrap::Unwrap<Error>(errObj);

    ClientUser* ui = ObjectWrap::Unwrap<ClientUser>(info.This());
    ui->_obj->ErrorPause(errBuf->operator*(), err->Unwrap());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientUser::Edit) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
      return Nan::ThrowTypeError("Edit requires at least 2 arguments");
    }

    if (!info[0]->IsObject()) {
      return Nan::ThrowTypeError("First argument must be a FileSys object");
    }

    if (!info[1]->IsObject()) {
      return Nan::ThrowTypeError("Second argument must be an Error object");
    }

    Local<Object> fileSysObj = info[0]->ToObject();
    FileSys* fileSys = ObjectWrap::Unwrap<FileSys>(fileSysObj);
    Local<Object> errObj = info[1]->ToObject();
    Error* err = ObjectWrap::Unwrap<Error>(errObj);

    ClientUser* ui = ObjectWrap::Unwrap<ClientUser>(info.This());
    ui->_obj->Edit(fileSys->Unwrap(), err->Unwrap());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientUser::Diff) {
    Nan::HandleScope scope;
    ClientUser* ui = ObjectWrap::Unwrap<ClientUser>(info.This());

    if (info.Length() < 5) {
      return Nan::ThrowTypeError("Diff requires at least 5 arguments");
    }

    if (!info[0]->IsObject()) {
      return Nan::ThrowTypeError("First argument must be a FileSys object");
    }

    Local<Object> fileSys1Obj = info[0]->ToObject();
    Local<Object> fileSys2Obj = info[1]->ToObject();

    FileSys* fileSys1 = ObjectWrap::Unwrap<FileSys>(fileSys1Obj);
    FileSys* fileSys2 = ObjectWrap::Unwrap<FileSys>(fileSys2Obj);

    if (!info[1]->IsObject()) {
      return Nan::ThrowTypeError("Second argument must be a FileSys object");
    }

    if (info.Length() == 6) {
      if (!info[2]->IsObject()) {
        return Nan::ThrowTypeError("Second argument must be a FileSys object");
      }

      if (!info[3]->IsNumber()) {
        return Nan::ThrowTypeError("Third argument must be a number");
      }

      if (!info[4]->IsString()) {
        return Nan::ThrowTypeError("Fourth argument must be a string");
      }

      if (!info[5]->IsObject()) {
        return Nan::ThrowTypeError("Fifth argument must be an Error object");
      }

      Local<Object> fileSysOutObj = info[2]->ToObject();
      Local<Int32> doPageInt32 = Nan::To<Int32>(info[3]).ToLocalChecked();
      Nan::Utf8String* diffFlagsString = new Nan::Utf8String(info[4]->ToString());
      Local<Object> errObj = info[5]->ToObject();

      FileSys* fileSysOut = ObjectWrap::Unwrap<FileSys>(fileSysOutObj);
      int doPage = doPageInt32->Value();
      char* diffFlags = diffFlagsString->operator*();
      Error* err = ObjectWrap::Unwrap<Error>(errObj);

      ui->_obj->Diff(
        fileSys1->Unwrap(),
        fileSys2->Unwrap(),
        fileSysOut->Unwrap(),
        doPage, diffFlags,
        err->Unwrap()
      );
    } else {
      if (!info[2]->IsNumber()) {
        return Nan::ThrowTypeError("Third argument must be a number");
      }

      if (!info[3]->IsString()) {
        return Nan::ThrowTypeError("Fourth argument must be a string");
      }

      if (!info[4]->IsObject()) {
        return Nan::ThrowTypeError("Fifth argument must be an Error object");
      }

      Local<Int32> doPageInt32 = Nan::To<Int32>(info[2]).ToLocalChecked();
      Nan::Utf8String* diffFlagsString = new Nan::Utf8String(info[3]->ToString());
      Local<Object> errObj = info[4]->ToObject();

      int doPage = doPageInt32->Value();
      char* diffFlags = diffFlagsString->operator*();
      Error* err = ObjectWrap::Unwrap<Error>(errObj);

      ui->_obj->Diff(
        fileSys1->Unwrap(),
        fileSys2->Unwrap(),
        doPage, diffFlags,
        err->Unwrap()
      );
    }

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientUser::Merge) {
    Nan::HandleScope scope;

    if (info.Length() < 5) {
      return Nan::ThrowTypeError("Merge requires at least 5 arguments");
    }

    if (!info[0]->IsObject()) {
      return Nan::ThrowTypeError("First argument must be a FileSys object");
    }

    if (!info[1]->IsObject()) {
      return Nan::ThrowTypeError("Second argument must be a FileSys object");
    }

    if (!info[2]->IsObject()) {
      return Nan::ThrowTypeError("Third argument must be a FileSys object");
    }

    if (!info[3]->IsObject()) {
      return Nan::ThrowTypeError("Fourth argument must be a FileSys object");
    }

    if (!info[4]->IsObject()) {
      return Nan::ThrowTypeError("Fifth argument must be an Error object");
    }

    Local<Object> fileSys1Obj = info[0]->ToObject();
    Local<Object> fileSys2Obj = info[1]->ToObject();
    Local<Object> fileSysOutObj = info[2]->ToObject();
    Local<Object> fileSysResultObj = info[3]->ToObject();
    Local<Object> errObj = info[4]->ToObject();

    FileSys* fileSys1 = ObjectWrap::Unwrap<FileSys>(fileSys1Obj);
    FileSys* fileSys2 = ObjectWrap::Unwrap<FileSys>(fileSys2Obj);
    FileSys* fileSysOut = ObjectWrap::Unwrap<FileSys>(fileSysOutObj);
    FileSys* fileSysResult = ObjectWrap::Unwrap<FileSys>(fileSysResultObj);
    Error* err = ObjectWrap::Unwrap<Error>(errObj);

    ClientUser* ui = ObjectWrap::Unwrap<ClientUser>(info.This());
    ui->_obj->Merge(
      fileSys1->Unwrap(),
      fileSys2->Unwrap(),
      fileSysOut->Unwrap(),
      fileSysResult->Unwrap(),
      err->Unwrap()
    );

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientUser::Help) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("Help requires at least 1 argument");
    }

    if (!info[0]->IsArray()) {
      return Nan::ThrowTypeError("First argument must be an array");
    }

    Local<Array> helpArray = Local<Array>::Cast(info[0]);
    char** charHelpArray = new char*[helpArray->Length()];

    // For every string in the array, convert it to a C-style string.
    for (uint32_t i = 0; i < helpArray->Length(); i++) {
      Local<Value> arrayItem = Nan::Get(helpArray, i).ToLocalChecked();
      if (!arrayItem->IsString()) {
        return Nan::ThrowTypeError("Elements of first argument must be strings");
      }

      Nan::Utf8String* arrayItemString = new Nan::Utf8String(arrayItem->ToString());
      charHelpArray[i] = arrayItemString->operator*();
    }

    ClientUser* ui = ObjectWrap::Unwrap<ClientUser>(info.This());
    ui->_obj->Help(charHelpArray);

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientUser::File) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("File requires at least 1 argument");
    }

    if (!info[0]->IsNumber()) {
      return Nan::ThrowTypeError("First argument must be a number");
    }

    Local<Int32> typeInt32 = Nan::To<Int32>(info[0]).ToLocalChecked();
    ClientUser* ui = ObjectWrap::Unwrap<ClientUser>(info.This());
    ::FileSys* newFileSys = ui->_obj->File(
      static_cast<::FileSysType>(typeInt32->Value())
    );

    Local<FunctionTemplate> tpl = Nan::New(FileSys::constructor_template);
    Local<Function> func = tpl->GetFunction();
    Handle<Value> newFuncArgs[] = { Nan::New<External>(newFileSys) };
    Local<Object> newObj = Nan::NewInstance(func, 1, newFuncArgs).ToLocalChecked();

    info.GetReturnValue().Set(newObj);
  }

  NAN_METHOD(ClientUser::ProgressIndicator) {
    Nan::HandleScope scope;
    ClientUser* ui = ObjectWrap::Unwrap<ClientUser>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(ui->_obj->ProgressIndicator()));
  }

  NAN_METHOD(ClientUser::Finished) {
    Nan::HandleScope scope;
    ClientUser* ui = ObjectWrap::Unwrap<ClientUser>(info.This());
    ui->_obj->Finished();
    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientUser::SetOutputCharset) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("SetOutputCharset requires at least 1 argument");
    }

    if (!info[0]->IsNumber()) {
      return Nan::ThrowTypeError("First argument must be a number");
    }

    Local<Int32> charsetInt32 = Nan::To<Int32>(info[0]).ToLocalChecked();
    ClientUser* ui = ObjectWrap::Unwrap<ClientUser>(info.This());
    ui->_obj->SetOutputCharset(charsetInt32->Value());

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientUser::DisableTmpCleanup) {
    Nan::HandleScope scope;
    ClientUser* ui = ObjectWrap::Unwrap<ClientUser>(info.This());
    ui->_obj->DisableTmpCleanup();
    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientUser::SetQuiet) {
    Nan::HandleScope scope;
    ClientUser* ui = ObjectWrap::Unwrap<ClientUser>(info.This());
    ui->_obj->SetQuiet();
    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(ClientUser::CanAutoLoginPrompt) {
    Nan::HandleScope scope;
    ClientUser* ui = ObjectWrap::Unwrap<ClientUser>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(ui->_obj->CanAutoLoginPrompt()));
  }

  NAN_METHOD(ClientUser::SetCallbacks) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("SetCallbacks requires at least 1 argument");
    }

    if (!info[0]->IsObject()) {
      return Nan::ThrowTypeError("First argument must be an object");
    }

    Local<Object> callbacksObj = info[0]->ToObject();
    ClientUser* ui = ObjectWrap::Unwrap<ClientUser>(info.This());
    ui->_obj->SetCallbacks(callbacksObj);

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_GETTER(ClientUser::GetVarList) {
    Nan::HandleScope scope;

    ClientUser* ui = ObjectWrap::Unwrap<ClientUser>(info.This());
    StrDict* newStrDict = new StrDict(ui->_obj->varList);

    Local<FunctionTemplate> tpl = Nan::New(StrDict::constructor_template);
    Local<Function> func = tpl->GetFunction();
    Handle<Value> newFuncArgs[] = { Nan::New<External>(newStrDict) };
    Local<Object> newObj = Nan::NewInstance(func, 1, newFuncArgs).ToLocalChecked();

    info.GetReturnValue().Set(newObj);
  }

  NAN_SETTER(ClientUser::SetVarList) {
    Nan::HandleScope scope;
    ClientUser* ui = ObjectWrap::Unwrap<ClientUser>(info.This());
    StrDict* strDict = ObjectWrap::Unwrap<StrDict>(value->ToObject());
    ui->_obj->varList = strDict->Unwrap();
  }
}
