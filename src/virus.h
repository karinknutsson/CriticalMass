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
        bool shrink;

        Virus(int xPos, int yPos, int size){

            x = xPos;
            y = yPos + 10;
            s = size;
            resizeFactor = 1.0;
            shrink = false;

            img.load("virus.png");

        }

        void draw() {

            img.draw(x, y, s, s);

        }

        void drawDeath() {

            if (!shrink) {

                // at first, the virus will grow, until it reaches 60% of its volume
                if (resizeFactor < 1.5) {

                    int xAdjusted = x - s * (resizeFactor - 1) / 2;
                    int yAdjusted = y - s * (resizeFactor - 1) / 2;
                    img.draw(xAdjusted, yAdjusted, s * resizeFactor, s * resizeFactor);
                    resizeFactor += 0.2;

                } else {

                    shrink = true;

                }

            } else {

                int xAdjusted;
                int yAdjusted;

                if (resizeFactor > 1) {

                    xAdjusted = x - s * (resizeFactor - 1) / 2;
                    yAdjusted = y - s * (resizeFactor - 1) / 2;

                } else {

                    xAdjusted = x + (s - s * resizeFactor) / 2;
                    yAdjusted = y + (s - s * resizeFactor) / 2;

                }

                img.draw(xAdjusted, yAdjusted, s * resizeFactor, s * resizeFactor);

                // now the virus will shrink, until it is no longer visible
                resizeFactor -= 0.2;

            }

        }

};
