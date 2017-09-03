# Makes an array of images for the Arduino sketch lightshow the persistence of vision wand.
# Each image is an array of unsigned LONGs that record on/off of each pixel in a COLUMN

import numpy as np
import cv2
import numpy as np
import os
#Get a list of all the BMP files in the current directory
def file_list(directory='filestoconvert', file_ext='.bmp'):
    return [x for x in os.listdir(directory) if x.lower().endswith(file_ext.lower())]

image_size = 32 #number pixels high and wide of each image
directory = 'filestoconvert'
bmps = file_list()
f = open('my_images.h', "w")

f.write('int numOfImages = {};\n'.format(len(bmps)))
f.write('int sizeOfImages = {};\n'.format(image_size))
f.write('// Array of images to display. Each image is made up of 16 longs; one long per column. C0R1 is most sig. bit. \n')
f.write('unsigned long images[{}][{}] = {{\n'.format(len(bmps), image_size))

for bmp in bmps:
    #Read the image from the file.
    img = cv2.imread(directory + '\\' + bmp, 0)
    img = np.transpose(img) # get read the image a column at a time
    f.write('// {}\n'.format(bmp)) # output the name of the image as a comment
    f.write('{')
    for col in img:
        pixels_as_num = 0;
        for pixel in col:
            pixels_as_num = pixels_as_num << 1;
            if pixel == 0:  # 0 means black in CV2
                pixels_as_num += 1
        f.write('{}, '.format(pixels_as_num))
    f.write('}, \n') # End of this image
f.write('};\n') # End of array of images
f.close()
