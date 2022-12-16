/*
(c) Westerveld 2022
Final project ICS 3UI
A sphere raytracer that includes ray casting, object shadows, object shading and  reflections

still to do - refraction, antialiasing, perspective, variable reflectivity
fix some issues with objects behind casting shadows on objects infront
OPTIMIZATION
neater code and clearer comments

uses ppm image format - to view ppm images you can downlaod them and view here:
https://www.cs.rhodes.edu/welshc/COMP141_F16/ppmReader.html
or install a ppm capable program on your computer such as GIMP
*/
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

//to randomize reflection rays for diffusion
#include <cstdlib>

using namespace std;

struct Sphere {int x; int y; int z; int rad; bool is_reflective; double r; double g; double b; bool floor; int diffusion;};

struct Coord {double x; double y; double z; bool exists; double dist;};

//normalize a vector, meaning make its magnitude one
Coord normalize(Coord vector);

//takes in ray origin, directions and a sphere and detects if ray collides with the sphere and returns a coordinate of collision
Coord ray_hit(Coord origin, Coord direct, Sphere sphere);

//takes in direction vector and origin, detects where it hits a sphere and finds surface normal for the part on sphere
//reflects vector over surface normal
Coord reflect(Coord origin, Coord direction, Sphere sphere);

//normal is ray from sphere centre to point of incidence
double intensity(Coord incident, Coord normal, bool reflective, bool floor);



//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////


