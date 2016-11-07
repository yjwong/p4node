#include <node_object_wrap.h>
#include "nan.h"
#include "Error.h"
#include "FileSys.h"
#include "StrDict.h"
#include "ClientUserWrapper.h"

#define P4NODE_CLIENT_USER_WRAPPER_GET_CALLBACK_OBJ(name, ifError)             \
  if (_callbacksObj.IsEmpty()) {                                               \
    return (ifError);                                                          \
  }                                                                            \
                                                                               \
  Local<Object> callbacksObj = Nan::New(_callbacksObj);                        \
  Local<Value> callbackValue = Nan::Get(                                       \
    callbacksObj,                                                              \
    Nan::New((name)).ToLocalChecked()                                          \
  ).ToLocalChecked();                                                          \
                                                                               \
  if (!callbackValue->IsObject()) {                                            \
    return (ifError);                                                          \
  }                                                                            \
                                                                               \
  Local<Object> callbackObj = Nan::To<Object>(callbackValue).ToLocalChecked(); \
  if (!callbackObj->IsFunction()) {                                            \
    return (ifError);                                                          \
  }

using v8::Local;
using v8::Object;
using v8::Value;
using v8::FunctionTemplate;
using v8::Function;
using v8::Handle;
using v8::External;
using v8::Number;
using v8::Array;
using v8::Int32;
using node::ObjectWrap;

namespace p4node {
  void ClientUserWrapper::SetCallbacks(Local<Object> callbacksObj) {
    _callbacksObj.Reset(callbacksObj);
  }

  void ClientUserWrapper::HandleError(::Error *err) {
    P4NODE_CLIENT_USER_WRAPPER_GET_CALLBACK_OBJ("HandleError",
      ::ClientUser::HandleError(err))

    // Convert error object to managed error.
    Local<FunctionTemplate> tpl = Nan::New(Error::constructor_template);
    Local<Function> func = tpl->GetFunction();
    Handle<Value> newFuncArgs[] = { Nan::New<External>(err) };
    Local<Object> newObj = Nan::NewInstance(func, 1, newFuncArgs).ToLocalChecked();

    // Invoke the callback.
    Handle<Value> callbackFuncArgs[] = { newObj };
    Nan::CallAsFunction(callbackObj, callbackObj, 1, callbackFuncArgs);
  }

  void ClientUserWrapper::Message(::Error *err) {
    P4NODE_CLIENT_USER_WRAPPER_GET_CALLBACK_OBJ("Message",
      ::ClientUser::Message(err))

    // Convert error object to managed error.
    Local<FunctionTemplate> tpl = Nan::New(Error::constructor_template);
    Local<Function> func = tpl->GetFunction();
    Handle<Value> funcArgs[] = { Nan::New<External>(err) };
    Local<Object> obj = Nan::NewInstance(func, 1, funcArgs).ToLocalChecked();

    // Invoke the callback.
    Handle<Value> callbackFuncArgs[] = { obj };
    Nan::CallAsFunction(callbackObj, callbackObj, 1, callbackFuncArgs);
  }

  void ClientUserWrapper::OutputError(const char *errBuf) {
    P4NODE_CLIENT_USER_WRAPPER_GET_CALLBACK_OBJ("OutputError",
      ::ClientUser::OutputError(errBuf))

    Handle<Value> callbackFuncArgs[] = { Nan::New(errBuf).ToLocalChecked() };
    Nan::CallAsFunction(callbackObj, callbackObj, 1, callbackFuncArgs);
  }

  void ClientUserWrapper::OutputInfo(char level, const char *data) {
    P4NODE_CLIENT_USER_WRAPPER_GET_CALLBACK_OBJ("OutputInfo",
      ::ClientUser::OutputInfo(level, data))

    Handle<Value> callbackFuncArgs[] = {
      Nan::New<Number>(level),
      Nan::New(data).ToLocalChecked()
    };

    Nan::CallAsFunction(callbackObj, callbackObj, 2, callbackFuncArgs);
  }

  void ClientUserWrapper::OutputBinary(const char* data, int length) {
    P4NODE_CLIENT_USER_WRAPPER_GET_CALLBACK_OBJ("OutputBinary",
      ::ClientUser::OutputBinary(data, length))

    Handle<Value> callbackFuncArgs[] = {
      Nan::CopyBuffer(data, length).ToLocalChecked()
    };

    Nan::CallAsFunction(callbackObj, callbackObj, 1, callbackFuncArgs);
  }

