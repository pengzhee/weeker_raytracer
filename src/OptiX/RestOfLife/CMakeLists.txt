INCLUDE_DIRECTORIES(.)

cuda_compile_and_embed(
  raygen_ptx_c   raygen/raygen.cu
  )

cuda_compile_and_embed(
  miss_ptx_c     raygen/miss.cu
  )

cuda_compile_and_embed(
  sphere_ptx_c          geometry/sphere.cu
  )

cuda_compile_and_embed(
  moving_sphere_ptx_c   geometry/movingSphere.cu
  )

cuda_compile_and_embed(
  volume_sphere_ptx_c   geometry/volumeSphere.cu
  )

cuda_compile_and_embed(
  aarect_ptx_c          geometry/aarect.cu
  )

cuda_compile_and_embed(
  volume_box_ptx_c      geometry/volumeBox.cu
  )

cuda_compile_and_embed(
  normal_material_ptx_c     material/normalMaterial.cu
  )

cuda_compile_and_embed(
  lambertian_material_ptx_c material/lambertianMaterial.cu
  )

cuda_compile_and_embed(
  metal_material_ptx_c      material/metalMaterial.cu
  )

cuda_compile_and_embed(
  dielectric_material_ptx_c material/dielectricMaterial.cu
  )

cuda_compile_and_embed(
  diffuse_light_material_ptx_c material/diffuseLight.cu
  )

cuda_compile_and_embed(
  isotropic_material_ptx_c material/isotropicMaterial.cu
  )

cuda_compile_and_embed(
  null_texture_ptx_c        texture/nullTexture.cu
  )

cuda_compile_and_embed(
  constant_texture_ptx_c    texture/constantTexture.cu
  )

cuda_compile_and_embed(
  checkered_texture_ptx_c   texture/checkeredTexture.cu
  )

cuda_compile_and_embed(
  noise_texture_ptx_c       texture/noiseTexture.cu
  )

cuda_compile_and_embed(
  image_texture_ptx_c       texture/imageTexture.cu
  )

cuda_compile_and_embed( cosine_pdf_ptx_c  pdf/cosinePdf.cu )
cuda_compile_and_embed( rect_pdf_ptx_c    pdf/rectPdf.cu )
cuda_compile_and_embed( mixture_pdf_ptx_c      pdf/mixturePdf.cu )
cuda_compile_and_embed( mixture_bias_pdf_ptx_c pdf/mixtureBiasPdf.cu )

add_executable(restOfLifeOptix
  Director.h
  Director.cpp
  main.cpp
  # Compiled cuda ptx programs
  ${raygen_ptx_c}
  ${miss_ptx_c}

  ${sphere_ptx_c}
  ${moving_sphere_ptx_c}
  ${aarect_ptx_c}
  ${volume_box_ptx_c}
  ${volume_sphere_ptx_c}

  ${cosine_pdf_ptx_c}
  ${rect_pdf_ptx_c}
  ${mixture_pdf_ptx_c}
  ${mixture_bias_pdf_ptx_c}

  ${normal_material_ptx_c}
  ${lambertian_material_ptx_c}
  ${metal_material_ptx_c}
  ${dielectric_material_ptx_c}
  ${diffuse_light_material_ptx_c}
  ${isotropic_material_ptx_c}

  ${null_texture_ptx_c}
  ${constant_texture_ptx_c}
  ${checkered_texture_ptx_c}
  ${noise_texture_ptx_c}
  ${image_texture_ptx_c}
  )


target_link_libraries(restOfLifeOptix  ${optix_LIBRARY})
