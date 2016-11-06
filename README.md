p4node
======
### Node.js Interface to Perforce

This is a set of bindings from `p4api` to node. `p4api` is a C++ library
used to access the Perforce Helix Versioning Engine.

Development
-----------

You will need `p4api` from Perforce's website. You can get it from:
https://www.perforce.com/downloads/helix

Download the appropriate archive, and extract into `deps/p4api`.

To compile the native code bindings, do this:

``` bash
$ cd p4node
$ node-pre-gyp configure build
```

Assuming it built without errors, you should be able to run the example and
then write programs that interface with Perforce servers.

Usage
-----

There is currently no documentation for the moment. Feel free to contribute
if you have some.

License
-------

MIT License. See the `LICENSE` file.
