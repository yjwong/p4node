#include "nan.h"
#include "Error.h"
#include "ClientUserWrapper.h"

using v8::Local;
using v8::Object;
using v8::Value;
using v8::FunctionTemplate;
using v8::Function;
using v8::Handle;
using v8::External;

namespace p4node {
  void ClientUserWrapper::SetCallbacks(Local<Object> callbacksObj) {
    _callbacksObj.Reset(callbacksObj);
  }

  void ClientUserWrapper::HandleError(::Error *err) {
    if (_callbacksObj.IsEmpty()) {
      ::ClientUser::HandleError(err);
      return;
    }

    Local<Object> callbacksObj = Nan::New(_callbacksObj);
    Local<Value> callbackValue = Nan::Get(
      callbacksObj,
      Nan::New("HandleError").ToLocalChecked()
    ).ToLocalChecked();

    if (!callbackValue->IsObject()) {
      ::ClientUser::HandleError(err);
      return;
    }

    // Convert value to object.
    Local<Object> callbackObj = Nan::To<Object>(callbackValue).ToLocalChecked();
    if (!callbackObj->IsFunction()) {
      ::ClientUser::HandleError(err);
      return;
    }

    // Convert error object to managed error.
    Local<FunctionTemplate> tpl = Nan::New(Error::constructor_template);
    Local<Function> func = tpl->GetFunction();
    Handle<Value> newFuncArgs[] = { Nan::New<External>(err) };
    Local<Object> newObj = Nan::NewInstance(func, 1, newFuncArgs).ToLocalChecked();

    // Invoke the callback.
    Handle<Value> callbackFuncArgs[] = { newObj };
    Nan::CallAsFunction(callbackObj, callbackObj, 1, callbackFuncArgs);
  }

  void ClientUserWrapper::Finished() {
    if (_callbacksObj.IsEmpty()) {
      ::ClientUser::Finished();
      return;
    }

    Local<Object> callbacksObj = Nan::New(_callbacksObj);
    Local<Value> callbackValue = Nan::Get(
      callbacksObj,
      Nan::New("Finished").ToLocalChecked()
    ).ToLocalChecked();

    if (!callbackValue->IsObject()) {
      ::ClientUser::Finished();
      return;
    }

    // Convert value to object.
    Local<Object> callbackObj = Nan::To<Object>(callbackValue).ToLocalChecked();
    if (!callbackObj->IsFunction()) {
      ::ClientUser::Finished();
      return;
    }

    // Invoke the callback.
    Nan::CallAsFunction(callbackObj, callbackObj, 0, NULL);
  }
}
