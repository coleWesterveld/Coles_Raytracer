#include <iostream>
#include <fstream>


int main()
{
    for (int x = 0; x < 240; x++)
    {
string file = "images/image" + i.str() + ".ppm";
ifstream image(file);
    
    while (!image.EOF)
    {

        string s;
        string mods;

        image >> s;
        for (int i = 0; i < s.length(); i++)
        {
            if (s[i] == 6 && s[i-1] == 5 && s[i-2] == 2)
            {
                mods[i] = 5;
            }
            else
            {
                mods[i] = s[i];
            }
        }
    }
    image.close();
    }
}
