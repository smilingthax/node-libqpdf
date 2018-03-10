{
  "targets": [{
    "target_name": "binding",
    "sources": [
      "src/main.cc",
      "src/helper.cc",
      "src/OpenSSL_RandomDataProvider.cc",
      "src/QPDFObjectHandle.cc"
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
