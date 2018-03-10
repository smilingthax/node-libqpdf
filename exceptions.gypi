{
# https://github.com/nodejs/node-gyp/issues/939
#  "msvs_settings": {
#    "VCCLCompilerTool": {
#      "ExceptionHandling": 1
#    }
#  },
  "conditions": [
#    ['OS=="win"', {
#      'defines': [ '_HAS_EXCEPTIONS=1' ]
#    }],
    [ 'OS=="linux" or OS=="freebsd" or OS=="openbsd" or OS=="solaris"', {
      "cflags_cc!": ['-fno-exceptions']
    }],
    [ 'OS=="mac"', {
      'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
    }]
  ]
}
