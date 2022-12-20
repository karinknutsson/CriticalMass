//
//  apple.h
//  appleGame
//
//  Created by Karin on 19.12.22.
//

class Apple {
    public:
    
        int x;
        int y;
        int s;
        ofImage img;
    
        Apple(int xPos, int yPos, int size){
            x = xPos;
            y = yPos;
            s = size;
            
            img.load("golden-torus.png");
        }
    
        void draw() {
            this->img.draw(x, y, s, s);
        }
};
