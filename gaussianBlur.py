import cv2
import numpy as np

# https://github.com/SamratSahoo/UT-Arlington-Research/blob/master/Week%206%20-%20Convolutions%20%26%20Wavelet%20Transforms/Convolutions.py
'''
def convolve2D(image, kernel, padding=0, strides=1):
    # Cross Correlation
    kernel = np.flipud(np.fliplr(kernel))

    # Gather Shapes of Kernel + Image + Padding
    xKernShape = kernel.shape[0]
    yKernShape = kernel.shape[1]
    xImgShape = image.shape[0]
    yImgShape = image.shape[1]

    # Shape of Output Convolution
    xOutput = int(((xImgShape - xKernShape + 2 * padding) / strides) + 1)
    yOutput = int(((yImgShape - yKernShape + 2 * padding) / strides) + 1)
    output = np.zeros((xOutput, yOutput))

    # Apply Equal Padding to All Sides
    if padding != 0:
        imagePadded = np.zeros((image.shape[0] + padding*2, image.shape[1] + padding*2))
        imagePadded[int(padding):int(-1 * padding), int(padding):int(-1 * padding)] = image
        # print(imagePadded)
    else:
        imagePadded = image

    # Iterate through image
    for y in range(image.shape[1]):
        # Exit Convolution
        if y > image.shape[1] - yKernShape:
            break
        # Only Convolve if y has gone down by the specified Strides
        if y % strides == 0:
            for x in range(image.shape[0]):
                # Go to next row once kernel is out of bounds
                if x > image.shape[0] - xKernShape:
                    break
                try:
                    # Only Convolve if x has moved by the specified Strides
                    if x % strides == 0:
                        output[x, y] = (kernel * imagePadded[x: x + xKernShape, y: y + yKernShape]).sum()
                except:
                    break

    return output
'''

def conv2d(image,kernal,padding=0,strides=1):
      xKernShape=kernal.shape[0]
      yKernShape=kernal.shape[1]
      xImgShape=image.shape[0]
      yImgShape=image.shape[1]
      padding=int(padding)
      strides=int(strides)

      xOutput = int(((xImgShape - xKernShape + 2 * padding) / strides) + 1)
      yOutput = int(((yImgShape - yKernShape + 2 * padding) / strides) + 1)
  
      output = np.zeros((xOutput, yOutput))
      
      ImgPadded=None
      if padding!=0:
          ImgPadded=np.zeros((xImgShape+2*padding,yImgShape+2*padding))
          ImgPadded[padding:-padding,padding:-padding]=image
      else:
          ImgPadded=image
      
      j=0
      for y in range(ImgPadded.shape[1]):
          if y > ImgPadded.shape[1] - yKernShape:
              break
              
          if y%strides==0:
              i=0
              for x in range(ImgPadded.shape[0]):
                  if x > ImgPadded.shape[0] - xKernShape:
                      break
                  
                  if x%strides==0:
                      output[i,j]=np.sum(kernal*(ImgPadded[x:x+xKernShape,y:y+yKernShape]))
                      i+=1
              j+=1
      return output
  
  
def add_noise(img, n):
    img2 = img
    for _ in range(n):
        x = int(np.random.random() * img.shape[0])
        y = int(np.random.random() * img.shape[1])
        img2[x, y] = 255 # white in gray scale is 255
    return img2

if __name__ == '__main__':
    # Input original image file
    # To use another image input file, please change the filename
    # Also the downloaded 'standard_test_images/' should be in the same folder
    #filename='lena_color_256'
 
    filenames = ['lena_color_256', 'woman_blonde', 'pirate']
    
    for filename in filenames:
        img = cv2.imread(f'./standard_test_images/{filename}.tif')
        # Write out original image in bmp format
        cv2.imwrite(f'./preprocessed/{filename}.bmp', img)
        #print(f'img shape:{img.shape}') 
        #print(f'img:{img}') 


        # Convert original image to grayscale bmp
        gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        cv2.imwrite(f'./preprocessed/{filename}_gray.bmp', gray_img)
        #print(f'gray shape:{gray_img.shape}') 
        #print(f'gray:{gray_img}')
        
        # Add white noise points and write out another bmp file
        noise_img = add_noise(img, 100)
        cv2.imwrite(f'./preprocessed/{filename}_noise.bmp', noise_img)
        print(f'noise gray shape:{noise_img.shape}') 
        print(f'noise gray:{noise_img}')
        
        # Convert to grayscale image
        image = cv2.cvtColor(src=img, code=cv2.COLOR_BGR2GRAY)

        # Gaussian blur Kernel 5x5
        kernel = np.array([[1, 4, 7, 4, 1], [4, 16, 26, 16, 4], [7, 26, 41, 26, 7], [4, 16, 26, 16, 4], [1, 4, 7, 4, 1]])
        kernel_scale= kernel/273
        # Convolve and Save Output
        #output = convolve2D(image, kernel_scale, padding=2)
        output = conv2d(image, kernel_scale, padding=2)
        cv2.imwrite(f'./golden/{filename}_noise_gaussian.bmp', output)
