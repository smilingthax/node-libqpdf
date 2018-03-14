"use strict"

module.exports=require('./build/Release/binding.node');

module.exports.QPDFObjGen.prototype.inspect = function() {
  return '<' + this.constructor.name+' '+this.getObj()+' '+this.getGen()+'>';
};

const QPDFObject = module.exports.QPDFObject;

module.exports.QPDFObjectHandle.prototype.inspect = function() {
  var extra;
  switch (this.getTypeCode()) {
  case QPDFObject.ot_boolean:
    extra = ': ' + this.getBoolValue();
    break;
  case QPDFObject.ot_integer:
    extra = ': ' + this.getNumericValue(); // TODO? ...
    break;
  case QPDFObject.ot_real:
    extra = ': ' + this.getNumericValue(); // TODO? (string vs. double)
//    extra = ': ' + this.getRealValue();
    break;
  case QPDFObject.ot_name:
    extra = ': ' + this.getName();
    break;
  default:
    extra = '';
    break;
  }
  return '<' + this.constructor.name+' '+this.getTypeName()+extra+'>';
};