int main() 
{
  //main image file
  ofstream image("image.ppm");
  
  //image dimensions
  int image_height = 1080;//1080;//256;
  int image_width = 1920;//1920;//480;

  //list of spheres
  vector<Sphere> spheres;
  
  //x, y, z, radius, reflective, r, g, b
  //define spheres, add to list of spheres
  // Sphere Sp1 = {360, 540, 11100, 360, false, 1, 0.75, 0.8};
  // spheres.push_back(Sp1);
  // //Sphere Sp2 = {50, 200, 200, 50, 0, 0, 1};
  // Sphere Sp2 = {960, 540, 11600, 239, true, 0, 0, 1};
  // spheres.push_back(Sp2);
  // Sphere Sp3 = {1560, 540, 11100, 360, false, 0, 1, 0};
  // spheres.push_back(Sp3);

  Sphere Sp1 = {0, 2000000, 0, 7000000, false, 0.4, 0.4, 0.4, true, 0};
  spheres.push_back(Sp1);
  Sphere Sp2 = {0, 250, 900, 400, true, 1, 1, 0, false, 0};
  spheres.push_back(Sp2);
  Sphere Sp3 = {-150, 540, 400, 60, false, 1, 1, 0, false, 0};
  spheres.push_back(Sp3);
  Sphere Sp4 = {250, 440, 400, 100, false, 0, 1, 1, false, 0};
  spheres.push_back(Sp4);
  Sphere Sp5 = {-450, 340, 500, 200, false, 1, 0, 1, false, 0};
  spheres.push_back(Sp5);
  Sphere Sp6 = {450, 340, 500, 150, false, 0.5, 0.5, 1, false, 0};
  spheres.push_back(Sp6);

  //buggy
  //Sphere Sp4 = {960, -1000, 11600, 1295, false, 1, 0, 1};
  //spheres.push_back(Sp4);


  double origin_x = 0;
  double origin_y = 150;
  double origin_z = -300;

  //followes quadrant system where straight is 0
  //from 0-1 where 1 is 90 degrees
  double angle_x = 0;
  double angle_y = -0.2;
  //for 1080p
  // Sphere Sp1 = {1000, 500, 700, 375, 1, 0, 1};
  // spheres.push_back(Sp1);
  // //Sphere Sp2 = {50, 200, 200, 50, 0, 0, 1};
  // Sphere Sp2 = {1500, 700, 400, 250, 0, 0, 1};
  // spheres.push_back(Sp2);
  // Sphere Sp3 = {196, 235, 150, 50, 0, 1, 0};
  // spheres.push_back(Sp3);

  // Sphere Sp1 = {150, 150, 300, 75, 1, 0, 0};
  // spheres.push_back(Sp1);
  // Sphere Sp2 = {150, 150, 150, 50, 0, 0, 1};
  // spheres.push_back(Sp2);
  
  //x, y, z
  //Coord Li1 = {256, 256, 00, true};
  //light source
  Coord Li1 = {0, -800, 100, true};


  //maximum reflections per ray
  int REFLECT_MAX = 5;
  
  //ppm file formatting
  image << "P3" << endl;
  image << image_width << ' ' << image_height << endl;
  image << "255" << endl;
  
  //loop through all pixels in image
  for (int y = image_height - 1; y > -1; y--)
  {
    for (int x = 0; x < image_width; x++)
    {
      //int x = 0;
      //int y = 255;
      // //default or backround colours
      // //replace 0.5 with commented code for a gradient
      float backr = 0.63;//double(x) / (image_height - 1);
      float backg = 0.63;//double(y) / (image_height - 1);
      float backb = 0.63;
      
      float r = backr;
      float g = backg;
      float b = backb;

      //get aspect ratio
      double width = (image_width * 1.0) / (image_height * 1.0);
      
      //define origin and direction of each ray from each pixel
      Coord Or = {origin_x, origin_y, origin_z, true};
      Coord Dir = {(-(width / 2.0) + x * (width / image_width) + angle_x), (0.5 - (y * (1.0 / image_height)) - angle_y), 1.0, true};

  //check all spheres for interceptions
  //leave as initial value if not hit

  //if initial value make backgorund colour
  //else check if its reflective

  //if not, get shading and be good
  double initial = -1; //index of sphere collision, if -1 means no collisions
  double distance = 2147483647; //find closest hit
  Coord Hit;

  //check distance of collision for all spheres
  for (int i = 0; i < spheres.size(); i++)
  {
    Hit = ray_hit(Or, Dir, spheres[i]);

    //if smaller than previous distance ray hits this sphere first
    if (Hit.exists && Hit.dist < distance)
    {
      distance = Hit.dist;
      initial = i;
    }
  }

  //check if no collisions occured (index is still default), set backround
  if (initial == -1)
  {
    r = backr;
    g = backg;
    b = backg;
  }
  else //get info on first sphere collision
  {
    //get ray hit for closest one sphere to work off of
    Coord Collision = ray_hit(Or, Dir, spheres[initial]);

    //if not reflective, shade it, check for shadows
    if (! spheres[initial].is_reflective)
    {

      //set rgb to colour of sphere to work with
      r = spheres[initial].r;
      g = spheres[initial].g;
      b = spheres[initial].b;

      //get vector from sphere collision to light for intensity and shadows
      Coord Towards_light;

      Towards_light.x = Li1.x - Collision.x;
      Towards_light.y = Li1.y - Collision.y;
      Towards_light.z = Li1.z - Collision.z;

      bool shade = true; //shade, unless otherwise directed by shadows

      //check for shadows based off of towards light vector
      for (int i = 0; i < spheres.size(); i++)
      {
        //check if ray collides with any other sphere on the way towards the light
        if (i != initial)
        {
          if (ray_hit(Collision, Towards_light, spheres[i]).exists)
          {
            //if so, reduce the brightness
            r *= 0.3;
            g *= 0.3;
            b *= 0.3;
            shade = false;
            break;
          }
        }
      }

      if (shade == true)//no casted shadow
      {
        //get sphere normal, from centre to collision
        Coord Normal;

        Normal.x = Collision.x - spheres[initial].x;
        Normal.y = Collision.y - spheres[initial].y;
        Normal.z = Collision.z - spheres[initial].z;

        //light to collision vector, sphere normal (from middle to collision), is reflective and is floor
        double brightness = intensity(Towards_light, Normal, spheres[initial].is_reflective, spheres[initial].floor);

        //apply brightness based on shading to sphere
        r *= brightness;
        g *= brightness;
        b *= brightness;
      }
      else //casted shadow
      {
        r *= 0.3;
        g *= 0.3;
        b *= 0.3;
      }

    } //not reflective close
    else
    {
      //if reflective, reflect one ray around
      if (spheres[initial].diffusion == 0)
      {
        
        //reflect around a maximum of REFLECT_MAX times
        for (int i = 0; i < REFLECT_MAX; i++)
        {
          //reflect initial ray
          Coord Reflected_dir = reflect(Or, Dir, spheres[initial]);

          double reflect_hit = -1; //index of sphere collision, if -1 means no collisions
          distance = 2147483647; //find closest hit
          Coord Reflect_check;

          //check distance of collision for all spheres
          for (int i = 0; i < spheres.size(); i++)
          {
            if (i != initial)
            {
              Reflect_check = ray_hit(Collision, Reflected_dir, spheres[i]);
            

              //if smaller than previous distance ray hits this sphere first
              if (Reflect_check.exists && Reflect_check.dist < distance)
              {
                distance = Reflect_check.dist;
                reflect_hit = i;
              }
            }
          }

          //if no reflection, reflect backround colour and break
          if (reflect_hit == -1)
          {
            double 
            r = backr;
            g = backg;
            b = backb;
            break;
          }
          else //does hit something
          {
            //reflected collision to work off of 
            Coord Reflected = ray_hit(Collision, Reflected_dir, spheres[reflect_hit]);

            if (spheres[reflect_hit].is_reflective)//hits reflective
            {
              r=0;g=0;b=0;
              initial = reflect_hit;
              Dir = Reflected_dir;
              Or = Collision;
            }
            else //hits non reflective
            {
              r = spheres[reflect_hit].r;
              g = spheres[reflect_hit].g;
              b = spheres[reflect_hit].b;

              //get vector from sphere collision to light for intensity and shadows
              Coord Towards_light;

              Towards_light.x = Li1.x - Reflected.x;
              Towards_light.y = Li1.y - Reflected.y;
              Towards_light.z = Li1.z - Reflected.z;

              // check if something is casting a shadow
              bool shadow = false;
              for (int i = 0; i < spheres.size(); i++)
              {
                if (i != reflect_hit && ray_hit(Reflected, Towards_light, spheres[i]).exists)
                {
                  shadow = true;
                  break;
                }

              }

              if (shadow == false)
              {
                //get sphere normal, from centre to collision
                Coord Normal;

                Normal.x = Reflected.x - spheres[reflect_hit].x;
                Normal.y = Reflected.y - spheres[reflect_hit].y;
                Normal.z = Reflected.z - spheres[reflect_hit].z;

                //shade reflected sphere
                double brightness = intensity(Towards_light, Normal, false, spheres[reflect_hit].floor);
                r *= brightness;
                g *= brightness;
                b *= brightness;
              }
              else
              {
                //darken if shadows
                r *= 0.3;
                g *= 0.3;
                b *= 0.3;
              }
            }
          }


        } //max reflections close
      } //no diffusion check close

      //shading reflective sphere
      Coord Reflective_normal;

      Reflective_normal.x = Collision.x - spheres[initial].x;
      Reflective_normal.y = Collision.y - spheres[initial].y;
      Reflective_normal.z = Collision.z - spheres[initial].z;

      Coord Towards_light_reflective;

      Towards_light_reflective.x = Li1.x - Collision.x;
      Towards_light_reflective.y = Li1.y - Collision.y;
      Towards_light_reflective.z = Li1.z - Collision.z;

      double reflected_brightness = intensity(Towards_light_reflective, Reflective_normal, true, spheres[initial].floor);

      r *= reflected_brightness;
      g *= reflected_brightness;
      b *= reflected_brightness;

    } //reflective close

  } //does collide with sphere close

      
      //colour definitions
      //get RGB values for each pixel, as int to fit file format
      int pixel_r = 255.999 * r;
      int pixel_g = 255.999 * g;
      int pixel_b = 255.999 * b;

      //print to image file
      image << pixel_r << ' ' << pixel_g << ' ' << pixel_b << endl;
    }
  }
  image.close();
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//normalize a vector, meaning make its magnitude one
Coord normalize(Coord vector)
{
  //get magnitude
  double magnitude = sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));

  //create unit vector
  Coord unit_vector;

  //unit vector (vector with magnitude 1)
  //calculated by dividing all values of x y z by magnitude
  unit_vector.x = vector.x / magnitude;
  unit_vector.y = vector.y / magnitude;
  unit_vector.z = vector.z / magnitude;

  return unit_vector;
};


