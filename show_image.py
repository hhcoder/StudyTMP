import sys, getopt
import matplotlib.pyplot as plt
import raw_reader as raw_reader 
import numpy as np
import scipy.misc

def ReadBayerRaw(src_fname, in_width, in_height):
    in_length = in_width * in_height
    raw_buffer = np.fromfile(src_fname, dtype=np.uint8, count=in_length)
    return np.reshape(raw_buffer, (in_height, in_width))

def ImShow(img, t, block=False):
    plt.figure()
    plt.imshow(img, cmap='gray')
    plt.title(t)
    plt.show(block)

def BayerRawToPng(bayer_raw, dst_fname):
    scipy.misc.toimage(bayer_raw, cmin=0.0, cmax=1023.0).save(dst_fname)    

def main(argv):
    inputfile = ''
    outputfile = ''
    width = 0
    height = 0
    try:
        opts, args = getopt.getopt(argv,'i:w:h:v:',["ifile=","width=","height="])
    except getopt.GetoptError:
        print 'show_image.py -i <inputfile> -w <width> -h <height>'
        sys.exit(2)

    for opt, arg in opts:
        if opt == '-v':
            print 'show_image.py -i <inputfile> -o <outputfile>'
            sys.exit()
        elif opt in ("-i", "--ifile"):
            inputfile = arg
        elif opt in ("-w", "--width"):
            width = int(arg)
        elif opt in ("-h", "--height"):
            height = int(arg)

    outputfile = inputfile[0:inputfile.find('.raw')] + '.png'

    print 'Input file is ', inputfile
    print 'Width is ', width
    print 'Height is ', height
    print 'Output PNG is ', outputfile

    ref_origin = ReadBayerRaw(inputfile, width, height)
    
    BayerRawToPng(ref_origin, outputfile);

    ImShow(ref_origin, "Raw Image", block=True)

