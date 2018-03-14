#include <nan.h>
#include <qpdf/QPDFObjectHandle.hh>

#define QOH_BOOL_GETTER \
  X(isInitialized, {}) \
  X(isBool,        { obj.assertInitialized(); }) \
  X(isNull,        { obj.assertInitialized(); }) \
  X(isInteger,     { obj.assertInitialized(); }) \
  X(isReal,        { obj.assertInitialized(); }) \
  X(isName,        { obj.assertInitialized(); }) \
  X(isString,      { obj.assertInitialized(); }) \
  X(isOperator,    { obj.assertInitialized(); }) \
  X(isInlineImage, { obj.assertInitialized(); }) \
  X(isArray,       { obj.assertInitialized(); }) \
  X(isDictionary,  { obj.assertInitialized(); }) \
  X(isStream,      { obj.assertInitialized(); }) \
  X(isReserved,    { obj.assertInitialized(); }) \
  X(isIndirect,    {}) \
  X(isScalar,      { obj.assertInitialized(); }) \
  X(isNumber,      { obj.assertInitialized(); }) \
  X(hasObjectDescription, {}) \
  X(getBoolValue,  { obj.assertInitialized(); }) \
  X(isDataModified,{ obj.assertInitialized(); }) \
  X(isPageObject,  { obj.assertInitialized(); }) \
  X(isPagesObject, { obj.assertInitialized(); })

#define QOH_GETTER \
  X(getTypeCode,     obj.getTypeCode())               \
  X(getTypeName,     Nan::New(obj.getTypeName()).ToLocalChecked()) \
  X(getRealValue,    (obj.assertInitialized(), ToString(obj.getRealValue()))) \
  X(getNumericValue, (obj.assertInitialized(), obj.getNumericValue())) \
  X(getName,         (obj.assertInitialized(), ToString(obj.getName()))) \
  X(getStringValue,  (obj.assertInitialized(), ToBuffer(obj.getName()))) \
  X(getUTF8Value,    (obj.assertInitialized(), ToString(obj.getUTF8Value()))) \
  X(getOperatorValue,(obj.assertInitialized(), ToString(obj.getOperatorValue()))) \
  X(getInlineImageValue, (obj.assertInitialized(), ToBuffer(obj.getInlineImageValue()))) \
  X(getArrayNItems,  (obj.assertInitialized(), obj.getArrayNItems())) \
  X(getArrayAsVector,(obj.assertInitialized(), VecToArray(obj.getArrayAsVector()))) \
  X(getDictAsMap,    (obj.assertInitialized(), StrMapToMap(obj.getDictAsMap()))) \
  X(shallowCopy,     NewInstance(obj.shallowCopy()))  \
  X(getDict,         (obj.assertInitialized(), NewInstance(obj.getDict()))) \
  X(getObjGen,       QOGen::NewInstance(obj.getObjGen())) \
  X(getObjectID,     obj.getObjectID())               \
  X(getGeneration,   obj.getGeneration())             \
  X(unparse,         ToBuffer(obj.unparse()))         \
  X(unparseResolved, (obj.assertInitialized(), ToBuffer(obj.unparseResolved()))) \
  X(getPageImages,   (obj.assertInitialized(), StrMapToMap(obj.getPageImages()))) \
  X(getPageContents, (obj.assertInitialized(), VecToArray(obj.getPageContents())))

#define QOH_MUTATE_NOARG \
  X(makeDirect, { obj.makeDirect(); }) \
  X(coalesceContentStreams, { obj.coalesceContentStreams(); })

#define QOH_MANUAL \
  X(getArrayItem) \
  X(hasKey) \
  X(getKey) \
  X(getKeys) \
  X(isOrHasName) \
  X(setArrayItem) \
  X(setArrayFromVector) \
  X(insertItem) \
  X(appendItem) \
  X(eraseItem) \
  X(replaceKey) \
  X(removeKey) \
  X(replaceOrRemoveKey) \
  X(replaceDict) \
  X(addPageContents) \
  X(rotatePage)

#define QOH_STATIC_NEW \
  X(newNull) \
  X(newBool) \
  X(newInteger) \
  X(newReal) \
  X(newName) \
  X(newString) \
  X(newOperator) \
  X(newInlineImage) \
  X(newArray) \
  X(newDictionary)

class QOH : public Nan::ObjectWrap {
public:
  static inline QOH *Unwrap(v8::Local<v8::Object> obj) {
    return Nan::ObjectWrap::Unwrap<QOH>(obj);
  }

  static inline bool Is(v8::Local<v8::Value> arg) {
    // alternative: store <v8::Value> GetPrototype() and compare
    return Nan::New(tmpl)->HasInstance(arg);
  }

  static inline QPDFObjectHandle &Ref(v8::Local<v8::Value> arg) {
    // only safe after Is(arg) ...
    return Unwrap(arg->ToObject())->obj;
  }

  static v8::Local<v8::Object> NewInstance(const QPDFObjectHandle &value);

  static bool VecFromArray(std::vector<QPDFObjectHandle> &ret, v8::Local<v8::Value> arg); // may throw!
  static v8::Local<v8::Array> VecToArray(const std::vector<QPDFObjectHandle> &vec);
  static v8::Local<v8::Map> StrMapToMap(const std::map<std::string,QPDFObjectHandle> &map);

  static NAN_MODULE_INIT(Init);

private:

  static QOH *This(Nan::NAN_METHOD_ARGS_TYPE info);

  static NAN_METHOD(New);

  static NAN_METHOD(parse);

#define X(name, code) static NAN_METHOD(name);
  QOH_BOOL_GETTER
  QOH_GETTER
  QOH_MUTATE_NOARG
#undef X

#define X(name) static NAN_METHOD(name);
  QOH_MANUAL
  QOH_STATIC_NEW
#undef X

  static Nan::Persistent<v8::Function> constructor;
  static Nan::Persistent<v8::FunctionTemplate> tmpl;

  QPDFObjectHandle obj;
};

