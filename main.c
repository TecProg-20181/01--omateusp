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

void grayscale(Image * img) {
    // Transforming each pixel in grayscale by keeping the
    // three colors (r,g,b) and making an average of it
    for (unsigned int i = 0; i < img->height; ++i) {
        for (unsigned int j = 0; j < img->width; ++j) {
            int average = img->pixel[i][j].r +
                          img->pixel[i][j].g +
                          img->pixel[i][j].b;
            average /= 3;
            img->pixel[i][j].r = average;
            img->pixel[i][j].g = average;
            img->pixel[i][j].b = average;
        }
    }
}

void sepiaFilter(Image * img) {
    // Aplly the sepia filter to the image
    for (unsigned int x = 0; x < img->height; ++x) {
        for (unsigned int j = 0; j < img->width; ++j) {
            Pixel pixel;
            pixel.r = img->pixel[x][j].r;
            pixel.g = img->pixel[x][j].g;
            pixel.b = img->pixel[x][j].b;

            int p =  pixel.r * .393 + pixel.g * .769 + pixel.b * .189;
            img->pixel[x][j].r = p;

            p =  pixel.r * .349 + pixel.g * .686 + pixel.b * .168;
            img->pixel[x][j].g = p;

            p =  pixel.r * .272 + pixel.g * .534 + pixel.b * .131;
            img->pixel[x][j].b = p;
        }
    }
}

int checkTopLeftBorder(int x, int size){
    if(x - size/2 > 0){
        return x - size/2;
    }
    else{
        return size/2 - x;
    }
}

int checkBottomRightBorder(int x, int size, int max){
    if(x + size/2 <= max-1){
        return x+size/2;
    }
    else{
        return max;
    }
}

void blur(Image * img){
    // size -> Range of blur
    unsigned int size = 0;
    scanf("%d", &size);
    
    // blur each pixel of the image
    for (unsigned int i = 0; i < img->height; ++i) {
        for (unsigned int j = 0; j < img->width; ++j) {
            Pixel average = {0, 0, 0};
            int nPixels = 0;
            // Getting each pixel that will blur the pixel and 
            // keeping the average on Pixel average
            unsigned int max_h = checkBottomRightBorder(i,size,img->height);
            unsigned int max_w = checkBottomRightBorder(j,size,img->width);
            for(unsigned int x = checkTopLeftBorder(i, size); x < max_h; ++x) {
                for(unsigned int y = checkTopLeftBorder(j, size); y < max_w; ++y){
                    average.r += img->pixel[x][y].r;
                    average.g += img->pixel[x][y].g;
                    average.b += img->pixel[x][y].b;
                    nPixels++;
                }
            }
            if(nPixels != 0){
                average.r /= nPixels;
                average.g /= nPixels;
                average.b /= nPixels;
            }
            // Moving pixel by pixel colors
            img->pixel[i][j].r = average.r;
            img->pixel[i][j].g = average.g;
            img->pixel[i][j].b = average.b;
        }
    }
}

void rotate90Right(Image * img) {
    // Creating a temporary image
    Image rotate = *img;
    // Getting image's sizes
    rotate.width = img->height;
    rotate.height = img->width;
    // nTimes -> Number of times the image will be rotated
    int nTimes = 0;
    scanf("%d", &nTimes);
    nTimes %= 4;
    for (int j = 0; j < nTimes; ++j) {
        for (unsigned int i = 0, y = 0; i < rotate.height; ++i, ++y) {
            for (int j = rotate.width - 1, x = 0; j >= 0; --j, ++x) {
                rotate.pixel[i][j].r = img->pixel[x][y].r;
                rotate.pixel[i][j].g = img->pixel[x][y].g;
                rotate.pixel[i][j].b = img->pixel[x][y].b;
            }
        }
    }
    *img = rotate;
}

void flip(Image * img){
    int horizontal;
    scanf("%d", &horizontal);
    // W and h are the width and the height of the image
    // after the flip
    int w = img->width, h = img->height;

    if (horizontal == 1) w /= 2;
    else h /= 2;

    for (int i2 = 0; i2 < h; ++i2) {
        for (int j = 0; j < w; ++j) {
            int x = i2, y = j;

            if (horizontal == 1) y = img->width - 1 - j;
            else x = img->height - 1 - i2;

            Pixel aux1;
            aux1.r = img->pixel[i2][j].r;
            aux1.g = img->pixel[i2][j].g;
            aux1.b = img->pixel[i2][j].b;

            img->pixel[i2][j].r = img->pixel[x][y].r;
            img->pixel[i2][j].g = img->pixel[x][y].g;
            img->pixel[i2][j].b = img->pixel[x][y].b;

            img->pixel[x][y].r = aux1.r;
            img->pixel[x][y].g = aux1.g;
            img->pixel[x][y].b = aux1.b;
        }
    }
}

void invertColors(Image * img) {
    // Inverts the color of the image by subtracting the maximum color
    // of the actual color of the image
    for (unsigned int i = 0; i < img->height; ++i) {
        for (unsigned int j = 0; j < img->width; ++j) {
            img->pixel[i][j].r = img->nColor - img->pixel[i][j].r;
            img->pixel[i][j].g = img->nColor - img->pixel[i][j].g;
            img->pixel[i][j].b = img->nColor - img->pixel[i][j].b;
        }
    }
}

void cropImage(Image * img) {
    // Coordinates
    // x, y -> beginning of the crop
    // h, w -> size of the image after the crop
    int x, y, w, h;
    scanf("%d %d", &x, &y);
    scanf("%d %d", &w, &h);
    
    // Moving pixels from the cropped part to the beginning of the image
    for(int i = 0; i < h; ++i) {
        for(int j = 0; j < w; ++j) {
            img->pixel[i][j].r = img->pixel[i + y][j + x].r;
            img->pixel[i][j].g = img->pixel[i + y][j + x].g;
            img->pixel[i][j].b = img->pixel[i + y][j + x].b;
        }
    }

    // Sets image's new sizes
    img->width = w;
    img->height = h;
}

void printImage(Image * img){
    // Print type of image
    printf("%s\n", img->type);

    // Print width, height and number of colors of the image
    printf("%u %u\n%d\n", img->width, img->height, img->nColor);

    // Print the image pixel by pixel
    for (unsigned int i = 0; i < img->height; ++i) {
        for (unsigned int j = 0; j < img->width; ++j) {
            printf("%hu %hu %hu ", img->pixel[i][j].r,
                                   img->pixel[i][j].g,
                                   img->pixel[i][j].b);

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
    
    // Modify image
    for(int i = 0; i < nOptions; ++i) {
        int option;
        scanf("%d", &option);

        switch(option) {
            case 1: { // Grayscale the image
                grayscale(&img);
                break;
            }
            case 2: { // Sepia Filter
                sepiaFilter(&img);
		        break;
            }
            case 3: { // Blur
                blur(&img);
                break;
            }
            case 4: { // Rotate the imagem 90 degrees to the right
                rotate90Right(&img);
                break;
            }
            case 5: { // Flip the image 0 = vertical, 1 = horizontal
                flip(&img);
                break;
            }
            case 6: { // Invert the colors
                invertColors(&img);
                break;
            }
            case 7: { // Cortar Imagem
                cropImage(&img);
                break;
            }
        }

    }

    // Print image
    printImage(&img);

    return 0;
}
