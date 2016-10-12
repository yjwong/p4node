#include <node.h>
#include "ClientApi.h"
#include "ClientUser.h"
#include "Error.h"
#include "StrDict.h"

using namespace v8;

namespace p4node {
  void InitAll(Handle<Object> exports) {
    // ClientApi depends on StrDict.
    StrDict::Init(exports);

    ClientApi::Init(exports);
    ClientUser::Init(exports);
    Error::Init(exports);
  }

  NODE_MODULE(p4node, InitAll);
}
