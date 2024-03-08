#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main()
{
    for (int x = 0; x < 240; x++)
    {
        stringstream v;
        v << x;
        string file = "images/image" + v.str() + ".ppm";
        ifstream image(file);

        string copy_file = "images2/copy_image" + v.str() + ".ppm";
        ofstream copy_image(copy_file);
    
    while (!image.eof())
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
        cout << s << endl;
        //cout << mods << endl;

    }
    copy_image.close();
    image.close();
    }
}
