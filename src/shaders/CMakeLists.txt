find_package(Qt6 COMPONENTS ShaderTools REQUIRED)

qt6_add_shaders(xyz.flafflar.panon "shaders"
  PREFIX /shaders
  FILES
    default.frag
)


