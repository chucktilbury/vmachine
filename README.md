# Virtual Machine

These notes are random thoughts about a potential virtual machine that I could create.

### High level

A program can have many modules. The modules are controlled by an ```import``` statement. When an import is encountered by the compiler, the contents of an already compiled module are copied into the current module and integrated with the value store so that the symbols contained in it can be accessed as if they were local. The normal access controls such as ```public``` and ```private``` apply as would normally be expected. There can only be classes and imports defined in a module. All classes in the compile object are global. Classes in imported objects can be set to be ```public``` or ```private```. Methods in a class can also be set to be ```protected```. The normal semantics apply. The compiler produces a single glob of code and data that represents the program. The entire glob is loaded into memory at runtime. 

All objects in the program are stored in a single global variable table. (GVT) That includes class definitions and constants. Everything. Things like scope are implemented virtually using serial numbers. When the definitions below refer to a variable store in a class, for example, it is an array of unsigned integers that represent indexes into this global array. When the VM allocates space for an object that is being defined at runtime, that is also defined and allocated into this global array.

The instruction stream consists of single 8 bit instructions and 32 bit operands. The first thing encountered in an instruction stream must be an instruction. 

The virtual machine maintains a single value stack. Values are maintained there to facilitate calling functions and eveluating expressions. All values are objects. 

## General features

* Compile stage and run stage are clearly separated. There is nothing in the VM that can add something that is not the result of a computation.
* Global heap is maintained by the VM as indexes, rather than pointers. Ultimately pointers are managed by the C runtime using an off-the-shelf garbage collector.

* Stack based to support a single pass compiler.
* Execute serialized objects
* Supports classes
* Supports garbage collection
* Supports exceptions

* Supports threads ??? 
* Support a single inheritance
* Does not support nested functions or classes (closures)
* Strong type system that supports diverse casting

* The execution stack uses a frame pointer with its own instruction stream and variable store and expression stack.
* Variables are stored in the instruction stream by index into the store.
* The compiler could store variables in something like a hash table, but that has to be converted to a linear array for the VM.

* All types are objects and all objects can have methods added via monkey patch. This includes literal values such as strings or numbers.
* All types have a default text representation.
* Lists and dictionaries are supported directly as native types. Lists and dictionaries are also strongly typed.
* Void type supported
* NULL value supported (Nothing or Noth)
* Support bitwise operations on numbers

## Instructions

Has the usual instructions for arithmetic and comparison operations. The binary and unary operators operate on the top of the value stack. Operators that produce a result push that result back to the top of the stack. All arithmetic and comparison operators update the result register.  If the result was zero, then the result register is set ```jump if``` instructions will branch. Branch taken instructions also clear the result.

* **noop**

  No operation. Has no effect on any registers but the instruction pointer

* **return**

  Causes the current frame to be popped and the instruction pointer to be loaded. The caller is expected to adjust the stack pointer to point to the return address that was pushed for the call. Then the instruction pointer is loaded and the number of parameters that were pushed on the stack for the call are popped and the return value of the call is pushed on the stack. If there is no return value then the immediate parameter is zero and zero is pushed on the stack. Otherwise the parameter is the index of the value in the value store.

* **call**

  Causes a new frame to be pushed on the call stack and the instruction pointer updated to point to the frame's instruction stream. Before this instruction can be used, the function parameters are pushed, then the number of parameters are pushed. When this instruction is execute, the address of the next instruction (an index into the instruction stream) is pushed.

* **callex**

  Calls an external function by index into the external function definition store. Returns to the same instruction. Pushes a value on the value stack as the return value. If there is no return value, then it's the responsibility of the caller to clear it. If the result is a pointer from the OS, then the caller has to allocate it in the VM to give access to the rest of the program.

* **end**

  Cause the VM to exit.

* **jmp**

  Loads the next word as a signed relative index of the instruction stream. Does not change the stack.

* **jmpif**

  Jump using a signed index from the current location if the result register is set. Otherwise do not jump. Clears the result register. Does not change the stack.

* **alloc**

  Allocate an object on the heap. This pushes an index onto the value stack for access by the caller. An object has a particular structure that supports things like a value... See below.

