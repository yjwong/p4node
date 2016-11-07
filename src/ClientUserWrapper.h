#ifndef P4NODE_CLIENT_USER_WRAPPER_H
#define P4NODE_CLIENT_USER_WRAPPER_H

#include "node.h"
#include "p4/clientapi.h"

namespace p4node {
  class ClientUserWrapper : public ::ClientUser {
  public:
    void SetCallbacks(v8::Local<v8::Object> callbacksObj);

    void HandleError(::Error *err);
    void Message(::Error *err);
    void OutputError(const char *errBuf);
    void OutputInfo(char level, const char *data);
    void OutputBinary(const char* data, int length);
    void OutputText(const char* data, int length);

    void OutputStat(::StrDict *varList);

    void ErrorPause(char *errBuf, ::Error *err);

    void Edit(::FileSys *fileSys, ::Error *err);
    void Diff(::FileSys *fileSys1, ::FileSys *fileSys2, int doPage,
      char *diffFlags, ::Error *err);
    void Diff(::FileSys *fileSys1, ::FileSys *fileSys2, ::FileSys *fileSysOut,
      int doPage, char* diffFlags, ::Error *err);
    void Merge(::FileSys *base, ::FileSys *fileSys1, ::FileSys *fileSys2,
      ::FileSys *result, ::Error *err);
    void Help(const char *const *help);

    ::FileSys* File(::FileSysType type);
    int ProgressIndicator();

    void Finished();

    void SetOutputCharset(int charset);
    void DisableTmpCleanup();
    void SetQuiet();
    int CanAutoLoginPrompt();

  private:
    Nan::Persistent<v8::Object> _callbacksObj;
  };
}

#endif /* P4NODE_CLIENT_USER_WRAPPER_H */
