![Assembler project](https://user-images.githubusercontent.com/102487763/186521866-78914c94-1289-4dad-b196-caaa40f01815.png)


**🖥️ What is the `assembler` project?**
---

The purpose of the `assembler`project is to compile an Assembly file to a Binary file. This project is written using C language and contains 
10 C source code files, and 9 header files. 

For further information regarding my project - I added a toturial video about the project, a link to a documentation landing page and some more general information down below. 


**🔸Video explanation of how to run the project**
---
<a  href="https://www.youtube.com/watch?v=sKKPmYWuc_Y"><img src="http://easytutorials.net/wp-content/uploads/2022/01/video-tutorial-image.jpg" data-canonical-src="https://gyazo.com/eb5c5741b6a9a16c692170a41a49c858.png" width="450" height="360" /></a>        

**💻 Link to a documentation landing page **

https://shoppit.store/assembler-project/
---


**📝 Important features**
---
1. Debugging and finding errors like invalid amount of arguments for a command,Unrecognized labels and more.
2. Macro supporting: Option of writing part of code and give it a name and the assembler will replace the code:
```
macro m1
  mov r1, r2
  jmp LABLE   => The assembler will replace m1 with the macro code.
endmacro
...
m1
```
3. All assembly basic syntax like labels, data, numbers, strings, command...
4. extern and entry
    + extern: Import labels from other .am file
    + entry: Export labels from the current file to other .am file
    
**📁 OutPut Files**
---
1. am_file_name.bin => The assembly code after translation to binary devided by code and data, exemple: 
```
--- CODE ---
0000111100
0001001000
0000000000
0000001100

--- DATA ---
0000000001
0000000100
1111111101
0000000101
```
2. am_file_name.ob => Same meening as the `.bin` file but in base 32, exemple: 
```
%	%
$%   @s
$^   #<
$&   i%
$*   d#
$<   !^
$>   #@
$a   ##
$b   !!
```
3. am_file_name.ext => File with all the external import labels and with there place in the code (base 32)
```
X   $f
```
4. am_file_name.ent => Same like the `.ext` file but about the entry labels
```
MAIN   $%
DATA1   $q
```

