{
  "variables": {
    "with_libjpeg%": 0   # do (not) require libjpeg. cfg.default/qpdf/Pl_DCT.hh injects no-op decoder
  },
  "targets": [{
    "target_name": "libqpdf",
    "type": "static_library",
    "sources": [
      "qpdf/libqpdf/BitStream.cc",
      "qpdf/libqpdf/BitWriter.cc",
      "qpdf/libqpdf/Buffer.cc",
      "qpdf/libqpdf/BufferInputSource.cc",
      "qpdf/libqpdf/ContentNormalizer.cc",
      "qpdf/libqpdf/FileInputSource.cc",
      "qpdf/libqpdf/InputSource.cc",
      "qpdf/libqpdf/InsecureRandomDataProvider.cc",
      "qpdf/libqpdf/MD5.cc",
      "qpdf/libqpdf/OffsetInputSource.cc",
      "qpdf/libqpdf/Pipeline.cc",
      "qpdf/libqpdf/Pl_AES_PDF.cc",
      "qpdf/libqpdf/Pl_ASCII85Decoder.cc",
      "qpdf/libqpdf/Pl_ASCIIHexDecoder.cc",
      "qpdf/libqpdf/Pl_Buffer.cc",
      "qpdf/libqpdf/Pl_Concatenate.cc",
      "qpdf/libqpdf/Pl_Count.cc",
      "qpdf/libqpdf/Pl_Discard.cc",
      "qpdf/libqpdf/Pl_Flate.cc",
      "qpdf/libqpdf/Pl_LZWDecoder.cc",
      "qpdf/libqpdf/Pl_MD5.cc",
      "qpdf/libqpdf/Pl_PNGFilter.cc",
      "qpdf/libqpdf/Pl_QPDFTokenizer.cc",
      "qpdf/libqpdf/Pl_RC4.cc",
      "qpdf/libqpdf/Pl_RunLength.cc",
      "qpdf/libqpdf/Pl_SHA2.cc",
      "qpdf/libqpdf/Pl_StdioFile.cc",
      "qpdf/libqpdf/Pl_TIFFPredictor.cc",
      "qpdf/libqpdf/QPDF.cc",
      "qpdf/libqpdf/QPDFExc.cc",
      "qpdf/libqpdf/QPDFObjGen.cc",
      "qpdf/libqpdf/QPDFObject.cc",
      "qpdf/libqpdf/QPDFObjectHandle.cc",
      "qpdf/libqpdf/QPDFTokenizer.cc",
      "qpdf/libqpdf/QPDFWriter.cc",
      "qpdf/libqpdf/QPDFXRefEntry.cc",
      "qpdf/libqpdf/QPDF_Array.cc",
      "qpdf/libqpdf/QPDF_Bool.cc",
      "qpdf/libqpdf/QPDF_Dictionary.cc",
      "qpdf/libqpdf/QPDF_InlineImage.cc",
      "qpdf/libqpdf/QPDF_Integer.cc",
      "qpdf/libqpdf/QPDF_Name.cc",
      "qpdf/libqpdf/QPDF_Null.cc",
      "qpdf/libqpdf/QPDF_Operator.cc",
      "qpdf/libqpdf/QPDF_Real.cc",
      "qpdf/libqpdf/QPDF_Reserved.cc",
      "qpdf/libqpdf/QPDF_Stream.cc",
      "qpdf/libqpdf/QPDF_String.cc",
      "qpdf/libqpdf/QPDF_encryption.cc",
      "qpdf/libqpdf/QPDF_linearization.cc",
      "qpdf/libqpdf/QPDF_optimization.cc",
      "qpdf/libqpdf/QPDF_pages.cc",
      "qpdf/libqpdf/QTC.cc",
      "qpdf/libqpdf/QUtil.cc",
      "qpdf/libqpdf/RC4.cc",
      "qpdf/libqpdf/SecureRandomDataProvider.cc",
      "qpdf/libqpdf/qpdf-c.cc",
      "qpdf/libqpdf/rijndael.cc",
      "qpdf/libqpdf/sha2.c",
      "qpdf/libqpdf/sha2big.c"
    ],
    "include_dirs": [
      "cfg.default",
      "qpdf/libqpdf",
      "qpdf/include"
    ],
    "direct_dependent_settings": {
      "include_dirs": [
        "qpdf/include"
      ]
    },
    "conditions": [
      [ 'with_libjpeg != 0', {
        "sources": [
          "qpdf/libqpdf/Pl_DCT.cc",
        ],
        "defines": ["WITH_LIBJPEG"],
        "direct_dependent_settings": {
          "libraries": ['-ljpeg']
        }
      }],
      [ 'OS=="linux" or OS=="freebsd" or OS=="openbsd" or OS=="solaris"', {
        "cflags_cc!": ['-fno-rtti']
      }],
      [ 'OS=="mac"', {
        'GCC_ENABLE_CPP_RTTI': 'YES'
      }]
    ],
    "includes": [
      "exceptions.gypi"
    ]
  }]
}
