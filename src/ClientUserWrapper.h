#ifndef P4NODE_CLIENT_USER_WRAPPER_H
#define P4NODE_CLIENT_USER_WRAPPER_H

#include "node.h"
#include "p4/clientapi.h"

namespace p4node {
  class ClientUserWrapper : public ::ClientUser {
  public:
    void SetCallbacks(v8::Local<v8::Object> callbacksObj);

    void HandleError(::Error *err);

  private:
    Nan::Persistent<v8::Object> _callbacksObj;
  };
}

#endif /* P4NODE_CLIENT_USER_WRAPPER_H */
