libqpdf native binding for nodejs
===

Based on https://github.com/qpdf/qpdf (version 8.0.2).

The interface deliberately follows the C++ interface, no attempt to
JS-ify the interface is made, except that std::vector is mapped to a JS Array
and std::set / std::map is a Set / Map.

Methods returning a std::string in C++ will return a JS Buffer or a JS String,
depending on what seems to be more appropriate in the particular case.

As JS can only represent integers up to 2**53 accurately,
QPDFObjectHandle::getIntValue (returns 64 bit integer) is not wrapped.
Use QPDFObjectHandle::getNumericValue().

TODO:

- add more methods of QPDFObjectHandle
- QPDFObjectHandle::parse from Buffer
- create wrappers for QPDF and QPDFWriter (esp. io/streaming [note: reader needs random access])

#### Specials:
npm run install --with-libjpeg
(requires -ljpeg to be available)

---

Copyright (c) 2018 Tobias Hoffmann

License: https://opensource.org/licenses/Apache-2.0

