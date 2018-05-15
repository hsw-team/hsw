# hsw

it is a final project for our data structure class.

**Apr. 20th 2018**

**Have dones**:
* Add toolbar and many icons of actions. So does the status buffer below. It means when you want to show some message, you can simply add this in your function:
```cpp
statusBar()->showMessage("this is the message", 2000) // last 2 sec
```
![image_1cbhn7pj61phdsa9a8vh3t19o5c.png-35.8kB][4]

  [4]: http://static.zybuluo.com/EtoDemerzel/j2kkxzd9e90io3x5lokiae6j/image_1cbhn7pj61phdsa9a8vh3t19o5c.png

As the pic shows above, when I put my cursor in the icon of "New", there will be a tool tip around it and a status tip at the botton of the window.

 
* Most of the actions of the menu/toolbar are not connected with any slots. For example *Copy/Paste/Print etc*.
* The first step of cursor movement.(done by DaoXu Sheng)

**To do**:
* **File operations**: Only the file-openning operation done.
* **Text operations**. *Select\Find\substitute\Redo\Undo etc.*
* **ScrollArea**.
