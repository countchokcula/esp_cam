# Edit following two lines to set component requirements (see docs)
set(COMPONENT_REQUIRES )
set(COMPONENT_PRIV_REQUIRES )


set(COMPONENT_SRCS "main.cpp" "src/wifi"
"src/socket" "src/camera" "src/camera_fb_t.pb")
set(COMPONENT_ADD_INCLUDEDIRS "include")


register_component()
