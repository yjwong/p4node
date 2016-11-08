#include <node.h>
#include "Error.h"
#include "StrPtr.h"
#include "StrDict.h"

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::Local;
using v8::Number;
using v8::Array;
using v8::Int32;
using v8::Value;
using v8::String;
using v8::Function;
using v8::External;

namespace p4node {
  Nan::Persistent<FunctionTemplate> StrDict::constructor_template;

  StrDict::StrDict(::StrDict* obj) : _obj(obj) {}

  StrDict::~StrDict() {}

  ::StrDict* StrDict::Unwrap() {
    return _obj;
  }

  void StrDict::Init(Handle<Object> exports) {
    Nan::HandleScope scope;

    // Prepare constructor template
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("StrDict").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "SetVar", SetVar);
    Nan::SetPrototypeMethod(tpl, "SetArgv", SetArgv);

    Nan::SetPrototypeMethod(tpl, "GetVar", GetVar);

    constructor_template.Reset(tpl);
    exports->Set(Nan::New("StrDict").ToLocalChecked(), tpl->GetFunction());
  }

  NAN_METHOD(StrDict::New) {
    Nan::HandleScope scope;
    info.GetReturnValue().Set(info.This());
  }

  NAN_METHOD(StrDict::SetVar) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("SetVar requires at least 1 argument");
    }

    StrDict* obj = ObjectWrap::Unwrap<StrDict>(info.This());

    if (info.Length() == 1) {
      // void	SetVar( const char *var );
      if (!info[0]->IsString()) {
        return Nan::ThrowTypeError("First argument must be a string");
      }

      Nan::Utf8String* var = new Nan::Utf8String(info[0]);
      obj->_obj->SetVar(var->operator*());

    } else if (info.Length() == 2 || info.Length() == 3) {
      // void	SetVar( const char *var, int value );
      // void	SetVar( const char *var, const char *value );
	    // void	SetVar( const char *var, const StrPtr *value );
	    // void	SetVar( const char *var, const StrPtr &value );
	    // void	SetVar( const StrPtr &var, const StrPtr &value );
      if (!info[0]->IsString() && !info[0]->IsObject()) {
        return Nan::ThrowTypeError("First argument must be either a string or a StrPtr object");
      }

      if (info[0]->IsString()) {
        // 2nd arg could be int, char or StrPtr.
        Nan::Utf8String* var = new Nan::Utf8String(info[0]);
        if (info[1]->IsInt32()) {
          Local<Int32> value = Nan::To<Int32>(info[1]).ToLocalChecked();
          obj->_obj->SetVar(var->operator*(), value->Value());
        } else if (info[1]->IsString()) {
          Nan::Utf8String* value = new Nan::Utf8String(info[1]);
          obj->_obj->SetVar(var->operator*(), value->operator*());
        } else if (info[1]->IsObject()) {
          Local<Object> valueObj = info[1]->ToObject();
          StrPtr* value = ObjectWrap::Unwrap<StrPtr>(valueObj);
          obj->_obj->SetVar(var->operator*(), value->Unwrap());
        } else {
          return Nan::ThrowTypeError("Second argument must be either an integer, string or StrPtr object");
        }
      } else {
        // 2nd arg must be a StrPtr.
        if (!info[1]->IsObject()) {
          return Nan::ThrowTypeError("Second argument must be a StrPtr object");
        }

        Local<Object> varObj = info[0]->ToObject();
        Local<Object> valueObj = info[1]->ToObject();
        StrPtr* var = ObjectWrap::Unwrap<StrPtr>(varObj);
        StrPtr* value = ObjectWrap::Unwrap<StrPtr>(valueObj);
        obj->_obj->SetVar(*var->Unwrap(), *value->Unwrap());
      }
      
    } else if (info.Length() == 3) {
      // void	SetVar( const StrPtr &var, int x, const StrPtr &val );
      // void	SetVar( const char *var, int x, const StrPtr &val );
      // Second argument is always an int, and third argument is always a StrPtr.
      if (!info[1]->IsInt32()) {
        return Nan::ThrowTypeError("Second argument must be an integer");
      }

      if (!info[2]->IsObject()) {
        return Nan::ThrowTypeError("Third argument must be a StrPtr object");
      }

      Local<Int32> x = Nan::To<Int32>(info[1]).ToLocalChecked();
      Local<Object> valObj = info[2]->ToObject();
      StrPtr* val = ObjectWrap::Unwrap<StrPtr>(valObj);

      if (info[0]->IsString()) {
        Nan::Utf8String* var = new Nan::Utf8String(info[0]);
        obj->_obj->SetVar(var->operator*(), x->Value(), *val->Unwrap());
      } else if (info[0]->IsObject()) {
        Local<Object> varObj = info[0]->ToObject();
        StrPtr* var = ObjectWrap::Unwrap<StrPtr>(varObj);
        obj->_obj->SetVar(*var->Unwrap(), x->Value(), *val->Unwrap());
      } else {
        return Nan::ThrowTypeError("First argument must be either a string or a StrPtr object");
      }

    } else if (info.Length() == 4) {
      // void	SetVar( const char *var, int x, int y, const StrPtr &val );
      if (!info[0]->IsString()) {
        return Nan::ThrowTypeError("First argument must be a string");
      }

      if (!info[1]->IsInt32()) {
        return Nan::ThrowTypeError("Second argument must be an integer");
      }

      if (!info[2]->IsInt32()) {
        return Nan::ThrowTypeError("Third argument must be an integer");
      }

      if (!info[3]->IsObject()) {
        return Nan::ThrowTypeError("Fourth argument must be a StrPtr object");

        Nan::Utf8String* var = new Nan::Utf8String(info[0]);
        Local<Int32> x = Nan::To<Int32>(info[1]).ToLocalChecked();
        Local<Int32> y = Nan::To<Int32>(info[2]).ToLocalChecked();
        Local<Object> valObj = info[3]->ToObject();
        StrPtr* val = ObjectWrap::Unwrap<StrPtr>(valObj);

        obj->_obj->SetVar(var->operator*(), x->Value(), y->Value(), *val->Unwrap());
      }
    } else {
      return Nan::ThrowTypeError("SetVar requires between 1 to 4 arguments");
    }

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(StrDict::SetArgv) {
    Nan::HandleScope scope;

    if (info.Length() < 2) {
      return Nan::ThrowTypeError("SetArgv requires at least 2 arguments");
    }

    if (!info[0]->IsNumber()) {
      return Nan::ThrowTypeError("First argument must be a number");
    }

    if (!info[1]->IsArray()) {
      return Nan::ThrowTypeError("Second argument must be an array");
    }

    Local<Int32> argcNumber = Nan::To<Int32>(info[0]).ToLocalChecked();
    Local<Array> argvArray = Local<Array>::Cast(info[1]);
    char** charArgvArray = new char*[argvArray->Length()];

    // For every string in the array, convert it to a C-style string.
    for (uint32_t i = 0; i < argvArray->Length(); i++) {
      Local<Value> arrayItem = Nan::Get(argvArray, i).ToLocalChecked();
      if (!arrayItem->IsString()) {
        return Nan::ThrowTypeError("Elements of second argument must be strings");
      }

      Nan::Utf8String* charArgvItem = new Nan::Utf8String(arrayItem->ToString());
      charArgvArray[i] = charArgvItem->operator*();
    }

    StrDict* dict = ObjectWrap::Unwrap<StrDict>(info.This());
    dict->_obj->SetArgv(argcNumber->Value(), charArgvArray);

    info.GetReturnValue().Set(Nan::Undefined());
  }

  NAN_METHOD(StrDict::GetVar) {
    Nan::HandleScope scope;

    if (info.Length() < 1) {
      return Nan::ThrowTypeError("GetVar requires at least 1 argument");
    }

    StrDict* obj = ObjectWrap::Unwrap<StrDict>(info.This());
    ::StrPtr* retval = NULL;

    if (info[0]->IsString()) {
      Nan::Utf8String* var = new Nan::Utf8String(info[0]);
      if (info.Length() == 1) {
        // StrPtr *GetVar( const char *var );
        obj->_obj->GetVar(var->operator*());
      } else if (info.Length() == 2) {
	      // StrPtr *GetVar( const char *var, Error *e );
        if (!info[1]->IsObject()) {
          return Nan::ThrowTypeError("Second argument must be an Error object");
        }

        Local<Object> errObj = Nan::To<Object>(info[1]).ToLocalChecked();
        Error* err = ObjectWrap::Unwrap<Error>(errObj);
        retval = obj->_obj->GetVar(var->operator*(), err->Unwrap());
      } else {
        return Nan::ThrowTypeError("GetVar requires between 1 to 2 arguments");
      }
    } else if (info[0]->IsObject()) {
      Local<Object> varObj = Nan::To<Object>(info[0]).ToLocalChecked();
      StrPtr* var = ObjectWrap::Unwrap<StrPtr>(varObj);

      if (info.Length() == 1) {
        // StrPtr *GetVar( const StrPtr &var );
        retval = obj->_obj->GetVar(*var->Unwrap());
      } else if (info.Length() == 2) {
        if (!info[1]->IsInt32()) {
          return Nan::ThrowTypeError("Second argument must be an integer");
        }

        Local<Int32> x = Nan::To<Int32>(info[1]).ToLocalChecked();
        retval = obj->_obj->GetVar(*var->Unwrap(), x->Value());
      } else if (info.Length() == 3) {
        if (!info[1]->IsInt32()) {
          return Nan::ThrowTypeError("Second argument must be an integer");
        }

        if (!info[2]->IsInt32()) {
          return Nan::ThrowTypeError("Third argument must be an integer");
        }

        Local<Int32> x = Nan::To<Int32>(info[1]).ToLocalChecked();
        Local<Int32> y = Nan::To<Int32>(info[2]).ToLocalChecked();
        retval = obj->_obj->GetVar(*var->Unwrap(), x->Value(), y->Value());
      } else {
        return Nan::ThrowTypeError("GetVar requires between 1 to 3 arguments");
      }

    } else {
      return Nan::ThrowTypeError("First argument must be either a string or a StrPtr object");
    }

    if (retval != NULL) {
      // Convert StrPtr object to managed StrPtr.
      Local<FunctionTemplate> tpl = Nan::New(StrPtr::constructor_template);
      Local<Function> func = tpl->GetFunction();
      Handle<Value> funcArgs[] = { Nan::New<External>(retval) };
      Local<Object> newObj = Nan::NewInstance(func, 1, funcArgs).ToLocalChecked();
      info.GetReturnValue().Set(newObj);
    } else {
      info.GetReturnValue().Set(Nan::Undefined());
    }
  }
}