  void ClientUserWrapper::OutputText(const char* data, int length) {
    P4NODE_CLIENT_USER_WRAPPER_GET_CALLBACK_OBJ("OutputText",
      ::ClientUser::OutputText(data, length));

    Handle<Value> callbackFuncArgs[] = {
      Nan::CopyBuffer(data, length).ToLocalChecked()
    };

    Nan::CallAsFunction(callbackObj, callbackObj, 1, callbackFuncArgs);
  }

  void ClientUserWrapper::OutputStat(::StrDict *varList) {
    P4NODE_CLIENT_USER_WRAPPER_GET_CALLBACK_OBJ("OutputStat",
      ::ClientUser::OutputStat(varList))

    // Convert StrDict object to managed StrDict.
    Local<FunctionTemplate> tpl = Nan::New(StrDict::constructor_template);
    Local<Function> func = tpl->GetFunction();
    Handle<Value> funcArgs[] = { Nan::New<External>(varList) };
    Local<Object> obj = Nan::NewInstance(func, 1, funcArgs).ToLocalChecked();

    // Invoke the callback.
    Handle<Value> callbackFuncArgs[] = { obj };
    Nan::CallAsFunction(callbackObj, callbackObj, 1, callbackFuncArgs);
  }

  void ClientUserWrapper::ErrorPause(char *errBuf, ::Error *err) {
    P4NODE_CLIENT_USER_WRAPPER_GET_CALLBACK_OBJ("ErrorPause",
      ::ClientUser::ErrorPause(errBuf, err))

    // Convert Error object to managed Error.
    Local<FunctionTemplate> tpl = Nan::New(Error::constructor_template);
    Local<Function> func = tpl->GetFunction();
    Handle<Value> funcArgs[] = { Nan::New<External>(err) };
    Local<Object> obj = Nan::NewInstance(func, 1, funcArgs).ToLocalChecked();

    // Invoke the callback.
    Handle<Value> callbackFuncArgs[] = {
      Nan::New(errBuf).ToLocalChecked(),
      obj
    };

    Nan::CallAsFunction(callbackObj, callbackObj, 2, callbackFuncArgs);
  }

  void ClientUserWrapper::Edit(::FileSys *fileSys, ::Error *err) {
    P4NODE_CLIENT_USER_WRAPPER_GET_CALLBACK_OBJ("Edit",
      ::ClientUser::Edit(fileSys, err))

    Local<FunctionTemplate> fileSysTpl = Nan::New(FileSys::constructor_template);
    Local<Function> fileSysFunc = fileSysTpl->GetFunction();
    Handle<Value> fileSysFuncArgs[] = { Nan::New<External>(fileSys) };
    Local<Object> fileSysObj =
      Nan::NewInstance(fileSysFunc, 1, fileSysFuncArgs).ToLocalChecked();

    Local<FunctionTemplate> errTpl = Nan::New(Error::constructor_template);
    Local<Function> errFunc = errTpl->GetFunction();
    Handle<Value> errFuncArgs[] = { Nan::New<External>(err) };
    Local<Object> errObj =
      Nan::NewInstance(errFunc, 1, errFuncArgs).ToLocalChecked();

    Handle<Value> callbackFuncArgs[] = { fileSysObj, errObj };
    Nan::CallAsFunction(callbackObj, callbackObj, 2, callbackFuncArgs);
  }

  void ClientUserWrapper::Diff(::FileSys *fileSys1, ::FileSys *fileSys2,
    int doPage, char *diffFlags, ::Error *err) {
    P4NODE_CLIENT_USER_WRAPPER_GET_CALLBACK_OBJ("Diff",
      ::ClientUser::Diff(fileSys1, fileSys2, doPage, diffFlags, err))

    Local<FunctionTemplate> fileSysTpl = Nan::New(FileSys::constructor_template);
    Local<Function> fileSysFunc = fileSysTpl->GetFunction();

    Handle<Value> fileSys1FuncArgs[] = { Nan::New<External>(fileSys1) };
    Handle<Value> fileSys2FuncArgs[] = { Nan::New<External>(fileSys2) };

    Local<Object> fileSys1Obj =
      Nan::NewInstance(fileSysFunc, 1, fileSys1FuncArgs).ToLocalChecked();
    Local<Object> fileSys2Obj =
      Nan::NewInstance(fileSysFunc, 1, fileSys2FuncArgs).ToLocalChecked();

    Local<FunctionTemplate> errTpl = Nan::New(Error::constructor_template);
    Local<Function> errFunc = errTpl->GetFunction();
    Handle<Value> errFuncArgs[] = { Nan::New<External>(err) };
    Local<Object> errObj =
      Nan::NewInstance(errFunc, 1, errFuncArgs).ToLocalChecked();

    Handle<Value> callbackFuncArgs[] = {
      fileSys1Obj, fileSys2Obj, Nan::New<Number>(doPage),
      Nan::New(diffFlags).ToLocalChecked(), errObj
    };

    Nan::CallAsFunction(callbackObj, callbackObj, 5, callbackFuncArgs);
  }

