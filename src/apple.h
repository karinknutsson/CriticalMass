//
//  apple.h
//  appleGame
//
//  Created by Karin on 19.12.22.
//

class Apple {
    public:
    
        int w;
        int h;
        int x;
        int y;
        ofImage img;
    
        Apple(int width, int height, int xPos, int yPos){
            w = width;
            h = height;
            x = xPos;
            y = yPos;
            
            img.load("apple.png");
        }
    
        void draw() {
            this->img.draw(x, y, w, h);
        }
};
