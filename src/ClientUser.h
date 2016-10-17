#ifndef P4NODE_CLIENT_USER_H
#define P4NODE_CLIENT_USER_H

#include <node.h>
#include <node_object_wrap.h>
#include "nan.h"
#include "ClientUserWrapper.h"

namespace p4node {
  class ClientUser : public node::ObjectWrap {
  public:
    static Nan::Persistent<v8::FunctionTemplate> constructor_template;
    static void Init(v8::Handle<v8::Object> exports);
    ClientUserWrapper* Unwrap();

  private:
    static NAN_METHOD(New);

    static NAN_METHOD(HandleError);
    static NAN_METHOD(Message);
    static NAN_METHOD(OutputError);
    static NAN_METHOD(OutputInfo);
    static NAN_METHOD(OutputBinary);
    static NAN_METHOD(OutputText);

    static NAN_METHOD(OutputStat);

    //static NAN_METHOD(Prompt);
    static NAN_METHOD(ErrorPause);

    static NAN_METHOD(Edit);
    static NAN_METHOD(Diff);
    static NAN_METHOD(Merge);
    //static NAN_METHOD(Resolve);
    static NAN_METHOD(Help);

    static NAN_METHOD(File);
    //static NAN_METHOD(CreateProgress);
    static NAN_METHOD(ProgressIndicator);

    static NAN_METHOD(Finished);

    static NAN_METHOD(SetOutputCharset);
    static NAN_METHOD(DisableTmpCleanup);
    static NAN_METHOD(SetQuiet);
    static NAN_METHOD(CanAutoLoginPrompt);

    static NAN_METHOD(SetCallbacks);

    static NAN_GETTER(GetVarList);
    static NAN_SETTER(SetVarList);
    //static NAN_GETTER(SetEnviro);
    //static NAN_GETTER(GetEnviro);

    explicit ClientUser();
    ~ClientUser();

    ClientUserWrapper* _obj;
  };
}

#endif /* P4NODE_CLIENT_USER_H */