  void ClientUserWrapper::Diff(::FileSys *fileSys1, ::FileSys *fileSys2,
    ::FileSys *fileSysOut, int doPage, char *diffFlags, ::Error *err) {
    P4NODE_CLIENT_USER_WRAPPER_GET_CALLBACK_OBJ("Diff",
      ::ClientUser::Diff(fileSys1, fileSys2, fileSysOut, doPage, diffFlags, err))

    Local<FunctionTemplate> fileSysTpl = Nan::New(FileSys::constructor_template);
    Local<Function> fileSysFunc = fileSysTpl->GetFunction();

    Handle<Value> fileSys1FuncArgs[] = { Nan::New<External>(fileSys1) };
    Handle<Value> fileSys2FuncArgs[] = { Nan::New<External>(fileSys2) };
    Handle<Value> fileSysOutFuncArgs[] = { Nan::New<External>(fileSysOut) };

    Local<Object> fileSys1Obj =
      Nan::NewInstance(fileSysFunc, 1, fileSys1FuncArgs).ToLocalChecked();
    Local<Object> fileSys2Obj =
      Nan::NewInstance(fileSysFunc, 1, fileSys2FuncArgs).ToLocalChecked();
    Local<Object> fileSysOutObj =
      Nan::NewInstance(fileSysFunc, 1, fileSysOutFuncArgs).ToLocalChecked();

    Local<FunctionTemplate> errTpl = Nan::New(Error::constructor_template);
    Local<Function> errFunc = errTpl->GetFunction();
    Handle<Value> errFuncArgs[] = { Nan::New<External>(err) };
    Local<Object> errObj =
      Nan::NewInstance(errFunc, 1, errFuncArgs).ToLocalChecked();

    Handle<Value> callbackFuncArgs[] = {
      fileSys1Obj, fileSys2Obj, fileSysOutObj, Nan::New<Number>(doPage),
      Nan::New(diffFlags).ToLocalChecked(), errObj
    };

    Nan::CallAsFunction(callbackObj, callbackObj, 6, callbackFuncArgs);
  }

  void ClientUserWrapper::Merge(::FileSys *base, ::FileSys *fileSys1,
    ::FileSys *fileSys2, ::FileSys *result, ::Error *err) {
    P4NODE_CLIENT_USER_WRAPPER_GET_CALLBACK_OBJ("Merge",
      ::ClientUser::Merge(base, fileSys1, fileSys2, result, err));

    Local<FunctionTemplate> fileSysTpl = Nan::New(FileSys::constructor_template);
    Local<Function> fileSysFunc = fileSysTpl->GetFunction();

    Handle<Value> baseFuncArgs[] = { Nan::New<External>(base) };
    Handle<Value> fileSys1FuncArgs[] = { Nan::New<External>(fileSys1) };
    Handle<Value> fileSys2FuncArgs[] = { Nan::New<External>(fileSys2) };
    Handle<Value> resultFuncArgs[] = { Nan::New<External>(result) };

    Local<Object> baseObj =
      Nan::NewInstance(fileSysFunc, 1, baseFuncArgs).ToLocalChecked();
    Local<Object> fileSys1Obj =
      Nan::NewInstance(fileSysFunc, 1, fileSys1FuncArgs).ToLocalChecked();
    Local<Object> fileSys2Obj =
      Nan::NewInstance(fileSysFunc, 1, fileSys2FuncArgs).ToLocalChecked();
    Local<Object> resultObj =
      Nan::NewInstance(fileSysFunc, 1, resultFuncArgs).ToLocalChecked();

    Local<FunctionTemplate> errTpl = Nan::New(Error::constructor_template);
    Local<Function> errFunc = errTpl->GetFunction();
    Handle<Value> errFuncArgs[] = { Nan::New<External>(err) };
    Local<Object> errObj =
      Nan::NewInstance(errFunc, 1, errFuncArgs).ToLocalChecked();

    Handle<Value> callbackFuncArgs[] = {
      baseObj, fileSys1Obj, fileSys2Obj, resultObj, errObj
    };

    Nan::CallAsFunction(callbackObj, callbackObj, 5, callbackFuncArgs);
  }

