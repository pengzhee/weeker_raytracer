#include "float.h"
#include "aarect.h"
#include "bvh.h"
#include "hittable_list.h"
#include "material.h"
#include "moving_sphere.h"
#include "sphere.h"
#include "camera.h"
#include "random.h"
#include "texture.h"
#include "surface_texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

vec3 color(const ray& r, hittable *world, int depth) {
  hit_record rec;
  if (world->hit(r, 0.0001, MAXFLOAT, rec)) {
    ray scattered;
    vec3 attenuation;
    vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
    if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
      return emitted + attenuation*color(scattered, world, depth+1);
    }
    else {
      return emitted;
    }
  }
  else {
    return vec3(0,0,0);
  }
}

hittable *cornell_box() {
    hittable **list = new hittable*[6];
    int i = 0;
    material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
    material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
    material *green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
    material *light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));

    list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
    list[i++] = new yz_rect(0, 555, 0, 555, 0, red);

    list[i++] = new xz_rect(213, 343, 227, 332, 554, light);     // light source
    list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
    list[i++] = new xz_rect(0, 555, 0, 555, 0, white);

    list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));

    return new hittable_list(list,i);
}


hittable *simple_light() {
  texture *pertext = new noise_texture(4);
  hittable **list = new hittable*[4];
  list[0] = new sphere(vec3(0,-1000, 0), 1000, new lambertian(pertext));
  list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
  // list[2] = new sphere(vec3(0, 7, 0), 2, new lambertian(pertext));
  list[2] = new sphere(vec3(0, 7, 0), 2,
                       new diffuse_light(new constant_texture(vec3(4,4,4))));
  list[3] = new xy_rect(3, 5, 1, 3, -2,
                        new diffuse_light(new constant_texture(vec3(4,4,4))));
  return new hittable_list(list,4);
}

hittable *globe() {
  int nx, ny, nn;
  unsigned char *tex_data = stbi_load("assets/earthmap.jpg", &nx, &ny, &nn, 0);
  material *mat =  new lambertian(new image_texture(tex_data, nx, ny));
  return new sphere(vec3(0,0, 0), 2, mat);

}

hittable *two_perlin_spheres() {
  texture *pertext = new noise_texture(4.0);
  hittable **list = new hittable*[2];
  list[0] = new sphere(vec3(0,-1000, 0), 1000, new lambertian(pertext));
  //list[0] = new sphere(vec3(0,-1000, 0), 1000, new metal(vec3(0.7, 0.6, 0.5), 0.0));
  list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));

  return new hittable_list(list, 2);
}

hittable *two_spheres() {
  texture *checker = new checker_texture(
                                         new constant_texture(vec3(0.2, 0.3, 0.4)),
                                         new constant_texture(vec3(0.9, 0.9, 0.9))
                                         );
  int n = 50;
  hittable **list = new hittable*[n+1];
  list[0] = new sphere(vec3(0,-10, 0), 10, new lambertian(checker));
  list[1] = new sphere(vec3(0, 10, 0), 10, new lambertian(checker));
  return new hittable_list(list,2);
}

hittable *random_scene() {
  int n = 50000;
  hittable **list = new hittable*[n+1];
  texture *checkered = new
    checker_texture(new constant_texture(vec3(0.2, 0.3, 0.4)),
                    new constant_texture(vec3(0.9, 0.9, 0.9))
                    );
  list[0] =  new sphere(vec3(0,-1000,0), 1000, new lambertian(checkered));
  int i = 1;
  for (int a = -10; a < 10; a++) {
    for (int b = -10; b < 10; b++) {
      float choose_mat = random_double();
      vec3 center(a+0.9*random_double(),0.2,b+0.9*random_double());
      if ((center-vec3(4,0.2,0)).length() > 0.9) {
        if (choose_mat < 0.8) {  // diffuse
          list[i++] = new
            moving_sphere(
                          center,
                          center+vec3(0, 0.5*random_double(), 0),
                          0.0, 1.0, 0.2,
                          new lambertian(
                                         new constant_texture(
                                                              vec3(random_double()*random_double(),
                                                                   random_double()*random_double(),
                                                                   random_double()*random_double())))
                          );
        }
        else if (choose_mat < 0.95) { // metal
          list[i++] = new
            sphere(
                   center, 0.2,
                   new metal(
                             vec3(0.5*(1 + random_double()),
                                  0.5*(1 + random_double()),
                                  0.5*(1 + random_double())),
                             0.5*random_double()
                             )
                   );
        }
        else {  // glass
          list[i++] = new sphere(center, 0.2, new dielectric(1.5));
        }
      }
    }
  }

  list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
  list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
  list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

  //return new hittable_list(list,i);
  return new bvh_node(list,i, 0.0, 1.0);

}


// ./build/apps/program > output.ppm


int main (int argc, char** argv) {

  // default values
  bool HIGH_QUALITY_RENDER = !true;
  bool MEDIUM_QUALITY_RENDER = !true;

  // handle command line arguments
  if (argc >= 2) {
    // first command line argument is "HQ"?
    if (std::string(argv[1]) == "HQ") {
      HIGH_QUALITY_RENDER = true;
    }
    // first command line argument is "MQ"?
    if (std::string(argv[1]) == "MQ") {
      MEDIUM_QUALITY_RENDER = true;
    }
  }

  int nx, ny, ns;

  if (HIGH_QUALITY_RENDER) {
    nx = 1200;
    ny = 800;
    ns = 50;
  } else if (MEDIUM_QUALITY_RENDER) {
    nx = 400;
    ny = 200;
    ns = 24;
  } else {
    nx = 200;
    ny = 100;
    ns = 10;
  }

  std::cerr << "Total Scanlines: " << ny << std::endl;

  std::cout << "P3\n" << nx << " " << ny << "\n255\n";

  //hittable *world = random_scene();
  //hittable *world = two_spheres();
  //hittable *world = two_perlin_spheres();
  //hittable *world = globe();
  //hittable *world = simple_light();
  hittable *world = cornell_box();

  // vec3 lookfrom(13,2,3);
  // vec3 lookat(0,0,0);
  // float dist_to_focus = 10.0;
  // float aperture = 0.0;
  // float vfov = 20.0;

  vec3 lookfrom(278, 278, -800);
  vec3 lookat(278,278,0);
  float dist_to_focus = 10.0;
  float aperture = 0.0;
  float vfov = 40.0;

  camera cam(lookfrom, lookat, vec3(0,1,0), vfov,
             float(nx)/float(ny), aperture, dist_to_focus,
             0.0, 1.0);

  for (int j = ny-1; j >= 0; j--) {
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < nx; i++) {
      vec3 col(0, 0, 0);
      for (int s = 0; s < ns; s++) {
        float u = float(i + random_double() - (1.0/2.0)) / float(nx);
        float v = float(j + random_double() - (1.0/2.0)) / float(ny);
        ray r = cam.get_ray(u, v);
        col += color(r, world, 0);
      }
      col /= float(ns);


      // gamma 2 correction -> pow(1/gamma) aka square root
      col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );

      int ir = int(255.99*col[0]);
      int ig = int(255.99*col[1]);
      int ib = int(255.99*col[2]);

      std::cout << ir << " " << ig << " " << ib << "\n";
    }
  }
  std::cerr << "\nDone.\n";

}
