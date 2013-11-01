#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>

using namespace std;

int maxStackSize = 500; // will be increased as needed

class FloodFiller {
public:
    int *xstack;
    int *ystack;
    int stackSize;
    int **pixels;
    int width, height;
	int targetColor;
  
    FloodFiller(int** image, int w, int h, int Color) {
    	width = w;
    	height = h;
    	xstack = new int[maxStackSize];
    	ystack = new int[maxStackSize];
    	pixels = image;
		targetColor = Color;
    }
    ~FloodFiller() {
    	delete [] xstack;
    	delete [] ystack;
    }

	int getPix( int ax,  int ay) {
		//__asm__("lda $9, 0($31)");
		int x = ax;
		int y = ay;
		if (x < 0 || x >= width || y < 0 || y >= height)
			return -1;
		else
			return pixels[x][y];
		//__asm__("lda $9, 0($31)");
	}
	void setPix( int ax,  int ay,  int c) {
		//__asm__("lda $9, 0($31)");
		int x = ax;
		int y = ay;
		if (x < 0 || x >= width || y < 0 || y >= height)
			return;
		else
			pixels[x][y] = c;
		//__asm__("lda $9, 0($31)");
	}

    /** Does a 4-connected flood fill using the current fill/draw
        value, which is defined by ImageProcessor.setValue(). */
    bool fill( int x,  int y) {
        int color = getPix(x, y);
        fillLine(x, x, y);
        int newColor = getPix(x, y);
		setPix(x, y, color);
        if ((color==newColor)) return false;
        stackSize = 0;
        push(x, y);
        while(true) {   
            x = popx(); 
            if ((x ==-1)) return true;
            y = popy();
            if ((getPix(x, y)!=color)) continue;
            int x1 = x; int x2 = x;
            
            while ((getPix(x1, y)==color && x1>=0)) x1--; // find start of scan-line
            x1++;
            while ((getPix(x2, y)==color && x2<width)) x2++;  // find end of scan-line                 
            x2--;
            
			fillLine(x1,x2,y); // fill scan-line
            bool inScanLine = false;
            for (int i=x1; (i<=x2); i++) { // find scan-lines above this one
                if (!inScanLine && (y>0 && getPix(i, y-1)==color))
                    {push(i, y-1); inScanLine = true;}
                else if (inScanLine && (y>0 && getPix(i, y-1)!=color))
                    inScanLine = false;
            }
            inScanLine = false;
            for (int i=x1; (i<=x2); i++) { // find scan-lines below this one
                if (!inScanLine && (y<height-1 && getPix(i, y+1)==color))
                    {push(i, y+1); inScanLine = true;}
                else if (inScanLine && (y<height-1 && getPix(i, y+1)!=color))
                    inScanLine = false;
            }
        }        
    }
    
private:
	void push( int x,  int y) {
        stackSize++;
        if (stackSize==maxStackSize) {
            int *newXStack = new  int[maxStackSize*2];
            int *newYStack = new  int[maxStackSize*2];
            for (int i = 0; i < maxStackSize; ++i) {
            	newXStack[i] = xstack[i];
            	newYStack[i] = ystack[i];
            }
            delete [] xstack;
            delete [] ystack;
            xstack = newXStack;
            ystack = newYStack;
            maxStackSize *= 2;
        }
        xstack[stackSize-1] = x;
        ystack[stackSize-1] = y;
    }
    
    int popx() {
        if (stackSize==0)
            return -1;
        else
            return xstack[stackSize-1];
    }

    int popy() {
        int value = ystack[stackSize-1];
        stackSize--;
        return value;
    }

    void fillLine(int x1, int x2, int y) {
        if ((x1>x2)) {int t = x1; x1=x2; x2=t;}
        for (int x=x1; (x<=x2); x++)
			setPix(x, y, targetColor);
		
    }

};

int main(int argc, char** argv) {
	const int width = 32;
	const int height = 32;
	
	int** image = new int*[width];
	for (int i = 0; i < width; ++i) {
		image[i] = new int[height];
	}
		
	ifstream file("input.txt");
	string line;
	for (int y = 0; getline(file, line); y++) {
		for (int x = 0; x < line.size(); x++) {
			image[x][y] = line[x] - '0';
		}
	}
    file.close();
    
	FloodFiller ff(image, width, height, 2);
	ff.fill(0, 0);
	
	freopen("output.txt", "w", stdout);	
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			cout << image[x][y];
		}
		cout << '\n';
	}
	fclose(stdout);
	for (int i = 0; i < width; ++i) {
		delete [] image[i];
	}
	delete [] image;
	return 0;
}