  void ClientUserWrapper::Help(const char *const *help) {
    P4NODE_CLIENT_USER_WRAPPER_GET_CALLBACK_OBJ("Help",
      ::ClientUser::Help(help))

    Local<Array> helpArray = Nan::New<Array>();
    int counter = 0;
    for (const char* helpItem = *help; helpItem != NULL; help++) {
      Nan::Set(helpArray, counter, Nan::New(helpItem).ToLocalChecked());
    }

    Handle<Value> callbackFuncArgs[] = { helpArray };
    Nan::CallAsFunction(callbackObj, callbackObj, 1, callbackFuncArgs);
  }

  ::FileSys* ClientUserWrapper::File(::FileSysType type) {
    P4NODE_CLIENT_USER_WRAPPER_GET_CALLBACK_OBJ("File",
      ::ClientUser::File(type))

    Handle<Value> callbackFuncArgs[] = { Nan::New<Number>(type) };
    Local<Value> retVal = Nan::CallAsFunction(callbackObj, callbackObj, 1,
      callbackFuncArgs).ToLocalChecked();

    // Check if the returned object is a FileSys.
    if (!retVal->IsObject()) {
      return NULL;
    }

    Local<Object> retValObj = Nan::To<Object>(retVal).ToLocalChecked();
    FileSys* fileSys = ObjectWrap::Unwrap<FileSys>(retValObj);
    return fileSys->Unwrap();
  }

  int ClientUserWrapper::ProgressIndicator() {
    P4NODE_CLIENT_USER_WRAPPER_GET_CALLBACK_OBJ("ProgressIndicator",
      ::ClientUser::ProgressIndicator())

    Local<Value> retVal = Nan::CallAsFunction(callbackObj, callbackObj, 0,
      NULL).ToLocalChecked();

    // Check if the return value is a number.
    if (!retVal->IsInt32()) {
      return -1;
    }

    Local<Int32> retValNumber = Nan::To<Int32>(retVal).ToLocalChecked();
    return retValNumber->Value();
  }

  void ClientUserWrapper::Finished() {
    P4NODE_CLIENT_USER_WRAPPER_GET_CALLBACK_OBJ("Finished",
      ::ClientUser::Finished())
    Nan::CallAsFunction(callbackObj, callbackObj, 0, NULL);
  }

  void ClientUserWrapper::SetOutputCharset(int charset) {
    P4NODE_CLIENT_USER_WRAPPER_GET_CALLBACK_OBJ("SetOutputCharset",
      ::ClientUser::SetOutputCharset(charset))
    Handle<Value> callbackFuncArgs[] = { Nan::New<Number>(charset) };
    Nan::CallAsFunction(callbackObj, callbackObj, 1, callbackFuncArgs);
  }

  void ClientUserWrapper::DisableTmpCleanup() {
    P4NODE_CLIENT_USER_WRAPPER_GET_CALLBACK_OBJ("DisableTmpCleanup",
      ::ClientUser::DisableTmpCleanup())
    Nan::CallAsFunction(callbackObj, callbackObj, 0, NULL);
  }

  void ClientUserWrapper::SetQuiet() {
    P4NODE_CLIENT_USER_WRAPPER_GET_CALLBACK_OBJ("SetQuiet",
      ::ClientUser::SetQuiet())
    Nan::CallAsFunction(callbackObj, callbackObj, 0, NULL);
  }

  int ClientUserWrapper::CanAutoLoginPrompt() {
    P4NODE_CLIENT_USER_WRAPPER_GET_CALLBACK_OBJ("CanAutoLoginPrompt",
      ::ClientUser::CanAutoLoginPrompt())

    Local<Value> retVal = Nan::CallAsFunction(callbackObj, callbackObj, 0,
      NULL).ToLocalChecked();

    // Check if the return value is a number.
    if (!retVal->IsInt32()) {
      return -1;
    }

    Local<Int32> retValNumber = Nan::To<Int32>(retVal).ToLocalChecked();
    return retValNumber->Value();
  }
}
