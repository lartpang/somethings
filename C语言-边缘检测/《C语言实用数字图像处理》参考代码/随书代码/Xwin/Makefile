OBJ = Affine.o AffineCoef.o Amplify.o Colorbar.o Dilation.o Disk.o \
	Dither.o DitherMulti.o Dpcm.o DpcmVlc.o Erosion.o Event.o Expand.o \
	ExpandRgb.o ExpandYsh.o ExtractRatio.o ExtractSize.o \
	Features.o Fft1.o Fft2.o FftFilt.o FftImage.o Gradient.o \
	Hist2Im.o Histgram.o HistImag.o HistPrin.o HistSmth.o \
	Hough.o HoughCross.o Idpcm.o IdpcmVlc.o ImageCom.o Ivlcode.o \
	Labeling.o Laplacian.o Lattice.o LoG.o Masking.o Median.o \
	Mosaic.o MosaicCoefBlk.o MosaicCoefBlkRgb.o \
	NoiseRand.o NoiseSpike.o PersCoef.o Perspect.o \
	Plane.o Prewitt.o PseudoColor.o RadDistort.o Range.o RgbYsh.o \
	Rotation.o Scale.o ScaleNear.o ScaleNg.o ScaleRotShift.o Stereo.o \
	Shift.o Smooth.o SmoothEdge.o SmoothWt.o Thinning.o \
	Thresh.o ThreshCd.o ThreshDiscrim.o ThreshDynamic.o ThreshMode.o \
	ThreshRgb.o ThreshYsh.o TranYsh.o Vlcode.o XDisp.o YshImage.o \
	ZeroCross.o

all: Image CImage

Image: Image.o $(OBJ)
	cc -o Image Image.o $(OBJ) -lX11 -lm -L/usr/X11R6/lib

CImage: CImage.o $(OBJ)
	cc -o CImage CImage.o $(OBJ) -lX11 -lm -L/usr/X11R6/lib

.c.o:
	cc -c -g $?

