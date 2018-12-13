// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dOdOdIDAQDecoderdISubSysEventDict

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
   static TClass *PixelDatacLcLSubSystemscLcLChannelInfo_Dictionary();
   static void PixelDatacLcLSubSystemscLcLChannelInfo_TClassManip(TClass*);
   static void *new_PixelDatacLcLSubSystemscLcLChannelInfo(void *p = 0);
   static void *newArray_PixelDatacLcLSubSystemscLcLChannelInfo(Long_t size, void *p);
   static void delete_PixelDatacLcLSubSystemscLcLChannelInfo(void *p);
   static void deleteArray_PixelDatacLcLSubSystemscLcLChannelInfo(void *p);
   static void destruct_PixelDatacLcLSubSystemscLcLChannelInfo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PixelData::SubSystems::ChannelInfo*)
   {
      ::PixelData::SubSystems::ChannelInfo *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::PixelData::SubSystems::ChannelInfo));
      static ::ROOT::TGenericClassInfo 
         instance("PixelData::SubSystems::ChannelInfo", "../DAQDecoder/ChannelInfo.h", 14,
                  typeid(::PixelData::SubSystems::ChannelInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &PixelDatacLcLSubSystemscLcLChannelInfo_Dictionary, isa_proxy, 4,
                  sizeof(::PixelData::SubSystems::ChannelInfo) );
      instance.SetNew(&new_PixelDatacLcLSubSystemscLcLChannelInfo);
      instance.SetNewArray(&newArray_PixelDatacLcLSubSystemscLcLChannelInfo);
      instance.SetDelete(&delete_PixelDatacLcLSubSystemscLcLChannelInfo);
      instance.SetDeleteArray(&deleteArray_PixelDatacLcLSubSystemscLcLChannelInfo);
      instance.SetDestructor(&destruct_PixelDatacLcLSubSystemscLcLChannelInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PixelData::SubSystems::ChannelInfo*)
   {
      return GenerateInitInstanceLocal((::PixelData::SubSystems::ChannelInfo*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::PixelData::SubSystems::ChannelInfo*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *PixelDatacLcLSubSystemscLcLChannelInfo_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::PixelData::SubSystems::ChannelInfo*)0x0)->GetClass();
      PixelDatacLcLSubSystemscLcLChannelInfo_TClassManip(theClass);
   return theClass;
   }

   static void PixelDatacLcLSubSystemscLcLChannelInfo_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *PixelDatacLcLSubSystemscLcLEvent_Dictionary();
   static void PixelDatacLcLSubSystemscLcLEvent_TClassManip(TClass*);
   static void *new_PixelDatacLcLSubSystemscLcLEvent(void *p = 0);
   static void *newArray_PixelDatacLcLSubSystemscLcLEvent(Long_t size, void *p);
   static void delete_PixelDatacLcLSubSystemscLcLEvent(void *p);
   static void deleteArray_PixelDatacLcLSubSystemscLcLEvent(void *p);
   static void destruct_PixelDatacLcLSubSystemscLcLEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PixelData::SubSystems::Event*)
   {
      ::PixelData::SubSystems::Event *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::PixelData::SubSystems::Event));
      static ::ROOT::TGenericClassInfo 
         instance("PixelData::SubSystems::Event", "../DAQDecoder/Event.h", 14,
                  typeid(::PixelData::SubSystems::Event), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &PixelDatacLcLSubSystemscLcLEvent_Dictionary, isa_proxy, 4,
                  sizeof(::PixelData::SubSystems::Event) );
      instance.SetNew(&new_PixelDatacLcLSubSystemscLcLEvent);
      instance.SetNewArray(&newArray_PixelDatacLcLSubSystemscLcLEvent);
      instance.SetDelete(&delete_PixelDatacLcLSubSystemscLcLEvent);
      instance.SetDeleteArray(&deleteArray_PixelDatacLcLSubSystemscLcLEvent);
      instance.SetDestructor(&destruct_PixelDatacLcLSubSystemscLcLEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PixelData::SubSystems::Event*)
   {
      return GenerateInitInstanceLocal((::PixelData::SubSystems::Event*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::PixelData::SubSystems::Event*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *PixelDatacLcLSubSystemscLcLEvent_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::PixelData::SubSystems::Event*)0x0)->GetClass();
      PixelDatacLcLSubSystemscLcLEvent_TClassManip(theClass);
   return theClass;
   }

   static void PixelDatacLcLSubSystemscLcLEvent_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_PixelDatacLcLSubSystemscLcLChannelInfo(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::PixelData::SubSystems::ChannelInfo : new ::PixelData::SubSystems::ChannelInfo;
   }
   static void *newArray_PixelDatacLcLSubSystemscLcLChannelInfo(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::PixelData::SubSystems::ChannelInfo[nElements] : new ::PixelData::SubSystems::ChannelInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_PixelDatacLcLSubSystemscLcLChannelInfo(void *p) {
      delete ((::PixelData::SubSystems::ChannelInfo*)p);
   }
   static void deleteArray_PixelDatacLcLSubSystemscLcLChannelInfo(void *p) {
      delete [] ((::PixelData::SubSystems::ChannelInfo*)p);
   }
   static void destruct_PixelDatacLcLSubSystemscLcLChannelInfo(void *p) {
      typedef ::PixelData::SubSystems::ChannelInfo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PixelData::SubSystems::ChannelInfo

namespace ROOT {
   // Wrappers around operator new
   static void *new_PixelDatacLcLSubSystemscLcLEvent(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::PixelData::SubSystems::Event : new ::PixelData::SubSystems::Event;
   }
   static void *newArray_PixelDatacLcLSubSystemscLcLEvent(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::PixelData::SubSystems::Event[nElements] : new ::PixelData::SubSystems::Event[nElements];
   }
   // Wrapper around operator delete
   static void delete_PixelDatacLcLSubSystemscLcLEvent(void *p) {
      delete ((::PixelData::SubSystems::Event*)p);
   }
   static void deleteArray_PixelDatacLcLSubSystemscLcLEvent(void *p) {
      delete [] ((::PixelData::SubSystems::Event*)p);
   }
   static void destruct_PixelDatacLcLSubSystemscLcLEvent(void *p) {
      typedef ::PixelData::SubSystems::Event current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PixelData::SubSystems::Event

namespace ROOT {
   static TClass *vectorlEfloatgR_Dictionary();
   static void vectorlEfloatgR_TClassManip(TClass*);
   static void *new_vectorlEfloatgR(void *p = 0);
   static void *newArray_vectorlEfloatgR(Long_t size, void *p);
   static void delete_vectorlEfloatgR(void *p);
   static void deleteArray_vectorlEfloatgR(void *p);
   static void destruct_vectorlEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<float>*)
   {
      vector<float> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<float>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<float>", -2, "vector", 210,
                  typeid(vector<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEfloatgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<float>) );
      instance.SetNew(&new_vectorlEfloatgR);
      instance.SetNewArray(&newArray_vectorlEfloatgR);
      instance.SetDelete(&delete_vectorlEfloatgR);
      instance.SetDeleteArray(&deleteArray_vectorlEfloatgR);
      instance.SetDestructor(&destruct_vectorlEfloatgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<float> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<float>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<float>*)0x0)->GetClass();
      vectorlEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEfloatgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<float> : new vector<float>;
   }
   static void *newArray_vectorlEfloatgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<float>[nElements] : new vector<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEfloatgR(void *p) {
      delete ((vector<float>*)p);
   }
   static void deleteArray_vectorlEfloatgR(void *p) {
      delete [] ((vector<float>*)p);
   }
   static void destruct_vectorlEfloatgR(void *p) {
      typedef vector<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<float>

namespace ROOT {
   static TClass *vectorlEPixelDatacLcLSubSystemscLcLChannelInfogR_Dictionary();
   static void vectorlEPixelDatacLcLSubSystemscLcLChannelInfogR_TClassManip(TClass*);
   static void *new_vectorlEPixelDatacLcLSubSystemscLcLChannelInfogR(void *p = 0);
   static void *newArray_vectorlEPixelDatacLcLSubSystemscLcLChannelInfogR(Long_t size, void *p);
   static void delete_vectorlEPixelDatacLcLSubSystemscLcLChannelInfogR(void *p);
   static void deleteArray_vectorlEPixelDatacLcLSubSystemscLcLChannelInfogR(void *p);
   static void destruct_vectorlEPixelDatacLcLSubSystemscLcLChannelInfogR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<PixelData::SubSystems::ChannelInfo>*)
   {
      vector<PixelData::SubSystems::ChannelInfo> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<PixelData::SubSystems::ChannelInfo>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<PixelData::SubSystems::ChannelInfo>", -2, "vector", 210,
                  typeid(vector<PixelData::SubSystems::ChannelInfo>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEPixelDatacLcLSubSystemscLcLChannelInfogR_Dictionary, isa_proxy, 0,
                  sizeof(vector<PixelData::SubSystems::ChannelInfo>) );
      instance.SetNew(&new_vectorlEPixelDatacLcLSubSystemscLcLChannelInfogR);
      instance.SetNewArray(&newArray_vectorlEPixelDatacLcLSubSystemscLcLChannelInfogR);
      instance.SetDelete(&delete_vectorlEPixelDatacLcLSubSystemscLcLChannelInfogR);
      instance.SetDeleteArray(&deleteArray_vectorlEPixelDatacLcLSubSystemscLcLChannelInfogR);
      instance.SetDestructor(&destruct_vectorlEPixelDatacLcLSubSystemscLcLChannelInfogR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<PixelData::SubSystems::ChannelInfo> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<PixelData::SubSystems::ChannelInfo>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEPixelDatacLcLSubSystemscLcLChannelInfogR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<PixelData::SubSystems::ChannelInfo>*)0x0)->GetClass();
      vectorlEPixelDatacLcLSubSystemscLcLChannelInfogR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEPixelDatacLcLSubSystemscLcLChannelInfogR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEPixelDatacLcLSubSystemscLcLChannelInfogR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<PixelData::SubSystems::ChannelInfo> : new vector<PixelData::SubSystems::ChannelInfo>;
   }
   static void *newArray_vectorlEPixelDatacLcLSubSystemscLcLChannelInfogR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<PixelData::SubSystems::ChannelInfo>[nElements] : new vector<PixelData::SubSystems::ChannelInfo>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEPixelDatacLcLSubSystemscLcLChannelInfogR(void *p) {
      delete ((vector<PixelData::SubSystems::ChannelInfo>*)p);
   }
   static void deleteArray_vectorlEPixelDatacLcLSubSystemscLcLChannelInfogR(void *p) {
      delete [] ((vector<PixelData::SubSystems::ChannelInfo>*)p);
   }
   static void destruct_vectorlEPixelDatacLcLSubSystemscLcLChannelInfogR(void *p) {
      typedef vector<PixelData::SubSystems::ChannelInfo> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<PixelData::SubSystems::ChannelInfo>

namespace {
  void TriggerDictionaryInitialization_SubSysEventDict_Impl() {
    static const char* headers[] = {
"../DAQDecoder/Event.h",
"../DAQDecoder/ChannelInfo.h",
0
    };
    static const char* includePaths[] = {
"/home/argonshef/root-6.08.00/builddir/include",
"/home/argonshef/LArAnalysis/srcs/SubSystems/CRT/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "SubSysEventDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace PixelData{namespace SubSystems{class __attribute__((annotate("$clingAutoload$../DAQDecoder/Event.h")))  ChannelInfo;}}
namespace PixelData{namespace SubSystems{class __attribute__((annotate("$clingAutoload$../DAQDecoder/Event.h")))  Event;}}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "SubSysEventDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "../DAQDecoder/Event.h"
#include "../DAQDecoder/ChannelInfo.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"PixelData::SubSystems::ChannelInfo", payloadCode, "@",
"PixelData::SubSystems::Event", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("SubSysEventDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_SubSysEventDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_SubSysEventDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_SubSysEventDict() {
  TriggerDictionaryInitialization_SubSysEventDict_Impl();
}
