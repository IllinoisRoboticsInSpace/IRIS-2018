How to use these test files.

Still trying to figure out how to use named pipes to transfer data from one program to another,
But these test files are an intro to how it might work.

What are named pipes? (https://en.wikipedia.org/wiki/Named_pipe)
  Named pipes are FIFO (First in First Out) files.
  They can be used to pass data to another program by having two programs read the same file.

To run this test, compile both Reader2.cpp and Writer2.cpp
-g++ Writer2.cpp -o writer2
-g++ Reader2.cpp -o reader2

Then open two seperate terminals and run both simultaniously, starting wither writer2.
-./writer2
-./reader2
