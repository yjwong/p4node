#ifndef P4NODE_FILE_SYS_H
#define P4NODE_FILE_SYS_H

#include <node.h>
#include <node_object_wrap.h>
#include "nan.h"
#include "p4/clientapi.h"

namespace p4node {
  class FileSys : public node::ObjectWrap {
  public:
    static Nan::Persistent<v8::FunctionTemplate> constructor_template;
    static void Init(v8::Handle<v8::Object> exports);

  private:
    static NAN_METHOD(New);
    static NAN_METHOD(Chmod);
    static NAN_METHOD(Close);
    static NAN_METHOD(Create);
    static NAN_METHOD(Open);
    static NAN_METHOD(Read);
    static NAN_METHOD(Rename);
    static NAN_METHOD(Stat);
    static NAN_METHOD(StatModTime);
    static NAN_METHOD(Truncate);
    static NAN_METHOD(Unlink);
    static NAN_METHOD(Write);

    explicit FileSys(::FileSys* obj);
    ~FileSys();

    ::FileSys* _obj;
  };
}

#endif /* P4NODE_FILE_SYS_H */
