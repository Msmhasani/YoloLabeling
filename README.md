# YoloLabeling

Easy to use Qt application to draw bounding boxes for making text file that are needed to train the images with YOLO.

Put all of your images into a folder. All images must be with **.bmp** extenstion.

After drawing and all the boxes you want, on the image, push **Accept** button. It will save a text file with the same name with the image, which can be feed directly to YOLO for training (No need for further modification.

It also saves a copy of the image with **.jpg** extenstion and the exact same name, having all the boxes, drawn in it. this image is not going to be used in Yolo, but it's for you to check your drawings.

**Notice:**

Right now it only works for **2-class** applications. If you draw the each box with **left-click, it'll be class '0'** and if you draw it with **right-click, it'll be class '1'**.
