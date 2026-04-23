#include <iostream>
using namespace std;
class Pixel
{
 int Red; 
 int Green ; 
 int Blue ; 
 public:
 Pixel( int Red , int Green , int Blue  )
 {
    this->Red = Red;
    this->Green = Green;
    this->Blue = Blue;
 }
 Pixel ()
 {
    Red = 0;
    Green = 0;
    Blue = 0;
 }
 int getter_Red()
 {
    return Red;
 }
 int getter_Green()
 {
    return Green;
 }
 int getter_Blue()
 {
    return Blue;
 }
 void setter_Red(int Red )
 {
    this->Red = Red;
 } 
 void setter_Green( int Green ) 
 {
    this->Green = Green;
 }
 void setter_Blue( int Blue )
 {
    this->Blue = Blue;
 }
static int clamp_unity(int value );
Pixel operator+(const Pixel &Pix)
{
    Pixel P;
    P.Red = clamp_unity(this->Red + Pix.Red);
    P.Blue = clamp_unity(this->Blue + Pix.Blue);
    P.Green = clamp_unity(this->Green + Pix.Green);

    return P;

}
friend ostream& operator<<(ostream& out, const Pixel& p)
{
    out<<"Red : "<<p.Red<<" , ";
    out<<"Blue : "<<p.Blue<<" , ";
    out<<"Green : "<<p.Green;
    return out;
}
};
int Pixel::clamp_unity(int value)
{
    if ((value <= 255) && (value >= 0))
    {
       return value;
    }
    else if(value > 255)
    {
        return 255;
    }
    else if(value < 0)
    {
        return 0;
    }
}
class image
{

}
int main() 
{
    
}