# MNK (Least-Squares-Method)
## About the program
Was inspired by a linear fit(regression) function in scientific programs like [OriginLab](https://en.wikipedia.org/wiki/Origin_(data_analysis_software)). 
Current program creates a chart with linear approximation for series of experimantal data points (and with data points themselves) from a text file. File is filled up with data by a user in advance.
Program also shows the data points from the file to the user in a practical text form.

Program built with Qt 6.5.0 on Windows 11 64-bit, and is designed exlusively for this operating system.
With all the dependencies it weights ~50 megabytes.
Memory usage - up to ~25 megabytes on average, depending on the input data size, how the window is resized and how long app is intended to be used.

## Usage
At first download and open `MNK_4_0_setup.exe` and install the program following fairly easy guidelines. It will add shortcuts to the desktop and the Start Menu. Your program now is ready to be used. 
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
- floating point numbers are allowed (as well as negative numbers), you can use commas or dots in floating point numbers (even both).
- you can use as many spaces or tabs to split x and y values of one point as you want, though it is recomended to keep it readable.

Once the file is ready browse to its location (in MNK's file browser) or paste its location to the field of the program and press button "FIT", if there are problems with the file you will see an appropriate message, if not - you will be shown a chart. To exit simply press the "EXIT" button.

To uninstall the program open the file browser and find `PATH\WHERE\YOU\INSTALLED\MNK_4_0\uninstall.exe`. 

## Working program screenshots:
![image](https://github.com/Andriy-Bilenko/MNK-Least-Squares-Method-/blob/main/src/Working_program_screenshot_1.png)
![image](https://github.com/Andriy-Bilenko/MNK-Least-Squares-Method-/blob/main/src/Working_program_screenshot_2.png)
![image](https://github.com/Andriy-Bilenko/MNK-Least-Squares-Method-/blob/main/src/Working_program_screenshot_3.png)
## What's new compared to the previous versions
- Now commas are accepted in floating point numbers in data files.
- Fixed MNK's file explorer resizing.
- Shows errors for slope and intercept in equation. <sub>(correctness was tested using OriginPro 9.1 32-bit with the same data sets)</sub>



If you ran into some issues by any chance or need to contact the developer, it would be great to recieve your valuable feedback on email: *bilenko.a.uni@gmail.com*.

<div align="right">
<table><td>
<a href="#start-of-content">↥ Scroll to top</a>
</td></table>
</div>