//////////////////////////////////////////////////////////////////////////


//takes in ray origin, directions and a sphere and detects if ray collides with the sphere and returns a coordinate of collision
Coord ray_hit(Coord origin, Coord direct, Sphere sphere)
{
  //parameters for quadratic discriminant
  //with discriminant b^2 - 4ac from the quadratic formula
  //progrom takes ray and factors into quadratic equation 
  //ax^2 + bx + c = 0 
  //the solves for x(distance) using quadratic formula.
  //distance =
  
  // -b +- sqrt(b^2 -4ac)
  //---------------------
  //    2a

  //a b and c are determined by substituting x y and z in the sphere equation which is:

  //(x - f)^2 + (y - g)^2 + (y - h)^2 = r^2 where (f, g, h) is the sphere centre

  //collision is then at origin + direction * distance for x, y and z
  
  //cout << "origin: " << origin.x << ", " << origin.y << ", " << origin.z << endl;
  

  //normalize direction
  Coord direction = normalize(direct);

  //a, b, and c are calculated by factoring to represent as coefficients of distance to collision, to solve for distance
  
  //if vector is normalized a should always be one
  double a = 1.0;

  //calculate b
  double b = (2.0 * direction.x * origin.x + 2.0 * direction.y * origin.y + 2.0 * direction.z * origin.z - 2.0 * sphere.x * direction.x - 2.0 * sphere.y * direction.y - 2.0 * sphere.z * direction.z);

  //calculate c
  double c = ((sphere.x * sphere.x) + (sphere.y * sphere.y) + (sphere.z * sphere.z) + (origin.x * origin.x) + (origin.y * origin.y) + (origin.z * origin.z) - 2.0 * sphere.x * origin.x - 2.0 * sphere.y * origin.y - 2.0 * sphere.z * origin.z - (sphere.rad * sphere.rad));

  //declare distance and collision point
  double distance;
  Coord collide;

  //solve for discrimant
  double discriminant = ((b * b) - 4.0 * a * c);

  //if discriminant is >= 0, there are 1 origin 2 real roots origin collisions with ray and sphere (with it being square rooted, any negative numbers will yield complex or imaginary numbers)
  if (discriminant >= 0)
  {
    //get positive and negative roots, find one closer to origin for if there is an "entrance and exit" intersection point on a sphere
    double positive = (-b + sqrt(discriminant)) / (2 * a);
    double negative = (-b - sqrt(discriminant)) / (2 * a);


    if (negative > positive)
    {
      distance = positive;
    }
    else
    {
      distance = negative;
    }
  }
  else
  {
    collide.exists = false;
    return collide;
  }

  if (distance >= 0)
  {
    //return point of collision
    collide.x = origin.x + direction.x * distance;
    collide.y = origin.y + direction.y * distance;
    collide.z = origin.z + direction.z * distance;
    collide.exists = true;
    collide.dist = distance;
    
    return collide;
  }
  else
  {
    collide.exists = false;
    return collide;
  }

}


