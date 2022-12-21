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
        float resizeFactor;
    
        Virus(int xPos, int yPos, int size){
            
            x = xPos;
            y = yPos;
            s = size;
            resizeFactor = 1.0;
            
            img.load("virus.png");
            
        }
    
        void draw() {
            
            this->img.draw(x, y, s, s);
            
        }
    
        void drawDeath() {
            
            if (resizeFactor > 0) {
                this->img.draw(x, y, s * resizeFactor, s * resizeFactor);
                this->resizeFactor -= 0.1;
            }
            
        }
    
};
