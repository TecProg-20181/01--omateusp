#include <stdio.h>

typedef struct _pixel {
    // color of each pixel
    // r -> red
    // g -> green
    // b -> blue
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct _image {
    // array of pixels that forms the image
    // width -> width of the image
    // height -> height of the image
    // [width][height][rgb]
    // 0 -> red
    // 1 -> green
    // 2 -> blue
    // nColor -> number of colors
    // type -> type of the image, usually P3
    Pixel pixel[512][512];
    unsigned int width;
    unsigned int height;
    unsigned int nColor;
    char type[4];
} Image;

void readImage(Image * img){
    // Reads the image type
    scanf("%s", img->type);

    // Reads width height and color of image
    scanf("%u %u %d", &img->width, &img->height, &img->nColor);
    for (unsigned int i = 0; i < img->height; ++i) {
        for (unsigned int j = 0; j < img->width; ++j) {
            scanf("%hu %hu %hu", &img->pixel[i][j].r,
                                 &img->pixel[i][j].g,
                                 &img->pixel[i][j].b);

        }
    }
}

Image grayscale(Image img) {
    // Transforming each pixel in grayscale by keeping the
    // three colors (r,g,b) and making an average of it
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            img.pixel[i][j].r += img.pixel[i][j].g + img.pixel[i][j].b;
            img.pixel[i][j].r /= 3;
            img.pixel[i][j].g = img.pixel[i][j].r;
            img.pixel[i][j].b = img.pixel[i][j].g;
        }
    }
    return img;
}

Image sepiaFilter(Image img) {
    // Aplly the sepia filter to the image
    for (unsigned int x = 0; x < img.height; ++x) {
        for (unsigned int j = 0; j < img.width; ++j) {
            Pixel pixel;
            pixel = img.pixel[x][j];

            int p =  pixel.r * .393 + pixel.g * .769 + pixel.b * .189;
            if(p>255) p = 255;
            img.pixel[x][j].r = p;

            p =  pixel.r * .349 + pixel.g * .686 + pixel.b * .168;
            if(p>255) p = 255;
            img.pixel[x][j].g = p;

            p =  pixel.r * .272 + pixel.g * .534 + pixel.b * .131;
            if(p>255) p = 255;
            img.pixel[x][j].b = p;
        }
    }
    return img;
}
    
int checkMin(int x){
    if(x >= 0) return x;
    else return 0;
}

int checkMax(int x, int y){
    if(x <= y) return x;
    else return y;
}

Image blur(Image img){
    // size -> Range of blur
    unsigned int size = 0;
    scanf("%d", &size);
    
    // blur each pixel of the image
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            Pixel average = {0, 0, 0};
            int nPixels = 0;
            // Getting each pixel that will blur the pixel and 
            // keeping the average on Pixel average
            unsigned int max_h = checkMax(i + size/2,img.height - 1);
            unsigned int max_w = checkMax(j + size/2,img.width - 1);
            for(unsigned int x = checkMin(i - size/2); x < max_h; ++x) {
                for(unsigned int y = checkMin(j - size/2); y < max_w; ++y){
                    average.r += img.pixel[x][y].r;
                    average.g += img.pixel[x][y].g;
                    average.b += img.pixel[x][y].b;
                    nPixels++;
                }
            }
            if(nPixels != 0){
                average.r /= nPixels;
                average.g /= nPixels;
                average.b /= nPixels;
            }
            // Moving pixel by pixel colors
            img.pixel[i][j] = average;
        }
    }
    return img;
}

Image rotate90Right(Image img) {
    // Creating a temporary image
    Image rotate = img;
    // Getting image's sizes
    rotate.width = img.height;
    rotate.height = img.width;
    // nTimes -> Number of times the image will be rotated
    int nTimes = 0;
    scanf("%d", &nTimes);
    nTimes %= 4;
    for (int j = 0; j < nTimes; ++j) {
        for (unsigned int i = 0, y = 0; i < rotate.height; ++i, ++y) {
            for (int j = rotate.width - 1, x = 0; j >= 0; --j, ++x) {
                rotate.pixel[i][j] = img.pixel[x][y];
            }
        }
    }
    return rotate;
}

Image flip(Image img){
    int horizontal;
    scanf("%d", &horizontal);
    // W and h are the width and the height of the image
    // after the flip
    int w = img.width, h = img.height;

    if (horizontal == 1) w /= 2;
    else h /= 2;

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            int x = i, y = j;

            if (horizontal == 1) y = img.width - 1 - j;
            else x = img.height - 1 - i;

            Pixel aux1;
            aux1 = img.pixel[i][j];

            img.pixel[i][j] = img.pixel[x][y];

            img.pixel[x][y] = aux1;
        }
    }
    return img;
}

Image invertColors(Image img) {
    // Inverts the color of the image by subtracting the maximum color
    // of the actual color of the image
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            img.pixel[i][j].r = img.nColor - img.pixel[i][j].r;
            img.pixel[i][j].g = img.nColor - img.pixel[i][j].g;
            img.pixel[i][j].b = img.nColor - img.pixel[i][j].b;
        }
    }
    return img;
}

Image cropImage(Image img) {
    // Coordinates
    // x, y -> beginning of the crop
    // h, w -> size of the image after the crop
    int x, y, w, h;
    scanf("%d %d", &x, &y);
    scanf("%d %d", &w, &h);
    
    // Moving pixels from the cropped part to the beginning of the image
    for(int i = 0; i < h; ++i) {
        for(int j = 0; j < w; ++j) {
            img.pixel[i][j] = img.pixel[i + y][j + x];
        }
    }

    // Sets image's new sizes
    img.width = w;
    img.height = h;
    return img;
}

void printImage(Image img){
    // Print type of image
    printf("%s\n", img.type);

    // Print width, height and number of colors of the image
    printf("%u %u\n%d\n", img.width, img.height, img.nColor);

    // Print the image pixel by pixel
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            printf("%hu %hu %hu ", img.pixel[i][j].r,
                                   img.pixel[i][j].g,
                                   img.pixel[i][j].b);

        }
        printf("\n");
    }
}


int main() {
    // Creating a container for the image code
    Image img;
    
    // Reads the image
    readImage(&img);
    
    
    // Number of operations at the image
    int nOptions;
    scanf("%d", &nOptions);
    
    for(int i = 0; i < nOptions; ++i) {
        int option;
        scanf("%d", &option);

        switch(option) {
            case 1: { // Grayscale the image
                img = grayscale(img);
                break;
            }
            case 2: { // Sepia Filter
                img = sepiaFilter(img);
		        break;
            }
            case 3: { // Blur
                img = blur(img);
                break;
            }
            case 4: { // Rotate the imagem 90 degrees to the right
                img = rotate90Right(img);
                break;
            }
            case 5: { // Flip the image 0 = vertical, 1 = horizontal
                img = flip(img);
                break;
            }
            case 6: { // Invert the colors
                img = invertColors(img);
                break;
            }
            case 7: { // Cortar Imagem
                img = cropImage(img);
                break;
            }
        }

    }

    // Print image
    printImage(img);

    return 0;
}
