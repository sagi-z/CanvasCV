Multithreading {#multithreading}
==============
As GUI toolkits go, they are mostly not multithreaded.
The other threads do they work and then GUI handling (update/paint/callbacks) is done in a dedicated thread.
* [see a related example in this blog post](https://www.theimpossiblecode.com/blog/faster-opencv-smiles-tbb "dedicated GUI thread")
