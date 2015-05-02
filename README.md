It uses OpenCv library to filter Image and have made own codes for Image Segmentation using Adaptive Background Subtraction and for Skeleton generation using Morphological Operations.

Just run the code :
g++ `pkg-config --cflags opencv` my_bagsub.cpp `pkg-config --libs opencv` -o bag
./bag
