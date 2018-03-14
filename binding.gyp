{
  "targets": [{
    "target_name": "binding",
    "sources": [
      "src/main.cc",
      "src/enums.cc",
      "src/helper.cc",
      "src/OpenSSL_RandomDataProvider.cc",
      "src/QPDFObjectHandle.cc",
      "src/QPDFObjGen.cc",
      "src/QPDF.cc",
      "src/QPDFWriter.cc"
    ],
    "dependencies": ["qpdf.gyp:libqpdf"],
    "include_dirs": [
      "<!(node -e \"require('nan')\")"
    ],
    "includes": [
      "exceptions.gypi"
    ]
  }]
}