/////////////////////////////////////////////////////////////////////////


//takes in direction vector and origin, detects where it hits a sphere and finds surface normal for the part on sphere
//reflects vector over surface normal
Coord reflect(Coord origin, Coord direction, Sphere sphere)
{
  //get where ray collides with sphere
  Coord collide = ray_hit(origin, direction, sphere);

  //create origin at centre of sphere
  Coord centre;
  centre.x = sphere.x;
  centre.y = sphere.y;
  centre.z = sphere.z;

  //get direction towards collision point from sphere centre
  Coord direc;
  direc.x = collide.x - centre.x;
  direc.y = collide.y - centre.y;
  direc.z = collide.z - centre.z;

  //normalize that direction, make it the normal to reflect over
  Coord direct = normalize(direc);
  
  //get dot product of ray direction and normal direction
  int dot_product = 1;//((direction.x * direct.x) + (direction.y * direct.y) + (direction.z * direct.z));

  //calculate reflected ray using r = d − 2 (d ⋅ n) n
  Coord reflected;
  reflected.x = direction.x - 2 * dot_product * direct.x;
  reflected.y = direction.y - 2 * dot_product * direct.y;
  reflected.z = direction.z - 2 * dot_product * direct.z;
  
  reflected.x *= -1;
  reflected.y *= -1;
  reflected.z *= -1;



  return reflected;
}


/////////////////////////////////////////////////////////////////////////


//normal is ray from sphere centre to point of incidence
double intensity(Coord incident, Coord normal, bool reflective, bool floor)
{
  //cout << "incident: " << incident.x << "," << incident.y << "," << incident.z << endl;
  //cout << "normal: " << normal.x << "," << normal.y << "," << normal.z << endl;
   //formula:
   //angle = acos((V1 * V2)/(magV1 * magV2))
   //                 ^
   //             dot product


  //arccos[(xa * xb + ya * yb + za * zb) / (√(xa2 + ya2 + za2) * √(xb2 + yb2 + zb2))]

  double dot_product = (incident.x * normal.x + incident.y * normal.y + incident.z * normal.z);
  
  //cout << "dot: " << dot_product << endl;
  
  double magnitude_product = sqrt((incident.x * incident.x) + (incident.y * incident.y) + (incident.z * incident.z)) * sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));
  
  //cout << "mangnitude: " << magnitude_product << endl;

  double angle = dot_product / magnitude_product;
  
  //cout << "pre acos: " << angle << endl;

  //angle *= 0.0174533;

  angle = acos(angle);

  angle *= 57.295779513;
  //cout << "post acos: " << angle << endl << endl << endl;


  
   //get dot product
  //  int dot_product = (incident.x * normal.x) + (incident.y * normal.y) + (incident.z * normal.z);
  //  //get mangitudes
  //  double incident_magnitude = sqrt((incident.x * incident.x) + (incident.y * incident.y) +(incident.z * incident.z));
  //  double normal_magnitude = sqrt((normal.x * normal.x) + (normal.y * normal.y) +(normal.z * normal.z));
  
  //  //calculate angle
  //  double angle = acos(dot_product/(incident_magnitude * normal_magnitude));
  
  //  //convert to degrees from radians
  //  angle = angle * 57.295779513;

  
  // cout << "angle << " << angle << endl;
   //calculate intensity
   //double intensity = (angle - 90) / 90;
  int hard_shadow = 110;
  
  if (reflective)
  {
    hard_shadow = 200;
  }
  
  double intensity;

  if (floor == false)
  {
    intensity = -(angle / hard_shadow) + 1;
  }
  else
  {
    intensity = 1;
  }
  
 
  return intensity;
};