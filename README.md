## OpenCV Template Matching with Qt GUI

With this application it is possible to use OpenCV Template Matching algorithms (https://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/template_matching/template_matching.html)
through a pretty Qt GUI or command line.

![Example Usage](templatematching.png)

### Requirements

* cmake
* Qt 5
* OpenCV 4

### Build
```
cmake CMakeList.txt -DCMAKE_PREFIX_PATH="path/to/Qt5/cmake" -DOPENCV_INCLUDE_PATH="/path/to/opencv/include/opencv4/" -DOPENCV_LIB_PATH="/path/to/opencv4/lib"
```
```
make
```

### Usage
```
./QtTemplateMatching [-help] [-nogui] [-src=source_image] [-templ=template_image] [-method=match_method] [-threshold=match_threshold]
```
* Options:
    1. -help shows this message and exits.
    2. -nogui (only valid when -src, -templ are specified) run Template Matching without selection interface.
    3. -src=source_image load source image from source_image path.
    4. -templ=template_image load template image from template_image path.
    5. -method=match_method set matching mode to match_method:
        * 1 SQ_DIFF
        * 2 SQ_DIFF_NORMED
        * 3 CC_CORR
        * 4 CC_CORR_NORMED
        * 5 CC_COEFF_NORMED
        * 6 CC_COEFF_NORMED
    6. -threshold=match_threshold: threshold for matching algorithm.


