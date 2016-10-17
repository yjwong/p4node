#include "nan.h"
#include "ClientUserWrapper.h"

using v8::Local;
using v8::Object;
using v8::Value;

namespace p4node {
  void ClientUserWrapper::SetCallbacks(Local<Object> callbacksObj) {
    _callbacksObj.Reset(callbacksObj);
  }

  void ClientUserWrapper::HandleError(::Error *err) {
    Local<Object> callbacksObj = Nan::New(_callbacksObj);
    Local<Value> callback = Nan::Get(
      callbacksObj,
      Nan::New("HandleError").ToLocalChecked()
    ).ToLocalChecked();

    if (!callback->IsFunction()) {
      ::ClientUser::HandleError(err);
    } else {
      
    }
  }
}
