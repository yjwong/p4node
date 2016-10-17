{
  "targets": [
    {
      "target_name": "p4node",
      "sources": [
        "src/p4node.cc",
        "src/ClientApi.cc",
        "src/ClientUser.cc",
        "src/ClientUserWrapper.cc",
        "src/Error.cc",
        "src/FileSys.cc",
        "src/StrDict.cc",
        "src/StrPtr.cc"
      ],
      "cflags_cc!": [],
      "dependencies": [],
      "conditions": [
        ["OS=='win'", {
          "link_settings": {
            "libraries": [
              "ws2_32.lib",
              "<(module_root_dir)/deps/p4api/lib/libclient.lib",
              "<(module_root_dir)/deps/p4api/lib/librpc.lib",
              "<(module_root_dir)/deps/p4api/lib/libsupp.lib",
              "<(module_root_dir)/deps/p4api/lib/libp4sslstub.lib"
            ]
          }
        }]
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "<(module_root_dir)/deps/p4api/include"
      ]
    },
    {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [ "<(module_name)" ],
      "copies": [
        {
          "files": [ "<(PRODUCT_DIR)/<(module_name).node" ],
          "destination": "<(module_path)"
        }
      ]
    }
  ]
}
