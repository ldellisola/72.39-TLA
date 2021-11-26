# VidTrx

## Installation

In order to use our VidTrx compiler, we need to set up some dependencies. Please install the following:
- [dotnet 6 or higher](https://dotnet.microsoft.com/download/dotnet/6.0)
- [ffmpeg](https://ffmpeg.org/download.html)
- [gcc](https://gcc.gnu.org/install/)
- [flex](https://github.com/westes/flex)
- [bison](https://www.gnu.org/software/bison/)

And add them to your PATH variable. We also provide a [script](./install.sh) to set everything in a second!

==TODO==

## Compilation
 ==TODO==


## Language Definition

VidTrix is a domain specific language developed exclusively to make video transformation operations simple and fast. The language is equiped with specific data types and operators to handle this use case.

### Entry point

Every VidTrx program must have an entrypoint. This entry point is defined as `do:`. So, for example if we wanted to do a simple "helloworld" program, it will look like this:
```
do:
:! "Hello, World!";
```

### Data Types

VidTrx has 6 data types:

- `Int`
- `Float`
- `String`
- `Bool`
- `Video`
- `AudioStream`
- `VideoStream`

### Operators

Similar to most languages, VidTrx suports aritmetic, logical and comparison operators, as well as our own custom set of video operators.

#### Artitmetic Operators

- The operator `+` has two functions. It allows the aritmetic operation of addition between two numerics (`Int` or `Float`), while at the same time they support string concatenation and string conversion. Almost any data type can be concatenated to `String`.

  | Type          | `Int`     | `Float`   | `String`  | `Bool`    | `Video`   | `AudioStream` | `VideoStream` |
  | ------------- | --------- | --------- | --------- | --------- | --------- | ------------- | ------------- |
  | `Int`         | `Int`     | `Float`   | `String`  | `Unknown` | `Unknown` | `Unknown`     | `Unknown`     |
  | `Float`       | `Float`   | `Float`   | `String`  | `Unknown` | `Unknown` | `Unknown`     | `Unknown`     |
  | `String`      | `String`  | `String`  | `String`  | `String`  | `Unknown` | `Unknown`     | `Unknown`     |
  | `Bool`        | `Unknown` | `Unknown` | `String`  | `Unknown` | `Unknown` | `Unknown`     | `Unknown`     |
  | `Video`       | `Unknown` | `Unknown` | `Unknown` | `Unknown` | `Unknown` | `Unknown`     | `Unknown`     |
  | `AudioStream` | `Unknown` | `Unknown` | `Unknown` | `Unknown` | `Unknown` | `Unknown`     | `Unknown`     |
  | `VideoStream` | `Unknown` | `Unknown` | `Unknown` | `Unknown` | `Unknown` | `Unknown`     | `Unknown`     |
  ```
   "number " + 1 = "number 1";
   "bool value " + true = "bool value true";
   123 + 0.5 = 123.5;
  ```
- The rest of the aritmetic operators `-`, `*`, and `/ ` only have their aritmentic function, and they can only be use among numeric types such as `Int` and `Float`.

#### Logical Operators

VidTrx supports the classic logical operators such as logical or (`||`), logical and (`&&`) and the negation operator (`!`). These operators only accept operations or variables or type `Bool`.
```
true || false;
true && true;
!false;
```

#### Comparison operators

Comparison operators allow to compare different data with each other. In this leanguage only the following operators are currently supported:
- `==`
- `!=`
- `>`
- `<`
In most cases, only data of the same type can be compared, so for example, the next line would not be valid:
```
1 == "1";
```
But there is one exception. Comparison across different types is only allowed if both elements are either `Int` or `Float`. Here we have some examples:
```
true == false;
"hola" != "chau";
1 > 1.4;
123 < 321;
```
Another detail to mention is that for the operators `>` and `<` only numeric types (`Int` or `Float`) are allowed;

#### Video Operators

Video operators are what sets this language appart from the rest. We implemented a set of operators that make handeling video easier and intuitive.
- The `{` opens a video file. From the right it will take a string and it will return a `Video` type on the left:
	
	```
	Video video { "path/to/file.mp4";
	```
	
-  The `<-` operator extracts the video stream from a `Video` variable into an `VideoStream` object:
	```
	VideoStream stream <- video;
	```
	This operator also accepts parameters to transcode the video. We can use the following parameters:
	- `codec`: It takes a `String` with the name of the codec. For example `"h264"`
	- `bitrate`: It takes a `Float` as the desired bitrate.
	- `speed`: It takes a `Float` as the desired speed. This value must be between `0.5` and `2.0`.
	- `size`: This is the resolution of the transcoded video. Even though it is a `String`, it must follow the format `"widthxheigth"` such as `"1080x720"`.
	- `framerate`: This parameter specifies the desired framerate. It has to be a `Float`.
	
	To use these parameters, we must call the operator as:
	
	```
	VideoStream stream <- codec: "h264", size: "1080x720", speed: 0.7 <- video;
	```
	
-  The `<~` operator extracts the audio stream from a `Video` variable into an `AudioStream` object:
	```
	AudioStream stream <~ video;
	```
	This operator also accepts parameters to transcode the audio. We can use the following parameters:
	- `codec`: It takes a `String` with the name of the codec. For example `"aac"`
	- `bitrate`: It takes a `Float` as the desired bitrate.
	- `speed`: It takes a `Float` as the desired speed. This value must be between `0.5` and `2.0`.
	- `channels`: This parameter specifies the numer of channels of the resulting audio stream. The value is an `Int`.
	
	To use these parameters, we must call the operator as:
	
	```
	AudioStream stream <- codec: "aac", channels: 3, speed: 0.7 <- video;
	```
	
- The `| | <<` operators will take streams and save them in a new file. This operator has the following definition:

	```
	| new_file | << stream_1 << stream_2 << .. << stream_n;
	```
	Where `new_file` is a string that represents the path to the new file we want to create, and the streams are already transcoded or simply extracted streams from other files.
	There is one more adition to this operator. If we add the symbol `!` in front, then we are forcing the file operator to overwrite the existing file in case there is one. This does not work if you pass the name of a file you are using, as it cannot overwrite a file it is using.
	```
	! | "path/to/new_file.mp4" | << stream_1 << stream_2;
	```

### Functions
This language does not allow the user to define their own functions, but we do have some functions of our own:

- The function `:!` allows the user to print to print anything data that can be concatenated to a `String`. Here's an example: 

	```
	:! "Hello, world!";
	```
	
- The function `:?` does the oposite, it allows the user to read a line from the console.

	```
	String input;
	:? input;
	```
	
- The function `datatype[value]` allows the programmer to parse a `String` into a desired type such as `Int`, `Bool` or `Float`. The correct use case is:

	```
	Int number = Int["123"];
	Float other_number = Float["123.456"];
	Bool boolean_val = Bool["true"];
	```

### Control Flow Statements
In VidTrx we only support two control flow statements:
- The `if/else` statement allows the user to evaluate a condition and do an action if it is valid, while doing another one if it's not. It is used as:

	```
	if (condition) then
	  do_something;
	end
	```
	
	We can also use the other variation of this statement:
	```
	if (condition) then
		do_something;
	else
		do_other_thing;
	end
	```
	
- The `while` statement allows the user to do loops on their programs:

	```
	while (condition) do
		do_something;
	end
	```
### Comments

Comments in VidTrx are simple, if you wish to comment out a line you just have to use the characters `//`. For example:
```
// This is a comment
:! "This prints stuff";
```

### C# Scripting
Because our language runs over C# and the dotnet runtime, we can pass through code and run it natively. In order to do this, we have to use the following operators:
```
^^
Console.WriteLine("Hello, World!");
^^
```
This is quite powerfull, as programmers can use native functions, data types and methods from the C# language. You can even interact with variables and code from VidTrx!
There are several uses, for example:
- We can use advanced string operations:

	```
	String full_name = "John Appleseed";
	String first_name = "";
	String last_name = "";
	^^
	var list = full_name.Split(' ');
	first_name = list[0];
	last_name = list[1]
	^^
	:! "first name: " + first_name + ", last name: " + last_name;
  ```
  
- We can receive arguments from the command line:

	```
	String arg = "";
	^^ 
	arg = args[0];
	^^
	:! "parameter: " + arg;
	```
	
#### Warning

While this feature is quite powerful (at least until our language is fully featured), it is not a *best practice* and it is **<u>not</u>** supported. 
This means that code from these blocks are not checked for type safety, reference errors or just invalid C# code. 
Another issue with it are the compilation errors. The compiler will simply pass through the code to the MsBuild (C# compiler). If there are any errors, these will probably be hard to debug and fix, as the errors will be over our transpiled program. 

##### Examples of possible errors

The following code does not compile, because the variable `arg` was never intialized in VidTrx code.
```
	String arg;
	^^ 
	arg = args[0];
	^^
	:! "parameter: " + arg;
```

The following code also does not compile, because the variabler `arg` was never declared in VidTrx:
```
	^^ 
	string arg = args[0];
	^^
	:! "parameter: " + arg;
```