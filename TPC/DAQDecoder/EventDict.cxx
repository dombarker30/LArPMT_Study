// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dOdOdIDAQDecoderdIEventDict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "../DAQDecoder/Event.h"
#include "../DAQDecoder/ChannelInfo.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *PixelDatacLcLTPCcLcLChannelInfo_Dictionary();
   static void PixelDatacLcLTPCcLcLChannelInfo_TClassManip(TClass*);
   static void *new_PixelDatacLcLTPCcLcLChannelInfo(void *p = 0);
   static void *newArray_PixelDatacLcLTPCcLcLChannelInfo(Long_t size, void *p);
   static void delete_PixelDatacLcLTPCcLcLChannelInfo(void *p);
   static void deleteArray_PixelDatacLcLTPCcLcLChannelInfo(void *p);
   static void destruct_PixelDatacLcLTPCcLcLChannelInfo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PixelData::TPC::ChannelInfo*)
   {
      ::PixelData::TPC::ChannelInfo *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::PixelData::TPC::ChannelInfo));
      static ::ROOT::TGenericClassInfo 
         instance("PixelData::TPC::ChannelInfo", "../DAQDecoder/ChannelInfo.h", 14,
                  typeid(::PixelData::TPC::ChannelInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &PixelDatacLcLTPCcLcLChannelInfo_Dictionary, isa_proxy, 4,
                  sizeof(::PixelData::TPC::ChannelInfo) );
      instance.SetNew(&new_PixelDatacLcLTPCcLcLChannelInfo);
      instance.SetNewArray(&newArray_PixelDatacLcLTPCcLcLChannelInfo);
      instance.SetDelete(&delete_PixelDatacLcLTPCcLcLChannelInfo);
      instance.SetDeleteArray(&deleteArray_PixelDatacLcLTPCcLcLChannelInfo);
      instance.SetDestructor(&destruct_PixelDatacLcLTPCcLcLChannelInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PixelData::TPC::ChannelInfo*)
   {
      return GenerateInitInstanceLocal((::PixelData::TPC::ChannelInfo*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::PixelData::TPC::ChannelInfo*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *PixelDatacLcLTPCcLcLChannelInfo_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::PixelData::TPC::ChannelInfo*)0x0)->GetClass();
      PixelDatacLcLTPCcLcLChannelInfo_TClassManip(theClass);
   return theClass;
   }

   static void PixelDatacLcLTPCcLcLChannelInfo_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *PixelDatacLcLTPCcLcLEvent_Dictionary();
   static void PixelDatacLcLTPCcLcLEvent_TClassManip(TClass*);
   static void *new_PixelDatacLcLTPCcLcLEvent(void *p = 0);
   static void *newArray_PixelDatacLcLTPCcLcLEvent(Long_t size, void *p);
   static void delete_PixelDatacLcLTPCcLcLEvent(void *p);
   static void deleteArray_PixelDatacLcLTPCcLcLEvent(void *p);
   static void destruct_PixelDatacLcLTPCcLcLEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PixelData::TPC::Event*)
   {
      ::PixelData::TPC::Event *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::PixelData::TPC::Event));
      static ::ROOT::TGenericClassInfo 
         instance("PixelData::TPC::Event", "../DAQDecoder/Event.h", 13,
                  typeid(::PixelData::TPC::Event), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &PixelDatacLcLTPCcLcLEvent_Dictionary, isa_proxy, 4,
                  sizeof(::PixelData::TPC::Event) );
      instance.SetNew(&new_PixelDatacLcLTPCcLcLEvent);
      instance.SetNewArray(&newArray_PixelDatacLcLTPCcLcLEvent);
      instance.SetDelete(&delete_PixelDatacLcLTPCcLcLEvent);
      instance.SetDeleteArray(&deleteArray_PixelDatacLcLTPCcLcLEvent);
      instance.SetDestructor(&destruct_PixelDatacLcLTPCcLcLEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PixelData::TPC::Event*)
   {
      return GenerateInitInstanceLocal((::PixelData::TPC::Event*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::PixelData::TPC::Event*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *PixelDatacLcLTPCcLcLEvent_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::PixelData::TPC::Event*)0x0)->GetClass();
      PixelDatacLcLTPCcLcLEvent_TClassManip(theClass);
   return theClass;
   }

   static void PixelDatacLcLTPCcLcLEvent_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_PixelDatacLcLTPCcLcLChannelInfo(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::PixelData::TPC::ChannelInfo : new ::PixelData::TPC::ChannelInfo;
   }
   static void *newArray_PixelDatacLcLTPCcLcLChannelInfo(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::PixelData::TPC::ChannelInfo[nElements] : new ::PixelData::TPC::ChannelInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_PixelDatacLcLTPCcLcLChannelInfo(void *p) {
      delete ((::PixelData::TPC::ChannelInfo*)p);
   }
   static void deleteArray_PixelDatacLcLTPCcLcLChannelInfo(void *p) {
      delete [] ((::PixelData::TPC::ChannelInfo*)p);
   }
   static void destruct_PixelDatacLcLTPCcLcLChannelInfo(void *p) {
      typedef ::PixelData::TPC::ChannelInfo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PixelData::TPC::ChannelInfo

namespace ROOT {
   // Wrappers around operator new
   static void *new_PixelDatacLcLTPCcLcLEvent(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::PixelData::TPC::Event : new ::PixelData::TPC::Event;
   }
   static void *newArray_PixelDatacLcLTPCcLcLEvent(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::PixelData::TPC::Event[nElements] : new ::PixelData::TPC::Event[nElements];
   }
   // Wrapper around operator delete
   static void delete_PixelDatacLcLTPCcLcLEvent(void *p) {
      delete ((::PixelData::TPC::Event*)p);
   }
   static void deleteArray_PixelDatacLcLTPCcLcLEvent(void *p) {
      delete [] ((::PixelData::TPC::Event*)p);
   }
   static void destruct_PixelDatacLcLTPCcLcLEvent(void *p) {
      typedef ::PixelData::TPC::Event current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PixelData::TPC::Event

namespace ROOT {
   static TClass *vectorlEunsignedsPintgR_Dictionary();
   static void vectorlEunsignedsPintgR_TClassManip(TClass*);
   static void *new_vectorlEunsignedsPintgR(void *p = 0);
   static void *newArray_vectorlEunsignedsPintgR(Long_t size, void *p);
   static void delete_vectorlEunsignedsPintgR(void *p);
   static void deleteArray_vectorlEunsignedsPintgR(void *p);
   static void destruct_vectorlEunsignedsPintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<unsigned int>*)
   {
      vector<unsigned int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<unsigned int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<unsigned int>", -2, "vector", 210,
                  typeid(vector<unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEunsignedsPintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<unsigned int>) );
      instance.SetNew(&new_vectorlEunsignedsPintgR);
      instance.SetNewArray(&newArray_vectorlEunsignedsPintgR);
      instance.SetDelete(&delete_vectorlEunsignedsPintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEunsignedsPintgR);
      instance.SetDestructor(&destruct_vectorlEunsignedsPintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<unsigned int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<unsigned int>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEunsignedsPintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<unsigned int>*)0x0)->GetClass();
      vectorlEunsignedsPintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEunsignedsPintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEunsignedsPintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned int> : new vector<unsigned int>;
   }
   static void *newArray_vectorlEunsignedsPintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned int>[nElements] : new vector<unsigned int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEunsignedsPintgR(void *p) {
      delete ((vector<unsigned int>*)p);
   }
   static void deleteArray_vectorlEunsignedsPintgR(void *p) {
      delete [] ((vector<unsigned int>*)p);
   }
   static void destruct_vectorlEunsignedsPintgR(void *p) {
      typedef vector<unsigned int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<unsigned int>

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = 0);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 210,
                  typeid(vector<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<int>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<int>*)0x0)->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace ROOT {
   static TClass *vectorlEPixelDatacLcLTPCcLcLChannelInfogR_Dictionary();
   static void vectorlEPixelDatacLcLTPCcLcLChannelInfogR_TClassManip(TClass*);
   static void *new_vectorlEPixelDatacLcLTPCcLcLChannelInfogR(void *p = 0);
   static void *newArray_vectorlEPixelDatacLcLTPCcLcLChannelInfogR(Long_t size, void *p);
   static void delete_vectorlEPixelDatacLcLTPCcLcLChannelInfogR(void *p);
   static void deleteArray_vectorlEPixelDatacLcLTPCcLcLChannelInfogR(void *p);
   static void destruct_vectorlEPixelDatacLcLTPCcLcLChannelInfogR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<PixelData::TPC::ChannelInfo>*)
   {
      vector<PixelData::TPC::ChannelInfo> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<PixelData::TPC::ChannelInfo>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<PixelData::TPC::ChannelInfo>", -2, "vector", 210,
                  typeid(vector<PixelData::TPC::ChannelInfo>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEPixelDatacLcLTPCcLcLChannelInfogR_Dictionary, isa_proxy, 0,
                  sizeof(vector<PixelData::TPC::ChannelInfo>) );
      instance.SetNew(&new_vectorlEPixelDatacLcLTPCcLcLChannelInfogR);
      instance.SetNewArray(&newArray_vectorlEPixelDatacLcLTPCcLcLChannelInfogR);
      instance.SetDelete(&delete_vectorlEPixelDatacLcLTPCcLcLChannelInfogR);
      instance.SetDeleteArray(&deleteArray_vectorlEPixelDatacLcLTPCcLcLChannelInfogR);
      instance.SetDestructor(&destruct_vectorlEPixelDatacLcLTPCcLcLChannelInfogR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<PixelData::TPC::ChannelInfo> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<PixelData::TPC::ChannelInfo>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEPixelDatacLcLTPCcLcLChannelInfogR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<PixelData::TPC::ChannelInfo>*)0x0)->GetClass();
      vectorlEPixelDatacLcLTPCcLcLChannelInfogR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEPixelDatacLcLTPCcLcLChannelInfogR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEPixelDatacLcLTPCcLcLChannelInfogR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<PixelData::TPC::ChannelInfo> : new vector<PixelData::TPC::ChannelInfo>;
   }
   static void *newArray_vectorlEPixelDatacLcLTPCcLcLChannelInfogR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<PixelData::TPC::ChannelInfo>[nElements] : new vector<PixelData::TPC::ChannelInfo>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEPixelDatacLcLTPCcLcLChannelInfogR(void *p) {
      delete ((vector<PixelData::TPC::ChannelInfo>*)p);
   }
   static void deleteArray_vectorlEPixelDatacLcLTPCcLcLChannelInfogR(void *p) {
      delete [] ((vector<PixelData::TPC::ChannelInfo>*)p);
   }
   static void destruct_vectorlEPixelDatacLcLTPCcLcLChannelInfogR(void *p) {
      typedef vector<PixelData::TPC::ChannelInfo> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<PixelData::TPC::ChannelInfo>

namespace {
  void TriggerDictionaryInitialization_EventDict_Impl() {
    static const char* headers[] = {
"../DAQDecoder/Event.h",
"../DAQDecoder/ChannelInfo.h",
0
    };
    static const char* includePaths[] = {
"/home/argonshef/root-6.08.00/builddir/include",
"/home/argonshef/LArAnalysis/srcs/TPC/Analysis/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "EventDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace PixelData{namespace TPC{class __attribute__((annotate("$clingAutoload$../DAQDecoder/Event.h")))  ChannelInfo;}}
namespace PixelData{namespace TPC{class __attribute__((annotate("$clingAutoload$../DAQDecoder/Event.h")))  Event;}}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "EventDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "../DAQDecoder/Event.h"
#include "../DAQDecoder/ChannelInfo.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"PixelData::TPC::ChannelInfo", payloadCode, "@",
"PixelData::TPC::Event", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("EventDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_EventDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_EventDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_EventDict() {
  TriggerDictionaryInitialization_EventDict_Impl();
}