* **free**

  Manually deallocate a memory object from the VM's heap, outside of the garbage collection.

* **push**

  Push a value on the value stack.

* **pop**

  Pop a value from the value stack. This does not "return" the value. It simply removes it from, the top of the stack.

* **except**

  Invokes an exception. The immediate operand is always a ```vlong``` operand. The current frame is searched to see if the correct handler is present. If not, then the frame is popped and the new frame is searched and so on until either a handler is found or there are no more handlers on the stack. If there are no more handlers, then a frame stack trace is printed and the VM aborts due to an unhandled exception.

## Object structure

Objects all have a common structure. Some of the object elements are built in and the program can add items to the object. Since all things are objects in the VM, then this creates a hierarchy of objects in the data structure.

Here is a partial list of standard elements in an object.

* **__type**

  This is a numeric value that defines the type of the object. User defined objects must be given a serial number when they are created. When an object is allocated by ```alloc```, it is given a new serial number. The user cannot override this but they can retrieve it.

* **__hash**

  This is a numeric value that is given to the object when it is loaded into the VM. This can be accessed by the user, but it may not be modified.

* **__name**

  The name of the object is optional. It is automatically defined for literals. The user can modify this value.

* **__value**

  This is the object payload for things like numbers and strings. Things like classes and frames have the frame lists or instruction streams as payloads. (among other things)

* **__string**

  String value of the object. All objects must have a string value, but it could be generated "on the fly". User defined objects have the simple name unless the element is overridden.

* **__compare**

  There must be a way to compare objects. For example, numbers have the default semantics defined automatically. User defined objects always compare the internal pointer unless the compare function has been overridden by the user.

## Class structure

A class is the basic unit of execution. All code that can be run is defined inside a class. All classes are global and are stored in the global object store. A class is a collection of two kinds of objects. These objects are:

* **variable objects**

  These are stored in the value store. Any kind of data can be store in the class value store, such as references to other classes, but they are store in the value store.

* **frame store**

  The frame store contains all of the code that is defined by the class. 

## Frame structure


Frames are objects that have support for executing instructions. These have all of the same data elements as other objects plus a few. A frame has the following elements:

* **instruction pointer**

  This is the index of the currently executing instruction in the current frame. 

* **instruction stream**

  An array of bytes that includes the instructions and their immediate operands.

* **value store**

  Values that are local to this frame. This value store mirrors the global value store. 

TODO: Think about frames stacks. Is there a single global frame stack, or do from have their own frame stack.

Note that the compiler can support in-line assembler by creating a frame and putting correct data in the instruction stream.

## Value storage

Values are stored in arrays and they are accessed by indexes. The indexes are encoded such that the actual store that they are located in can be ascertained at run time.  There are at least 3 value stores. These virtual locations of variables are encoded into the index such that the location of the variable can be decoded from the instruction stream.

* **global variables**

  These values are available to all frames. 

* **class variables**

  Class members include all objects, including methods, that are defined for a class. These are only accessible by frames that are defined by the class.

* **local variables**

  Local objects are only accessible by a frame.

## External functions

External functions are functions that are not defined in the instruction stream. Usually they are implemented by an external library that can be loaded at runtime. The interface is defined by compiled code and before they are called, the parameters have to be pushed on the value stack along with the number of parameters and the returning address. When the external function returns, the RETURN instruction is implied and the parameters are popped, etc. 

##  Runtime heap

The runtime heap is managed by the VM. It is a value store that is held separately from the compile-time value store. To ensure that the runtime heap values have unique indexes, the number of compile time objects is added or subtracted. Naturally, all garbage collection happens on the runtime heap using the value stack.

## File format

The file format describes the serialized program. It is in several sections and the compiler is responsible for properly linking modules together. This describes generally what that looks like. 

The global value table does not contain the names of objects as a search-able item. Instead all references to all objects are by indexes into the global value table. All of the sections of code are concatenated together and "fixed up" so that the indexes correctly match up. If there is a literal, then it is written to the output file. String literals are managed with a special data structure that allows the VM to load them. Data that is defined, but not initialized, such as class variables, have a spot reserved for them in the GVT.

The first instruction in the instruction stream is a jump to the entry point of the program. 