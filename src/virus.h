//
//  virus.h
//  virusGame
//
//  Created by Karin on 19.12.22.
//

class Virus {
    public:
    
        int x;
        int y;
        int s;
        ofImage img;
    
        Virus(int xPos, int yPos, int size){
            x = xPos;
            y = yPos;
            s = size;
            
            img.load("virus.png");
        }
    
        void draw() {
            this->img.draw(x, y, s, s);
        }
    
        void drawDeath() {
            this->img.draw(x, y, s * 1.2, s * 1.2);
        }
};
