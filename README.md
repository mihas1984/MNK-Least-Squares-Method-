# MNK (Least-Squares-Method)
## About the program
Was inspired by a linear fit(regression) function in scientific programs like [OriginLab](https://en.wikipedia.org/wiki/Origin_(data_analysis_software)). 
Current program creates a chart with linear approximation for series of experimantal data points (and with data points themselves) from a text file. File is filled up with data by a user in advance.
Program also shows the data points from the file to the user in a practical text form.

Program built with Qt 6.4.2 on Windows 11 64-bit, and is designed exlusively for this operating system.
With all the dependencies it weights ~60 megabytes.
Memory usage - up to ~20 megabytes, depending on the input data size and how the window is resized.

## Usage
At first download and open `MNK_2_2_setup.exe` and install the program following fairly easy guidelines. It will add shortcuts to the desktop and the Start Menu. Your program now is ready to be used. 
Now it's time to get ready the file with experimantal data points. It should be a text file (.txt). The data inside should be formatted as follows:
```
1.2     2.43
23.1	-3.21
90	    1
12.3	6.6
```
Here the first column are the X values and the second are Y values, every line is one data point. 
### Requirements to the data format:
- there should be only two numbers in one line, no less - no more.
- there shouldn't be any empty lines.
- floating point numbers are allowed (as well as negative numbers), but **please mind: program doesn't accept commas**, meaning `.`(point) is allowed, but `,`(comma) is not.
- you can use as many spaces or tabs to split x and y values of one point as you want, though it is recomended to keep it readable.

Once the file is ready paste its location to the field of the program and press button "FIT", if there are problems with the file you will see an appropriate message, if not - you will be shown a chart. To exit simply press the "EXIT" button.

To uninstall the program open the file browser and find `PATH\WHERE\YOU\INSTALLED\MNK_2_2\uninstall.exe`. 

## Working program screenshots:
![image](https://github.com/Andriy-Bilenko/MNK-Least-Squares-Method-/blob/main/src/Working_program_screenshot1.png)
![image](https://github.com/Andriy-Bilenko/MNK-Least-Squares-Method-/blob/main/src/Working_program_screenshot2.png)
![image](https://github.com/Andriy-Bilenko/MNK-Least-Squares-Method-/blob/main/src/Working_program_screenshot3.png)
## What's new compared to the previous versions
- Added some lacking documentation, styled and cleaned the code.
- Changed the required format of data in an input file for the better.
- Added layouts to the window.
- Added some sleek looks and design.
- Added error checks.
- Cleared major memory leak. <sub>(version 1_4 program was loosing 0.5 megabytes per button click)</sub>
- Created better setup.exe for current version <sub>(previous version's setup.exe did not create shortcuts of a program; both made with NSIS 3.08)</sub>
- reduced the working program folder size twofold.
- now input data is visible inside of a program.



If you ran into some issues by any chance or need to contact the developer, it would be great to recieve your valuable feedback on email: *bilenko.a.uni@gmail.com*.